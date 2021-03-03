/********************************************************************************************************
 * @file     pm.c
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 Driver Group
 * @date     May 8, 2018
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
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/

#include "bsp.h"

#include "compiler.h"
#include "register.h"
#include "gpio.h"
#include "analog.h"
#include "clock.h"
#include "timer.h"
#include "irq.h"
#include "pm.h"
#include "random.h"
#include "adc.h"


_attribute_data_retention_  unsigned char 		tl_24mrc_cal = 0x80;
_attribute_data_retention_  unsigned char       pm_long_suspend;


_attribute_data_retention_  _attribute_aligned_(4) misc_para_t 		blt_miscParam;
cpu_pm_handler_t 		  	cpu_sleep_wakeup;  //no need retention,  cause it will be set every wake_up
pm_tim_recover_handler_t    pm_tim_recover;
check_32k_clk_handler_t   	pm_check_32k_clk_stable = 0;
pm_get_32k_clk_handler_t  	pm_get_32k_tick = 0;

/* wakeup source :is_deepretn_back,is_pad_wakeup,wakeup_src*/
_attribute_aligned_(4) pm_para_t pmParam;
/* enable status :conn_mark,ext_cap_en,pad32k_en,pm_enter_en */
_attribute_data_retention_	unsigned short 		tick_32k_calib;
_attribute_data_retention_	unsigned int 		tick_cur;
_attribute_data_retention_	unsigned int 		tick_32k_cur;
/* tick :tick_sysClk,tick_32k,recover_flag */
#if (PM_TIM_RECOVER_MODE)
	_attribute_data_retention_	_attribute_aligned_(4) pm_tim_recover_t	pm_timRecover;
#endif

_attribute_data_retention_	suspend_handler_t	func_before_suspend = 0;

void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func )
{
	func_before_suspend = func;
}
#define FLASH_DEEP_EN				1   //if flash sleep,  when deep retention, must wakeup flash in cstartup
#define FLASH_STILL_ON				1


//get Efuse SRAM cfg[61:63]
//0b000: 64K SRAM
//0b001: 16K SRAM
//0b010: 32K SRAM
//0b100: 48K SRAM
_attribute_data_retention_ unsigned char pm_bit_info = 0; //SRAM type

unsigned long pm_curr_stack;
static void get_sp_normal(void)
{
	asm volatile (
	"tpush {r0};"
	"tpush {r1};"
	"tloadr r0,MYSEC;"
	"tmov r1,r13;"
	"tstorer r1, [r0];"
	"tpop {r1};"
	"tpop {r0};"
	"tjex lr;"
	);

	asm volatile(
	".align 4;"
	"MYSEC:;"
	".word(pm_curr_stack);"
	);
}


/**
 * @brief     this function servers to get data(BIT0~BIT31) from EFUSE.
 * @param[in] none
 * @return    data(BIT0~BIT31)
 */
unsigned int  pm_get_info0(void)
{
	unsigned int efuse_info;
	write_reg8(0x74, 0x62);
	efuse_info= read_reg32(0x48);
	write_reg8(0x74, 0x00);
	return efuse_info ;
}

/**
 * @brief     this function servers to get data(BIT32~BIT63) from EFUSE.
 * @param[in] none
 * @return    data(BIT32~BIT63)
 */
unsigned int  pm_get_info1(void)
{
	unsigned int efuse_info;
	write_reg8(0x74, 0x62);
	efuse_info = read_reg32(0x4c);
	write_reg8(0x74, 0x00);
	return efuse_info ;
}

extern void start_suspend(void);
/**
 * @brief     this function srevers to start sleep mode.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_ _attribute_no_inline_ void  sleep_start(void)
{
	analog_write(0x34,0x87);  //BIT<2:0> audio/USB/ZB  power down, save 50uA +

	volatile unsigned int i;

#if FLASH_DEEP_EN
    write_reg8(0x00d, 0);
    write_reg8(0x00c, 0xb9);
    for(i = 0; i < 2; i++); //1440ns when 32M clk
    write_reg8(0x00d, 1);

    write_reg8(0x5a1, 0x00); //MSPI ie disable
#endif

#if FLASH_STILL_ON
	#if MCU_8258_A0
    	analog_write(0x0c, 0xcc);   //suspend wakeup recover to  0xc4
	#endif
    	analog_write(0x82, 0x0c);   //suspend wakeup recover to  0x64
#else
    analog_write(0x0c, 0xcc);
#endif


    unsigned int T_reg = read_reg8(0x60d);
    T_reg = (T_reg << 8) + 0x40058;

    unsigned int T_regValue = read_reg32(T_reg);
    write_reg32(T_reg, 0x06c006c0);

    start_suspend();
    write_reg32(T_reg, T_regValue);

    analog_write(0x82, 0x64);

#if MCU_8258_A0
	analog_write(0x0c, 0xc4);
#endif

#if FLASH_DEEP_EN
    write_reg8(0x5a1, 0x0f); //MSPI ie enable

    write_reg8(0x00d, 0);
    write_reg8(0x00c, 0xab);
    for(i = 0; i < 2; i++); //1440ns when 32M clk
    write_reg8(0x00d, 1);
#endif

    analog_write(0x34,0x80);  //BIT<2:0> audio/USB/ZB  power on
}

/**
 * @brief     this function servers to start reboot.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_ void start_reboot(void)
{
	if(blt_miscParam.pad32k_en){
		analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear
	}

	irq_disable ();
	sleep_us(13000);   //delay 12ms to avoid soft start problem
	REG_ADDR8(0x6f) = 0x20;  //reboot
	while (1);
}


/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  variable of 32k tick.
 */
