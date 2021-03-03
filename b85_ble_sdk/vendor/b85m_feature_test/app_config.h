/********************************************************************************************************
 * @file     app_config.h
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
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


#include "feature_config.h"


#if(FEATURE_TEST_MODE == TEST_ADVERTISING_ONLY || FEATURE_TEST_MODE == TEST_SCANNING_ONLY || FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE || \
	FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE || FEATURE_TEST_MODE == TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE)
	#include "feature_ll_state/app_config.h"
#elif(FEATURE_TEST_MODE == TEST_POWER_ADV)
	#include "feature_adv_power/app_config.h"
#elif(FEATURE_TEST_MODE == TEST_SMP_SECURITY)
	#include "feature_smp_security/app_config.h"
#elif(FEATURE_TEST_MODE == TEST_GATT_SECURITY)
	#include "feature_gatt_security/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION)
	#include "feature_slave_dle/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_MDATA_LENGTH_EXTENSION)
	#include "feature_master_dle/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_BLE_PHY)
	#include "feature_PHY_test/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_EXTENDED_ADVERTISING)
	#include "feature_extend_adv/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_EXT_ADV)
	#include "feature_phy_extend_adv/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_CONNECTION)
	#include "feature_phy_conn/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_OTA_BIG_PDU)
	#include "feature_ota_big_pdu/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_USER_BLT_SOFT_TIMER || FEATURE_TEST_MODE == TEST_WHITELIST || FEATURE_TEST_MODE == TEST_CSA2)
	#include "feature_misc/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_FEATURE_BACKUP)
	#include "feature_backup/app_config.h"
#endif


