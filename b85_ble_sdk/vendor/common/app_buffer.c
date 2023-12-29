/********************************************************************************************************
 * @file    app_buffer.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app_buffer.h"



/***************** ACL connection L2CAP layer MTU and potential RX & TX buffer, Begin ********************************/

/*
 * if MTU no greater than "ATT_MTU_MAX_SDK_DFT_BUF", default L2CAP RX & TX buffer(stack inside) is enough,
 * if MTU greater than "ATT_MTU_MAX_SDK_DFT_BUF", default L2CAP RX & TX buffer is not enough,
	 	 user must allocate new buffer and set it
 */
#if (MTU_SIZE_SETTING > ATT_MTU_MAX_SDK_DFT_BUF)

_attribute_data_retention_	u8 app_l2cap_rx_fifo[ACL_L2CAP_BUFF_SIZE];
_attribute_data_retention_	u8 app_l2cap_tx_fifo[ACL_L2CAP_BUFF_SIZE];

#endif

/***************** ACL connection L2CAP layer MTU and potential RX & TX buffer, End ********************************/
