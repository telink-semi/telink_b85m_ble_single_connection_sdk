/********************************************************************************************************
 * @file    compiler.h
 *
 * @brief   This is the header file for B85
 *
 * @author  Driver Group
 * @date    May 8,2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once


#define _attribute_packed_				__attribute__((packed))
#define _attribute_aligned_(s)			__attribute__((aligned(s)))
#define _attribute_session_(s)			__attribute__((section(s)))
#define _attribute_ram_code_  			_attribute_session_(".ram_code")
#define _attribute_ram_code_noinline_	__attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_custom_code_  		_attribute_session_(".custom") volatile
#define _attribute_no_inline_   		__attribute__((noinline))
#define _inline_ 						inline

#define _attribute_ram_code_sec_      		__attribute__((section(".ram_code")))
#define _attribute_ram_code_sec_noinline_	__attribute__((section(".ram_code"))) __attribute__((noinline))

/**
 *  _always_inline needs to be added in the following two cases:
 * 1. The subfunctions in the pm_sleep_wakeup function need to use _always_inline and _attribute_ram_code_sec_noinline_, as detailed in the internal comments of pm_sleep_wakeup.
 */
#define _always_inline                      inline __attribute__((always_inline))

#ifndef	BLC_PM_DEEP_RETENTION_MODE_EN
#define	BLC_PM_DEEP_RETENTION_MODE_EN		1
#endif

#if (BLC_PM_DEEP_RETENTION_MODE_EN)
	#define _attribute_data_retention_sec_	__attribute__((section(".retention_data")))
	#define _attribute_data_retention_   	__attribute__((section(".retention_data")))
	#define _attribute_ble_data_retention_  __attribute__((section(".retention_data")))
	#define _attribute_data_no_init_   		__attribute__((section(".data_no_init")))    //save retention ram size
#else
	#define _attribute_data_retention_sec_
    #define _attribute_data_retention_
	#define _attribute_ble_data_retention_
	#define _attribute_data_no_init_
#endif