_attribute_ram_code_ _attribute_no_inline_ unsigned int cpu_get_32k_tick (void)
{
	unsigned int		t0 = 0, t1 = 0, n;

	n = 0;
	REG_ADDR8(0x74c) = 0x28;							//system timer manual mode, interrupt disable
	while (1)
	{
		REG_ADDR8(0x74f) = BIT(3);							//start read

		//16 tnop is enough
		CLOCK_DLY_8_CYC;
		CLOCK_DLY_8_CYC;

		while (REG_ADDR8(0x74f) & BIT(3));
		t0 = t1;
		t1 = REG_ADDR32(0x754);
		if (n)
		{
			if ((unsigned int)(t1 - t0) < 2)
			{
				return t1;
			}
			else if ( (t0^t1) == 1 )	// -1
			{
				return t0;
			}
		}
		n++;
	}
	return t1;
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer0.
 * @param[in]   tick - capture value of timer0.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer0(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr0_tick = 0;

    reg_tmr0_capt = tick;
    reg_tmr_ctrl8 |= FLD_TMR0_EN;//enable TIMER0,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR0_EN;//timer0 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR0; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR0; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR0_EN;//disable TIMER0
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer1.
 * @param   tick - capture value of timer1.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer1(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr1_tick = 0;

    reg_tmr1_capt = tick;
    reg_tmr_ctrl8 |= FLD_TMR1_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR1_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR1_EN;//disable TIMER1
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer2.
 * @param[in]   tick - capture value of timer2.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer2(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr2_tick = 0;

    reg_tmr2_capt = tick;
    reg_tmr_ctrl8 |= FLD_TMR2_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR2_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR2; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR2; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR2_EN;//disable TIMER1
}

#if 0
/**
 * @brief   This function serves to wake up cpu from stall mode by timer1 or RF TX done irq.
 * @param   WakeupSrc  - timer1.
 * @param   IntervalUs - capture value of timer1.
 * @param   sysclktick - tick value of per us based on system clock.
 * @return  none.
 */
_attribute_ram_code_ unsigned int cpu_stall(int WakeupSrc, unsigned int IntervalUs,unsigned int sysclktick)
{
    unsigned int wakeup_src = 0;

    if (IntervalUs) {
        reg_tmr1_tick = 0;
        reg_tmr1_capt = IntervalUs * sysclktick;
        reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt
        reg_tmr_ctrl8 &= (~FLD_TMR1_MODE); //select mode0: using sclk
        reg_tmr_ctrl8 |= FLD_TMR1_EN; //enable TIMER1
    }
    REG_ADDR32(0x78) |= WakeupSrc;  //FLD_IRQ_TMR1_EN | FLD_IRQ_ZB_RT_EN

    reg_irq_mask &= (~FLD_IRQ_ZB_RT_EN);
    reg_irq_mask &= (~FLD_IRQ_TMR1_EN);

    write_reg8(0x6f, 0x80); //stall mcu
    asm("tnop");
    asm("tnop");

    if (IntervalUs) {
        reg_tmr1_tick = 0;
        reg_tmr_ctrl8 &= (~FLD_TMR1_EN); //disable TIMER1
    }

    //store the wakeup source
    wakeup_src = REG_ADDR32(0x40);

    //clear the source
    reg_tmr_sta = FLD_TMR_STA_TMR1; // clear timer1 irq source
    write_reg16(0xf20, 0xffff); //clear rf irq status

    return wakeup_src;
}
#endif

/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   kick_ms - duration of kick.
 * @return  	none.
 */
static _attribute_no_inline_ void pwm_kick_32k_pad(unsigned int kick_ms)
{
	//2.set pc3 as pwm output
	write_reg8(0x66,  0x43); //sys clk use 16M crystal
	write_reg8(0x596, 0xf7); //pc3 as pwm func
	write_reg32(0x798,0x20001); //pwm1's high time or low time && pwm1's cycle time
	write_reg8(0x780, 0x02); //enable pwm1
	write_reg8(0x782, 0xf3); //pwm clk div

	//3.wait for PWM wake up Xtal

	sleep_us(kick_ms*1000);

	//4.Recover PC3 as Xtal pin
	write_reg8(0x66,   0x06); //default value
	write_reg8(0x596,  0xff); //default value
	write_reg32(0x798, 0x00); //default value
	write_reg8(0x780,  0x00); //default value
	write_reg8(0x782,  0x00); //default value
}

/**
 * @brief   	This function serves to delay 13ms based on 24M rc clk.
 * @param[in]   none.
 * @return  	none.
 */
__attribute__((optimize("-O0")))
_attribute_no_inline_ void soft_reboot_dly13ms_use24mRC(void)
{
	for(int i = 0; i < 15500; i++); //about 13ms when 24M rc clk
}

/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   times - the frequency of being kicked by pwm.
 * @return  	none.
 */
static _attribute_no_inline_ void pwm_kick_32k_pad_times(unsigned int times)
{
	if(times){

		//1. select 32k xtal
		analog_write(0x2d, 0x95);//32k select:[7]:0 sel 32k rc,1:32k XTAL

		#if 0 //must close, reason is as follows:
			/*
			 * This problem occurs with suspend and deep and deep retention. When the power supply voltage is low, suspend/deep/deep retention cannot
			 * be reset within 12ms, otherwise softstart will work. However, there is not enough delay to wait for softstart to complete. This delay
			 * will be postponed to the code execution area and it will not be able to handle larger code, otherwise it will be dropped by 1.8v, load
			 * error, and finally stuck.(Root: DCDC dly depends on the 32k rc clock, so the 32k rc power supply can't be turned off here.)
			 */
			analog_write(0x05, 0x01);//Power down 32KHz RC,  Power up [32KHz crystal, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]
		#else
			analog_write(0x05, 0x00);//Power up 32KHz RC,  Power up 32KHz crystal
		#endif

		//analog_write(0x03,0x4f); //<7:6>current select(default value, not need)

		int last_32k_tick;
		int curr_32k_tick;
		int i = 0;
		for(i = 0; i< times; i++){

			//After 10ms, the external 32k crystal clk is considered stable(when using PWM to accelerate the oscillation process)
			pwm_kick_32k_pad(10);//PWM kick external 32k pad (duration 10ms)

			//Check if 32k pad vibration and basically works stably
			last_32k_tick = pm_get_32k_tick();

			sleep_us(305);//for 32k tick accumulator, tick period: 30.5us, dly 10 ticks

			curr_32k_tick = pm_get_32k_tick();

			if(last_32k_tick != curr_32k_tick){ //pwm kick 32k pad success
				break;
			}
		}

		#if (0) //blt_sdk_main_loop: check if 32k pad stable, if not, reboot MCU

			if(i >= times){
				analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
				start_reboot(); //reboot the MCU
			}
		#endif
	}
}

/**
 * @brief      This function configures a GPIO pin as the wakeup pin.
 * @param[in]  Pin - the pin needs to be configured as wakeup pin
 * @param[in]  Pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup)
 * @param[in]  En  - enable or disable the wakeup function for the pan pin(1: Enable, 0: Disable)
 * @return     none
 */
void cpu_set_gpio_wakeup (GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en)
{
	///////////////////////////////////////////////////////////
	// 		  PA[7:0]	    PB[7:0]		PC[7:0]		PD[7:0]
	// en: 	ana_21<7:0>	 ana_22<7:0>  ana_23<7:0>  ana_24<7:0>
	// pol:	ana_27<7:0>	 ana_28<7:0>  ana_29<7:0>  ana_2a<7:0>
    unsigned char mask = pin & 0xff;
	unsigned char areg;
	unsigned char val;

	////////////////////////// polarity ////////////////////////

	areg = ((pin>>8) + 0x21);
	val = analog_read(areg);
	if (pol) {
		val &= ~mask;
	}
	else {
		val |= mask;
	}
	analog_write (areg, val);

	/////////////////////////// enable /////////////////////
	areg = ((pin>>8) + 0x27);
	val = analog_read(areg);
	if (en) {
		val |= mask;
	}
	else {
		val &= ~mask;
	}
	analog_write (areg, val);
}

/**
 * @brief   	This function serves to initiate the cpu after power on or deepsleep mode.
 * @param[in]   none.
 * @return  	none.
 */
static _attribute_no_inline_ void cpu_wakeup_no_deepretn_back_init(void)
{
	extern void random_generator_pre_init(void);
	random_generator_pre_init();  //use other initialization timing as random channel delay

	//Set 32k clk src: external 32k crystal, only need init when deep+pad wakeup or 1st power on
	if(blt_miscParam.pad32k_en){

		//in this case: ext 32k clk was closed, here need re-init.
		//when deep_pad wakeup or first power on, it needs pwm acc 32k pad vibration time(dly 10ms)
		if(!(analog_read(SYS_DEEP_ANA_REG) & SYS_NEED_REINIT_EXT32K)){
			analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) | SYS_NEED_REINIT_EXT32K); //if initialized, the FLG is set to "1"

			pwm_kick_32k_pad_times(10);
		}
		else{
			WaitMs(6);// wait for random generator pre init
		}
	}
	else{
		//default 32k clk src: internal 32k rc, here can be optimized
		//analog_write(0x2d, 0x15); //32k select:[7]:0 sel 32k rc,1:32k pad
		//analog_write(0x05, 0x02); //Power down 32k crystal,  Power up [32KHz RC, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]

		rc_32k_cal();  //6.69 ms/6.7 ms
	}


	//rc_24m_cal();  //469 us/474 us
	//flash_rdid = flash_get_jedec_id();
	doubler_calibration();  //when 32m/48m crystal clock used, add this, takes 160us / 16m


	//////////////////// get Efuse bit32~63 info ////////////////////
	unsigned char efuse_8359_skip_flg = 0;
	unsigned int  efuse_32to63bit_info = pm_get_info1();
	unsigned char sram_size_cfg = (efuse_32to63bit_info>>29)&0x7;	//bit61 ~ bit63
	pm_bit_info = sram_size_cfg;

#if 1
	///////////// Efuse 8359 chk && ADC vref calib //////////////////
	if((efuse_32to63bit_info & 0xC0) == 0xC0){ //Efuse use new rule(efuse_bit38 and bit39 are all '1')
		if(efuse_32to63bit_info & BIT(8)){
			//efuse_bit40 is '1' means TLSR8359 select
			efuse_8359_skip_flg = 1;
		}
		else if(adc_vref_cfg.adc_calib_en){
			unsigned char adc_ref_idx = efuse_32to63bit_info & 0x3f; //ADC Ref: efuse bit32~bit36
			adc_vref_cfg.adc_vref = 1015 + adc_ref_idx * 5; //ADC ref voltage: g_adc_vref (unit: mV)
		}
	}
	////////////////// Efuse SRAM size check ////////////////////////
	if(sram_size_cfg != 0 || efuse_8359_skip_flg){ //MCU NOT TLSR8258

		get_sp_normal(); //get normal stack pointer,
		unsigned int stack_chg_min_area = (pm_curr_stack - 100)&0xffffff00;
		unsigned int stack_chg_max_area = (pm_curr_stack + 100)&0xffffff00;

		//normal stack pointer check: 32K SRAM && SP > 32K or 48K SRAM && SP > 48K
		if(efuse_8359_skip_flg || (sram_size_cfg == 0x02 && pm_curr_stack > 0x848000) || (sram_size_cfg == 0x04 && pm_curr_stack > 0x84c000)){
	  //if((sram_size_cfg == 0x04 && pm_curr_stack > 0x84c000)){ // 48K SRAM && SP > 48K
			for(int u = stack_chg_min_area; u < stack_chg_max_area; u += 16){
				write_reg32(u, 0);
			}
		}
	}
#endif

	extern unsigned int	ota_program_offset;
	extern unsigned int	ota_program_bootAddr;
	extern int	ota_firmware_size_k;
	//boot flag storage
	unsigned char boot_flag = read_reg8(0x63e);
	if (boot_flag)
	{
		boot_flag = (boot_flag & 0x3) | ((ota_firmware_size_k>>2)<<2);
		REG_ADDR8(0x63e) = boot_flag;
		write_reg8(0x40004, 0xA5);   	//mark
		write_reg8(0x40005, boot_flag);

		ota_program_offset = 0;
	}
	else
	{
		ota_program_offset = ota_program_bootAddr;
	}
}

/**
 * @brief   This function serves to initialize MCU
 * @param   none
 * @return  none
 */
_attribute_ram_code_
void cpu_wakeup_init(void)    //must on ramcode
{
	write_reg8(0x60, 0x00);
	write_reg8(0x61, 0x00);
	write_reg8(0x62, 0x00);
	write_reg8(0x63, 0xff);
	write_reg8(0x64, 0xff);
	write_reg8(0x65, 0xff);

	analog_write(0x82, 0x64);  //poweron_dft 0x0c, BIT<6>:clk_24M_tosar_en;  BIT<5>:clk_48M_todig_en;  BIT<3>:pd_dccc_doubler, 0 for power on
	analog_write(0x34, 0x80);
	analog_write(0x0b, 0x38);  //BIT<2> is removed, so 0x38 equals 0x3c



#if MCU_8258_A0
	analog_write(0x0c, 0xc4);  //power on flash power supply, must do this before any flash read when deep back,
#endif						   //so data copy in cstartup.S must be handled,  will do it later

	analog_write(0x8c, 0x02);  //poweron_dft: 0x00,  <1> set 1: reg_xo_en_clk_ana_ana, to enable external 24M crystal
	analog_write(0x02, 0xa2);  //poweron_dft: 0xa4, <2:0> ret_ldo_trim,  set 0x02: 0.8V

	analog_write(0x27, 0x00);
	analog_write(0x28, 0x00);
	analog_write(0x29, 0x00);
	analog_write(0x2a, 0x00);

	write_reg32(0xc40, 0x04040404);
	write_reg32(0xc44, 0x04040404);
	write_reg8(0xc48, 0x04);

	write_reg16(0x750, 8000);    			//set 32KTimer cal Result reg a value


	//set DCDC concerned
	if(read_reg8(0x7d) == 0x01){ //kite A1 ver
		analog_write(0x01, 0x3c);
	}
	else{ // kite A2 or above ver
		analog_write(0x01, 0x4c);
	}
	//analog_0x26:default is 0x00;can be reset by watch dog, chip reset, RESET Pin, power cycle.
	if(blt_miscParam.pad32k_en && (analog_read(0x26) == 0x00)){//power on & reboot
		analog_write(SYS_DEEP_ANA_REG,(analog_read(SYS_DEEP_ANA_REG) &( ~SYS_NEED_REINIT_EXT32K)));
	}

	if( !(analog_read(0x7f) & 0x01) ){
		pmParam.is_deepretn_back = 1;
	}

	if(pmParam.is_deepretn_back){

		pmParam.wakeup_src = analog_read(0x44);
		if ( (analog_read(0x44) & WAKEUP_STATUS_TIMER_PAD ) == WAKEUP_STATUS_PAD)  //pad, no timer
		{
			pmParam.is_pad_wakeup = 1;
		}
		unsigned int deepRet_32k_tick = cpu_get_32k_tick();

		unsigned int deepRet_tick = pm_tim_recover(deepRet_32k_tick);
		reg_system_tick = deepRet_tick;

		REG_ADDR8(0x74c) = 0x00;
		REG_ADDR8(0x74c) = 0x92;
		REG_ADDR8(0x74f) = BIT(0);    //enable system tick
	}
	else
	{
		REG_ADDR8(0x74f) = BIT(0);    //enable system tick
		cpu_wakeup_no_deepretn_back_init(); // to save ramcode
	}


	//core_c20/c21 power on default all enable, so we disable them first,  then if use, enable them by manual
	//note that: PWM/RF Tx/RF Rx/AES code/AES dcode dma may be affected by this, you must handle them when initialization
	reg_dma_chn_en = 0;
	reg_dma_chn_irq_msk = 0;
	reg_gpio_wakeup_irq |= (FLD_GPIO_CORE_WAKEUP_EN | FLD_GPIO_CORE_INTERRUPT_EN);
}







#if PM_LONG_SLEEP_WAKEUP_EN
//definitions for U64 operations
typedef struct {            /*The result of the multiplication with 64 bits.*/
    unsigned int High32;    /*The high 32 bits.*/
    unsigned int Low32;     /*The low 32 bits.*/
} U64_VALUE_t;

/** Get the high word and low word of a variable with 32 bits.*/
#define LOWORD(p)           ((p) & 0x0000FFFFU)
#define HIWORD(p)           (((p) >> 16) & 0x0000FFFFU)

#define MAXVALUEOF32BITS    (0xFFFFFFFFU)

/**
* @brief Calculate the multiplication between two factors,
*        and one factor is the high 32 bit of one 64 bit unsigned interger and the other factor is 32 bit unsigned interger.
* @param Factor64H           [IN]The high 32 bit of one 64 bit unsigned interger.
*                               - Type: unsigned int  \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Factor32            [IN]A 32 bit unsigned interger.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Result          [OUT]A pointer to the production between the two factors.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the multiplication.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_Cal64Multi32_H(
    const unsigned int Factor64H,                /*The high 32 bit of one 64 bit unsigned interger.*/
    const unsigned int Factor32,               /*A 32 bit unsigned interger.*/
    U64_VALUE_t * const Result)       /*A pointer to the production between the two factors.*/

