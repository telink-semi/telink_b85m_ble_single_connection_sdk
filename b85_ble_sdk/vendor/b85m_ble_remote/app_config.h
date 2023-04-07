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
#define BATT_CHECK_ENABLE       			1   //must enable
#define BLE_AUDIO_ENABLE					1
#define BLT_APP_LED_ENABLE					1
#define BLT_TEST_SOFT_TIMER_ENABLE			0

#define UNUSED_GPIO_PULLDOWN_ENABLE			0
#define FIRMWARE_CHECK_ENABLE				0   //flash firmware_check
#define FIRMWARES_SIGNATURE_ENABLE          0   //firmware check
#define AUDIO_TRANS_USE_2M_PHY_ENABLE		0

#if (BLT_TEST_SOFT_TIMER_ENABLE)
	#define BLT_SOFTWARE_TIMER_ENABLE		1
#endif

/////////////////////// RCU Board Select Configuration ///////////////////////////////
#define BOARD_825X_RCU_C1T139A5						1     //TLSR8258DK48
#define BOARD_827X_RCU_C1T197A5						2     //

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
 *  @brief  Battery_check Configuration
 */
#if (BATT_CHECK_ENABLE)
	#if(BOARD_SELECT == BOARD_825X_RCU_C1T139A5 || BOARD_SELECT == BOARD_827X_RCU_C1T197A5)
		#if 0//(__PROJECT_8278_BLE_REMOTE__)
			//use VBAT(8278) , then adc measure this VBAT voltage
			#define ADC_INPUT_PCHN					VBAT    //corresponding  ADC_InputPchTypeDef in adc.h
		#else
			//telink device: you must choose one gpio with adc function to output high level(voltage will equal to vbat), then use adc to measure high level voltage
			//use PB7(8258) output high level, then adc measure this high level voltage
			#define GPIO_VBAT_DETECT				GPIO_PB7
			#define PB7_FUNC						AS_GPIO
			#define PB7_INPUT_ENABLE				0
			#define ADC_INPUT_PCHN					B7P    //corresponding  ADC_InputPchTypeDef in adc.h
		#endif
	#endif
#endif


/**
 *  @brief  LED Configuration
 */
#if (BLT_APP_LED_ENABLE)
	#if (BOARD_SELECT == BOARD_825X_RCU_C1T139A5 || BOARD_SELECT == BOARD_827X_RCU_C1T197A5)
		#define LED_ON_LEVAL 						1 			//gpio output high voltage to turn on led
		#define	GPIO_LED							GPIO_PC6
		#define PC6_FUNC							AS_GPIO
	#endif
#endif


/**
 *  @brief  AUDIO Configuration
 */
#if (BLE_AUDIO_ENABLE)

	#define BLE_DMIC_ENABLE					0  //0: Amic   1: Dmic
	#define IIR_FILTER_ENABLE				0

	#if(BOARD_SELECT == BOARD_827X_RCU_C1T197A5)
		#if BLE_DMIC_ENABLE
			#define GPIO_DMIC_BIAS					GPIO_PC4
			#define GPIO_DMIC_DI					GPIO_PA0
			#define GPIO_DMIC_CK					GPIO_PA1
		#else
			#define GPIO_AMIC_BIAS					GPIO_PC0// need check ,v1.0 PC4, V1.1 PC0
			#define GPIO_AMIC_SP					GPIO_PC1
		#endif
	#elif(BOARD_SELECT == BOARD_825X_RCU_C1T139A5)
		#if BLE_DMIC_ENABLE
			#define GPIO_DMIC_BIAS					GPIO_PC4
			#define GPIO_DMIC_DI					GPIO_PA0
			#define GPIO_DMIC_CK					GPIO_PA1
		#else
			#define GPIO_AMIC_BIAS					GPIO_PC4
			#define GPIO_AMIC_SP					GPIO_PC0
			#define GPIO_AMIC_SN					GPIO_PC1
		#endif
	#endif
	/* RCU Audio MODE:
	 * TL_AUDIO_RCU_ADPCM_GATT_TLEINK
	 * TL_AUDIO_RCU_ADPCM_GATT_GOOGLE
	 * TL_AUDIO_RCU_ADPCM_HID
	 * TL_AUDIO_RCU_SBC_HID						//need config 32k retention
	 * TL_AUDIO_RCU_ADPCM_HID_DONGLE_TO_STB
	 * TL_AUDIO_RCU_SBC_HID_DONGLE_TO_STB		//need config 32k retention
	 * TL_AUDIO_RCU_MSBC_HID					//need config 32k retention
	 */
	#define TL_AUDIO_MODE  						TL_AUDIO_RCU_ADPCM_GATT_TLEINK

