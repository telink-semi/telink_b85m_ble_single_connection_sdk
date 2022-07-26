/********************************************************************************************************
 * @file	blm_host.c
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
#include "blm_ota.h"

#include "application/audio/audio_config.h"
#include "application/audio/tl_audio.h"
#include "app_audio.h"

main_service_t		main_service = 0;

#define SMP_PENDING					1   //security management
#define SDP_PENDING					2   //service discovery

int	app_host_smp_sdp_pending = 0; 		//security & service discovery


extern u8 read_by_type_req_uuidLen;
extern u8 read_by_type_req_uuid[16];
extern bool		blm_push_fifo (int connHandle, u8 *dat);


dev_char_info_t cur_conn_device;

////////////////////////////////////////////////////////////////////
u32 host_update_conn_param_req = 0;
u16 host_update_conn_min;
u16 host_update_conn_latency;
u16 host_update_conn_timeout;

u16 final_MTU_size = 23;

int master_connected_led_on = 0;




int master_auto_connect = 0;
int user_manual_paring;



const u8 	telink_adv_trigger_paring[] = {5, 0xFF, 0x11, 0x02, 0x01, 0x00};
const u8 	telink_adv_trigger_unpair[] = {5, 0xFF, 0x11, 0x02, 0x01, 0x01};

const u8 	telink_adv_trigger_paring_8258[] = {7, 0xFF, 0x11, 0x02, 0x01, 0x00, 0x58, 0x82};
const u8 	telink_adv_trigger_unpair_8258[] = {7, 0xFF, 0x11, 0x02, 0x01, 0x01, 0x58, 0x82};


#if (BLE_HOST_SIMPLE_SDP_ENABLE)
	extern void host_att_service_disccovery_clear(void);
	int host_att_client_handler (u16 connHandle, u8 *p);
	ble_sts_t  host_att_discoveryService (u16 handle, att_db_uuid16_t *p16, int n16, att_db_uuid128_t *p128, int n128);


	#define				ATT_DB_UUID16_NUM		20
	#define				ATT_DB_UUID128_NUM		8

	u8 	conn_char_handler[11] = {0};


	u8	serviceDiscovery_adr_type;
	u8	serviceDiscovery_address[6];


	extern const u8 my_MicUUID[16];
	extern const u8 my_SpeakerUUID[16];
	extern const u8 my_OtaUUID[16];
	extern const u8 sAudioGoogleTXUUID[16];
	extern const u8 sAudioGoogleRXUUID[16];
	extern const u8 sAudioGoogleCTLUUID[16];
#if(TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE)
	u8 google_voice_model = 0;
#endif

#if	LL_FEATURE_ENABLE_LL_PRIVACY
	#if (MASTER_RESOLVABLE_ADD_EN)
		void att_rsp_read_by_type (u8 *p, u8 pair_length, u16 attHandle, u8 value)
		{
			p[0] = 2;
			p[1] = 9 ;
			p[2] = 5 ;
			p[3] = 0;
			p[4] = 4;
			p[5] = 0;
			p[6] = ATT_OP_READ_BY_TYPE_RSP;
			p[7] = pair_length;
			p[8] = attHandle;
			p[9] = attHandle>>8;
			p[10]= value;
		}
	#endif
#endif

	/**
	 * @brief      callback function of service discovery
	 * @param[in]  none
	 * @return     none
	 */
	void app_service_discovery ()
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

#if(TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE)
			conn_char_handler[8] = blm_att_findHandleOfUuid128 (db128, sAudioGoogleTXUUID);
			conn_char_handler[9] = blm_att_findHandleOfUuid128 (db128, sAudioGoogleRXUUID);
			conn_char_handler[10] = blm_att_findHandleOfUuid128 (db128, sAudioGoogleCTLUUID);
#endif

			//save current service discovery conn address
			serviceDiscovery_adr_type = cur_conn_device.mac_adrType;
			memcpy(serviceDiscovery_address, cur_conn_device.mac_addr, 6);

#if(TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE)
		#if (GOOGLE_VOICE_OVER_BLE_SPCE_VERSION == GOOGLE_VERSION_1_0)
 			u8 dat[32]={0};
 			u8 caps_data[6]={0};
 			caps_data[0] = 0x0A; //get caps
 			caps_data[1] = 0x00;
 			caps_data[2] = 0x01; // version 0x0100;
 			caps_data[3] = 0x00;
 			caps_data[4] = 0x03; // legacy 0x0003;
			caps_data[5] = GOOGLE_VOICE_MODE;
 			att_req_write_cmd(dat, conn_char_handler[8], caps_data, 6);/*AUDIO_GOOGLE_TX_DP_H*/
			blm_push_fifo(BLM_CONN_HANDLE, dat);

			u8 rx_ccc[2] = {0x00, 0x01};		//Write Rx CCC value for PTV use case
			att_req_write_cmd(dat, conn_char_handler[9]+1, rx_ccc, 2);
			blm_push_fifo(BLM_CONN_HANDLE, dat);
		#else
			u8 dat[32]={0};
			u8 caps_data[5]={0};
			caps_data[0] = 0x0A; //get caps
			caps_data[1] = 0x00;
			caps_data[2] = 0x04; // version 0x0004;
			caps_data[3] = 0x00;
			caps_data[4] = 0x03; // legacy 0x0003;
			att_req_write_cmd(dat, conn_char_handler[8], caps_data, 5);/*AUDIO_GOOGLE_TX_DP_H*/
			blm_push_fifo(BLM_CONN_HANDLE, dat);
		#endif
