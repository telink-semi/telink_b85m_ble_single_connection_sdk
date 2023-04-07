/********************************************************************************************************
 * @file	app_config.h
 *
 * @brief	This is the header file for BLE SDK
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
#pragma once

#define		BQB_5P0_TEST_ENABLE									0


/////////////////////HCI ACCESS OPTIONS/////////////////////
#define HCI_USE_UART	1
#define HCI_USE_USB		0
#define HCI_ACCESS		HCI_USE_UART



/////////////////// MODULE /////////////////////////////////
#define BLE_MODULE_PM_ENABLE				0

/////////////////// DEBUG  /////////////////////////////////
//826x module's pin simulate as a uart tx, Just for debugging
#define PRINT_DEBUG_INFO               		0	//open/close myprintf





/////////////////// led pin /////////////////////////////////
#define LED_ON_LEVAL 						1 			//gpio output high voltage to turn on led
#define	GPIO_LED							GPIO_PC6
#define PC6_FUNC							AS_GPIO

#define ON            						1
#define OFF           						0

#define BOARD_825X_EVK_C1T139A30			1     //TLSR8258DK48
#define BOARD_827X_EVK_C1T197A30			2	  //TLSR8278DK48

#if (__PROJECT_8258_HCI__)
	/* can only choose BOARD_825X_EVK_C1T139A30,*/
	#define BOARD_SELECT							BOARD_825X_EVK_C1T139A30
#elif (__PROJECT_8278_HCI__)
	/* can only choose BOARD_827X_EVK_C1T197A30,*/
	#define BOARD_SELECT							BOARD_827X_EVK_C1T197A30
#endif

#if (BOARD_SELECT == BOARD_825X_EVK_C1T139A30)
	//////////////////////////// MODULE PM GPIO	/////////////////////////////////
	#define GPIO_WAKEUP_MODULE					GPIO_PC3   //mcu wakeup module
	#define	PC3_FUNC							AS_GPIO
	#define PC3_INPUT_ENABLE					1
	#define	PC3_OUTPUT_ENABLE					0
	#define	PC3_DATA_OUT						0
	#define GPIO_WAKEUP_MODULE_HIGH				gpio_setup_up_down_resistor(GPIO_WAKEUP_MODULE, PM_PIN_PULLUP_10K);
	#define GPIO_WAKEUP_MODULE_LOW				gpio_setup_up_down_resistor(GPIO_WAKEUP_MODULE, PM_PIN_PULLDOWN_100K);

	#define GPIO_WAKEUP_MCU						GPIO_PD0   //module wakeup mcu
	#define	PD0_FUNC							AS_GPIO
	#define PD0_INPUT_ENABLE					1
	#define	PD0_OUTPUT_ENABLE					1
	#define	PD0_DATA_OUT						0
	#define GPIO_WAKEUP_MCU_HIGH				do{gpio_set_output_en(GPIO_WAKEUP_MCU, 1); gpio_write(GPIO_WAKEUP_MCU, 1);}while(0)
	#define GPIO_WAKEUP_MCU_LOW					do{gpio_set_output_en(GPIO_WAKEUP_MCU, 1); gpio_write(GPIO_WAKEUP_MCU, 0);}while(0)
	#define GPIO_WAKEUP_MCU_FLOAT				do{gpio_set_output_en(GPIO_WAKEUP_MCU, 0); gpio_write(GPIO_WAKEUP_MCU, 0);}while(0)

#elif (BOARD_SELECT == BOARD_827X_EVK_C1T197A30)
	//////////////////////////// MODULE PM GPIO	/////////////////////////////////
	#define GPIO_WAKEUP_MODULE					GPIO_PC3   //mcu wakeup module
	#define	PC3_FUNC							AS_GPIO
	#define PC3_INPUT_ENABLE					1
	#define	PC3_OUTPUT_ENABLE					0
	#define	PC3_DATA_OUT						0
	#define GPIO_WAKEUP_MODULE_HIGH				gpio_setup_up_down_resistor(GPIO_WAKEUP_MODULE, PM_PIN_PULLUP_10K);
	#define GPIO_WAKEUP_MODULE_LOW				gpio_setup_up_down_resistor(GPIO_WAKEUP_MODULE, PM_PIN_PULLDOWN_100K);

	#define GPIO_WAKEUP_MCU						GPIO_PA2   //module wakeup mcu
	#define	PA2_FUNC							AS_GPIO
	#define PA2_INPUT_ENABLE					1
	#define	PA2_OUTPUT_ENABLE					1
	#define	PA2_DATA_OUT						0
	#define GPIO_WAKEUP_MCU_HIGH				do{gpio_set_output_en(GPIO_WAKEUP_MCU, 1); gpio_write(GPIO_WAKEUP_MCU, 1);}while(0)
	#define GPIO_WAKEUP_MCU_LOW					do{gpio_set_output_en(GPIO_WAKEUP_MCU, 1); gpio_write(GPIO_WAKEUP_MCU, 0);}while(0)
	#define GPIO_WAKEUP_MCU_FLOAT				do{gpio_set_output_en(GPIO_WAKEUP_MCU, 0); gpio_write(GPIO_WAKEUP_MCU, 0);}while(0)
#endif





///////////////////////// System Clock  Configuration /////////////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  								16000000

#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	#define SYS_CLK_TYPE  									SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	#define SYS_CLK_TYPE  									SYS_CLK_24M_Crystal
#elif(CLOCK_SYS_CLOCK_HZ == 32000000)
	#define SYS_CLK_TYPE  									SYS_CLK_32M_Crystal
#elif(CLOCK_SYS_CLOCK_HZ == 48000000)
	#define SYS_CLK_TYPE  									SYS_CLK_48M_Crystal
#else
	#error "unsupported system clock !"
#endif

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};



/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		500  //ms





/////////////////////HCI UART variables///////////////////////////////////////
//module spp Tx / Rx fifo
/*----------------------------------------------*
 *	HCI TX FIFO  = 2 Bytes LEN + n Bytes Data.	*
 *												*
 *	T_txdata_buf = 4 Bytes LEN + n Bytes Data.	*
 *												*
 *	HCI_TXFIFO_SIZE = 2 + n.					*
 *												*
 *	UART_DATA_LEN = n.							*
 *												*
 *	UART_DATA_LEN = HCI_TXFIFO_SIZE - 2.		*
 * ---------------------------------------------*/
#define HCI_RXFIFO_SIZE		80
#define HCI_RXFIFO_NUM		4

#define HCI_TXFIFO_SIZE		80
#define HCI_TXFIFO_NUM		8

#define UART_DATA_LEN    	(HCI_TXFIFO_SIZE - 2)      // data max 252
typedef struct{
    unsigned int len;        // data max 252
    unsigned char data[UART_DATA_LEN];
}uart_data_t;





#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)
	#if(BOARD_SELECT == BOARD_825X_EVK_C1T139A30 || BOARD_SELECT == BOARD_827X_EVK_C1T197A30)
		//define debug GPIO here according to your hardware
		#define GPIO_CHN0							GPIO_PB4
		#define GPIO_CHN1							GPIO_PB5
		#define GPIO_CHN2							GPIO_PB6
		#define GPIO_CHN3							GPIO_PB7

		#define PB4_OUTPUT_ENABLE					1
		#define PB5_OUTPUT_ENABLE					1
		#define PB6_OUTPUT_ENABLE					1
		#define PB7_OUTPUT_ENABLE					1
	#endif
#endif  //end of DEBUG_GPIO_ENABLE



/////////////////// set default   ////////////////

#include "../common/default_config.h"
