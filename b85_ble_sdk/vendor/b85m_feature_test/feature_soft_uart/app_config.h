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

#include "../feature_config.h"

#if (FEATURE_TEST_MODE == TEST_USER_BLT_SOFT_UART)

///////////////////////// Feature Configuration////////////////////////////////////////////////
#define FEATURE_PM_ENABLE								0
#define PM_DEEPSLEEP_RETENTION_ENABLE				0

#define APP_DEFAULT_HID_BATTERY_OTA_ATTRIBUTE_TABLE		1

///////////////////////// SOFT UART Feature Configuration////////////////////////////////////////////////
/**
 *  @brief  software uart enable and setting
 *   (The maximum baud rate is 9600)
 */
#define  	SOFT_UART_ENABLE                       1

#define 	SOFT_UART_BAUD_RATE                     9600
#define 	SOFT_UART_TX_IO                   		GPIO_PA4
#define 	SOFT_UART_RX_IO                   		GPIO_PB4



#define    TEST_RX_TX_RUN                           1
#define    TEST_ONLY_TX_RUN                         2

#define    TEST_SOFT_UART_RUN_MODEL                 TEST_RX_TX_RUN


///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE								0
#define UART_PRINT_DEBUG_ENABLE							1
#define APP_FLASH_INIT_LOG_EN							0
#define APP_LOG_EN										1

/////////////////////// Feature Test Board Select Configuration ///////////////////////////////
#if (__PROJECT_8258_FEATURE_TEST__)
	#define BOARD_SELECT								BOARD_825X_EVK_C1T139A30
#elif (__PROJECT_8278_FEATURE_TEST__)
	#define BOARD_SELECT								BOARD_827X_EVK_C1T197A30
#endif



///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define	UI_KEYBOARD_ENABLE								0
#define	UI_LED_ENABLE									0







///////////////////////// System Clock  Configuration /////////////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  								48000000	//must select 48M clock

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







/////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
#if (UART_PRINT_DEBUG_ENABLE)
	#define DEBUG_INFO_TX_PIN           	GPIO_PB1
	#define PULL_WAKEUP_SRC_PB1         	PM_PIN_PULLUP_10K
	#define PB1_OUTPUT_ENABLE         		1
	#define PB1_DATA_OUT                    1
#endif

#include "../common/default_config.h"


#endif  //end of (FEATURE_TEST_MODE == ...)