#endif
		}

		app_host_smp_sdp_pending = 0;  //service discovery finish

	}

	void app_register_service (void *p)
	{
		main_service = p;
	}




	#define			HID_HANDLE_CONSUME_REPORT			conn_char_handler[3]
	#define			HID_HANDLE_KEYBOARD_REPORT			conn_char_handler[4]
	#define			HID_HANDLE_MOUSE_REPORT				conn_char_handler[5]
	#define			AUDIO_HANDLE_MIC					conn_char_handler[0]
	#define			HID_HANDLE_KEYBOARD_REPORT_OUT		conn_char_handler[6]
	#define			AUDIO_FIRST_REPORT					conn_char_handler[7]

	#if(TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE)

	#define 		GOOGLE_AUDIO_HANDLE_MIC_CMD			conn_char_handler[8]//52
	#define 		GOOGLE_AUDIO_HANDLE_MIC_DATA		conn_char_handler[9]//54
	#define 		GOOGLE_AUDIO_HANDLE_MIC_RSP			conn_char_handler[10]//57
	#else
	#endif

#else  //no service discovery

	//need define att handle same with slave
	#define 		HID_HANDLE_MOUSE_REPORT
	#define			HID_HANDLE_CONSUME_REPORT			25
	#define			HID_HANDLE_KEYBOARD_REPORT			29
	#define			AUDIO_HANDLE_MIC					52


#endif




#if (BLE_HOST_SMP_ENABLE)
/**
 * @brief      callback function of smp finish
 * @param[in]  none
 * @return     0
 */
int app_host_smp_finish (void)  //smp finish callback
{
	#if (BLE_HOST_SIMPLE_SDP_ENABLE)  //smp finish, start sdp
		if(app_host_smp_sdp_pending == SMP_PENDING)
		{
			//new slave device, should do service discovery again
			if (cur_conn_device.mac_adrType != serviceDiscovery_adr_type || \
				memcmp(cur_conn_device.mac_addr, serviceDiscovery_address, 6))
			{
				app_register_service(&app_service_discovery);
				app_host_smp_sdp_pending = SDP_PENDING; //service discovery busy
			}
			else
			{
				app_host_smp_sdp_pending = 0;  //no need sdp
#if(TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE)
			#if (GOOGLE_VOICE_OVER_BLE_SPCE_VERSION == GOOGLE_VERSION_1_0)
				u8 dat[32]={0};
				u8 caps_data[6]={0};
				caps_data[0] = 0x0A; //get caps
				caps_data[1] = 0x01;
				caps_data[2] = 0x00; // version 0x0100;
				caps_data[3] = 0x00;
				caps_data[4] = 0x03; // legacy 0x0003;
				caps_data[5] = GOOGLE_VOICE_MODE;
				att_req_write_cmd(dat, conn_char_handler[8], caps_data, 6);/*AUDIO_GOOGLE_TX_DP_H*/
				blm_push_fifo(BLM_CONN_HANDLE, dat);

				u8 rx_ccc[2] = {0x00, 0x01};		//Write Rx CCC value for PTV use case
				att_req_write_cmd(dat, conn_char_handler[9]+1, rx_ccc, 2);
				blm_push_fifo(BLM_CONN_HANDLE, dat);
			#else
				u8 dat[32]={0};
				u8 caps_data[5]={0};
				caps_data[0] = 0x0A; //get caps
				caps_data[1] = 0x00;
				caps_data[2] = 0x04; // version 0x0004;
				caps_data[3] = 0x00;
				caps_data[4] = 0x03; // legacy 0x0003;
				att_req_write_cmd(dat, conn_char_handler[8], caps_data, 5);/*AUDIO_GOOGLE_TX_DP_H*/
				blm_push_fifo(BLM_CONN_HANDLE, dat);
			#endif
#endif
			}
		}
	#else
		app_host_smp_sdp_pending = 0;  //no sdp
	#endif

	return 0;
}
#endif



/**
 * @brief      call this function when  HCI Controller Event :HCI_SUB_EVT_LE_ADVERTISING_REPORT
 *     		   after controller is set to scan state, it will report all the adv packet it received by this event
 * @param[in]  p - data pointer of event
 * @return     0
 */
