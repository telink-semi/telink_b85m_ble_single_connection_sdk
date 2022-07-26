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

#if (FEATURE_TEST_MODE == TEST_LL_PRIVACY_MASTER)

#define	PEER_ADDR_RPA_EN		1
#define	LOCAL_ADDR_RPA_EN		0

//If Flash size is 512K, Recommend "FLASH_ADR_PARING" as 0x78000
//If Flash size is 1M,   Recommend "FLASH_ADR_PARING" as 0xFC000
#define FLASH_ADR_PARING   							0x78000



/////////////////// MODULE /////////////////////////////////
#define BLE_HOST_SMP_ENABLE							1  //1 for standard security management,  0 for telink referenced paring&bonding(no security)
#define BLE_HOST_SIMPLE_SDP_ENABLE					1  //simple service discovery

#define UI_BUTTON_ENABLE							1
#define UI_LED_ENABLE								1


#define APPLICATION_DONGLE							1
#if(APPLICATION_DONGLE)
	#define PA5_FUNC				AS_USB
	#define PA6_FUNC				AS_USB
	#define PA5_INPUT_ENABLE		1
	#define PA6_INPUT_ENABLE		1

	#define	USB_PRINTER_ENABLE 		1
	#define	USB_SPEAKER_ENABLE 		0

	#if ((TL_AUDIO_MODE & TL_AUDIO_MASK_HID_SERVICE_CHANNEL) && (TL_AUDIO_MODE & TL_AUDIO_MASK_DONGLE_TO_STB))//HIDdongle͸STB
	#define	USB_MIC_ENABLE 			0
	#define AUDIO_HOGP				1	//Audio HID Over GATT Profile
	#else
	#define AUDIO_HOGP				0	//Audio HID Over GATT Profile
	#define	USB_MIC_ENABLE 			1
	#endif

	#define	USB_MOUSE_ENABLE 		1
	#define	USB_KEYBOARD_ENABLE 	1
	#define	USB_SOMATIC_ENABLE      0   //  when USB_SOMATIC_ENABLE, USB_EDP_PRINTER_OUT disable
	#define USB_CUSTOM_HID_REPORT	1
#endif


//////////// product  Information  //////////////////////////////
#define ID_VENDOR				0x248a			// for report
#define ID_PRODUCT_BASE			0x880C//AUDIO_HOGP
#define STRING_VENDOR			L"Telink"
#define STRING_PRODUCT			L"BLE Remote KMA Dongle"
#define STRING_SERIAL			L"TLSR8278"


//----------------------- GPIO for UI --------------------------------
#if (UI_BUTTON_ENABLE)//kite(C1T139A3)/vulture(C1T201A3) dongle board has the same button gpio
	#define	SW1_GPIO				GPIO_PD5
	#define	SW2_GPIO				GPIO_PD6
	#define PD5_FUNC				AS_GPIO
	#define PD6_FUNC				AS_GPIO
	#define PD5_INPUT_ENABLE		1
	#define PD6_INPUT_ENABLE		1
	#define PULL_WAKEUP_SRC_PD5     PM_PIN_PULLUP_10K
	#define PULL_WAKEUP_SRC_PD6     PM_PIN_PULLUP_10K
#endif

#if (UI_LED_ENABLE)//kite(C1T139A3)/vulture(C1T201A3) dongle board has the same led gpio
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



///////////////////////// System Clock  Configuration /////////////////////////////////////////
#if (TL_AUDIO_MODE & (TL_AUDIO_MASK_SBC_MODE|TL_AUDIO_MASK_MSBC_MODE))
#define CLOCK_SYS_CLOCK_HZ  								48000000
#else
#define CLOCK_SYS_CLOCK_HZ  								32000000
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




#define UART_PRINT_DEBUG_ENABLE               1   //GPIO simulate uart print func

#if (UART_PRINT_DEBUG_ENABLE)

    //the baud rate should not bigger than 1M(system timer clock is constant 16M)
    #define PRINT_BAUD_RATE                   1000000
    #define DEBUG_INFO_TX_PIN                 GPIO_PB4
    #define PULL_WAKEUP_SRC_PB4               PM_PIN_PULLUP_10K
    #define PB4_OUTPUT_ENABLE                 1
    #define PB4_DATA_OUT                      1 //must
    #include "application/print/u_printf.h"

#endif




#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)
//define debug GPIO here according to your hardware
	#define GPIO_CHN0							GPIO_PB2
	#define GPIO_CHN1							GPIO_PB3
	#define GPIO_CHN2							GPIO_PB4
	#define GPIO_CHN3							GPIO_PB5
	#define GPIO_CHN4							GPIO_PC2
	#define GPIO_CHN5							GPIO_PC3

	#define PB2_OUTPUT_ENABLE					1
	#define PB3_OUTPUT_ENABLE					1
	#define PB4_OUTPUT_ENABLE					1
	#define PB5_OUTPUT_ENABLE					1
	#define PC2_OUTPUT_ENABLE					1
	#define PC3_OUTPUT_ENABLE					1
#endif  //end of DEBUG_GPIO_ENABLE

#define	FLOW_NO_OS						1

//////////////////// Audio /////////////////////////////////////
#define MIC_RESOLUTION_BIT		16
#define MIC_SAMPLE_RATE			16000
#define MIC_CHANNLE_COUNT		1
#define	MIC_ENOCDER_ENABLE		0




/////////////////// set default   ////////////////

#include "../common/default_config.h"


#endif
