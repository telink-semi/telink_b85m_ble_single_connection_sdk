/********************************************************************************************************
 * @file	blm_att.c
 *
 * @brief	This is the source file for BLE SDK
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app.h"
#include "blm_att.h"
#include "blm_pair.h"
#include "blm_host.h"

#include "application/keyboard/keyboard.h"
#include "application/rf_frame.h"

#if (FEATURE_TEST_MODE == TEST_LL_PRIVACY_MASTER)

#define     TELINK_UNPAIR_KEYVALUE		0xFF  //conn state, unpair


const u8 my_MicUUID[16]		= WRAPPING_BRACES(TELINK_MIC_DATA);
const u8 my_SpeakerUUID[16]	= WRAPPING_BRACES(TELINK_SPEAKER_DATA);
const u8 my_OtaUUID[16]		= WRAPPING_BRACES(TELINK_SPP_DATA_OTA);
const u8 my_SppS2CUUID[16]		= WRAPPING_BRACES(TELINK_SPP_DATA_SERVER2CLIENT);
const u8 my_SppC2SUUID[16]		= WRAPPING_BRACES(TELINK_SPP_DATA_CLIENT2SERVER);

const u8 sAudioGoogleTXUUID[16]   = WRAPPING_BRACES(AUDIO_GOOGL_TX_CHAR_UUID);
const u8 sAudioGoogleRXUUID[16]   = WRAPPING_BRACES(AUDIO_GOOGL_RX_CHAR_UUID);
const u8 sAudioGoogleCTLUUID[16]   = WRAPPING_BRACES(AUDIO_GOOGL_CTL_CHAR_UUID);

u8 read_by_type_req_uuid[16] = {};
u8 read_by_type_req_uuidLen;

u16 	current_read_req_handle;

/**
 * @brief       host layer set current readByTypeRequest UUID
 * @param[in]	uuid
 * @param[in]	uuid_len - uuid byte number
 * @return      none
 */
void host_att_set_current_readByTypeReq_uuid(u8 *uuid, u8 uuid_len)
{
	read_by_type_req_uuidLen = uuid_len;
	memcpy(read_by_type_req_uuid, uuid, uuid_len);
}







u8	*p_att_response = 0;

volatile u32	host_att_req_busy = 0;

/**
 * @brief       host layer client handle
 * @param[in]	connHandle - connect handle
 * @param[in]	p - Pointer point to event parameter.
 * @return      none
 */
int host_att_client_handler (u16 connHandle, u8 *p)
{
	att_readByTypeRsp_t *p_rsp = (att_readByTypeRsp_t *) p;
	if (p_att_response)
	{
		if ((connHandle & 7) == (host_att_req_busy & 7) && p_rsp->chanId == 0x04 &&
				(p_rsp->opcode == 0x01 || p_rsp->opcode == ((host_att_req_busy >> 16) | 1)))
		{
			memcpy (p_att_response, p, p_rsp->l2capLen + 6);	//+6 indicate type(1B)+rf_len(1B)+l2cap_len
			host_att_req_busy = 0;
		}
	}
	return 0;
}

/**
 * @brief       host layer clear service discovery
 * @param[in]	none
 * @return      none
 */
void host_att_service_disccovery_clear(void)
{
	p_att_response = 0;
}

typedef int (*host_att_idle_func_t) (void);
host_att_idle_func_t host_att_idle_func = 0;