int blm_le_adv_report_event_handle(u8 *p)
{
	event_adv_report_t *pa = (event_adv_report_t *)p;
	s8 rssi = pa->data[pa->len];

	 //if previous connection smp&sdp not finish, can not create a new connection
	if(app_host_smp_sdp_pending){
		return 1;
	}

	/****************** Button press or Adv pair packet triggers pair ***********************/
	int master_auto_connect = 0;
	int user_manual_paring = 0;

	//manual paring methods 1: button triggers
	user_manual_paring = dongle_pairing_enable && (rssi > -56);  //button trigger pairing(rssi threshold, short distance)

	//manual paring methods 2: special paring adv data
	if(!user_manual_paring){  //special adv pair data can also trigger pairing
		user_manual_paring = (memcmp(pa->data, telink_adv_trigger_paring_8258, sizeof(telink_adv_trigger_paring_8258)) == 0) && (rssi > -56);
	}


	#if (BLE_HOST_SMP_ENABLE)
		master_auto_connect = tbl_bond_slave_search(pa->adr_type, pa->mac);
	#else
		//search in slave mac table to find whether this device is an old device which has already paired with master
		master_auto_connect = user_tbl_slave_mac_search(pa->adr_type, pa->mac);
	#endif

	if(master_auto_connect || user_manual_paring)
	{
#if	LL_FEATURE_ENABLE_LL_PRIVACY
		#if (MASTER_RESOLVABLE_ADD_EN)
		printf("OK!\n");
		u8 status;
		extern bond_slave_t  tbl_bondSlave;
		if(tbl_bondSlave.curNum != 0)
		{

			status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS, INITIATE_FP_ADV_WL,  \
									 pa->adr_type, pa->mac, OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC, \
									 CONN_INTERVAL_10MS, CONN_INTERVAL_10MS, 0, CONN_TIMEOUT_4S, \
									 0, 0xFFFF);
		}
		else
		{
			status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS, INITIATE_FP_ADV_SPECIFY,  \
									 pa->adr_type, pa->mac, OWN_ADDRESS_PUBLIC, \
									 CONN_INTERVAL_10MS, CONN_INTERVAL_10MS, 0, CONN_TIMEOUT_4S, \
									 0, 0xFFFF);
		}
		#else

		u8 status;
		extern bond_slave_t  tbl_bondSlave;
		if(tbl_bondSlave.curNum != 0)
		{
			status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS, INITIATE_FP_ADV_SPECIFY,  \
									 pa->adr_type, pa->mac, OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC, \
									 CONN_INTERVAL_10MS, CONN_INTERVAL_10MS, 0, CONN_TIMEOUT_4S, \
									 0, 0xFFFF);
		}
		else
		{
			status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS, INITIATE_FP_ADV_SPECIFY,  \
									 pa->adr_type, pa->mac, OWN_ADDRESS_PUBLIC, \
									 CONN_INTERVAL_10MS, CONN_INTERVAL_10MS, 0, CONN_TIMEOUT_4S, \
									 0, 0xFFFF);
		}
		#endif

#else
			//send create connection cmd to controller, trigger it switch to initiating state, after this cmd,
			//controller will scan all the adv packets it received but not report to host, to find the specified
			//device(adr_type & mac), then send a connection request packet after 150us, enter to connection state
			// and send a connection complete event(HCI_SUB_EVT_LE_CONNECTION_COMPLETE)
			u8 status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS, INITIATE_FP_ADV_SPECIFY,  \
									 pa->adr_type, pa->mac, BLE_ADDR_PUBLIC, \
									 CONN_INTERVAL_10MS, CONN_INTERVAL_10MS, 0, CONN_TIMEOUT_4S, \
									 0, 0xFFFF);

#endif
		if(status == BLE_SUCCESS)   //create connection success
		{
			#if (!BLE_HOST_SMP_ENABLE)
				if(user_manual_paring && !master_auto_connect){  //manual pair
					blm_manPair.manual_pair = 1;
					blm_manPair.mac_type = pa->adr_type;
					memcpy(blm_manPair.mac, pa->mac, 6);
					blm_manPair.pair_tick = clock_time();
				}
			#endif
		}

	}


	/****************** Adv unpair packet triggers unpair ***********************/
	int adv_unpair_en = !memcmp(pa->data, telink_adv_trigger_unpair_8258, sizeof(telink_adv_trigger_unpair_8258));
	if(adv_unpair_en)
	{
		int device_is_bond;

		#if (BLE_HOST_SMP_ENABLE)
			device_is_bond = tbl_bond_slave_search(pa->adr_type, pa->mac);
			if(device_is_bond){ //this adv mac is bonded in master
				tbl_bond_slave_delete_by_adr(pa->adr_type, pa->mac);  //by telink stack host smp
			}
		#else
			device_is_bond = user_tbl_slave_mac_search(pa->adr_type, pa->mac);
			if(device_is_bond){ //this adv mac is bonded in master
				user_tbl_slave_mac_delete_by_adr(pa->adr_type, pa->mac);  //by user application code
			}
		#endif
	}


	return 0;
}


