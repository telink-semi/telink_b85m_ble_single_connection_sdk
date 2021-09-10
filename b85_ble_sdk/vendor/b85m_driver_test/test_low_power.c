/********************************************************************************************************
 * @file	test_low_power.c
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
/*
 * test_low_power.c
 *
 *  Created on: 2018-4-17
 *      Author: Administrator
 */
#include "app_config.h"
#include "tl_common.h"
#include "drivers.h"

#if (DRIVER_TEST_MODE == TEST_LOW_POWER)


#define TEST_LP_SUSPEND_NO_WAKEUP						1 //ext32k+suspend+none: 45.09uA
#define TEST_LP_SUSPEND_GPIO_PAD_WAKEUP					2
#define TEST_LP_SUSPEND_TIMER_WAKEUP					3

#define TEST_LP_SUSPEND_TIMER_GPIO_PAD_WAKEUP			4

#define TEST_LP_DEEPSLEEP_NO_WAKEUP						5 //ext32k+deep+none: 0.43uA    //int32k+deep+none: 0.41uA
#define TEST_LP_DEEPSLEEP_GPIO_PAD_WAKEUP				6 //ext32k+deep+pad:  0.43uA    //int32k+deep+pad:  0.41uA
#define TEST_LP_DEEPSLEEP_TIMER_WAKEUP					7 //ext32k+deep+tmr:  2.03uA    //int32k+deep+tmr:  0.92uA

#define TEST_LP_DEEPSLEEP_RET16K_NO_WAKEUP				8 //ext32k+deepret16k+none(tmr should open: 3.00uA    //int32k+deepret16k+none: 1.41uA
#define TEST_LP_DEEPSLEEP_RET16K_GPIO_PAD_WAKEUP		9 //ext32k+deepret16k+pad(tmr should open:  3.00uA    //int32k+deepret16k+pad:  1.41uA
#define TEST_LP_DEEPSLEEP_RET16K_TIMER_WAKEUP			10//ext32k+deepret16k+tmr:  3.00uA                    //int32k+deepret16k+tmr:  1.87uA


#define TEST_LP_MODE									TEST_LP_DEEPSLEEP_GPIO_PAD_WAKEUP



int first_power_on = 0;

u8 AA_wakeupSrc[256];
u8 AA_wkp_index;
/**
 * @brief		this function is used to test low power
 * @param[in]	none
 * @return      none
 */
void test_low_power(void)
{
	if(analog_read(0x3a) == 0x5a){  //read flag
		first_power_on = 0;
	}
	else{
		first_power_on = 1;
	}


	if(first_power_on){
		DBG_CHN0_TOGGLE;
		sleep_us(500000);
		DBG_CHN0_TOGGLE;
		sleep_us(500000);
		DBG_CHN0_TOGGLE;
		sleep_us(500000);
		DBG_CHN0_TOGGLE;
		sleep_us(500000);
	}
	else{
		DBG_CHN0_TOGGLE;
		sleep_us(500000);
		DBG_CHN0_TOGGLE;
		sleep_us(500000);
		DBG_CHN0_TOGGLE;
		sleep_us(500000);
		DBG_CHN0_TOGGLE;
		sleep_us(500000);
	}

	analog_write(0x3a, 0x5a); //set flag



#if(TEST_LP_MODE == TEST_LP_SUSPEND_NO_WAKEUP)

	gpio_shutdown(GPIO_ALL);  //all GPIO high_Z state, no current leakage

	cpu_sleep_wakeup(SUSPEND_MODE , 0, 0);

#elif(TEST_LP_MODE == TEST_LP_SUSPEND_GPIO_PAD_WAKEUP)

	gpio_setup_up_down_resistor(GPIO_PB6, PM_PIN_PULLDOWN_100K);
	cpu_set_gpio_wakeup(GPIO_PB6, Level_High, 1);


	while(1){

		u32 wakeup_src = cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_PAD, 0);  //PM_WAKEUP_PAD

		AA_wakeupSrc[AA_wkp_index++] = wakeup_src;

		DBG_CHN1_TOGGLE;
		sleep_us(300000);
		DBG_CHN1_TOGGLE;
		sleep_us(100000);
		DBG_CHN1_TOGGLE;
		sleep_us(600000);
		DBG_CHN1_TOGGLE;
		sleep_us(100000);
	}

