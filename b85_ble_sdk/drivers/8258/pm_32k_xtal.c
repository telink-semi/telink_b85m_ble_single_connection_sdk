/********************************************************************************************************
 * @file     pm_32k_xtal.c
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 Driver Group
 * @date     Jan 24, 2019
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/



#include "bsp.h"
#include "compiler.h"
#include "register.h"
#include "analog.h"
#include "clock.h"
#include "irq.h"
#include "pm.h"
#include "timer.h"

//system timer clock source is constant 16M, never change
//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through the 750 and 751
//register, the conversion error(use 32k:16 cycle, count 16M sys tmr's ticks), at least the introduction of 64ppm.
#define CRYSTAL32768_TICK_PER_32CYCLE		15625  // 7812.5 * 2

#if PM_TIM_RECOVER_MODE
extern _attribute_aligned_(4) pm_tim_recover_t	pm_timRecover;
#endif

extern  unsigned char       	 pm_long_suspend;


static _attribute_no_inline_ void switch_ext32kpad_to_int32krc(unsigned char bit567_ana2c)
{
	analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear

	//switch 32k clk src: select internal 32k rc, if not do this, when deep+pad wakeup: there's no stable 32k clk(therefore, the pad wake-up time
	//is a bit long, the need for external 32k crystal vibration time) to count DCDC dly and XTAL dly. High temperatures even make it impossible
	//to vibrate, as the code for PWM excitation crystals has not yet been effectively executed. SO, we can switch 32k clk to the internal 32k rc.
	analog_write(0x2d, 0x15); //32k select:[7]:0 sel 32k rc,1:32k pad
	analog_write(0x05, 0x02); //[0]:32k rc;[1]:32k xtal (1->pwdn,0->pwup). Power up 32k rc.

	//deep + no tmr wakeup(we need  32k clk to count dcdc dly and xtal dly, but this case, ext 32k clk need close, here we use 32k rc instead)
	analog_write(0x2c, bit567_ana2c | 0x1f);
}


_attribute_ram_code_ int cpu_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick)
{
	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);

	//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through the 750 and 751 register, the conversion error, at least the introduction of 64ppm
	//tick_32k_calib = REG_ADDR16(0x750);
	//unsigned short  tick_32k_halfCalib = tick_32k_calib>>1;

	unsigned int span = (unsigned int)(wakeup_tick - clock_time ());


	if(timer_wakeup_enable){
		if (span > 0xE0000000)  //BIT(31)+BIT(30)+BIT(19)   7/8 cylce of 32bit
		{
			return  analog_read (0x44) & 0x0f;
		}
		else if (span < EMPTYRUN_TIME_US * SYSTEM_TIMER_TICK_1US) // 0 us base
		{
			unsigned int t = clock_time ();
			analog_write (0x44, 0x0f);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x0f;
			} while ( ((unsigned int)clock_time () - t < span) && !st);
			return st;

		}
		else
		{
			//						thres			  16M		 24M		32M			48M
			//use 16 cycle   BIT(28) = 0x10000000   16.777 S   11.185 S   8.388 S      5.592 S
			//use 32 cycle   BIT(27) = 0x08000000    8.389 S 	5.592 S   4.194 S	   2.796 S
			//use 64 cycle   BIT(26) = 0x04000000 				2.796 S
			if(span >= 0x07FF0000){  //BIT(28) = 0x08000000   16M:8.389S
				pm_long_suspend = 1;
			}
			else{
				pm_long_suspend = 0;
			}
		}
	}



	////////// disable IRQ //////////////////////////////////////////
	unsigned char r = irq_disable ();

	if(func_before_suspend){
		if (!func_before_suspend())
		{
			irq_restore(r);
			return WAKEUP_STATUS_PAD;
		}
	}


	tick_cur = clock_time () + 35 * SYSTEM_TIMER_TICK_1US;  //cpu_get_32k_tick will cost 30~40 us
	tick_32k_cur = cpu_get_32k_tick ();

#if (PM_TIM_RECOVER_MODE)
	pm_timRecover.recover_flag = 0;
	if(wakeup_src & PM_TIM_RECOVER_START){
		pm_timRecover.tick_sysClk = tick_cur;
		pm_timRecover.tick_32k = tick_32k_cur;
	}
	if(wakeup_src & PM_TIM_RECOVER_END){
		if(pm_timRecover.tick_sysClk && (unsigned int)(clock_time() - pm_timRecover.tick_sysClk) < BIT(26) ){  //BIT(26) = 2^26 = (16<<20)*4   about 4S
			pm_timRecover.recover_flag = 0x01;
		}
	}
#endif

	unsigned int earlyWakeup_us = EARLYWAKEUP_TIME_US_SUSPEND;
	if(sleep_mode){
		earlyWakeup_us = EARLYWAKEUP_TIME_US_DEEP;
	}
	unsigned int tick_wakeup_reset = wakeup_tick - earlyWakeup_us * SYSTEM_TIMER_TICK_1US;

	/////////////////// set wakeup source /////////////////////////////////
	analog_write (0x26, wakeup_src);   //@@@@@@@ [3] wakeup pad sel,1: 16usfilter (need 32k is on) 0: non-filter
//	write_reg8(0x6e, wakeup_src & PM_WAKEUP_CORE ? 0x08 : 0x00);

	analog_write (0x44, 0x0f);				//clear all flag


	///////////////////////// change to 24M RC clock before suspend /////////////
	unsigned char reg66 = read_reg8(0x066);			//
	write_reg8 (0x066, 0);				//change to 24M rc clock

#if (SYSCLK_RC_CLOCK_EN)
	unsigned char ana04 = analog_read(0x04);
	unsigned char ana33 = analog_read(0x33);

	analog_write(0x04, ana04&0xfc);     //select 24M RC
	analog_write(0x33, tl_24mrc_cal); 			//default cal value for 24M RC
#endif

	unsigned char bit567_ana2c;
	unsigned char bit012_ana07;

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG) { //deepsleep with retention
		bit567_ana2c = 0x40;  //ana_2c<7:5>: 010
		bit012_ana07 = 0x01;  //ana_07<2:0>:001
//		analog_write(0x02, (analog_read(0x02) & 0xf8) | 0x02);  //ana_02<2:0>:010-> 0.8v ret ldo
		analog_write(0x02, (analog_read(0x02) & 0xf8) | 0x05);  //ana_02<2:0>:101-> 1.1v ret ldo
		analog_write(0x7e, sleep_mode);
	}
	else if(sleep_mode){  //deepsleep no retention
		bit567_ana2c = 0xc0;  //ana_2c<7:5>: 110
		bit012_ana07 = 0x05;  //ana_07<2:0>: 101
	}
	else{  //suspend
		bit567_ana2c = 0x80;  //ana_2c<7:5>: 100    //BIT<5>: iso_ana,  set 0 now
		bit012_ana07 = 0x04;  //ana_07<2:0>: 100
		analog_write(0x04,0x48);  //ana_04<6:4> suspend LDO output voltage trim  default:100-1.0v
		analog_write(0x7e, 0x00);
	}

	analog_write(0x2b, (sleep_mode ? 0xde : 0x5e) );

#if 1 //must
	if(sleep_mode == DEEPSLEEP_MODE && !timer_wakeup_enable){ //if deep mode and no tmr wakeup
		switch_ext32kpad_to_int32krc(bit567_ana2c);
	}
	else{ //suspend mode or deepretn mode or tmr wakup (we don't pwdn ext 32k pad clk,even though three's no tmr wakup src in susped or deepretn mode)
		analog_write(0x2c, bit567_ana2c | 0x1d);//if use tmr wakeup, auto pad 32k pwdn shoule be disabled
	}
#else
	analog_write(0x2c, bit567_ana2c | 0x1d | (!timer_wakeup_enable)<<1);  //if use tmr wakeup, auto pad 32k pwdn shoule be disabled
#endif

	analog_write(0x07, (analog_read(0x07) & 0xf8) | bit012_ana07);

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG ) {
		analog_write(0x7f, 0x00);
	}
	else{
		write_reg8(0x602,0x08);
		analog_write(0x7f, 0x01);
	}


	//set xtal delay duration
	span = (PM_XTAL_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1))/ CRYSTAL32768_TICK_PER_32CYCLE;
	unsigned char rst_cycle = 0x7f - span;
	analog_write (0x20, rst_cycle);


	//set DCDC delay duration
    span = (PM_DCDC_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1))/ CRYSTAL32768_TICK_PER_32CYCLE;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);


	unsigned int tick_reset;


	if(pm_long_suspend){
		tick_reset = tick_32k_cur + (unsigned int)(tick_wakeup_reset - tick_cur)/ CRYSTAL32768_TICK_PER_32CYCLE * 32;
	}
	else{
		tick_reset = tick_32k_cur + ((unsigned int)(tick_wakeup_reset - tick_cur) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1)) / CRYSTAL32768_TICK_PER_32CYCLE;
	}


#if (PM_TIM_RECOVER_MODE)
	unsigned int tick_reset_timRecover;
	if( (wakeup_src & PM_TIM_RECOVER_END) && pm_timRecover.recover_flag){


		if(pm_long_suspend){
			tick_reset_timRecover = pm_timRecover.tick_32k + (unsigned int)(tick_wakeup_reset - pm_timRecover.tick_sysClk)/ CRYSTAL32768_TICK_PER_32CYCLE * 32;
		}
		else{
			tick_reset_timRecover = pm_timRecover.tick_32k + ((unsigned int)(tick_wakeup_reset - pm_timRecover.tick_sysClk) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1)) / CRYSTAL32768_TICK_PER_32CYCLE;
		}


		int rest_32k_cycle = (int )(tick_reset_timRecover - tick_32k_cur);
		//if( rest_32k_cycle > 6  &&  abs(tick_reset - tick_reset_timRecover) < BIT(7)  ){  //128 * 30us(1/32k) = 3800us
		if( rest_32k_cycle > 6  &&  (unsigned int)(tick_reset + BIT(7) - tick_reset_timRecover) < BIT(8)  ){  //128 * 30us(1/32k) = 3800us
			pm_timRecover.recover_flag = 0x03;
			tick_reset = tick_reset_timRecover;
		}
		else{
			//debug
		}
	}
#endif



	REG_ADDR8(0x74c) = 0x2c;
	REG_ADDR32(0x754) = tick_reset;
	REG_ADDR8(0x74f) = BIT(3);									//start write
	//16 tnop is enough
	CLOCK_DLY_8_CYC;
	CLOCK_DLY_8_CYC;
	while (REG_ADDR8(0x74f) & BIT(3));



	write_reg8(0x74c, 0x20);
	if(analog_read(0x44)&0x0f){

	}
	else{
		DBG_CHN0_LOW;
		sleep_start();
		DBG_CHN0_HIGH;
	}

	if(sleep_mode == DEEPSLEEP_MODE){
		analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear
		soft_reboot_dly13ms_use24mRC();
		write_reg8 (0x6f, 0x20);  //reboot
	}


	unsigned int now_tick_32k = cpu_get_32k_tick ();

#if (PM_TIM_RECOVER_MODE)
	if(pm_timRecover.recover_flag == 0x03){

		if(pm_long_suspend){
			tick_cur = pm_timRecover.tick_sysClk+ (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
		}
		else{
			tick_cur = pm_timRecover.tick_sysClk + (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
		}
	}
	else
#endif
	{
		if(pm_long_suspend){
			tick_cur += (unsigned int)(now_tick_32k - tick_32k_cur) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
		}
		else{
			tick_cur += (unsigned int)(now_tick_32k - tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
		}
	}



 //sync to junwei's driver(1st: Compensation system Timer Count value; 2nd: restore system clock)
	reg_system_tick = tick_cur + 20 * SYSTEM_TIMER_TICK_1US;  //20us compensation
	REG_ADDR8(0x74c) = 0x00;
	//at least 5~6cyc confirmed by guanjun: system timer clock domain(16M) and system clock(24M)inconsistent,
	//set the system timer related registers need clock synchronization, to ensure correct writing, otherwise there is a risk of crash.
	CLOCK_DLY_6_CYC;					//match 24M RC clock
	REG_ADDR8(0x74c) = 0x92;  			//reg_system_tick_mode |= FLD_SYSTEM_TICK_IRQ_EN;
	//at least 3~4cyc confirmed by guanjun: system timer clock domain(16M) and system clock(24M)inconsistent,
	//set the system timer related registers need clock synchronization, to ensure correct writing, otherwise there is a risk of crash.
	CLOCK_DLY_4_CYC;					//match 24M RC clock
	REG_ADDR8(0x74f) = BIT(0);

	//restore system clock
	#if (SYSCLK_RC_CLOCK_EN)
		analog_write(0x33, ana33);
		analog_write(0x04, ana04);
	#endif
	write_reg8 (0x066, reg66);		//restore system clock


	unsigned char anareg44 = analog_read(0x44);

	if ( (anareg44 & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )	//wakeup from timer only
	{
		while ((unsigned int)(clock_time () -  wakeup_tick) > BIT(30));
	}

	irq_restore(r);

	return (anareg44 ? (anareg44 | STATUS_ENTER_SUSPEND) : STATUS_GPIO_ERR_NO_ENTER_PM );
}


_attribute_ram_code_ unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k)
{
	unsigned int deepRet_tick;

#if (PM_TIM_RECOVER_MODE)
	if(pm_timRecover.recover_flag == 0x03){
		deepRet_tick = pm_timRecover.tick_sysClk + (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
	}
	else
#endif
	{

#if PM_LONG_SUSPEND_EN  //do not consider  deepsleep retention  timer wake_up(bigger than 8S)
		if(pm_long_suspend){
			deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
		}
		else{
			deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
		}
#else
		deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
#endif
	}

	return deepRet_tick;
}


/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  variable of 32k tick.
 */