/**
 * @brief		this connection event is defined by telink, not a standard ble controller event
 * 				after master controller send connection request packet to slave, when slave received this packet
 * 				and enter to connection state, send a ack packet within 6 connection event, master will send
 *              connection establish event to host(HCI_SUB_EVT_LE_CONNECTION_ESTABLISH)
 * @param[in]	p - data pointer of event
 * @return      none
 */
int blm_le_connection_establish_event_handle(u8 *p)
{

	event_connection_complete_t *pCon = (event_connection_complete_t *)p;
	if (pCon->status == BLE_SUCCESS)	// status OK
	{
		#if (UI_LED_ENABLE)
			//led show connection state
			master_connected_led_on = 1;
			gpio_write(GPIO_LED_RED, LED_ON_LEVAL);     //red on
			gpio_write(GPIO_LED_WHITE, !LED_ON_LEVAL);  //white off
		#endif


		cur_conn_device.conn_handle = pCon->handle;   //mark conn handle, in fact this equals to BLM_CONN_HANDLE

		//save current connect address type and address
		cur_conn_device.mac_adrType = pCon->peer_adr_type;
		memcpy(cur_conn_device.mac_addr, pCon->mac, 6);


		#if (BLE_HOST_SMP_ENABLE)
			app_host_smp_sdp_pending = SMP_PENDING; //pair & security first
		#else


			//manual paring, device match, add this device to slave mac table
			if(blm_manPair.manual_pair && blm_manPair.mac_type == pCon->peer_adr_type && !memcmp(blm_manPair.mac, pCon->mac, 6)){
				blm_manPair.manual_pair = 0;

				user_tbl_slave_mac_add(pCon->peer_adr_type, pCon->mac);
			}


				#if (BLE_HOST_SIMPLE_SDP_ENABLE)
						//new slave device, should do service discovery again
						if (pCon->peer_adr_type != serviceDiscovery_adr_type || memcmp(pCon->mac, serviceDiscovery_address, 6)){
							app_register_service(&app_service_discovery);
							app_host_smp_sdp_pending = SDP_PENDING;  //service discovery busy
						}
						else{
							app_host_smp_sdp_pending = 0;  //no need sdp
						}
				#endif
		#endif
	}



	return 0;
}




/**
 * @brief		this function serves to connect terminate
 * @param[in]	p - data pointer of event
 * @return      none
 */
int 	blm_disconnect_event_handle(u8 *p)
{
	event_disconnection_t	*pd = (event_disconnection_t *)p;

	//terminate reason
	//connection timeout
	if(pd->reason == HCI_ERR_CONN_TIMEOUT){

	}
	//peer device(slave) send terminate cmd on link layer
	else if(pd->reason == HCI_ERR_REMOTE_USER_TERM_CONN){

	}
	//master host disconnect( blm_ll_disconnect(current_connHandle, HCI_ERR_REMOTE_USER_TERM_CONN) )
	else if(pd->reason == HCI_ERR_CONN_TERM_BY_LOCAL_HOST){

	}
	 //master create connection, send conn_req, but did not received acked packet in 6 connection event
	else if(pd->reason == HCI_ERR_CONN_FAILED_TO_ESTABLISH){
		//when controller is in initiating state, find the specified device, send connection request to slave,
		//but slave lost this rf packet, there will no ack packet from slave, after 6 connection events, master
		//controller send a disconnect event with reason HCI_ERR_CONN_FAILED_TO_ESTABLISH
		//if slave got the connection request packet and send ack within 6 connection events, controller
		//send connection establish event to host(telink defined event)


	}
	else{

	}

	#if (UI_LED_ENABLE)
		//led show none connection state
		if(master_connected_led_on){
			master_connected_led_on = 0;
			gpio_write(GPIO_LED_WHITE, LED_ON_LEVAL);   //white on
			gpio_write(GPIO_LED_RED, !LED_ON_LEVAL);    //red off
		}
	#endif


	cur_conn_device.conn_handle = 0;  //when disconnect, clear conn handle


	//if previous connection smp&sdp not finished, clear this flag
	if(app_host_smp_sdp_pending){
		app_host_smp_sdp_pending = 0;
	}

	host_update_conn_param_req = 0; //when disconnect, clear update conn flag

	host_att_data_clear();


	//MTU size reset to default 23 bytes when connection terminated
	blt_att_resetEffectiveMtuSize(BLM_CONN_HANDLE);  //stack API, user can not change

	final_MTU_size = 23;

	//should set scan mode again to scan slave adv packet
	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,
							OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
	blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);


	return 0;
}


/**
 * @brief      call this function when  HCI Controller Event :HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE
 * @param[in]  p - data pointer of event
 * @return     0
 */
