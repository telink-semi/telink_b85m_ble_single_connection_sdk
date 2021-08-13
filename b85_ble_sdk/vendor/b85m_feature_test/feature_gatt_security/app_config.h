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

#if (FEATURE_TEST_MODE == TEST_GATT_SECURITY)

///////////////////////// Feature Configuration////////////////////////////////////////////////
#define FEATURE_PM_ENABLE								1
#define FEATURE_DEEPSLEEP_RETENTION_ENABLE				0

#define APP_DEFAULT_HID_BATTERY_OTA_ATTRIBUTE_TABLE		0

// LE_Security_Mode_1_Level_1, no authentication and no encryption
#define 	SMP_TEST_NO_SECURITY				1


// LE_Security_Mode_1_Level_2, unauthenticated paring with encryption
#define 	SMP_TEST_LEGACY_PAIRING_JUST_WORKS	2 //JustWorks
#define 	SMP_TEST_SC_PAIRING_JUST_WORKS		3 //JustWorks

// LE_Security_Mode_1_Level_3, authenticated paring with encryption
#define 	SMP_TEST_LEGACY_PASSKEY_ENTRY_SDMI	4 //PK_Resp_Dsply_Init_Input
#define 	SMP_TEST_LEGACY_PASSKEY_ENTRY_MDSI	5 //PK_Init_Dsply_Resp_Input
#define 	SMP_TEST_LEGACY_PASSKEY_ENTRY_MISI	6 //PK_BOTH_INPUT, not test
#define 	SMP_TEST_LEGACY_PASSKEY_ENTRY_OOB	7 //OOB_Authentication, not test

// LE_Security_Mode_1_Level_4, authenticated paring with encryption
#define 	SMP_TEST_SC_NUMERIC_COMPARISON		8 //Numric_Comparison
#define 	SMP_TEST_SC_PASSKEY_ENTRY_SDMI		9 //PK_Resp_Dsply_Init_Input
#define 	SMP_TEST_SC_PASSKEY_ENTRY_MDSI		10//PK_Init_Dsply_Resp_Input
#define 	SMP_TEST_SC_PASSKEY_ENTRY_MISI		11//PK_BOTH_INPUT, not test
#define 	SMP_TEST_SC_PASSKEY_ENTRY_OOB		12//OOB_Authentication, not test

// LE security mode select
#define 	LE_SECURITY_MODE_1_LEVEL_1			SMP_TEST_NO_SECURITY
#define 	LE_SECURITY_MODE_1_LEVEL_2			SMP_TEST_LEGACY_PAIRING_JUST_WORKS
#define 	LE_SECURITY_MODE_1_LEVEL_3			SMP_TEST_LEGACY_PASSKEY_ENTRY_SDMI
#define 	LE_SECURITY_MODE_1_LEVEL_4			SMP_TEST_SC_PASSKEY_ENTRY_SDMI


#define     SMP_TEST_MODE						LE_SECURITY_MODE_1_LEVEL_2

//use lightblue or nrf connect app, after connected, enable notify, write some data into characteristic Telink SPP:Phone->Module

//client to server RX character permission
//refer to core5.0 Vol3,Part C, Table 10.2 for more information
#if (SMP_TEST_MODE == LE_SECURITY_MODE_1_LEVEL_1)
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_RDWR
	#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_ENCRYPT_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_AUTHEN_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_SECURE_CONN_RDWR
#elif(SMP_TEST_MODE == LE_SECURITY_MODE_1_LEVEL_2)
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_ENCRYPT_RDWR
	#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_AUTHEN_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_SECURE_CONN_RDWR
#elif(SMP_TEST_MODE == LE_SECURITY_MODE_1_LEVEL_3)
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_ENCRYPT_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_AUTHEN_RDWR
	#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_SECURE_CONN_RDWR
#elif(SMP_TEST_MODE == LE_SECURITY_MODE_1_LEVEL_4)
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_ENCRYPT_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_AUTHEN_RDWR
	#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_SECURE_CONN_RDWR
#endif


///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define	UI_KEYBOARD_ENABLE								0
#define UI_LED_ENABLE                                   0

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE								0
#define UART_PRINT_DEBUG_ENABLE  						1

#define BOARD_825X_EVK_C1T139A30						1     //TLSR8258DK48
#define BOARD_825X_DONGLE_C1T139A3						2     //
#define BOARD_827X_EVK_C1T197A30						3	  //TLSR8278DK48
#define BOARD_827X_DONGLE_C1T201A3						4	  //


