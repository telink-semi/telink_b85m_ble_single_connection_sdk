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


#include  "application/audio/audio_common.h"

/////////////////// FEATURE SELECT /////////////////////////////////
#define BLE_REMOTE_PM_ENABLE				1
#define PM_DEEPSLEEP_RETENTION_ENABLE		1
#define BLE_REMOTE_SECURITY_ENABLE      	1
#define BLE_REMOTE_OTA_ENABLE				1
#define REMOTE_IR_ENABLE					0
#define REMOTE_IR_LEARN_ENABLE				0
#define APP_BATT_CHECK_ENABLE       			1   //must enable
#define BLE_AUDIO_ENABLE					1
#define UI_LED_ENABLE						1
#define UI_KEYBOARD_ENABLE						1	//must enable
#define BLT_TEST_SOFT_TIMER_ENABLE			0

#define UNUSED_GPIO_PULLDOWN_ENABLE			0
#define FIRMWARE_CHECK_ENABLE				0   //flash firmware_check
#define FIRMWARES_SIGNATURE_ENABLE          0   //firmware check
#define AUDIO_TRANS_USE_2M_PHY_ENABLE		0
#define APP_FLASH_PROTECTION_ENABLE			0

#if (BLT_TEST_SOFT_TIMER_ENABLE)
	#define BLT_SOFTWARE_TIMER_ENABLE		1
#endif


/////////////////// DEBUG Configuration /////////////////////////////////
#define UART_PRINT_DEBUG_ENABLE             1   //GPIO simulate uart print func

#define APP_LOG_EN							1
#define APP_SMP_LOG_EN						0
#define APP_KEY_LOG_EN						1
#define APP_CONTR_EVENT_LOG_EN				1  //controller event log
#define APP_HOST_EVENT_LOG_EN				1  //host event log
#define APP_OTA_LOG_EN						1
#define APP_FLASH_INIT_LOG_EN				1
#define APP_FLASH_PROT_LOG_EN				1
#define APP_BATT_CHECK_LOG_EN				1

/////////////////////// RCU Board Select Configuration ///////////////////////////////
#if (__PROJECT_8258_BLE_REMOTE__)
	/* can only choose BOARD_825X_RCU_C1T139A5*/
	#define BOARD_SELECT							BOARD_825X_RCU_C1T139A5
#elif (__PROJECT_8278_BLE_REMOTE__)
	/* can only choose BOARD_827X_RCU_C1T197A5*/
	#define BOARD_SELECT							BOARD_827X_RCU_C1T197A5
#endif

/////////////////// DEEP SAVE FLG //////////////////////////////////
#define USED_DEEP_ANA_REG                   DEEP_ANA_REG0 //u8,can save 8 bit info when deep
#define	LOW_BATT_FLG					    BIT(0) //if 1: low battery
#define CONN_DEEP_FLG	                    BIT(1) //if 1: conn deep, 0: adv deep
#define IR_MODE_DEEP_FLG	 				BIT(2) //if 1: IR mode, 0: BLE mode
#define LOW_BATT_SUSPEND_FLG				BIT(3) //if 1 : low battery, < 1.8v

/**
 *  @brief  LED Configuration
 */
#if (UI_LED_ENABLE)
	#define BLT_APP_LED_ENABLE 1
#endif


/**
 *  @brief  AUDIO Configuration
 */
#if (BLE_AUDIO_ENABLE)

	#define BLE_DMIC_ENABLE					0  //0: Amic   1: Dmic
	#define IIR_FILTER_ENABLE				0

	/* RCU Audio MODE:
	 * TL_AUDIO_RCU_ADPCM_GATT_TLEINK
	 * TL_AUDIO_RCU_ADPCM_GATT_GOOGLE
	 * TL_AUDIO_RCU_ADPCM_HID
	 * TL_AUDIO_RCU_SBC_HID
	 * TL_AUDIO_RCU_ADPCM_HID_DONGLE_TO_STB
	 * TL_AUDIO_RCU_SBC_HID_DONGLE_TO_STB
	 * TL_AUDIO_RCU_MSBC_HID
	 */
	#define TL_AUDIO_MODE  						TL_AUDIO_RCU_ADPCM_GATT_GOOGLE

