/********************************************************************************************************
 * @file     user_config.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2015
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

#if (__PROJECT_8255_BLE_REMOTE__)
	#include "../b85m_ble_remote/app_config.h"
#elif (__PROJECT_8258_BLE_REMOTE__ || __PROJECT_8278_BLE_REMOTE__)
	#include "../b85m_ble_remote/app_config.h"
#elif (__PROJECT_8258_BLE_SAMPLE__ || __PROJECT_8278_BLE_SAMPLE__)
	#include "../b85m_ble_sample/app_config.h"
#elif (__PROJECT_8258_MODULE__ || __PROJECT_8278_MODULE__)
	#include "../b85m_module/app_config.h"
#elif (__PROJECT_8258_HCI__ || __PROJECT_8278_HCI__)
	#include "../b85m_hci/app_config.h"
#elif (__PROJECT_8258_FEATURE_TEST__ || __PROJECT_8278_FEATURE_TEST__)
	#include "../b85m_feature_test/app_config.h"
#elif(__PROJECT_8258_MASTER_KMA_DONGLE__ || __PROJECT_8278_MASTER_KMA_DONGLE__ )
	#include "../b85m_master_kma_dongle/app_config.h"
#elif(__PROJECT_8258_MASTER_SLAVE__ )
	#include "../b85m_master_slave/app_config.h"
#elif(__PROJECT_8258_BQB_LOWER_TESTER__ )
	#include "../b85m_bqb_lowertester/app_config.h"
#elif(__PROJECT_8258_INTERNAL_TEST__ ||  __PROJECT_8278_INTERNAL_TEST__)
	#include "../b85m_internal_test/app_config.h"
#else
	#include "../common/default_config.h"
#endif