#if (__PROJECT_8258_FEATURE_TEST__)
	#define BOARD_SELECT								BOARD_825X_EVK_C1T139A30
#elif (__PROJECT_8278_FEATURE_TEST__)
	#define BOARD_SELECT								BOARD_827X_EVK_C1T197A30
#endif

#if (UI_KEYBOARD_ENABLE)   // if test pure power, kyeScan GPIO setting all disabled

    #define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
	#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K
	#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid

	//repeat key
	#define 		KB_REPEAT_KEY_ENABLE			0
	#define			KB_REPEAT_KEY_INTERVAL_MS		200
	#define 		KB_REPEAT_KEY_NUM				1
	#define 		KB_MAP_REPEAT					{VK_1, }

	#define			CR_VOL_UP						0xf0  ////
	#define			CR_VOL_DN						0xf1
	#define			CR_VOL_MUTE						0xf2
	#define			CR_CHN_UP						0xf3
	#define			CR_CHN_DN						0xf4  ////
	#define			CR_POWER						0xf5
	#define			CR_SEARCH						0xf6
	#define			CR_RECORD						0xf7
	#define			CR_PLAY							0xf8  ////
	#define			CR_PAUSE						0xf9
	#define			CR_STOP							0xfa
	#define			CR_FAST_BACKWARD				0xfb
	#define			CR_FAST_FORWARD					0xfc  ////
	#define			CR_HOME							0xfd
	#define			CR_BACK							0xfe
	#define			CR_MENU							0xff

	/**
	 *  @brief  Normal keyboard map
	 */

	#define			KB_MAP_NORMAL	{\
						VK_B,		CR_POWER,		VK_NONE,		VK_C,				CR_HOME,	 \
						VK_NONE,	VK_NONE,		VK_NONE,		CR_VOL_UP,			CR_VOL_DN,	 \
						VK_2,		VK_RIGHT,		CR_VOL_DN,		VK_3,				VK_1,	 \
						VK_5,		VK_ENTER,		CR_VOL_UP,		VK_6,				VK_4,	 \
						VK_8,		VK_DOWN,		VK_UP ,			VK_9,				VK_7,	 \
						VK_0,		CR_BACK,		VK_LEFT,		CR_VOL_MUTE,		CR_MENU,	 }

	#define  		KB_DRIVE_PINS  {GPIO_PD5, GPIO_PD2, GPIO_PD4, GPIO_PD6, GPIO_PD7}
	#define  		KB_SCAN_PINS   {GPIO_PC5, GPIO_PA0, GPIO_PB2, GPIO_PA4, GPIO_PA3, GPIO_PD3}

	//drive pin as gpio
	#define	PD5_FUNC				AS_GPIO
	#define	PD2_FUNC				AS_GPIO
	#define	PD4_FUNC				AS_GPIO
	#define	PD6_FUNC				AS_GPIO
	#define	PD7_FUNC				AS_GPIO
	//drive pin need 100K pulldown
	#define	PULL_WAKEUP_SRC_PD5		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PD2		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PD4		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PD6		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PD7		MATRIX_ROW_PULL
	//drive pin open input to read gpio wakeup level
	#define PD5_INPUT_ENABLE		1
	#define PD2_INPUT_ENABLE		1
	#define PD4_INPUT_ENABLE		1
	#define PD6_INPUT_ENABLE		1
	#define PD7_INPUT_ENABLE		1
	//scan pin as gpio
	#define	PC5_FUNC				AS_GPIO
	#define	PA0_FUNC				AS_GPIO
	#define	PB2_FUNC				AS_GPIO
	#define	PA4_FUNC				AS_GPIO
	#define	PA3_FUNC				AS_GPIO
	#define	PD3_FUNC				AS_GPIO
	//scan  pin need 10K pullup
	#define	PULL_WAKEUP_SRC_PC5		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PA0		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PB2		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PA4		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PA3		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PD3		MATRIX_COL_PULL
	//scan pin open input to read gpio level
	#define PC5_INPUT_ENABLE		1
	#define PA0_INPUT_ENABLE		1
	#define PB2_INPUT_ENABLE		1
	#define PA4_INPUT_ENABLE		1
	#define PA3_INPUT_ENABLE		1
	#define PD3_INPUT_ENABLE		1

	#define	KB_MAP_NUM			    KB_MAP_NORMAL
	#define	KB_MAP_FN			    KB_MAP_NORMAL

#endif


/**
 *  @brief  LED Configuration
 */