#endif

//////////////////////////// KEYSCAN/MIC  GPIO //////////////////////////////////
#define DEEPBACK_FAST_KEYSCAN_ENABLE	0   //proc fast scan when deepsleep back triggered by key press, in case key loss
#define LONG_PRESS_KEY_POWER_OPTIMIZE	1   //lower power when pressing key without release

//stuck key
#define STUCK_KEY_PROCESS_ENABLE		0
#define STUCK_KEY_ENTERDEEP_TIME		60  //in s

//repeat key
#define KB_REPEAT_KEY_ENABLE			0
#define	KB_REPEAT_KEY_INTERVAL_MS		200
#define KB_REPEAT_KEY_NUM				1
#define KB_MAP_REPEAT					{VK_1, }

//////////////////// key type ///////////////////////
#define IDLE_KEY	   			0
#define CONSUMER_KEY   	   		1
#define KEYBOARD_KEY   	   		2
#define IR_KEY   	   			3

#define			CR_VOL_UP				0xf0  ////
#define			CR_VOL_DN				0xf1
#define			CR_VOL_MUTE				0xf2
#define			CR_CHN_UP				0xf3
#define			CR_CHN_DN				0xf4  ////
#define			CR_POWER				0xf5
#define			CR_SEARCH				0xf6
#define			CR_RECORD				0xf7
#define			CR_PLAY					0xf8  ////
#define			CR_PAUSE				0xf9
#define			CR_STOP					0xfa
#define			CR_FAST_BACKWARD		0xfb
#define			CR_FAST_FORWARD			0xfc  ////
#define			CR_HOME					0xfd
#define			CR_BACK					0xfe
#define			CR_MENU					0xff

//special key
#define		 	VOICE					0xc0
#define 		KEY_MODE_SWITCH			0xc1
#define		 	PHY_TEST				0xc2


#define 		IR_VK_0			0x00
#define 		IR_VK_1			0x01
#define 		IR_VK_2			0x02
#define			IR_VK_3			0x03
#define			IR_VK_4			0x04
#define 		IR_VK_5			0x05
#define 		IR_VK_6			0x06
#define 		IR_VK_7			0x07
#define 		IR_VK_8			0x08
#define 		IR_VK_9			0x09

#define 		IR_POWER		0x12
#define			IR_AUDIO_MUTE	0x0d
#define 		IR_NETFLIX		0x0f
#define			IR_BACK			0x0e
#define			IR_VOL_UP		0x0b
#define			IR_VOL_DN		0x0c
#define 		IR_NEXT			0x20
#define 		IR_PREV			0x21
#define			IR_MENU			0x23
#define 		IR_HOME			0x24
#define 		IR_OPER_KEY		0x2e
#define 		IR_INFO			0x2f
#define			IR_REWIND		0x32
#define 		IR_FAST_FORWARD	0x34
#define 		IR_PLAY_PAUSE	0x35
#define			IR_GUIDE		0x41
#define 		IR_UP			0x45
#define			IR_DN			0x44
#define 		IR_LEFT			0x42
#define 		IR_RIGHT		0x43
#define			IR_SEL			0x46
#define 		IR_RED_KEY		0x6b
#define 		IR_GREEN_KEY	0x6c
#define 		IR_YELLOW_KEY	0x6d
#define 		IR_BLUE_KEY		0x6e
#define 		IR_RECORD		0x72
#define 		IR_OPTION		0x73
#define 		IR_STOP			0x74
#define 		IR_SEARCH		0x75
#define 		IR_TEXT			0x76
#define 		IR_VOICE		0x77
#define 		IR_PAUSE		0x78