int blm_le_conn_update_event_proc(u8 *p)
{
//	event_connection_update_t *pCon = (event_connection_update_t *)p;


	#if (BLE_MASTER_OTA_ENABLE)
		event_connection_update_t *pCon = (event_connection_update_t *)p;

		extern void host_ota_update_conn_complete(u16, u16, u16);
		host_ota_update_conn_complete( pCon->interval, pCon->latency, pCon->timeout );
	#endif


	return 0;
}

/**
 * @brief      call this function when  HCI Controller Event :HCI_SUB_EVT_LE_PHY_UPDATE_COMPLETE
 * @param[in]  p - data pointer of event
 * @return     0
 */
int blm_le_phy_update_complete_event_proc(u8 *p)
{
//	hci_le_phyUpdateCompleteEvt_t *pPhyUpt = (hci_le_phyUpdateCompleteEvt_t *)p;



	return 0;
}



//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
/**
 * @brief      callback function of HCI Controller Event
 * @param[in]  h - HCI Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     0
 */
int controller_event_callback (u32 h, u8 *p, int n)
{


	static u32 event_cb_num;
	event_cb_num++;

	if (h &HCI_FLAG_EVENT_BT_STD)		//ble controller hci event
	{
		u8 evtCode = h & 0xff;
		//------------ disconnect -------------------------------------
		if(evtCode == HCI_EVT_DISCONNECTION_COMPLETE)  //connection terminate
		{
			blm_disconnect_event_handle(p);
		}
#if (BLE_HOST_SMP_ENABLE)
		else if(evtCode == HCI_EVT_ENCRYPTION_CHANGE)
		{
			event_enc_change_t *pe = (event_enc_change_t *)p;
			blm_smp_encChangeEvt(pe->status, pe->handle, pe->enc_enable);
		}
		else if(evtCode == HCI_EVT_ENCRYPTION_KEY_REFRESH)
		{
			event_enc_refresh_t *pe = (event_enc_refresh_t *)p;
			blm_smp_encChangeEvt(pe->status, pe->handle, 1);
		}
#endif
		else if(evtCode == HCI_EVT_LE_META)
		{
			u8 subEvt_code = p[0];

			//------hci le event: le connection complete event---------------------------------
			if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_COMPLETE)	// connection complete
			{
				//after controller is set to initiating state by host (blc_ll_createConnection(...) )
				//it will scan the specified device(adr_type & mac), when find this adv packet, send a connection request packet to slave
				//and enter to connection state, send connection complete event. but notice that connection complete not
				//equals to connection establish. connection complete means that master controller set all the ble timing
				//get ready, but has not received any slave packet, if slave rf lost the connection request packet, it will
				//not send any packet to master controller
				printf("conn_success");

			}
			//------hci le event: le connection establish event---------------------------------
			else if(subEvt_code == HCI_SUB_EVT_LE_CONNECTION_ESTABLISH)  //connection establish(telink private event)
			{
				//notice that: this connection event is defined by telink, not a standard ble controller event
				//after master controller send connection request packet to slave, when slave received this packet
				//and enter to connection state, send a ack packet within 6 connection event, master will send
				//connection establish event to host(HCI_SUB_EVT_LE_CONNECTION_ESTABLISH)

				blm_le_connection_establish_event_handle(p);
			}
			//--------hci le event: le adv report event ----------------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT)	// ADV packet
			{
				//after controller is set to scan state, it will report all the adv packet it received by this event

				blm_le_adv_report_event_handle(p);
			}
			//------hci le event: le connection update complete event-------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE)	// connection update
			{
				//after master host send update conn param req cmd to controller( blm_ll_updateConnection(...) ),
				//when update take effect, controller send update complete event to host
				blm_le_conn_update_event_proc(p);
			}
			//------hci le event: le phy update complete event-------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_PHY_UPDATE_COMPLETE)	// connection update
			{
				//2 situation can trigger this event:
				//   1) master host trigger: by calling API  blc_ll_setPhy(...)
				//   2) peer slave device trigger: send "LL_PHY_REQ" on linklayer
				//when update take effect, controller send update complete event to host
				blm_le_phy_update_complete_event_proc(p);
			}

		}
	}


	return 0;

}



/**
 * @brief      update connection parameter in mainloop
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_
void host_update_conn_proc(void)
{
	//at least 50ms later and make sure smp/sdp is finished
	if( host_update_conn_param_req && clock_time_exceed(host_update_conn_param_req, 50000) && !app_host_smp_sdp_pending)
	{
		host_update_conn_param_req = 0;

		if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){  //still in connection state
			blm_ll_updateConnection (cur_conn_device.conn_handle,
					host_update_conn_min, host_update_conn_min, host_update_conn_latency,  host_update_conn_timeout,
											  0, 0 );
		}
	}
}

#if (TL_AUDIO_MODE & TL_AUDIO_MASK_HID_SERVICE_CHANNEL)
	u8 audio_start = 0x01;
	u8 adpcm_hid_audio_stop  = 0x00;
	u8 audio_config  = 0x02;
	extern u8 mic_cnt;
	extern u8 att_mic_rcvd;
	u8  host_write_dat[32] = {0};
	u8		tmp_mic_data[MIC_ADPCM_FRAME_SIZE];
#endif


volatile int app_l2cap_handle_cnt = 0;

/**
 * @brief      callback function of L2CAP layer handle packet data
 * @param[in]  conn_handle - connect handle
 * @param[in]  raw_pkt - Pointer point to l2cap data packet
 * @return     0
 */