/**
 * @brief       host layer clear service discovery
 * @param[in]	p - Pointer point to main idle loop function.
 * @return      none
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

	extern bool		blm_push_fifo (int connHandle, u8 *dat);
	blm_push_fifo (handle, p);

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

		att_req_read_by_type (dat, s, 0xffff, (u8 *)&uuid, 2);
		if (host_att_service_wait_event(handle, dat, 1000000))
		{
			return  GATT_ERR_SERVICE_DISCOVERY_TIEMOUT;			//timeout
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

			att_req_find_info (dat, p16->handle, 0xffff);
			if (host_att_service_wait_event(handle, dat, 1000000))
			{
				return  GATT_ERR_SERVICE_DISCOVERY_TIEMOUT;			//timeout
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

						att_req_read (dat, pd[0]);
						if (host_att_service_wait_event(handle, dat, 1000000))
						{
								return  GATT_ERR_SERVICE_DISCOVERY_TIEMOUT;			//timeout
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







rf_packet_mouse_t	pkt_mouse = {
		sizeof (rf_packet_mouse_t) - 4,	// dma_len

		sizeof (rf_packet_mouse_t) - 5,	// rf_len
		RF_PROTO_BYTE,		// proto
		PKT_FLOW_DIR,		// flow
		FRAME_TYPE_MOUSE,					// type

//		U32_MAX,			// gid0

		0,					// rssi
		0,					// per
		0,					// seq_no
		1,					// number of frame
};


extern void usbmouse_add_frame (rf_packet_mouse_t *packet_mouse);

/**
 * @brief       call this function when attribute handle:HID_HANDLE_MOUSE_REPORT
 * @param[in]	conn - connect handle
 * @param[in]	p - pointer of l2cap data packet
 * @return      none
 */
void	att_mouse (u16 conn, u8 *p)
{
	memcpy (pkt_mouse.data, p, 4);
	pkt_mouse.seq_no++;
    usbmouse_add_frame(&pkt_mouse);
}



extern void usbkb_hid_report(kb_data_t *data);
extern void report_to_KeySimTool(u8 len,u8 * keycode);
extern void usbkb_report_consumer_key(u16 consumer_key);

extern void report_media_key_to_KeySimTool(u16);

/**
 * @brief       call this function when report consumer key
 * @param[in]	conn - connect handle
 * @param[in]	p - pointer of l2cap data packet
 * @return      none
 */
void	att_keyboard_media (u16 conn, u8 *p)
{
	u16 media_key = p[0] | p[1]<<8;

	usbkb_report_consumer_key(media_key);
}

//////////////// keyboard ///////////////////////////////////////////////////
int Adbg_att_kb_cnt = 0;
kb_data_t		kb_dat_report = {1, 0, {0,0,0,0,0,0} };
int keyboard_not_release = 0;
extern int 	dongle_unpair_enable;

/**
 * @brief       call this function when report keyborad
 * @param[in]	conn - connect handle
 * @param[in]	p - pointer of l2cap data packet
 * @return      none
 */
void	att_keyboard (u16 conn, u8 *p)
{
	Adbg_att_kb_cnt ++;

	memcpy(&kb_dat_report, p, sizeof(kb_data_t));

	if(kb_dat_report.keycode[0] == TELINK_UNPAIR_KEYVALUE){ //slave special unpair cmd

		if(!dongle_unpair_enable){
			dongle_unpair_enable = 1;
		}

		return;  //TELINK_UNPAIR_KEYVALUE not report
	}


	if (kb_dat_report.keycode[0])  			//keycode[0]
	{
		kb_dat_report.cnt = 1;  //1 key value
		keyboard_not_release = 1;
	}
	else{
		kb_dat_report.cnt = 0;  //key release
		keyboard_not_release = 0;
	}


	usbkb_hid_report((kb_data_t *) &kb_dat_report);
}



/**
 * @brief       call this function when keyboard release
 * @param[in]	none
 * @return      none
 */
void att_keyboard_release(void)
{
	kb_dat_report.cnt = 0;  //key release
//	usbkb_hid_report((kb_data_t *) &kb_dat_report);
}



////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

/**
 * @brief       this function serves to clear host attribute data
 * @param[in]	none
 * @return      none
 */
void host_att_data_clear(void)
{
	if(keyboard_not_release){
		keyboard_not_release = 0;
		att_keyboard_release();
	}
}


/**
 * @brief       this function serves to set current ReadRequest attribute handle
 * @param[in]	handle - connect handle
 * @return      none
 */
void app_setCurrentReadReq_attHandle(u16 handle)
{
	current_read_req_handle = handle;
}

/**
 * @brief       this function serves to get current ReadRequest attribute handle
 * @param[in]	none
 * @return      current ReadRequest attribute handle
 */
u16 app_getCurrentReadReq_attHandle(void)
{
	return current_read_req_handle;
}

#endif