#endif


/**
 *  @brief  IR Configuration
 */
#if (REMOTE_IR_ENABLE)
	#if(BOARD_SELECT == BOARD_825X_RCU_C1T139A5 || BOARD_SELECT == BOARD_827X_RCU_C1T197A5)
		//PB3 IRout 100K pulldown when  IR not working,  when IR begin, disable this 100K pulldown
		#define	PULL_WAKEUP_SRC_PB3		PM_PIN_PULLDOWN_100K
	#endif
#endif


//////////////////////////// KEYSCAN/MIC  GPIO //////////////////////////////////
#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid
#define DEEPBACK_FAST_KEYSCAN_ENABLE	0   //proc fast scan when deepsleep back trigged by key press, in case key loss
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

#if(BOARD_SELECT == BOARD_825X_RCU_C1T139A5 || BOARD_SELECT == BOARD_827X_RCU_C1T197A5)
	//////////////////// KEY CONFIG (RCU board) ///////////////////////////
	#define  KB_DRIVE_PINS  {GPIO_PD5, GPIO_PD2, GPIO_PD4, GPIO_PD6, GPIO_PD7}			// last pin 'GPIO_PD7' abnormal
	#define  KB_SCAN_PINS   {GPIO_PC5, GPIO_PA0, GPIO_PB2, GPIO_PA4, GPIO_PA3, GPIO_PD3}// second pin 'GPIO_PA0' abnormal

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
#endif

#if UNUSED_GPIO_PULLDOWN_ENABLE
	#define PULL_WAKEUP_SRC_PA5			PM_PIN_PULLDOWN_100K
	#define PULL_WAKEUP_SRC_PA6			PM_PIN_PULLDOWN_100K
	#define PULL_WAKEUP_SRC_PB0			PM_PIN_PULLDOWN_100K
	#define PULL_WAKEUP_SRC_PB1			PM_PIN_PULLDOWN_100K
	#define PULL_WAKEUP_SRC_PB4			PM_PIN_PULLDOWN_100K
	#define PULL_WAKEUP_SRC_PB5			PM_PIN_PULLDOWN_100K
	#define PULL_WAKEUP_SRC_PB6			PM_PIN_PULLDOWN_100K
	#define PULL_WAKEUP_SRC_PB7			PM_PIN_PULLDOWN_100K
	#define PULL_WAKEUP_SRC_PD0			PM_PIN_PULLDOWN_100K    //note: A0 version, if enable pull down 100k, will make current leakage
	#define PULL_WAKEUP_SRC_PD1			PM_PIN_PULLDOWN_100K

#endif

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


/**
 *  @brief  DEBUG_GPIO Configuration
 */
#define DEBUG_GPIO_ENABLE							0
#if(DEBUG_GPIO_ENABLE)
	#if(BOARD_SELECT == BOARD_825X_RCU_C1T139A5 || BOARD_SELECT == BOARD_827X_RCU_C1T197A5)
		//define debug GPIO here according to your hardware
		#define GPIO_CHN0							GPIO_PB4
		#define GPIO_CHN1							GPIO_PB5
		#define GPIO_CHN2							GPIO_PB6
		#define GPIO_CHN3							//GPIO_PC2  // PC2/PC3 may used for external crystal input
		#define GPIO_CHN4							//GPIO_PC3  // PC2/PC3 may used for external crystal input
		#define GPIO_CHN5							GPIO_PB0
		#define GPIO_CHN6							GPIO_PB1


		#define PB4_OUTPUT_ENABLE					1
		#define PB5_OUTPUT_ENABLE					1
		#define PB6_OUTPUT_ENABLE					1
		//#define PC2_OUTPUT_ENABLE					1
		//#define PC3_OUTPUT_ENABLE					1
		#define PB0_OUTPUT_ENABLE					1
		#define PB1_OUTPUT_ENABLE					1
	#endif
#endif  //end of DEBUG_GPIO_ENABLE


#define BLE_PHYTEST_MODE						PHYTEST_MODE_DISABLE


#include "../common/default_config.h"