int app_l2cap_handler (u16 conn_handle, u8 *raw_pkt)
{
	app_l2cap_handle_cnt ++;  //debug




	//l2cap data packeted, make sure that user see complete l2cap data
	rf_packet_l2cap_t *ptrL2cap = blm_l2cap_packet_pack (conn_handle, raw_pkt);
	if (!ptrL2cap)
	{
		return 0;
	}



	//l2cap data channel id, 4 for ATT, 5 for Signal, 6 for SMP
	if(ptrL2cap->chanId == L2CAP_CID_ATTR_PROTOCOL)  //att data
	{

		#if (BLE_HOST_SIMPLE_SDP_ENABLE)
			if(app_host_smp_sdp_pending == SDP_PENDING)  //ATT service discovery is ongoing
			{
				//when service discovery function is running, all the ATT data from slave
				//will be processed by it,  user can only send your own att cmd after  service discovery is over
				host_att_client_handler (conn_handle, (u8 *)ptrL2cap); //handle this ATT data by service discovery process
			}
		#endif


		rf_packet_att_t *pAtt = (rf_packet_att_t*)ptrL2cap;
		u16 attHandle = pAtt->handle0 | pAtt->handle1<<8;


		if(pAtt->opcode == ATT_OP_EXCHANGE_MTU_REQ || pAtt->opcode == ATT_OP_EXCHANGE_MTU_RSP)
		{
			rf_packet_att_mtu_exchange_t *pMtu = (rf_packet_att_mtu_exchange_t*)ptrL2cap;

			if(pAtt->opcode ==  ATT_OP_EXCHANGE_MTU_REQ){
				blc_att_responseMtuSizeExchange(conn_handle, ATT_RX_MTU_SIZE_MAX);
			}

			u16 peer_mtu_size = (pMtu->mtu[0] | pMtu->mtu[1]<<8);
			final_MTU_size = min(ATT_RX_MTU_SIZE_MAX, peer_mtu_size);

			blt_att_setEffectiveMtuSize(conn_handle , final_MTU_size); //stack API, user can not change
		}
		else if(pAtt->opcode == ATT_OP_READ_BY_TYPE_RSP)  //slave ack ATT_OP_READ_BY_TYPE_REQ data
		{
			#if (BLE_MASTER_OTA_ENABLE)
				//when use ATT_OP_READ_BY_TYPE_REQ to find ota atthandle, should get the result
				extern void host_find_slave_ota_attHandle(u8 *p);
				host_find_slave_ota_attHandle( (u8 *)pAtt );
			#endif
			//u16 slave_ota_handle;
		}

#if	LL_FEATURE_ENABLE_LL_PRIVACY
		#if (MASTER_RESOLVABLE_ADD_EN)
		else if(pAtt->opcode == ATT_OP_READ_BY_TYPE_REQ)  //slave ack ATT_OP_READ_BY_TYPE_REQ data
		{
			printf("dat[2]=%x,dat[3]=%x\n",pAtt->dat[2],pAtt->dat[3]);
			if(pAtt->dat[2]==0xa6 && pAtt->dat[3]==0x2a)
			{
				u8	tmp[37];
				att_rsp_read_by_type (tmp, 3, 0x001a , 1);
				if( !blm_push_fifo (BLM_CONN_HANDLE, tmp) ){
					printf("push_fail\n");
				}

			}
		}
		#endif
#endif
		else if(pAtt->opcode == ATT_OP_HANDLE_VALUE_NOTI)  //slave handle notify
		{

			if(attHandle == HID_HANDLE_CONSUME_REPORT)
			{
				static u32 app_key;
				app_key++;

				#if (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_HID_DONGLE_TO_STB)
					static u32 rcu_cmd = 0;
					rcu_cmd = (pAtt->dat[3]<<24)|(pAtt->dat[2]<<16) | (pAtt->dat[1]<<8)|(pAtt->dat[0]);

					if(rcu_cmd == MIC_OPEN_FROM_RCU){//open mic

						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&audio_start, 1);
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
//							while(1);
						}
						u8 key[20] = {0};
						u32 mic_open_to_stb = MIC_OPEN_TO_STB;
						memcpy(key,(u8 *)&mic_open_to_stb,4);
						mic_packet_reset();
						usb_report_hid_mic(key, 2);
					}
					else if(rcu_cmd == MIC_CLOSE_FROM_RCU){ //close mic

						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&adpcm_hid_audio_stop, 1);
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
//							while(1);
						}
						u8 key[20] = {0};
						u32 mic_close_to_stb = MIC_CLOSE_TO_STB;
						memcpy(key,(u8 *)&mic_close_to_stb,4);
						usb_report_hid_mic(key, 2);
					}
					else
					{
						att_keyboard_media (conn_handle, pAtt->dat);
					}

				#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_HID)
					static u16 tem_data = 0;
					tem_data = pAtt->dat[3];

					if(tem_data == 0x21){

						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&audio_start, 1);  //open mic
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
							while(1);
						}
						abuf_init ();
					}
					else if(tem_data == 0x24){
						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&adpcm_hid_audio_stop, 1);   //close mic
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
							while(1);
						}
					}
					else{
						att_keyboard_media (conn_handle, pAtt->dat);
					}
				#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID_DONGLE_TO_STB)
					static u32 rcu_cmd = 0;
					rcu_cmd = (pAtt->dat[3]<<24)|(pAtt->dat[2]<<16) | (pAtt->dat[1]<<8)|(pAtt->dat[0]);

					if(rcu_cmd == MIC_OPEN_FROM_RCU){//open mic

						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&audio_start, 1);
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
//							while(1);
						}
						u8 key[20] = {0};
						u32 mic_open_to_stb = MIC_OPEN_TO_STB;
						memcpy(key,(u8 *)&mic_open_to_stb,4);
						mic_packet_reset();
						usb_report_hid_mic(key, 2);
					}
					else if(rcu_cmd == MIC_CLOSE_FROM_RCU){ //close mic

						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&adpcm_hid_audio_stop, 1);
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
//							while(1);
						}
						u8 key[20] = {0};
						u32 mic_close_to_stb = MIC_CLOSE_TO_STB;
						memcpy(key,(u8 *)&mic_close_to_stb,4);
						usb_report_hid_mic(key, 2);
					}
					else
					{
						att_keyboard_media (conn_handle, pAtt->dat);
					}

				#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID)
					static u16 tem_data = 0;
					tem_data = pAtt->dat[3];

					if(tem_data == 0x31){

						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&audio_start, 1);  //open mic
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
							while(1);
						}
					}
					else if(tem_data == 0x34){
						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&adpcm_hid_audio_stop, 1);   //close mic
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
							while(1);
						}
					}
					else{
						att_keyboard_media (conn_handle, pAtt->dat);
					}
				#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_MSBC_HID)
					static u16 tem_data = 0;
					tem_data = pAtt->dat[3];

					if(tem_data == 0x31){

						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&audio_start, 1);  //open mic
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
							while(1);
						}
					}
					else if(tem_data == 0x34){
						att_req_write_cmd (host_write_dat, HID_HANDLE_KEYBOARD_REPORT_OUT, (u8 *)&adpcm_hid_audio_stop, 1);   //close mic
						if( !blm_push_fifo (BLM_CONN_HANDLE, host_write_dat) ){
							//fail
							while(1);
						}
					}
					else{
						att_keyboard_media (conn_handle, pAtt->dat);
					}
				#else
					att_keyboard_media (conn_handle, pAtt->dat);
				#endif
			}
			else if(attHandle == HID_HANDLE_KEYBOARD_REPORT)
			{
				static u32 app_key;
				app_key++;
				att_keyboard (conn_handle, pAtt->dat);

			}
