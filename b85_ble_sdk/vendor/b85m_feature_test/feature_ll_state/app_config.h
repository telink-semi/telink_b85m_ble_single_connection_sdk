/********************************************************************************************************
 * @file     app_config.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

#include "../feature_config.h"

#if (FEATURE_TEST_MODE == TEST_ADVERTISING_ONLY || FEATURE_TEST_MODE == TEST_SCANNING_ONLY || FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE || \
	FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE || FEATURE_TEST_MODE == TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE)

///////////////////////// Feature Configuration////////////////////////////////////////////////
#if (FEATURE_TEST_MODE == TEST_ADVERTISING_ONLY || FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE)
	#define FEATURE_PM_ENABLE							1
	#define FEATURE_DEEPSLEEP_RETENTION_ENABLE			1
#else
	#define FEATURE_PM_ENABLE							0
	#define FEATURE_DEEPSLEEP_RETENTION_ENABLE			0
#endif

#define APP_DEFAULT_HID_BATTERY_OTA_ATTRIBUTE_TABLE		1

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define	UI_LED_ENABLE									1


///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE								0


/////////////////////// Feature Test Board Select Configuration ///////////////////////////////
#define BOARD_825X_EVK_C1T139A30						1     //TLSR8258DK48
#define BOARD_825X_DONGLE_C1T139A3						2     //
#define BOARD_827X_EVK_C1T197A30						3	  //TLSR8278DK48
#define BOARD_827X_DONGLE_C1T201A3						4	  //

#if (__PROJECT_8258_FEATURE_TEST__)
	#define BOARD_SELECT								BOARD_825X_EVK_C1T139A30
#elif (__PROJECT_8278_FEATURE_TEST__)
	#define BOARD_SELECT								BOARD_827X_EVK_C1T197A30
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

	#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
	#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
	#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
	#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
	#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#define DBG_CHN2_LOW		gpio_write(GPIO_CHN2, 0)
	#define DBG_CHN2_HIGH		gpio_write(GPIO_CHN2, 1)
	#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
	#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
	#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
	#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
	#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
	#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
	#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
	#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#define DBG_CHN6_LOW		gpio_write(GPIO_CHN6, 0)
	#define DBG_CHN6_HIGH		gpio_write(GPIO_CHN6, 1)
	#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)
	#define DBG_CHN7_LOW		gpio_write(GPIO_CHN7, 0)
	#define DBG_CHN7_HIGH		gpio_write(GPIO_CHN7, 1)
	#define DBG_CHN7_TOGGLE		gpio_toggle(GPIO_CHN7)
#else
	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE


#include "../common/default_config.h"


#endif  //end of (FEATURE_TEST_MODE == ...)
