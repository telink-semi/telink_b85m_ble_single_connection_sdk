/********************************************************************************************************
 * @file	app_gpio_irq.c
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
 * app_gpio_irq.c
 *
 *  Created on: 2017-8-25
 *      Author: Administrator
 */
#include "app_config.h"
#include "tl_common.h"
#include "drivers.h"


#if (DRIVER_TEST_MODE == TEST_GPIO_IRQ)




//you can change IO here
#define  GPIO_LED1_PIN				GPIO_PA0
#define  GPIO_LED2_PIN				GPIO_PA1
#define  GPIO_LED3_PIN				GPIO_PA2


#define  LED_INIT_LEVEL				0


#define GPIO_TEST_PIN1				GPIO_PD0
#define GPIO_TEST_PIN2				GPIO_PD1
#define GPIO_TEST_PIN3				GPIO_PD2


unsigned char gpio_irq_test_mode = 0;
/**
 * @brief      Initialize the gpio irq test
 * @param[in]  none
 * @return     none
 */
void app_gpio_irq_test_init(void)
{

//demo1: IRQ_GPIO,       TEST_PIN1, falling edge triggers
	/***step1. set pin as gpio and enable input********/
	gpio_set_func(GPIO_TEST_PIN1, AS_GPIO);           //enable GPIO func
	gpio_set_input_en(GPIO_TEST_PIN1, 1);             //enable input
	gpio_set_output_en(GPIO_TEST_PIN1, 0);            //disable output

	/***step2.      set the polarity and open pullup ***/
	gpio_setup_up_down_resistor(GPIO_TEST_PIN1, PM_PIN_PULLUP_10K);  //open pull up resistor
	gpio_set_interrupt_pol(GPIO_TEST_PIN1, pol_falling);    //falling edge

	/***step3.      set irq enable  ***/
	reg_irq_src = FLD_IRQ_GPIO_EN; //clean irq status
	reg_irq_mask |= FLD_IRQ_GPIO_EN;
	gpio_en_interrupt(GPIO_TEST_PIN1, 1);





//demo2: IRQ_GPIO_RISC0,  TEST_PIN2, falling edge triggers
	/***step1. set pin as gpio and enable input********/
	gpio_set_func(GPIO_TEST_PIN2, AS_GPIO);           //enable GPIO func
	gpio_set_input_en(GPIO_TEST_PIN2, 1);             //enable input
	gpio_set_output_en(GPIO_TEST_PIN2, 0);            //disable output

	/***step2.      set the polarity and open pullup ***/
	gpio_setup_up_down_resistor(GPIO_TEST_PIN2, PM_PIN_PULLUP_10K);  //open pull up resistor
	gpio_set_interrupt_pol(GPIO_TEST_PIN2, pol_falling);    //falling edge

	/***step3.      set irq enable  ***/
	reg_irq_src = FLD_IRQ_GPIO_RISC0_EN; //clean irq status
	reg_irq_mask |= FLD_IRQ_GPIO_RISC0_EN;
	gpio_en_interrupt_risc0(GPIO_TEST_PIN2, 1);




//demo3: IRQ_GPIO_RISC1,    TEST_PIN3, rising edge triggers
	/***step1. set pin as gpio and enable input********/
	gpio_set_func(GPIO_TEST_PIN3, AS_GPIO);           //enable GPIO func
	gpio_set_input_en(GPIO_TEST_PIN3, 1);             //enable input
	gpio_set_output_en(GPIO_TEST_PIN3, 0);            //disable output

	/***step2.      set the polarity and open pullup ***/
	gpio_setup_up_down_resistor(GPIO_TEST_PIN3, PM_PIN_PULLDOWN_100K);  //open pull down resistor
	gpio_set_interrupt_pol(GPIO_TEST_PIN3, pol_rising);    //rising edge

	/***step3.      set irq enable  ***/
	reg_irq_src = FLD_IRQ_GPIO_RISC1_EN; //clean irq status
	reg_irq_mask |= FLD_IRQ_GPIO_RISC1_EN;
	gpio_en_interrupt_risc1(GPIO_TEST_PIN3, 1);







	irq_enable();  //must

}


/**
 * @brief		Initialization of LED pin for indication
 * @param[in]	none
 * @return      none
 */
void app_led_init(void)
{

	gpio_set_func(GPIO_LED1_PIN, AS_GPIO);
	gpio_set_output_en(GPIO_LED1_PIN, 1);//enable output
	gpio_set_input_en(GPIO_LED1_PIN, 0); //disable input
	gpio_write(GPIO_LED1_PIN, LED_INIT_LEVEL);

	gpio_set_func(GPIO_LED2_PIN, AS_GPIO);
	gpio_set_output_en(GPIO_LED2_PIN, 1);//enable output
	gpio_set_input_en(GPIO_LED2_PIN, 0); //disable input
	gpio_write(GPIO_LED2_PIN, LED_INIT_LEVEL);

	gpio_set_func(GPIO_LED3_PIN, AS_GPIO);
	gpio_set_output_en(GPIO_LED3_PIN, 1);//enable output
	gpio_set_input_en(GPIO_LED3_PIN, 0); //disable input
	gpio_write(GPIO_LED3_PIN, LED_INIT_LEVEL);

}


int gpio_all_irq_cnt;
int gpio_irq_cnt;
int gpio_irq_risr0_cnt;
int gpio_irq_risr1_cnt;
/**
 * @brief		this function is used to process apio irq
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void app_gpio_irq_test_proc(void)
{
	gpio_all_irq_cnt ++;


	/************ gpio irq  ***************/
	if(reg_irq_src & FLD_IRQ_GPIO_EN){
		reg_irq_src = FLD_IRQ_GPIO_EN;        // clear irq_gpio irq flag

		gpio_irq_cnt ++;
		DBG_CHN0_TOGGLE;
		gpio_toggle(GPIO_LED1_PIN);
	}


	/************* gpio irq risc0 *************/
	if(reg_irq_src & FLD_IRQ_GPIO_RISC0_EN){
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;        // clear irq_gpio irq flag


		gpio_irq_risr0_cnt ++;
		DBG_CHN1_TOGGLE;
		gpio_toggle(GPIO_LED2_PIN);
	}


	/************* gpio irq risc1 *************/
	if(reg_irq_src & FLD_IRQ_GPIO_RISC1_EN){
		reg_irq_src = FLD_IRQ_GPIO_RISC1_EN;        // clear irq_gpio irq flag

		gpio_irq_risr1_cnt ++;
		DBG_CHN2_TOGGLE;
		gpio_toggle(GPIO_LED3_PIN);
	}


}



#endif