_attribute_ram_code_ unsigned int get_32k_tick(void)
{
    unsigned int t0 = 0;
    unsigned int t1 = 0;
    unsigned int n = 0;

    while (1) {

        t0 = t1;
        t1 = analog_read(0x43);
        t1 <<= 8;
        t1 += analog_read(0x42);
        t1 <<= 8;
        t1 += analog_read(0x41);
        t1 <<= 8;
        t1 += analog_read(0x40);
        if (n)
        {
            if ((unsigned int)(t1 - t0) < 2) {
                return t1;
            }
            else if ( (t0^t1) == 1 ) {
                return t0;
            }
        }
        n++;
    }
    return t1;
}

_attribute_data_retention_ static unsigned int tick_check32kPad = 0;


_attribute_no_inline_ void check_32k_clk_stable(void)
{
	if(clock_time_exceed(tick_check32kPad, 10000)){ //every 10ms, check if 32k pad clk is stable
		tick_check32kPad = clock_time();

		unsigned int last_32k_tick;
		unsigned int curr_32k_tick;

		//Check if 32k pad vibration and basically works stably
		last_32k_tick = pm_get_32k_tick();
		WaitUs(50); //for 32k tick accumulator, tick period: 30.5us, if stable: delta tick > 0
		curr_32k_tick = pm_get_32k_tick();

		if(last_32k_tick != curr_32k_tick){
			blt_miscParam.pm_enter_en = 1;//allow enter pm
			return;
		}
	}
	else{
		return;
	}

	// T > 2s , 32k pad clk still unstable: reboot MCU
	if(!blt_miscParam.pm_enter_en && clock_time_exceed(0, 2000000)){
		analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
		start_reboot(); //reboot the MCU
	}
}