#if (UI_LED_ENABLE)
	#if (BOARD_SELECT == BOARD_825X_EVK_C1T139A30 || BOARD_SELECT == BOARD_827X_EVK_C1T197A30)
		/* 825X EVK and 827X EVK use same GPIO for LED: PD2/PD3/PD4/PD5 */
		#define	GPIO_LED_BLUE			GPIO_PD2
		#define	GPIO_LED_GREEN			GPIO_PD3
		#define	GPIO_LED_WHITE			GPIO_PD4
		#define	GPIO_LED_RED			GPIO_PD5

		#define PD2_FUNC				AS_GPIO
		#define PD3_FUNC				AS_GPIO
		#define PD4_FUNC				AS_GPIO
		#define PD5_FUNC				AS_GPIO

		#define	PD2_OUTPUT_ENABLE		1
		#define	PD3_OUTPUT_ENABLE		1
		#define PD4_OUTPUT_ENABLE		1
		#define	PD5_OUTPUT_ENABLE		1

		#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led
	#elif (BOARD_SELECT == BOARD_825X_DONGLE_C1T139A3 || BOARD_SELECT == BOARD_827X_DONGLE_C1T201A3)
		/* 825X Dongle and 827X Dongle use same GPIO for LED: PA3/PB1/PA2/PB0/PA4 */
		#define	GPIO_LED_RED			GPIO_PA3
		#define	GPIO_LED_WHITE			GPIO_PB1
		#define	GPIO_LED_GREEN			GPIO_PA2
		#define	GPIO_LED_BLUE			GPIO_PB0
		#define	GPIO_LED_YELLOW			GPIO_PA4

		#define PA3_FUNC				AS_GPIO
		#define PB1_FUNC				AS_GPIO
		#define PA2_FUNC				AS_GPIO
		#define PB0_FUNC				AS_GPIO
		#define PA4_FUNC				AS_GPIO

		#define	PA3_OUTPUT_ENABLE		1
		#define	PB1_OUTPUT_ENABLE		1
		#define PA2_OUTPUT_ENABLE		1
		#define	PB0_OUTPUT_ENABLE		1
		#define	PA4_OUTPUT_ENABLE		1

		#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led
	#endif
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




#if(DEBUG_GPIO_ENABLE)
	#if (BOARD_SELECT == BOARD_825X_EVK_C1T139A30)
		#define GPIO_CHN0							GPIO_PD0
		#define GPIO_CHN1							GPIO_PD1
		#define GPIO_CHN2							GPIO_PD6
		#define GPIO_CHN3							GPIO_PD7
		#define GPIO_CHN4							GPIO_PA2
		#define GPIO_CHN5							GPIO_PA3
		#define GPIO_CHN6							GPIO_PA4
		#define GPIO_CHN7							0

		#define PD0_OUTPUT_ENABLE					1
		#define PD1_OUTPUT_ENABLE					1
		#define PD6_OUTPUT_ENABLE					1
		#define PD7_OUTPUT_ENABLE					1
		#define PA2_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1
	#elif (BOARD_SELECT == BOARD_827X_EVK_C1T197A30)
		#define GPIO_CHN0							GPIO_PD0
		#define GPIO_CHN1							GPIO_PD1
		#define GPIO_CHN2							GPIO_PD6
		#define GPIO_CHN3							GPIO_PD7
		#define GPIO_CHN4							GPIO_PA2
		#define GPIO_CHN5							GPIO_PA3
		#define GPIO_CHN6							GPIO_PA4
		#define GPIO_CHN7							GPIO_PB0

		#define PD0_OUTPUT_ENABLE					1
		#define PD1_OUTPUT_ENABLE					1
		#define PD6_OUTPUT_ENABLE					1
		#define PD7_OUTPUT_ENABLE					1
		#define PA2_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1
		#define PB0_OUTPUT_ENABLE					1
	#endif
#endif  //end of DEBUG_GPIO_ENABLE

/////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
#if (UART_PRINT_DEBUG_ENABLE)
		//the baud rate should not bigger than 1M(system timer clock is constant 16M)
		#define PRINT_BAUD_RATE             					1000000
		#define DEBUG_INFO_TX_PIN           					GPIO_PB1
		#define PULL_WAKEUP_SRC_PB1         					PM_PIN_PULLUP_10K
		#define PB1_OUTPUT_ENABLE         						1
        #define PB1_DATA_OUT                                    1 //must

	    #include "application/print/u_printf.h"
#endif

#include "../common/default_config.h"


#endif  //end of (FEATURE_TEST_MODE == ...)