//			else if(HID_HANDLE_MOUSE_REPORT){
//				static u32 app_mouse_dat;
//				att_mouse(conn_handle,pAtt->dat);
//			}

#if (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_TELINK)
			else if(attHandle == AUDIO_HANDLE_MIC)
			{
				static u32 app_mic;
				app_mic	++;
				att_mic (conn_handle, pAtt->dat);
			}
#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_HID_DONGLE_TO_STB)
			else if(attHandle == AUDIO_FIRST_REPORT || attHandle == (AUDIO_FIRST_REPORT + 4) || attHandle == (AUDIO_FIRST_REPORT + 8))
			{
				push_mic_packet(pAtt->dat);
			}
#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_HID)
			else if(attHandle == AUDIO_FIRST_REPORT || attHandle == (AUDIO_FIRST_REPORT + 4) || attHandle == (AUDIO_FIRST_REPORT + 8))//HIDdongle
			{
				att_mic_rcvd = 1;

				static u32 app_mic;
				app_mic	++;

				mic_cnt++;

				if(mic_cnt <=6 ){
					memcpy(tmp_mic_data+(mic_cnt-1)*20, pAtt->dat, 20);
				}

				if(mic_cnt == 6){
					mic_cnt = 0;
					att_mic (conn_handle, tmp_mic_data);
				}
			}
