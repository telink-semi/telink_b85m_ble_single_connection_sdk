/********************************************************************************************************
 * @file    simple_sdp.h
 *
 * @brief   This is the header file for BLE SDK
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
#ifndef SIMPLE_SDP_H_
#define SIMPLE_SDP_H_

#include "vendor/common/user_config.h"



#ifndef ACL_CENTRAL_SIMPLE_SDP_ENABLE
#define ACL_CENTRAL_SIMPLE_SDP_ENABLE         		0
#endif



/**
 * @brief	2 bytes UUID get ATT handle structure
 */
typedef struct {
	u8	num;
	u8	property;
	u16	handle;
	u16	uuid;
	u16 ref;
} att_db_uuid16_t;			//8-byte

/**
 * @brief 	16 bytes UUID get ATT handle structure
 */
typedef struct {
	u8	num;
	u8	property;
	u16	handle;
	u8	uuid[16];
} att_db_uuid128_t;			//20-byte

#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)

/**
 * @brief	Simple SDP service discovery callback declaration
 */
typedef void (*main_service_t) (void);


/**
 * @brief	Get some ATT handle with service discovery result callback declaration
 */
typedef void (*sdp_get_handle_callback_t) (att_db_uuid16_t*, att_db_uuid128_t*);



extern int 	central_sdp_pending;
extern u8 	conn_char_handler[11];
extern u8	serviceDiscovery_adr_type;
extern u8	serviceDiscovery_address[6];



/**
 * @brief       This function is used to register SDP handler.
 * @param[in]   p       - Pointer point to SDP handler.
 * @return      none.
 */
void app_register_service (void *p);

/**
 * @brief       This function is used to process ATT packets related to SDP
 * @param[in]   connHandle  - connection handle
 * @param[in]   p           - Pointer point to ATT data buffer.
 * @return      no used
 */
int host_att_client_handler (u16 connHandle, u8 *p);

/**
 * @brief       This function is used to register ble stack mainloop function.
 * @param[in]   p           - Pointer point to ble stack mainloop function.
 * @return
 */
int host_att_register_idle_func (void *p);

/**
 * @brief       this function serves to find handle of uuid16
 * @param[in]	p - pointer of data attribute
 * @param[in]	uuid
 * @param[in]	ref - HID Report
 * @return      0 - fail to find handle of uuid16
 *              1 - the handle of uuid16 that find
 */
u16 blm_att_findHandleOfUuid16 (att_db_uuid16_t *p, u16 uuid, u16 ref);

/**
 * @brief       this function serves to find handle of uuid128
 * @param[in]	p - pointer of data attribute
 * @param[in]	uuid - pointer of uuid
 * @return      0 - fail to find handle of uuid128
 *              1 - the handle of uuid128 that find
 */
u16 blm_att_findHandleOfUuid128 (att_db_uuid128_t *p, const u8 * uuid);

/**
 * @brief       host layer wait service
 * @param[in]	handle - connect handle
 * @param[in]	p - pointer of data event
 * @param[in]	timeout
 * @return      0
 */
int host_att_service_wait_event (u16 handle, u8 *p, u32 timeout);

/**
 * @brief     SDP loop
 * @param[in]  none.
 * @return     none.
 */
void simple_sdp_loop (void);

/**
 * @brief      callback function of service discovery
 * @param[in]  none
 * @return     none
 */
void app_service_discovery (void);


/**
 * @brief		This function is used to register a callback function to getting some ATT handle with the service discovery result.
 * @param[in]	cb - callback function
 * @return      none.
 */
void app_sdp_register_get_att_handle_callback(sdp_get_handle_callback_t cb);



#endif //end of ACL_CENTRAL_SIMPLE_SDP_ENABLE


#endif /* SIMPLE_SDP_H_ */