{
    unsigned char IsOverFlow = 0;   /*The overflow state of the product.*/
    if (0 != Result) {    /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;

        if ((0U == Factor64H) || (0U == Factor32)) {/*Check whether one of the multiplier factor is zero.*/
            /*Set the product to zero when one of the multiplier factor is zero.*/
            Result->High32 = 0U;
            Result->Low32 = 0U;
        }
        else {
            if ((Factor64H * Factor32) / Factor32 == Factor64H) {/*Check whether the product is not overflow.*/
                /*Calculate the product when the product is not overflow*/
                Result->High32 = 0U;
                Result->Low32 = Factor64H * Factor32;
            }
            else {
                /*Set the overflow flag of the product and print the overflow message for debug.*/
                IsOverFlow = 1;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the multiplication between two factors,
*        and one factor is the low 32 bit of one 64 bit unsigned interger and the other factor is 32 bit unsigned interger.
* @param Factor64L             [IN]The low 32 bit of one 64 bit unsigned interger
*                               - Type: unsigned int  \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Factor32               [IN]A 32 bit unsigned interger.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Result          [OUT]A pointer to the production between the two factors.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return none
*/
__attribute__((section(".ram_code"))) static void TN_Cal64Multi32_L(
    const unsigned int Factor64L,            /*The low 32 bit of one 64 bit unsigned interger.*/
    const unsigned int Factor32,              /*An 32 bit unsigned interger.*/
    U64_VALUE_t * const Result)   /*A pointer to the production between the two factors.*/

    {
    unsigned int  A0    = 0U;   /*The low 16 bits of the multiplier factor with 64 bits.*/
    unsigned int  A1    = 0U;   /*The 16~31 bits of the multiplier factor with 64 bits.*/
    unsigned int  B0    = 0U;   /*The low 16 bits of the multiplier factor with 32 bits.*/
    unsigned int  B1    = 0U;   /*The 16~31 bits of the multiplier factor with 32 bits.*/
    unsigned int  A0B0  = 0U;   /*The product between A0 and B0.*/
    unsigned int  A1B0  = 0U;   /*The product between A1 and B0.*/
    unsigned int  A0B1  = 0U;   /*The product between A0 and B1.*/
    unsigned int  A1B1  = 0U;   /*The product between A1 and B1.*/
    if (0 != Result)   /*Check the output buffer.*/
    {
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if((0U == Factor64L) || (0U == Factor32)) /*Check whether one of the multiplier factor with low 32 bits is zero.*/
        {
            /*Set the product to zero when one of the multiplier factor with 32 bits is zero.*/
            Result->High32 = 0U;
            Result->Low32 = 0U;
        }
        else
        {
            if((Factor32 * Factor64L )/Factor32   ==   Factor64L )/*Check whether the product between two multiplier factor with 32 bits is not oveeflow.*/
            {
                /*Calculate the product when the product is not overflow.*/
                Result->High32 = 0U;
                Result->Low32 = Factor64L  * Factor32;
            }
            else
            {
                /*Calculate the product when the product between two multiplier factor with 32 bits is not overflow.*/
                A0 = LOWORD( Factor64L );    /*Get the low 16 bits of the multiplier factor with 64 bits.*/
                A1 = HIWORD( Factor64L );    /*Get the 16~31 bits of the multiplier factor with 64 bits.*/
                B0 = LOWORD( Factor32 );       /*Get the low 16 bits of the multiplier factor with 32 bits.*/
                B1 = HIWORD( Factor32 );       /*Get the 16~31 bits of the multiplier factor with 32 bits.*/
                /*Calculate the product high 32 bit.*/
                /* Factor64L * Factor32 = (A0+A1*2^16)*(B0+B1*2^16) = A0*B0 + (A0*B1+A1*B0)*2^16 + A1*B1*2^32 */
                A0B0 = A0 * B0;
                A1B0 = A1 * B0 + HIWORD( A0B0 );
                A0B1 = A0 * B1 + LOWORD( A1B0 );
                A1B1 = A1 * B1 + HIWORD( A1B0 ) + HIWORD( A0B1 );
                /*Calculate the product low 32 bit.*/
                Result->High32 = A1B1;
                Result->Low32 = Factor64L * Factor32;
            }
        }
    }

}

/**
* @brief   Calculate the addition between the two 64 bit factors.
* @param Factor32             [IN]A pointer to one factor of the addition.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Factor32             [IN]A pointer to another factor of the addition.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param Result              [OUT]A pointer to the result of the addition.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the addition.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_Cal64Add(
    const U64_VALUE_t * const Factor1,    /*One factor of the addition.*/
    const U64_VALUE_t * const Factor2,    /*Another factor of the addition.*/
    U64_VALUE_t * const Result)           /*The result of the addition.*/

{
    unsigned char IsOverFlow = 0;   /*The overflow state of the product.*/
    if (0 != Result) {  /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if ((0 != Factor1) && (0 != Factor2)) {  /*Check the input buffer.*/
            if( (Factor1->Low32) > ((unsigned int)(0xFFFFFFFFu) - (Factor2->High32)) ) { /*Check whether the high 32 bits of the product is overflow.*/
                /*Set the overflow flag of the product and print the overflow message for debug.*/
                IsOverFlow = 1;
            }
            else {
                /*Calculate the final product and reset the overflow state.*/
                Result->High32 = Factor2->High32 + Factor1->Low32;
                Result->Low32 = Factor2->Low32;
                IsOverFlow = 0;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the multiplication between two factors, and one factor is 64 bit unsigned interger while the other factor 32 bit.
* @param U64Factor           [IN]One of the multiplier factor with 64 bits.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param U32Factor           [IN]One of the multiplier factor with 32 bits.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF]\n
* @param Result              [OUT]A pointer to the multiplication result with 64 bits.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the multiplication.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_UINT64Multiply(
    const U64_VALUE_t * const U64Factor,      /*One of the multiplier factor with 64 bits.*/
    const unsigned int U32Factor,             /*One of the multiplier factor with 32 bits.*/
    U64_VALUE_t * const Result)               /*The multiplication result with 64 bits.*/

{
    unsigned char IsOverFlow = 0;       /*The overflow state of the U64Factor.*/
    U64_VALUE_t Product64H_32 = {0};    /*The U64Factor between the high 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
    U64_VALUE_t Product64L_32 = {0};    /*The U64Factor between the low 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
    /*Initiate the output buffer.*/
    if (0 != Result) { /*Check the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if (0 != U64Factor) {  /*Check the input buffer.*/
            if ((0U == U32Factor)||((0U == U64Factor->High32) && (0U == U64Factor->Low32))) { /*Check whether one of the multiplier factor is zero.*/
                /*The result is zero when one of the multiplier factor is zero.*/
                Result->High32 = 0U;
                Result->Low32 = 0U;
                IsOverFlow = 0;
            }
            else {
                /*Calculate the multiplication between the high 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
                IsOverFlow = TN_Cal64Multi32_H(U64Factor->High32,U32Factor,&Product64H_32);
                if (0 == IsOverFlow) {/*Check whether the multiplication between the high 32 bits of the 64 bits multiplier factor and the 32 bits multiplier factor is not overflow.*/
                    /*Calculate multiplication between the low 32 bits of the two multiplier factors.*/
                    TN_Cal64Multi32_L(U64Factor->Low32,U32Factor,&Product64L_32);
                    /*Calculate the addition between the two multiplication results calculating by the upper two functions.*/
                    IsOverFlow = TN_Cal64Add(&Product64H_32,&Product64L_32,Result);
                }
            }
            /*The U64Factor is zero when it is overflow.*/
            if(1 == IsOverFlow) {
                Result->High32 = 0U;
                Result->Low32 = 0U;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the remainder with 64 bits.
* @param MulRes             [IN]The multiplication result.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Dividend           [IN]The dividend.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
*
* @return The remainder
*                               - Type: U64_VALUE_t \n
*                               - Range: N/A.\n
*
*/
__attribute__((section(".ram_code"))) static U64_VALUE_t TN_CalRemaider(const U64_VALUE_t MulRes,const U64_VALUE_t Dividend)
{
    U64_VALUE_t Remaider = {0};                  /*The division remainder with 64 bit.*/
    if( MulRes.Low32 <= Dividend.Low32)/*Do not need to carry calculation*/
    {
        Remaider.Low32 = Dividend.Low32 - MulRes.Low32;
        Remaider.High32 = Dividend.High32 - MulRes.High32;
    }
    else    /*Need to carry calculation*/
    {
        Remaider.Low32 = MAXVALUEOF32BITS - MulRes.Low32 + 1U + Dividend.Low32;
        Remaider.High32 = Dividend.High32 - MulRes.High32 - 1U;
    }
    return Remaider;
}

/**
* @brief Two numbers with 64 bit are divided by 2 respectively until the high 32 bit of the second number is zero.
* @param Number1             [IN]A pointer to the first number with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Number2             [IN]A pointer to the second number with 64 bit.
*                               - Type: unsigned char *\n
*                               - Range: N/A.\n
* @param Number1Div          [OUT]A pointer to the first divided number which has been divided by 2.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Number2Div          [OUT]A pointer to the second number which has been divided by 2.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return none
*
*
*/
__attribute__((section(".ram_code"))) static void TN_Div2NumBy2(
    const U64_VALUE_t * const Number1,    /*A pointer to the first number with 64 bit.*/
    const U64_VALUE_t * const Number2,    /*A pointer to the second number with 64 bit.*/
    U64_VALUE_t * const Number1Div,       /*A pointer to the first number which has been divided by 2.*/
    U64_VALUE_t * const Number2Div)       /*A pointer to the second number which has been divided by 2.*/
{
    if ((0 != Number1Div) && (0 != Number2Div) && (0 != Number1) && (0 != Number2)) {/*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Number1Div->Low32 = Number1->Low32;
        Number1Div->High32 = Number1->High32;
        Number2Div->Low32 = Number2->Low32;
        Number2Div->High32 = Number2->High32;
        while (0U != Number2Div->High32) {
                /*The high 32 bit and low 32 bit of the first number are divided by 2.*/
                Number1Div->Low32 >>= 1U;
                if ((Number1Div->High32 % 2U) > 0U) {
                    Number1Div->Low32 += 0x80000000U;
                }
                Number1Div->High32 >>= 1U;
                /*The high 32 bit and low 32 bit of the second number are divided by 2.*/
                Number2Div->Low32 >>= 1U;
                if((Number2Div->High32 %2U) > 0U) {
                    Number2Div->Low32 += 0x80000000U;
                }
                Number2Div->High32 >>= 1U;
        }
    }
}

/**
* @brief Calculate the quotient between the 64 bit dividend and 32 bit divisor. The value of the dividend and divisor are treated as unsigned interger.
*        The division used in this function is the standard algorithm used for pen-and-paper division of multidigit numbers expressed in decimal notation.
* @param U64Dividend         [IN]The dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param U32Divisor          [IN]The divisor with 32 bit.
*                               - Type: unsigned int \n
*                               - Range: N/A.\n
*
* @return none
*
*/
__attribute__((section(".ram_code"))) static unsigned int TN_CalQuto(
    const U64_VALUE_t * const U64Dividend,    /*The dividend with 64 bit.*/
    const unsigned int U32Divisor)                 /*The divisor with 32 bit.*/
{
    unsigned int i = 0U; /*The index for the FOR loop.*/
    unsigned int l_uFlag=0U;  /*The most significant bit of the dividend which has been shifted.*/
    U64_VALUE_t l_DivdShift={0};   /*The dividend which has been shifted.*/
    unsigned int l_Quto = 0U;         /*The quotient.*/
    if (0 != U64Dividend) {/*Check the input buffer.*/
        l_DivdShift.High32 = U64Dividend->High32;
        l_DivdShift.Low32 = U64Dividend->Low32;
        for (i = 0U; i < 32U; i++) {
            l_uFlag = (unsigned int)l_DivdShift.High32 >> 31U;    /*The 'uFlag' is 0xffffffff when the most significant bit of the 'l_DivdShift.High32' is one, otherwise 0x00000000.*/
            /*Left-shift dividend by 1 bit.*/
            l_DivdShift.High32 = (l_DivdShift.High32 << 1)|(l_DivdShift.Low32 >> 31);
            l_DivdShift.Low32 <<= 1;
            /*Left-shift quotient by 1 bit.*/
            l_Quto = l_Quto<<1;
            if((l_DivdShift.High32 >= U32Divisor) || (l_uFlag >0U)) { /*Check whether the dividend high 32 bit is greater than the divisor low 32 bit after left shifting.*/
                /*Calculate the quotient and remainder between the dividend high 32 bit and divisor low 32 bit.*/
                l_DivdShift.High32 -= U32Divisor;
                l_Quto++;
            }
        }
    }
    return l_Quto;
}

/**
* @brief Check the validation of the function input and output.
* @param i_pDividend            [IN]A pointer to the dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param i_pDivisor             [IN]A pointer to the divisor with 64 bit.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param o_pQuto                [OUT]A pointer to the division quotient with 32 bit.
*                               - Type: unsigned int  *\n
*                               - Range: N/A.\n
* @param ErrorCode           [OUT]A pointer to the division error code.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
* @param DivStatus           [OUT]A pointer to the division successful status.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
*
* @return none
*
*/
__attribute__((section(".ram_code"))) static void TN_CheckUINT64AccurateDiv(
    const U64_VALUE_t * const i_pDividend,               /*A pointer to the dividend with 64 bit.*/
    const U64_VALUE_t * const i_pDivisor,                /*A pointer to the divisor with 64 bit.*/
    unsigned int * const o_pQuto,                             /*A pointer to the division quotient with 32 bit.*/
    unsigned char * const ErrorCode,    /*A pointer to the division error code.*/
    unsigned char * const o_pDivStatus)                     /*A pointer to the division successful status.*/
{
    if ((0 != o_pQuto) && (0 != ErrorCode) && (0 != o_pDivStatus)) { /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        *ErrorCode = 0;
        *o_pQuto = 0U;
        *o_pDivStatus = 1;
        if ((0 == i_pDividend) || (0 == i_pDivisor)) { /*Check the input buffer.*/
            *ErrorCode = 0xff;
            *o_pQuto = 0U;
            *o_pDivStatus = 0;
        }
    }
}

/**
* @brief Calculate the 64 bit accurate division, and the quotient is rounded.
* @param i_pDividend            [IN]A pointer to the dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param i_pDivisor             [IN]A pointer to the divisor with 64 bit.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param o_pQuto                [OUT]A pointer to the division quotient with 32 bit.
*                               - Type: unsigned int  *\n
*                               - Range: N/A.\n
* @param ErrorCode           [OUT]A pointer to the division error code.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
* @param o_pDivStatus           [OUT]A pointer to the division successful status.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
*
* @return N/A
*
*/
__attribute__((section(".ram_code"))) static void TN_U64AccurateDiv(
    const U64_VALUE_t * const Dividend,               /*A pointer to the dividend with 64 bit.*/
    const U64_VALUE_t * const Divisor,                /*A pointer to the divisor with 64 bit.*/
    unsigned int * const Quto,
    U64_VALUE_t * const Remainder,                            /*A pointer to the division quotient with 32 bit.*/
    unsigned char * const ErrorCode,    /*A pointer to the division error code.*/
    unsigned char * const DivStatus)                      /*A pointer to the division successful status.*/
{
    U64_VALUE_t l_DividendDiv2 = {0};        /*The temporary buffer.*/
    U64_VALUE_t l_DivisorDiv2 = {0};         /*The temporary buffer.*/
    unsigned int l_QutoTemp = 0U;             /*The quotient has not been rounded.*/
    TN_CheckUINT64AccurateDiv(Dividend, Divisor, Quto, ErrorCode, DivStatus);
    if (0 == *ErrorCode) {
        if ((0U == Divisor->High32) && (0U == Divisor->Low32)) {/*Check whether the divisor is zero.*/
            /*The division result is zero and division status is not successful when the divisor is zero.*/
            *Quto = 0;
            *ErrorCode  = 1;
            *DivStatus =  0;
        }
        else {
            if ((0U == Divisor->High32) && (Dividend->High32 >= Divisor->Low32)) {/*Check whether the dividend high 32 bit is greater than the divisor low 32 bit.*/
                /*The division result is overflow when the dividend high 32 bit is greater than the divisor low 32 bit.*/
                *Quto = 0;
                *ErrorCode = 2;
                *DivStatus =  0;
            }
            else {
                if ((0U == Divisor->High32) && (0U == Dividend->High32)) { /*Check whether the high 32 bit of the divisor and dividend is zero.*/
                    /*Get the division result directly the high 32 bit of the divisor and dividend is zero.*/
                    l_QutoTemp = Dividend->Low32 / Divisor->Low32;
                }
                else {
                    /*The dividend and divisor are divided by 2 respectively until the high 32 bit of the divisor is zero.*/
                    TN_Div2NumBy2(Dividend,Divisor,& l_DividendDiv2,& l_DivisorDiv2);
                    /*Calculate the quotient and remainder of the 64 bit division by subtracting with borrow.*/
                    l_QutoTemp = TN_CalQuto(&l_DividendDiv2,l_DivisorDiv2.Low32);
                }

                U64_VALUE_t U64MulRes = {0};
                TN_UINT64Multiply(Divisor, l_QutoTemp, &U64MulRes);/*Calculate the multiplication result between the 'Divisor' and 'QutoNotRound'.*/
                /*Calculate the remainder.*/
                *Remainder = TN_CalRemaider(U64MulRes, *Dividend);
                *Quto = l_QutoTemp;
            }
        }
    }
}




/**
 * @brief      This function serves to set the working mode of MCU,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  SleepDurationUs - the time of long sleep, which means MCU can sleep for up to one hour.
 * @return     indicate whether the cpu is wake up successful.
 */
_attribute_ram_code_ int pm_long_sleep_wakeup (SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int SleepDurationUs)
{
	unsigned int   t0 = clock_time();
	unsigned int   tl_tick_cur;
	unsigned int   tl_tick_32k_cur;
	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);
	//NOTICE:We think that the external 32k crystal clock is very accurate, does not need to read 2 bytes through reg_750
	//the conversion offset is less than 64ppm
#if (!CLK_32K_XTAL_EN)
	unsigned short  tl_tick_32k_calib = REG_ADDR16(0x750);
	unsigned short  tick_32k_halfCalib = tl_tick_32k_calib>>1;
#endif

	unsigned int span = SleepDurationUs * SYSTEM_TIMER_TICK_1US ;


	if(timer_wakeup_enable){
		if (span < EMPTYRUN_TIME_US * SYSTEM_TIMER_TICK_1US) // 0 us base
		{
			analog_write (0x44, 0x0f);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x0f;
			} while ( ((unsigned int)clock_time () - t0 < span) && !st);
			return st;
		}
	}


	////////// disable IRQ //////////////////////////////////////////
	unsigned char r = irq_disable ();

	//set 32k wakeup tick
	U64_VALUE_t TmpVal[4] = {
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0}
	};
	unsigned char ErrorCode = 0;
	unsigned char DivStatus = 0;
	//1. calculate the absolute System wakeup tick
	TmpVal[0].Low32 = SleepDurationUs;
	TN_UINT64Multiply(&TmpVal[0], SYSTEM_TIMER_TICK_1US, &TmpVal[1]);
	TmpVal[2].Low32 = t0;
	TN_Cal64Add(&TmpVal[1], &TmpVal[2], &TmpVal[3]);
	unsigned int wakeup_tick = TmpVal[3].Low32;
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TmpVal[1].Low32 = 0;
	TmpVal[1].High32 = 0;
	TmpVal[2].Low32 = 0;
	TmpVal[2].High32 = 0;
	TmpVal[3].Low32 = 0;
	TmpVal[3].High32 = 0;

	//2. calculate and set the 32K timer wakeup tick
#if(!CLK_32K_XTAL_EN)
	TmpVal[2].Low32 = tl_tick_32k_calib;
#else
	TmpVal[2].Low32 = TICK_16M_PER_32CYC_XTAL32P768K;
#endif
	tl_tick_cur = clock_time ();
	tl_tick_32k_cur = cpu_get_32k_tick ();
    unsigned int M = 0;
    unsigned int m = 0;
	unsigned int EARLYWAKEUP_TIME_US = EARLYWAKEUP_TIME_US_SUSPEND;
	if(sleep_mode){
		EARLYWAKEUP_TIME_US = EARLYWAKEUP_TIME_US_DEEP;
	}
	TmpVal[0].Low32 = SleepDurationUs - (((unsigned int)(tl_tick_cur - t0)) / SYSTEM_TIMER_TICK_1US) - EARLYWAKEUP_TIME_US;
	TN_UINT64Multiply(&TmpVal[0], SYSTEM_TIMER_TICK_1US, &TmpVal[1]);
	TN_U64AccurateDiv(&TmpVal[1], &TmpVal[2], &M, &TmpVal[3], &ErrorCode, &DivStatus);
#if(!CLK_32K_XTAL_EN)
	m = M*16 + (TmpVal[3].Low32)/(tl_tick_32k_calib/16);
#else
	m = M*32 + (TmpVal[3].Low32)/(TICK_16M_PER_32CYC_XTAL32P768K/32);
#endif


	/////////////////// set wakeup source /////////////////////////////////
	analog_write (0x26, wakeup_src);   //@@@@@@@ [3] wakeup pad sel,1: 16usfilter (need 32k is on) 0: non-filter
	write_reg8(0x6e, 0x00);

	analog_write (0x44, 0x0f);				//clear all flag

#if(SYS_CLK_48MRC_EN)
	///////////////////////// change to 24M RC clock before suspend /////////////
	unsigned char ana_04 = analog_read(0x04);
	analog_write(0x04, ana_04&0xfc);
	unsigned char ana_33 = analog_read(0x33);
	analog_write(0x33, tl_24mrc_cal);
#endif
	unsigned char reg66 = read_reg8(0x066);			//
	write_reg8 (0x066, 0);				//change to 24M rc clock

	unsigned char bit567_ana2c;
	unsigned char bit012_ana07;

	if(sleep_mode & 0x7f) { //deepsleep with retention
		bit567_ana2c = 0x40;  //ana_2c<7:5>: 010
		bit012_ana07 = 0x01;  //ana_07<2:0>:001
		analog_write(0x02, (analog_read(0x02) & 0xf8) | 0x05);  //ana_02<2:0>:010-> 0.8v ret ldo
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

	unsigned char shutdown = sleep_mode + 1;
	analog_write(0x2b, (sleep_mode ? 0xde : 0x5e)|((shutdown<sleep_mode)? 1:0));// turn off soft start switch
#if (!CLK_32K_XTAL_EN)
		analog_write(0x2c, bit567_ana2c | 0x1e | (!timer_wakeup_enable));
#else
		analog_write(0x2c, bit567_ana2c | 0x1d | ((!timer_wakeup_enable)<<1));
#endif
	analog_write(0x07, (analog_read(0x07) & 0xf8) | bit012_ana07);

	if(sleep_mode & 0x7f ) {
		analog_write(0x7f, 0x00);
	}
	else{
		write_reg8(0x602,0x08);
		analog_write(0x7f, 0x01);
	}

#if(!CLK_32K_XTAL_EN)
	//set xtal delay duration
	span = (PM_XTAL_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 16 + tick_32k_halfCalib)/ tl_tick_32k_calib;
	unsigned char rst_cycle = 0x7f - span;
	analog_write (0x20, rst_cycle);

	//set DCDC delay duration
    span = (PM_DCDC_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 16 + tick_32k_halfCalib)/ tl_tick_32k_calib;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);
#else
	//set xtal delay duration
	span = (PM_XTAL_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 32 + (TICK_16M_PER_32CYC_XTAL32P768K>>1))/ TICK_16M_PER_32CYC_XTAL32P768K;
	unsigned char rst_cycle = 0x7f - span;
	analog_write (0x20, rst_cycle);

	//set DCDC delay duration
    span = (PM_DCDC_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 32 + (TICK_16M_PER_32CYC_XTAL32P768K>>1))/ TICK_16M_PER_32CYC_XTAL32P768K;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);
#endif

	REG_ADDR8(0x74c) = 0x2c;
	REG_ADDR32(0x754) = tl_tick_32k_cur + m;
	REG_ADDR8(0x74f) = BIT(3);									//start write
	//16 tnop is enough
	CLOCK_DLY_8_CYC;
	CLOCK_DLY_8_CYC;
	while (REG_ADDR8(0x74f) & BIT(3));

	write_reg8(0x74c, 0x20);
	if(analog_read(0x44)&0x0f){

	}
	else{
		sleep_start();
	}

	/* long press pad to wake up from deep */
	if(sleep_mode == DEEPSLEEP_MODE){
		write_reg8 (0x6f, 0x20);  //reboot
	}
	else if(sleep_mode&0x7f)
	{
		write_reg8 (0x602, 0x88);  //mcu run from the beginning
	}

  //recover system clock from 32k clock
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TmpVal[1].Low32 = 0;
	TmpVal[1].High32 = 0;
	TmpVal[2].Low32 = 0;
	TmpVal[2].High32 = 0;
	TmpVal[3].Low32 = 0;
	TmpVal[3].High32 = 0;
	span = (unsigned int)(cpu_get_32k_tick () - tl_tick_32k_cur);
#if(!CLK_32K_XTAL_EN)
	M = span / 16;
	m = span % 16;
	TmpVal[0].Low32 = M;
	TN_UINT64Multiply(&TmpVal[0], tl_tick_32k_calib, &TmpVal[1]);
	TmpVal[0].Low32 = m*tl_tick_32k_calib/16;
#else
	M = span / 32;
	m = span % 32;
	TmpVal[0].Low32 = M;
	TN_UINT64Multiply(&TmpVal[0], TICK_16M_PER_32CYC_XTAL32P768K, &TmpVal[1]);
	TmpVal[0].Low32 = m*TICK_16M_PER_32CYC_XTAL32P768K/32;
#endif
	TN_Cal64Add(&TmpVal[1], &TmpVal[0], &TmpVal[2]);
	TmpVal[3].Low32 = tl_tick_cur;
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TN_Cal64Add(&TmpVal[2], &TmpVal[3], &TmpVal[0]);
	tl_tick_cur = TmpVal[0].Low32;

	reg_system_tick = tl_tick_cur + 20*SYSTEM_TIMER_TICK_1US;  //20us compensation
	REG_ADDR8(0x74c) = 0x00;
	//caution: at least 5~6 cycle confirmed by guangjun: system timer clock domain(16M) and system clock(24M) is inconsistent,
	//set the system timer related registers need clock synchronization, to ensure correct writing, otherwise there is a risk of crash.
	CLOCK_DLY_6_CYC;
	REG_ADDR8(0x74c) = 0x92;
	//caution: at least 3~4 cycle confirmed by guangjun: system timer clock domain(16M) and system clock(24M) is inconsistent,
	//set the system timer related registers need clock synchronization, to ensure correct writing, otherwise there is a risk of crash.
	CLOCK_DLY_4_CYC;
	REG_ADDR8(0x74f) = BIT(0);

#if(SYS_CLK_48MRC_EN)
	analog_write(0x33, ana_33);
	analog_write(0x04, ana_04);
#endif

	write_reg8 (0x066, reg66);		//restore system clock

	unsigned char anareg44 = analog_read(0x44);
	if(!anareg44){
		anareg44 = STATUS_GPIO_ERR_NO_ENTER_PM;
	}
	else if ( (anareg44 & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )	//wakeup from timer only
	{
		while ((unsigned int)(clock_time () -  wakeup_tick) > BIT(30));
	}

	irq_restore(r);

	return anareg44;
}

#endif


int cpu_long_sleep_wakeup(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick)
{
	int sys_tick0 = clock_time();
	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);

	tick_32k_calib = REG_ADDR16(0x750);

	unsigned short  tick_32k_halfCalib = tick_32k_calib>>1;

	unsigned int span = (unsigned int)(wakeup_tick);


	if(timer_wakeup_enable){
		if (span < PM_EMPTYRUN_TIME_US * tick_32k_tick_per_ms) // 1.5ms
		{
			analog_write (0x44, 0x0f);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x0f;
			} while ( ((unsigned int)clock_time () - sys_tick0 < span) && !st);
			return st;
		}
	}
	pm_long_suspend = 0;


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



	unsigned int earlyWakeup_us = EARLYWAKEUP_TIME_US_SUSPEND;
	if(sleep_mode){
		earlyWakeup_us = EARLYWAKEUP_TIME_US_DEEP;
	}
	unsigned int tick_wakeup_reset = wakeup_tick - ((earlyWakeup_us * 16 + tick_32k_halfCalib) / tick_32k_calib);

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
		tick_reset = tick_32k_cur + (unsigned int)(tick_wakeup_reset - ((tick_cur - sys_tick0)/ tick_32k_calib * 16));
	}
	else{
		tick_reset = tick_32k_cur + (unsigned int)(tick_wakeup_reset - (((tick_cur - sys_tick0) * 16 + tick_32k_halfCalib) / tick_32k_calib));
	}



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
		sleep_start();
	}

	//Just for BLE
	if(sleep_mode == DEEPSLEEP_MODE){
	   soft_reboot_dly13ms_use24mRC();
	   write_reg8 (0x6f, 0x20);  //reboot
	}

	unsigned int now_tick_32k = cpu_get_32k_tick ();

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

	irq_restore(r);

	return (anareg44 ? (anareg44 | STATUS_ENTER_SUSPEND) : STATUS_GPIO_ERR_NO_ENTER_PM );
}

//just for pm test
void shutdown_gpio(void)
{
	//output disable
	reg_gpio_pa_oen = 0xff;
	reg_gpio_pb_oen = 0xff;
	reg_gpio_pc_oen = 0xff;
	reg_gpio_pd_oen = 0xff;

	//dataO = 0
	reg_gpio_pa_out = 0x00;
	reg_gpio_pb_out = 0x00;
	reg_gpio_pc_out = 0x00;
	reg_gpio_pd_out = 0x00;

	//ie = 0
	//SWS   581<7>
	//DM DP  581<6:5>
	reg_gpio_pa_ie = 0x80;
	analog_write(areg_gpio_pb_ie, 0);
	analog_write(areg_gpio_pc_ie, 0);
	reg_gpio_pd_ie = 0x00;
}
