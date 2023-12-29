/********************************************************************************************************
 * @file    simple_sdp.c
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

#include "simple_sdp.h"




#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)

ble_sts_t  host_att_discoveryService (u16 handle, att_db_uuid16_t *p16, int n16, att_db_uuid128_t *p128, int n128);

main_service_t	main_service = 0;

sdp_get_handle_callback_t  sdp_get_handle_cb = NULL;


#define				ATT_DB_UUID16_NUM		20
#define				ATT_DB_UUID128_NUM		8

int	central_sdp_pending = 0;			// SDP: service discovery

u8 	conn_char_handler[11] = {0};
u8	serviceDiscovery_adr_type;
u8	serviceDiscovery_address[6];

const u8 my_MicUUID[16]			= WRAPPING_BRACES(TELINK_MIC_DATA);
const u8 my_SpeakerUUID[16]		= WRAPPING_BRACES(TELINK_SPEAKER_DATA);
const u8 my_OtaUUID[16]			= WRAPPING_BRACES(TELINK_SPP_DATA_OTA);
const u8 my_SppS2CUUID[16]		= WRAPPING_BRACES(TELINK_SPP_DATA_SERVER2CLIENT);
const u8 my_SppC2SUUID[16]		= WRAPPING_BRACES(TELINK_SPP_DATA_CLIENT2SERVER);


/**
 * @brief      callback function of service discovery
 * @param[in]  none
 * @return     none
 */
void app_service_discovery(void)
{

	att_db_uuid16_t 	db16[ATT_DB_UUID16_NUM];
	att_db_uuid128_t 	db128[ATT_DB_UUID128_NUM];
	memset (db16, 0, ATT_DB_UUID16_NUM * sizeof (att_db_uuid16_t));
	memset (db128, 0, ATT_DB_UUID128_NUM * sizeof (att_db_uuid128_t));

	if ( IS_CONNECTION_HANDLE_VALID(cur_conn_device.conn_handle) && \
		 host_att_discoveryService (cur_conn_device.conn_handle, db16, ATT_DB_UUID16_NUM, db128, ATT_DB_UUID128_NUM) == BLE_SUCCESS)	// service discovery OK
	{
		//int h = current_connHandle & 7;
		conn_char_handler[0] = blm_att_findHandleOfUuid128 (db128, my_MicUUID);			//MIC
		conn_char_handler[1] = blm_att_findHandleOfUuid128 (db128, my_SpeakerUUID);		//Speaker
		conn_char_handler[2] = blm_att_findHandleOfUuid128 (db128, my_OtaUUID);			//OTA


		conn_char_handler[3] = blm_att_findHandleOfUuid16 (db16, CHARACTERISTIC_UUID_HID_REPORT,
					HID_REPORT_ID_CONSUME_CONTROL_INPUT | (HID_REPORT_TYPE_INPUT<<8));		//consume report

		conn_char_handler[4] = blm_att_findHandleOfUuid16 (db16, CHARACTERISTIC_UUID_HID_REPORT,
					HID_REPORT_ID_KEYBOARD_INPUT | (HID_REPORT_TYPE_INPUT<<8));				//normal key report in

		conn_char_handler[5] = blm_att_findHandleOfUuid16 (db16, CHARACTERISTIC_UUID_HID_REPORT,
					HID_REPORT_ID_MOUSE_INPUT | (HID_REPORT_TYPE_INPUT<<8));				//mouse report

		conn_char_handler[6] = blm_att_findHandleOfUuid16 (db16, CHARACTERISTIC_UUID_HID_REPORT,
					HID_REPORT_ID_KEYBOARD_INPUT | (HID_REPORT_TYPE_OUTPUT<<8));				//normal key report out

		conn_char_handler[7] = blm_att_findHandleOfUuid16 (db16, CHARACTERISTIC_UUID_HID_REPORT,
					HID_REPORT_ID_AUDIO_FIRST_INPUT | (HID_REPORT_TYPE_INPUT<<8));				//Audio first report


		if(sdp_get_handle_cb){
			sdp_get_handle_cb(db16, db128);
		}

		//save current service discovery conn address
		serviceDiscovery_adr_type = cur_conn_device.mac_adrType;
		memcpy(serviceDiscovery_address, cur_conn_device.mac_addr, 6);
	}

	central_sdp_pending = 0;  //service discovery finish

}

void simple_sdp_loop (void)
{
	if (main_service)
	{
		main_service ();
		main_service = 0;
	}
}

/**
 * @brief       This function is used to register SDP handler.
 * @param[in]   p       - Pointer point to SDP handler.
 * @return      none.
 */