#elif(TEST_LP_MODE == TEST_LP_SUSPEND_TIMER_WAKEUP)


	u32 now_tick;
	while(1){

		now_tick = clock_time();

		u32 wakeup_src = cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER,  now_tick + 20 * CLOCK_16M_SYS_TIMER_CLK_1S);

		AA_wakeupSrc[AA_wkp_index++] = wakeup_src;

		DBG_CHN1_TOGGLE;
		sleep_us(20000);
		DBG_CHN1_TOGGLE;
		sleep_us(10000);
		DBG_CHN1_TOGGLE;
		sleep_us(60000);
		DBG_CHN1_TOGGLE;
		sleep_us(10000);
	}

#elif(TEST_LP_MODE == TEST_LP_SUSPEND_TIMER_GPIO_PAD_WAKEUP)

	gpio_setup_up_down_resistor(GPIO_PB6, PM_PIN_PULLDOWN_100K);
	cpu_set_gpio_wakeup(GPIO_PB6, Level_High, 1);


	u32 now_tick;
	while(1){

		now_tick = clock_time();

		u32 wakeup_src = cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD,  now_tick + 20 * CLOCK_16M_SYS_TIMER_CLK_1S);

		AA_wakeupSrc[AA_wkp_index++] = wakeup_src;

		DBG_CHN1_TOGGLE;
		sleep_us(300000);
		DBG_CHN1_TOGGLE;
		sleep_us(100000);
		DBG_CHN1_TOGGLE;
		sleep_us(600000);
		DBG_CHN1_TOGGLE;
		sleep_us(100000);
	}

#elif(TEST_LP_MODE == TEST_LP_DEEPSLEEP_NO_WAKEUP)

	gpio_shutdown(GPIO_ALL); //all GPIO high_Z state, no current leakage

	cpu_sleep_wakeup(DEEPSLEEP_MODE , 0, 0);

#elif(TEST_LP_MODE == TEST_LP_DEEPSLEEP_GPIO_PAD_WAKEUP)


	gpio_setup_up_down_resistor(GPIO_PB6, PM_PIN_PULLDOWN_100K);
	cpu_set_gpio_wakeup(GPIO_PB6, Level_High, 1);

	while(1){
		cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, 0);  //PM_WAKEUP_PAD
	}


#elif(TEST_LP_MODE == TEST_LP_DEEPSLEEP_TIMER_WAKEUP)

	gpio_shutdown(GPIO_ALL);  //all GPIO high_Z state, no current leakage

	while(1){

		cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_TIMER, clock_time() + 20*CLOCK_16M_SYS_TIMER_CLK_1S);

		DBG_CHN1_TOGGLE;
		sleep_us(700000);
		DBG_CHN1_TOGGLE;
		sleep_us(300000);
		DBG_CHN1_TOGGLE;
		sleep_us(700000);
		DBG_CHN1_TOGGLE;
		sleep_us(300000);
	}

#elif(TEST_LP_MODE == TEST_LP_DEEPSLEEP_RET16K_NO_WAKEUP)

	gpio_shutdown(GPIO_ALL);  //all GPIO high_Z state, no current leakage
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , 0, 0);

#elif(TEST_LP_MODE == TEST_LP_DEEPSLEEP_RET16K_GPIO_PAD_WAKEUP)

	gpio_setup_up_down_resistor(GPIO_PB6, PM_PIN_PULLDOWN_100K);
	cpu_set_gpio_wakeup(GPIO_PB6, Level_High, 1);

	while(1){
		cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K, PM_WAKEUP_PAD, 0);  //PM_WAKEUP_PAD
	}

#elif(TEST_LP_MODE == TEST_LP_DEEPSLEEP_RET16K_TIMER_WAKEUP)

	gpio_shutdown(GPIO_ALL);  //all GPIO high_Z state, no current leakage

	while(1){

		cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_TIMER, clock_time() + 20*CLOCK_16M_SYS_TIMER_CLK_1S);

		DBG_CHN1_TOGGLE;
		sleep_us(700000);
		DBG_CHN1_TOGGLE;
		sleep_us(300000);
		DBG_CHN1_TOGGLE;
		sleep_us(700000);
		DBG_CHN1_TOGGLE;
		sleep_us(300000);
	}
#else


#endif


}


#endif
