/********************************************************************************************************
 * @file     pm_32krc.c
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

#if PM_TIM_RECOVER_MODE
extern _attribute_aligned_(4) pm_tim_recover_t	pm_timRecover;
#endif

extern  unsigned char       	 pm_long_suspend;
/**
 * @brief      This function serves to set the working mode of MCU,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 5 minutes.
 * @return     indicate whether the cpu is wake up successful.
 */
_attribute_ram_code_ int cpu_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick)
{

	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);

	tick_32k_calib = REG_ADDR16(0x750);

	unsigned short  tick_32k_halfCalib = tick_32k_calib>>1;

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
			if( span > 0x0ff00000 ){  //BIT(28) = 0x10000000   16M:16S
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
	analog_write(0x2c, bit567_ana2c | 0x1e | (!timer_wakeup_enable)<<0); //if use tmr wakeup, auto osc 32k pwdn shoule be disabled
	analog_write(0x07, (analog_read(0x07) & 0xf8) | bit012_ana07);

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG ) {
		analog_write(0x7f, 0x00);
	}
	else{
		write_reg8(0x602,0x08);
		analog_write(0x7f, 0x01);
	}


	//set xtal delay duration
	span = (PM_XTAL_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 16 + tick_32k_halfCalib)/ tick_32k_calib;
	unsigned char rst_cycle = 0x7f - span;
	analog_write (0x20, rst_cycle);


	//set DCDC delay duration
    span = (PM_DCDC_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 16 + tick_32k_halfCalib)/ tick_32k_calib;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);


	unsigned int tick_reset;
	if(pm_long_suspend){
		tick_reset = tick_32k_cur + (unsigned int)(tick_wakeup_reset - tick_cur)/ tick_32k_calib * 16;
	}
	else{
		tick_reset = tick_32k_cur + ((unsigned int)(tick_wakeup_reset - tick_cur) * 16 + tick_32k_halfCalib) / tick_32k_calib;
	}


#if (PM_TIM_RECOVER_MODE)
	unsigned int tick_reset_timRecover;
	if( (wakeup_src & PM_TIM_RECOVER_END) && pm_timRecover.recover_flag){

		if(pm_long_suspend){
			tick_reset_timRecover = pm_timRecover.tick_32k + (unsigned int)(tick_wakeup_reset - pm_timRecover.tick_sysClk)/ tick_32k_calib * 16;
		}
		else{
			tick_reset_timRecover = pm_timRecover.tick_32k + ((unsigned int)(tick_wakeup_reset - pm_timRecover.tick_sysClk) * 16 + tick_32k_halfCalib) / tick_32k_calib;
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

	//Just for BLE
	if(sleep_mode == DEEPSLEEP_MODE){
	   soft_reboot_dly13ms_use24mRC();
	   write_reg8 (0x6f, 0x20);  //reboot
	}

	unsigned int now_tick_32k = cpu_get_32k_tick ();

#if (PM_TIM_RECOVER_MODE)
	if(pm_timRecover.recover_flag == 0x03){
		if(pm_long_suspend){
			tick_cur = pm_timRecover.tick_sysClk+ (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) / 16 * tick_32k_calib;
		}
		else{
			tick_cur = pm_timRecover.tick_sysClk + (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) * tick_32k_calib / 16;		// current clock
		}
	}
	else
#endif
	{
		if(pm_long_suspend){
			tick_cur += (unsigned int)(now_tick_32k - tick_32k_cur) / 16 * tick_32k_calib;
		}
		else{
			tick_cur += (unsigned int)(now_tick_32k - tick_32k_cur) * tick_32k_calib / 16;		// current clock
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

_attribute_ram_code_ unsigned int pm_tim_recover_32k_rc(unsigned int now_tick_32k)
{
	unsigned int deepRet_tick;

#if (PM_TIM_RECOVER_MODE)
	if(pm_timRecover.recover_flag == 0x03){
		deepRet_tick = pm_timRecover.tick_sysClk + (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) * tick_32k_calib / 16;		// current clock
	}
	else
#endif
	{
#if PM_LONG_SUSPEND_EN  //do not consider  deepsleep retention  timer wake_up(bigger than 16S)
		if(pm_long_suspend){
			deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) / 16 * tick_32k_calib;
		}
		else{
			deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) * tick_32k_calib / 16;		// current clock
		}
#else
		deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) * tick_32k_calib / 16;		// current clock
#endif
	}

	return deepRet_tick;
}