#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID_DONGLE_TO_STB)
			else if(attHandle == AUDIO_FIRST_REPORT || attHandle == (AUDIO_FIRST_REPORT + 4) || attHandle == (AUDIO_FIRST_REPORT + 8))
			{
				push_mic_packet(pAtt->dat);
			}
#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID)
			else if(attHandle == AUDIO_FIRST_REPORT || attHandle == (AUDIO_FIRST_REPORT + 4) || attHandle == (AUDIO_FIRST_REPORT + 8))
			{
				static u32 app_mic;
				app_mic	++;
				att_mic (conn_handle, pAtt->dat);
			}
#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_MSBC_HID)
			else if(attHandle == AUDIO_FIRST_REPORT || attHandle == (AUDIO_FIRST_REPORT + 4) || attHandle == (AUDIO_FIRST_REPORT + 8))
			{
				static u32 app_mic;
				app_mic	++;
				att_mic (conn_handle, pAtt->dat);
			}
#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE)
			else if(attHandle == GOOGLE_AUDIO_HANDLE_MIC_DATA)
			{
				app_audio_data(pAtt->dat, pAtt->rf_len-7);
			}
			else if(attHandle == GOOGLE_AUDIO_HANDLE_MIC_RSP)
			{
				extern u8 google_audio_start;
				//if(pAtt->dat[0] == (google_voice_model ? 0x04 : 0x08))
				if(pAtt->dat[0] == 0x08) //google voice 0.4
				{
					u8 dat[32]={0};
					#if (MIC_SAMPLE_RATE == 16000)
					u8 data[10] = {0x0C, 0x00, 0x02, 0x00, 0x03};//16000
					#else
					u8 data[10] = {0x0C, 0x00, 0x01, 0x00, 0x01};//8000
					#endif

					att_req_write(dat, GOOGLE_AUDIO_HANDLE_MIC_CMD, data, 5);

					if(blm_push_fifo(BLM_CONN_HANDLE, dat)){

					}
					//printf("audio start");
					google_audio_start = true;//app_audio_start();

				}
				else if(pAtt->dat[0] == 0x00)
				{
					google_audio_start = false;//app_audio_stop();
				}
				else if(pAtt->dat[0] == 0x0A)
				{
					//printf("AUDIO_HANDLE_MIC_RSP: sync\r\n");
				}
				else if(pAtt->dat[0] == 0x0B)
				{
					//get caps rsp
					google_voice_model = pAtt->dat[4];

				}
			}
#endif
			else
			{

			}
		}
		else if (pAtt->opcode == ATT_OP_HANDLE_VALUE_IND)
		{
			u8 format =  ATT_OP_HANDLE_VALUE_CFM;
			blc_l2cap_pushData_2_controller(conn_handle, L2CAP_CID_ATTR_PROTOCOL, &format, 1, NULL, 0);
		}

	}
	else if(ptrL2cap->chanId == L2CAP_CID_SIG_CHANNEL)  //signal
	{
		if(ptrL2cap->opcode == L2CAP_CMD_CONN_UPD_PARA_REQ)  //slave send conn param update req on l2cap
		{
			rf_packet_l2cap_connParaUpReq_t  * req = (rf_packet_l2cap_connParaUpReq_t *)ptrL2cap;

			u32 interval_us = req->min_interval*1250;  //1.25ms unit
			u32 timeout_us = req->timeout*10000; //10ms unit
			u32 long_suspend_us = interval_us * (req->latency+1);

			//interval < 200ms
			//long suspend < 11S
			// interval * (latency +1)*2 <= timeout
			if( interval_us < 200000 && long_suspend_us < 20000000 && (long_suspend_us*2<=timeout_us) )
			{
				//when master host accept slave's conn param update req, should send a conn param update response on l2cap
				//with CONN_PARAM_UPDATE_ACCEPT; if not accpet,should send  CONN_PARAM_UPDATE_REJECT
				blc_l2cap_SendConnParamUpdateResponse(conn_handle, req->id, CONN_PARAM_UPDATE_ACCEPT);  //send SIG Connection Param Update Response


				//if accept, master host should mark this, add will send  update conn param req on link layer later
				//set a flag here, then send update conn param req in mainloop
				host_update_conn_param_req = clock_time() | 1 ; //in case zero value
				host_update_conn_min = req->min_interval;  //backup update param
				host_update_conn_latency = req->latency;
				host_update_conn_timeout = req->timeout;
			}
			else
			{
				blc_l2cap_SendConnParamUpdateResponse(conn_handle, req->id, CONN_PARAM_UPDATE_REJECT);  //send SIG Connection Param Update Response
			}
		}


	}
	else if(ptrL2cap->chanId == L2CAP_CID_SMP) //smp
	{
		#if (BLE_HOST_SMP_ENABLE)
			if(app_host_smp_sdp_pending == SMP_PENDING)
			{
				blm_host_smp_handler(conn_handle, (u8 *)ptrL2cap);
			}

		#endif
	}
	else
	{

	}


	return 0;
}
