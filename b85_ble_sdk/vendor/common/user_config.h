/********************************************************************************************************
 * @file	user_config.h
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
#elif(__PROJECT_8258_DRIVER_TEST__ ||  __PROJECT_8278_DRIVER_TEST__)
	#include "../b85m_driver_test/app_config.h"
#else
	#include "../common/default_config.h"
#endif