void app_register_service (void *p)
{
	main_service = p;
}

/**
 * @brief		This function is used to register a callback function to getting some ATT handle with the service discovery result.
 * @param[in]	cb - callback function
 * @return      none.
 */
void app_sdp_register_get_att_handle_callback(sdp_get_handle_callback_t cb)
{
	sdp_get_handle_cb = cb;
}


u8	*p_att_response = 0;

volatile u32	host_att_req_busy = 0;

/**
 * @brief       This function is used to process ATT packets related to SDP
 * @param[in]   connHandle  - connection handle
 * @param[in]   p           - Pointer point to ATT data buffer.
 * @return
 */
int host_att_client_handler (u16 connHandle, u8 *p)
{
	att_readByTypeRsp_t *p_rsp = (att_readByTypeRsp_t *) p;
	if (p_att_response)
	{
		if ((connHandle & 7) == (host_att_req_busy & 7) && p_rsp->chanId == 0x04 &&
			(p_rsp->opcode == 0x01 || p_rsp->opcode == ((host_att_req_busy >> 16) | 1)))
		{
			memcpy (p_att_response, p, p_rsp->l2capLen+6);
			host_att_req_busy = 0;
		}
	}
	return 0;
}

typedef int (*host_att_idle_func_t) (void);
host_att_idle_func_t host_att_idle_func = 0;


/**
 * @brief       This function is used to register ble stack mainloop function.
 * @param[in]   p           - Pointer point to ble stack mainloop function.
 * @return
 */
int host_att_register_idle_func (void *p)
{
	if (host_att_idle_func)
		return 1;

	host_att_idle_func = p;
	return 0;
}

/**
 * @brief       host layer response
 * @param[in]	none
 * @return      none
 */
int host_att_response ()
{
	return host_att_req_busy == 0;
}

/**
 * @brief       host layer wait service
 * @param[in]	handle - connect handle
 * @param[in]	p - pointer of data event
 * @param[in]	timeout
 * @return      0
 */
int host_att_service_wait_event (u16 handle, u8 *p, u32 timeout)
{
	host_att_req_busy = handle | (p[6] << 16);
	p_att_response = p;

	u32 t = clock_time ();
	while (!clock_time_exceed (t, timeout))
	{
		if (host_att_response ())
		{
			return 0;
		}
		if (host_att_idle_func)
		{
			if (host_att_idle_func ())
			{
				break;
			}
		}
	}
	return 1;
}

/**
 * @brief       this function serves to find handle of uuid16
 * @param[in]	p - pointer of data attribute
 * @param[in]	uuid
 * @param[in]	ref - HID Report
 * @return      0 - fail to find handle of uuid16
 *              1 - the handle of uuid16 that find
 */
u16 blm_att_findHandleOfUuid16 (att_db_uuid16_t *p, u16 uuid, u16 ref)
{
	for (int i=0; i<p->num; i++)
	{
		if (p[i].uuid == uuid && p[i].ref == ref)
		{
			return p[i].handle;
		}
	}
	return 0;
}

/**
 * @brief       this function serves to find handle of uuid128
 * @param[in]	p - pointer of data attribute
 * @param[in]	uuid - pointer of uuid
 * @return      0 - fail to find handle of uuid128
 *              1 - the handle of uuid128 that find
 */
u16 blm_att_findHandleOfUuid128 (att_db_uuid128_t *p, const u8 * uuid)
{
	for (int i=0; i<p->num; i++)
	{
		if (memcmp (p[i].uuid, uuid, 16) == 0)
		{
			return p[i].handle;
		}
	}
	return 0;
}

int app_char_discovery(u8* result, u16 connHandle, u16 startAttHandle, u16 endAttHandle, u8*uuid, u8 uuidLen)
{
	blc_gatt_pushReadByTypeRequest(connHandle, startAttHandle, endAttHandle, uuid, uuidLen);
	return host_att_service_wait_event(connHandle,result,1000000);
}

int app_find_char_info(u8* result, u16 connHandle, u16 startAttHandle, u16 endAttHandle)
{
	blc_gatt_pushFindInformationRequest(connHandle, startAttHandle, endAttHandle);

	return host_att_service_wait_event(connHandle, result, 1000000);
}

int app_read_char_value(u8* result, u16 connHandle, u16 attHandle)
{
	blc_gatt_pushReadRequest(connHandle, attHandle);

	return host_att_service_wait_event(connHandle, result, 1000000);
}

