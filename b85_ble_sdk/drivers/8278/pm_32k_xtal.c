/********************************************************************************************************
 * @file     pm_32k_xtal.c
 *
 * @brief    This is the source file for TLSR8278
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

#include "compiler.h"


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



static _attribute_no_inline_ void switch_ext32kpad_to_int32krc(unsigned char bit567_ana2c)
{
	analog_write(SYS_DEEP_ANA_REG,  analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear

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
	//tick_32k_calib = REG_ADDR32(0x754);
	//unsigned short  tick_32k_halfCalib = tick_32k_calib>>1;

	unsigned int   span = (unsigned int)(wakeup_tick - clock_time ());
#if(RAM_CRC_EN)
	unsigned char  ana_00 = analog_read(0x00);
	unsigned char  ram_crc_en = 0;
	unsigned short ram_crc_check_tick = 0;
#endif
	if(timer_wakeup_enable){
		if (span > 0xE0000000)  //BIT(31)+BIT(30)+BIT(19)   7/8 cylce of 32bit
		{
			return  analog_read (0x44) & 0x1f;
		}
		else if (span < EMPTYRUN_TIME_US * SYSTEM_TIMER_TICK_1US) // 0 us base
		{
			unsigned int t = clock_time ();
			analog_write (0x44, 0x1d);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x1d; ////clear the timer wake_up status(bit[1])before read 32k tick may cause get the error tick number from 32K.
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
#if (RAM_CRC_EN)
		if(((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW16K)&&(span > 2500*16*1000)) \
				|| ((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW32K)&&(span > 6*16*1000*1000))){
			analog_write(0x00, ana_00|0x80);  	//poweron_dft: 0x18,  <7> MSB of ret_ldo_trim,  0: 0.8-1.15V; 1: 0.6-0.95V
			analog_write(0x02, 0xa0);  	//poweron_dft: 0xa4,  <2:0> ret_ldo_trim,  set 0x00: 0.6V
			ram_crc_en = 1;
		}
	}
	else if((sleep_mode & DEEPSLEEP_RETENTION_FLAG)&&(wakeup_src == PM_WAKEUP_PAD))
	{
		analog_write(0x00, ana_00|BIT(7));  	//poweron_dft: 0x18,  <7> MSB of ret_ldo_trim,  0: 0.8-1.15V; 1: 0.6-0.95V
		analog_write(0x02, 0xa0);  	//poweron_dft: 0xa4,  <2:0> ret_ldo_trim,  set 0x00: 0.6V
		ram_crc_en = 1;
#endif
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

	tick_cur = clock_time () + 15 * SYSTEM_TIMER_TICK_1US;  //cpu_get_32k_tick will cost 30~40 us
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
	if(sleep_mode == DEEPSLEEP_MODE){
		earlyWakeup_us = EARLYWAKEUP_TIME_US_DEEP;
	}
	else if(sleep_mode & DEEPSLEEP_RETENTION_FLAG){
		earlyWakeup_us = EARLYWAKEUP_TIME_US_DEEP_RET;
	}
	unsigned int tick_wakeup_reset = wakeup_tick - earlyWakeup_us * SYSTEM_TIMER_TICK_1US;

	/////////////////// set wakeup source /////////////////////////////////
	analog_write (0x26, wakeup_src);   //[2] wakeup pad sel,1: 16usfilter (need 32k is on) 0: non-filter

	analog_write (0x44, 0x1f);				//clear all flag


	///////////////////////// change to 24M RC clock before suspend /////////////
	unsigned char reg66 = read_reg8(0x066);			//
	write_reg8 (0x066, 0);				//change to 24M rc clock

	unsigned char bit67_ana06;

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG) { //deepsleep with retention
		bit67_ana06 = 0x40;  		//ana_06<7:6> -> poweron_dft: 11 -> 01 power up ret ldo and power down spd ldo
	}
	else if(sleep_mode){  //deepsleep no retention
		bit67_ana06  = 0xc0;  		//ana_06<7:6> -> poweron_dft: 11 -> 11 power down ret ldo and power down spd ldo
	}
	else{  //suspend
		bit67_ana06  = 0x80;  		//ana_06<7:6> -> poweron_dft: 10 -> 10 power down ret ldo and power up spd ldo
		analog_write(0x04,0x48);  	//poweron_dft: 0x48
		                            //ana_04<6:4> suspend LDO output voltage trim  default:100-1.2v
	}
	analog_write(0x7e, sleep_mode);
	analog_write(0x2b, (sleep_mode ? 0xce : 0x4e) );

#if 1 //must
	if(sleep_mode == DEEPSLEEP_MODE && !timer_wakeup_enable){ //if deep mode and no tmr wakeup
		switch_ext32kpad_to_int32krc(0);
	}
	else{ //suspend mode or deepretn mode or tmr wakup (we don't pwdn ext 32k pad clk,even though three's no tmr wakup src in susped or deepretn mode)
		analog_write(0x2c, 0x1d);//if use tmr wakeup, auto pad 32k pwdn shoule be disabled
	}
#else
	analog_write(0x2c, 0x1d | (!timer_wakeup_enable)<<1);  //if use tmr wakeup, auto pad 32k pwdn shoule be disabled
#endif

	analog_write(0x06, (analog_read(0x06) & 0x3f) | bit67_ana06);

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG ) {
#if (RAM_CRC_EN)
		if(ram_crc_en){
			if(sleep_mode & 0x20){	//16k retention,check 16k sram crc
				ram_crc_check_tick = 200 * 16;//Theoretical value:170*16
				write_reg8(0x40008,0x0f);
			}
			else{					//32k retention,check 32k sram crc
				ram_crc_check_tick = 350 * 16;//Theoretical value:340*16
				write_reg8(0x40008,0x1f);
			}
			write_reg8(0x6f, 0x10);
			analog_write(0x7f, 0x02);
			tick_wakeup_reset -= ram_crc_check_tick;
		}
		else
#endif
		{
			analog_write(0x7f, 0x00);
		}

	}
	else{
		write_reg8(0x602,0x08);
		analog_write(0x7f, 0x01);
	}

	//set DCDC delay duration
#if 1 //2 * 1/16k = 125 uS
	if(sleep_mode == DEEPSLEEP_MODE){
		analog_write (0x1f, 0xff - PM_DCDC_DELAY_CYCLE - SOFT_START_DELAY);//(0xff - n): if timer wake up : ((n+1)*2) 32k cycle; else pad wake up: (((n+1)*2-1) ~ (n+1)*2 )32k cycle
	}else{
		analog_write (0x1f, 0xff - PM_DCDC_DELAY_CYCLE);//(0xff - n): if timer wake up : ((n+1)*2) 32k cycle; else pad wake up: (((n+1)*2-1) ~ (n+1)*2 )32k cycle
	}
#else
    span = (PM_DCDC_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1))/ CRYSTAL32768_TICK_PER_32CYCLE;
    unsigned char rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);
#endif

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



	// write 32k tick
	cpu_set_32k_tick(tick_reset);

	if(analog_read(0x44)&0x1f){

	}
	else{
		sleep_start();
	}

	if(sleep_mode == DEEPSLEEP_MODE){
	   write_reg8 (0x6f, 0x20);  //reboot
	}

	unsigned int now_tick_32k = cpu_get_32k_tick();

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

	unsigned int tick_cur_adjust = tick_cur	+ 20 * SYSTEM_TIMER_TICK_1US ;
	if(wakeup_src & PM_WAKEUP_TIMER){
		if((unsigned int)((wakeup_tick - 80) - tick_cur_adjust ) > BIT(30)) // need 3-4us
		{
			tick_cur_adjust = wakeup_tick - 160;
		}
	}
	reg_system_tick = tick_cur_adjust;


	reg_system_ctrl |= FLD_SYSTEM_32K_CAL_EN|FLD_SYSTEM_TIMER_MANUAL_EN;    //system timer auto mode
	reg_system_irq_mask |= BIT(2);   //enable system timer irq										//enable system timer irq


	write_reg8 (0x066, reg66);		//restore system clock


	unsigned char anareg44 = analog_read(0x44);
//	DBG_C HN2_HIGH;
	if ( (anareg44 & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )	//wakeup from timer only
	{
		while ((unsigned int)(clock_time () -  wakeup_tick) > BIT(30));
	}
//	DBG_C HN2_LOW;
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