#define			T_VK_CH_UP		0xd0
#define			T_VK_CH_DN		0xd1


#if (REMOTE_IR_ENABLE)  //with IR keymap
//	#define 		GPIO_IR_CONTROL			GPIO_PD0

	#define		KB_MAP_NORMAL	{\
					{0, 	    1,				2,			3,		4,		}, \
					{VOICE,	 	KEY_MODE_SWITCH,7,			8,		9		}, \
					{10,		11,				12,			13,		14,		}, \
					{15,		16,				17,			18,		19,		}, \
					{20,		21,				22,			23,		24,		}, \
					{25,		26,				27,			28,		29,		}, }


	#define		KB_MAP_BLE	{\
					VK_NONE,	VK_NONE,		VK_NONE,		VK_NONE,			VK_NONE,	 \
					VOICE,		VK_NONE,		VK_NONE,		CR_VOL_UP,			CR_VOL_DN,	 \
					VK_2,		VK_NONE,		VK_NONE,		VK_3,				VK_1,	 \
					VK_5,		VK_NONE,		VK_NONE,		VK_6,				VK_4,	 \
					VK_8,		VK_NONE,		VK_NONE,		VK_9,				VK_7,	 \
					VK_0,		VK_NONE,		VK_NONE,		VK_NONE,			VK_NONE,	 }


	#define		KB_MAP_IR	{\
					VK_NONE,	VK_NONE,		VK_NONE,	VK_NONE,			VK_NONE,	 \
					VK_NONE,	VK_NONE,		VK_NONE,	VK_NONE,			VK_NONE,	 \
					IR_VK_2,	VK_NONE,		VK_NONE,	IR_VK_3,			IR_VK_1, 	 \
					IR_VK_5,	VK_NONE,		VK_NONE,	IR_VK_6,			IR_VK_4,	 \
					IR_VK_8,	VK_NONE,		VK_NONE,	IR_VK_9,			IR_VK_7,	 \
					IR_VK_0,	VK_NONE,		VK_NONE,	VK_NONE,			VK_NONE,	 }

#else   //key map

	#define		KB_MAP_NORMAL	{\
					{VK_B,		CR_POWER,		VK_NONE,		VK_C,				CR_HOME},	 \
					{VOICE,		VK_NONE,		VK_NONE,		CR_VOL_UP,			CR_VOL_DN},	 \
					{VK_2,		VK_RIGHT,		CR_VOL_DN,		VK_3,				VK_1},	 \
					{VK_5,		VK_ENTER,		CR_VOL_UP,		VK_6,				VK_4},	 \
					{VK_8,		VK_DOWN,		VK_UP ,			VK_9,				VK_7},	 \
					{VK_0,		CR_BACK,		VK_LEFT,		VOICE,				CR_MENU},	 }

#endif  //end of REMOTE_IR_ENABLE


#define		KB_MAP_NUM		KB_MAP_NORMAL
#define		KB_MAP_FN		KB_MAP_NORMAL


///////////////////////// System Clock  Configuration /////////////////////////////////////////
#if (TL_AUDIO_MODE & (TL_AUDIO_MASK_SBC_MODE | TL_AUDIO_MASK_MSBC_MODE))
	#define CLOCK_SYS_CLOCK_HZ  								32000000
	#else
	#define CLOCK_SYS_CLOCK_HZ  								16000000
#endif

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

#define BLE_PHYTEST_MODE						PHYTEST_MODE_DISABLE


/////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
#if (UART_PRINT_DEBUG_ENABLE)
	#define DEBUG_INFO_TX_PIN           					GPIO_PB5
	#define PULL_WAKEUP_SRC_PB5         					PM_PIN_PULLUP_10K
	#define PB5_OUTPUT_ENABLE         						1
	#define PB5_DATA_OUT                                    1 //must
#endif


#include "../common/default_config.h"