/**
 * @brief       host layer discovery service
 * @param[in]	handle - connect handle
 * @param[in]	p16 - pointer of data attribute
 * @param[in]	n16 - attribute uuid16 num
 * @param[in]	p128 - pointer of data attribute
 * @param[in]   n128 - attribute uuid128 num
 * @return      ble status
 */
ble_sts_t  host_att_discoveryService (u16 handle, att_db_uuid16_t *p16, int n16, att_db_uuid128_t *p128, int n128)
{
	att_db_uuid16_t *ps16 = p16;
	att_db_uuid128_t *ps128 = p128;
	int i16 = 0;
	int i128 = 0;

	ps16->num = 0;
	ps128->num = 0;

	// char discovery: att_read_by_type
		// hid discovery: att_find_info
	u8  dat[256];		//247(MTU) +2(LL Data channel PDU header) +4(L2CAP header) +3(align(4))
	u16 s = 1;
	u16 uuid = GATT_UUID_CHARACTER;
	do {

		dat[6] = ATT_OP_READ_BY_TYPE_REQ;
		if(app_char_discovery(dat, handle, s, 0xffff, (u8 *)&uuid, 2))
		{
			return GATT_ERR_SERVICE_DISCOVERY_TIMEOUT;
		}



		// process response data
		att_readByTypeRsp_t *p_rsp = (att_readByTypeRsp_t *) dat;
		if (p_rsp->opcode != ATT_OP_READ_BY_TYPE_RSP)
		{
			break;
		}

		if (p_rsp->datalen == 21)		//uuid128
		{
			u8 *pd = p_rsp->data;
			while (p_rsp->l2capLen > 21)
			{
				s = pd[3] + pd[4] * 256;
				if (i128 < n128)
				{
					p128->property = pd[2];
					p128->handle = s;
					memcpy (p128->uuid, pd + 5, 16);
					i128++;
					p128++;
				}
				p_rsp->l2capLen -= 21;
				pd += 21;
			}
		}
		else if (p_rsp->datalen == 7) //uuid16
		{
			u8 *pd = p_rsp->data;
			while (p_rsp->l2capLen > 7)
			{
				s = pd[3] + pd[4] * 256;
				if (i16 < n16)
				{
					p16->property = pd[2];
					p16->handle = s;
					p16->uuid = pd[5] | (pd[6] << 8);
					p16->ref = 0;
					i16 ++;
					p16++;
				}
				p_rsp->l2capLen -= 7;
				pd += 7;
			}
		}
	} while (1);

	ps16->num = i16;
	ps128->num = i128;

	//--------- use att_find_info to find the reference property for hid ----------
	p16 = ps16;
	for (int i=0; i<i16; i++)
	{
		if (p16->uuid == CHARACTERISTIC_UUID_HID_REPORT)		//find reference
		{
			dat[6] = ATT_OP_FIND_INFO_REQ;
			if (app_find_char_info(dat, handle, p16->handle, 0xffff))
			{
				return  GATT_ERR_SERVICE_DISCOVERY_TIMEOUT;			//timeout
			}

			att_findInfoRsp_t *p_rsp = (att_findInfoRsp_t *) dat;
			if (p_rsp->opcode == ATT_OP_FIND_INFO_RSP && p_rsp->format == 1)
			{
				int n = p_rsp->l2capLen - 2;
				u8 *pd = p_rsp->data;
				while (n > 0)
				{
					if ((pd[2]==U16_LO(GATT_UUID_CHARACTER) && pd[3]==U16_HI(GATT_UUID_CHARACTER)) ||
						(pd[2]==U16_LO(GATT_UUID_PRIMARY_SERVICE) && pd[3]==U16_HI(GATT_UUID_PRIMARY_SERVICE))	)
					{
						break;
					}

					if (pd[2]==U16_LO(GATT_UUID_REPORT_REF) && pd[3]==U16_HI(GATT_UUID_REPORT_REF))
					{
					//-----------		read attribute ----------------

						dat[6] = ATT_OP_READ_REQ;
						if (app_read_char_value(dat, handle, pd[0]))
						{
							return  GATT_ERR_SERVICE_DISCOVERY_TIMEOUT;			//timeout
						}

						att_readRsp_t *pr = (att_readRsp_t *) dat;
						if (pr->opcode == ATT_OP_READ_RSP)
						{
							p16->ref = pr->value[0] | (pr->value[1] << 8);
						}

						break;
					}
					n -= 4;
					pd += 4;
				}
			}
		} //----- end for if CHARACTERISTIC_UUID_HID_REPORT

		p16++;
	}

	return  BLE_SUCCESS;
}


#endif
