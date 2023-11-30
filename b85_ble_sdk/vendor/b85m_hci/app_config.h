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




///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE					0
#define PRINT_DEBUG_INFO               		0	//open/close myprintf
#define APP_FLASH_INIT_LOG_EN				0
#define APP_LOG_EN							1
#define UART_PRINT_DEBUG_ENABLE				1

/////////////////////// HCI Board Select Configuration ///////////////////////////////
#if (__PROJECT_8258_HCI__)
	/* can only choose BOARD_825X_EVK_C1T139A30,*/
	#define BOARD_SELECT							BOARD_825X_EVK_C1T139A30
#elif (__PROJECT_8278_HCI__)
	/* can only choose BOARD_827X_EVK_C1T197A30,*/
	#define BOARD_SELECT							BOARD_827X_EVK_C1T197A30
#endif



///////////////////////// UI Configuration ////////////////////////////////////////////////////

#define	UI_LED_ENABLE									0

//////////////////////////// MODULE PM GPIO	(EVK board) /////////////////////////////////
#define GPIO_WAKEUP_MODULE					GPIO_PC6   //mcu wakeup module
#define	PC6_FUNC							AS_GPIO
#define PC6_INPUT_ENABLE					1
#define	PC6_OUTPUT_ENABLE					0
#define	PC6_DATA_OUT						0
#define GPIO_WAKEUP_MODULE_HIGH				gpio_setup_up_down_resistor(GPIO_WAKEUP_MODULE, PM_PIN_PULLUP_10K);
#define GPIO_WAKEUP_MODULE_LOW				gpio_setup_up_down_resistor(GPIO_WAKEUP_MODULE, PM_PIN_PULLDOWN_100K);

#define GPIO_WAKEUP_MCU						GPIO_PC7   //module wakeup mcu
#define	PC7_FUNC							AS_GPIO
#define PC7_INPUT_ENABLE					1
#define	PC7_OUTPUT_ENABLE					1
#define	PC7_DATA_OUT						0
#define GPIO_WAKEUP_MCU_HIGH				do{gpio_set_output_en(GPIO_WAKEUP_MCU, 1); gpio_write(GPIO_WAKEUP_MCU, 1);}while(0)
#define GPIO_WAKEUP_MCU_LOW					do{gpio_set_output_en(GPIO_WAKEUP_MCU, 1); gpio_write(GPIO_WAKEUP_MCU, 0);}while(0)
#define GPIO_WAKEUP_MCU_FLOAT				do{gpio_set_output_en(GPIO_WAKEUP_MCU, 0); gpio_write(GPIO_WAKEUP_MCU, 0);}while(0)




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
#define MODULE_WATCHDOG_ENABLE				0
#define WATCHDOG_INIT_TIMEOUT				500  //ms





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
    unsigned int len; // data max 252
    unsigned char data[UART_DATA_LEN];
}uart_data_t;


/////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
#if (UART_PRINT_DEBUG_ENABLE)
	#define DEBUG_INFO_TX_PIN           	GPIO_PB5
	#define PULL_WAKEUP_SRC_PB5         	PM_PIN_PULLUP_10K
	#define PB5_OUTPUT_ENABLE         		1
	#define PB5_DATA_OUT                    1
#endif

/////////////////// set default   ////////////////

#include "../common/default_config.h"
