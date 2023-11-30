/********************************************************************************************************
 * @file	app.c
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
#include "application/audio/tl_audio.h"
#include "application/audio/audio_config.h"
#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"

#if (FEATURE_TEST_MODE == TEST_LL_PRIVACY_MASTER)

#define		MY_RF_POWER_INDEX					RF_POWER_P3dBm

MYFIFO_INIT(blt_rxfifo, 64, 16);
MYFIFO_INIT(blt_txfifo, 40, 8);

#if (UI_KEYBOARD_ENABLE)
	#define CONSUMER_KEY   	   		1
	#define KEYBOARD_KEY   	   		2
	_attribute_data_retention_	int 	key_not_released;
	_attribute_data_retention_	u8 		key_type;
	_attribute_data_retention_		static u32 keyScanTick = 0;
	extern u32	scan_pin_need;

	#define KEY_PAIR				VK_1
	#define KEY_UNPAIR				VK_2


	/**
	 * @brief   Check changed key value.
	 * @param   none.
	 * @return  none.
	 */
	void key_change_proc(void)
	{
		u8 key0 = kb_event.keycode[0];

		key_not_released = 1;
		if (kb_event.cnt == 2)   //two key press, do  not process
		{

		}
		else if(kb_event.cnt == 1)
		{
			if(key0 == KEY_PAIR)
			{
				pairing_enable = 1;

			}
			else if(key0 == KEY_UNPAIR)
			{
				unpair_enable = 1;

			}

		}
		else   //kb_event.cnt == 0,  key release
		{
			key_not_released = 0;
			if(pairing_enable)
			{
				pairing_enable = 0;
			}

			if(unpair_enable)
			{
				unpair_enable = 0;
			}
		}


	}





	/**
	 * @brief      keyboard task handler
	 * @param[in]  e    - event type
	 * @param[in]  p    - Pointer point to event parameter.
	 * @param[in]  n    - the length of event parameter.
	 * @return     none.
	 */
	void proc_keyboard (u8 e, u8 *p, int n)
	{
	    (void)e;(void)p;(void)n;
		if(clock_time_exceed(keyScanTick, 8000)){
			keyScanTick = clock_time();
		}
		else{
			return;
		}

		kb_event.keycode[0] = 0;
		int det_key = kb_scan_key (0, 1);



		if (det_key){
			key_change_proc();
		}

	}


#endif








u8	local_addr_type = OWN_ADDRESS_PUBLIC;

#if	LL_FEATURE_ENABLE_PRIVACY

	smp_param_master_t  dev_msg;
	/**
	 * @brief      callback function of Host Event
	 * @param[in]  h - Host Event type
	 * @param[in]  para - data pointer of event
	 * @param[in]  n - data length of event
	 * @return     0
	 */
	int app_host_event_callback (u32 h, u8 *para, int n)
	{
	    (void)h;(void)para;(void)n;
		u8 event = h & 0xFF;

		switch(event)
		{
			case GAP_EVT_SMP_PAIRING_SUCCESS:
			{
				gap_smp_pairingSuccessEvt_t* p = (gap_smp_pairingSuccessEvt_t*)para;

				if(p->bonding_result){

				}
				else{
					printf("save smp key failed %x,handle %x\n",p->bonding_result,p->connHandle);
				}
			}
			break;
		}
		return 0;
	}

#endif

/**
 * @brief		user initialization
 * @param[in]	none
 * @return      none
 */
void user_init(void)
{
	//random number generator must be initiated here( in the beginning of user_init_nromal)
	//when deepSleep retention wakeUp, no need initialize again
	random_generator_init();  //this is must

	#if(UART_PRINT_DEBUG_ENABLE)
		tlkapi_debug_init();
		blc_debug_enableStackLog(STK_LOG_DISABLE);
	#endif

	blc_readFlashSize_autoConfigCustomFlashSector();

	/* attention that this function must be called after "blc_readFlashSize_autoConfigCustomFlashSector" !!!*/
	blc_app_loadCustomizedParameters_normal();






///////////// BLE stack Initialization ////////////////
	u8  mac_public[6];
	u8  mac_random_static[6];
	//for 512K Flash, flash_sector_mac_address equals to 0x76000
	//for 1M  Flash, flash_sector_mac_address equals to 0xFF000
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);
	tlkapi_send_string_data(APP_LOG_EN,"[APP][INI]Public Address", mac_public, 6);

	////// Controller Initialization  //////////
	blc_ll_initBasicMCU();
	blc_ll_initStandby_module(mac_public);				//mandatory
	blc_ll_initScanning_module(mac_public); 			//scan module: 		 mandatory for BLE master,
	blc_ll_initInitiating_module();						//initiate module: 	 mandatory for BLE master,
	blc_ll_initConnection_module();						//connection module  mandatory for BLE slave/master
	blc_ll_initMasterRoleSingleConn_module();			//master module: 	 mandatory for BLE master,

	rf_set_power_level_index (MY_RF_POWER_INDEX);

	////// Host Initialization  //////////
	blc_gap_central_init();										//gap initialization
	blc_l2cap_register_handler (app_l2cap_handler);    			//l2cap initialization
	blc_hci_registerControllerEventHandler(controller_event_callback); //controller hci event to host all processed in this func

	//bluetooth event
	blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE | HCI_EVT_MASK_ENCRYPTION_CHANGE);

	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(		HCI_LE_EVT_MASK_CONNECTION_COMPLETE 		\
									|	HCI_LE_EVT_MASK_ADVERTISING_REPORT 			\
									|   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE  \
									|	HCI_LE_EVT_MASK_PHY_UPDATE_COMPLETE			\
									|   HCI_LE_EVT_MASK_CONNECTION_ESTABLISH );         //connection establish: telink private event



	blm_smp_configPairingSecurityInfoStorageAddr(flash_sector_master_pairing);
	blm_smp_registerSmpFinishCb(app_host_smp_finish);

	blc_smp_central_init();

	//SMP trigger by master
	blm_host_smp_setSecurityTrigger(MASTER_TRIGGER_SMP_FIRST_PAIRING | MASTER_TRIGGER_SMP_AUTO_CONNECT);


	extern int host_att_register_idle_func (void *p);
	host_att_register_idle_func (main_idle_loop);

	blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_SUCCESS );
	blc_gap_registerHostEventHandler( app_host_event_callback );

	extern u8 tbl_get_bond_slave_num(void);
	u8	bond_number = tbl_get_bond_slave_num();
	if(bond_number != 0)	//No bondind device
	{
		u32 device_add = tbl_get_bond_msg_by_index(bond_number-1);

		tlkapi_printf(APP_LOG_EN,"bond number addr not 0 , is %x\n",device_add);

		//read bonding device
		flash_read_page(device_add,sizeof(smp_param_master_t),(unsigned char *)(&dev_msg) );

		//add bonding message to resolve list
		blc_ll_addDeviceToResolvingList(dev_msg.peer_id_adrType,dev_msg.peer_id_addr,dev_msg.peer_irk,dev_msg.local_irk);

		local_addr_type = OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC;

		blc_ll_setAddressResolutionEnable(1);
	}

	//set scan parameter and scan enable

	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,	\
											local_addr_type, SCAN_FP_ALLOW_ADV_ANY);
	blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);

	/* Check if any Stack(Controller & Host) Initialization error after all BLE initialization done!!! */
	u32 error_code1 = blc_contr_checkControllerInitialization();
	u32 error_code2 = blc_host_checkHostInitialization();
	if(error_code1 != INIT_SUCCESS || error_code2 != INIT_SUCCESS){
		/* It's recommended that user set some UI alarm to know the exact error, e.g. LED shine, print log */
		#if (UART_PRINT_DEBUG_ENABLE)
			tlkapi_printf(APP_LOG_EN, "[APP][INI] Stack INIT ERROR 0x%04x, 0x%04x", error_code1, error_code2);
		#endif

		#if (UI_LED_ENABLE)
			gpio_write(GPIO_LED_RED, LED_ON_LEVEL);
		#endif
		while(1);
	}
	tlkapi_printf(APP_LOG_EN, "[APP][INI] feature_privacy_master init \n");
}




int main_idle_loop (void)
{

	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();


	/////////////////////////////////////// HCI ///////////////////////////////////////
	blc_hci_proc ();

#if (UI_KEYBOARD_ENABLE)
	proc_keyboard (0,0, 0);
#endif
	host_pair_unpair_proc();

#if 1
	//proc master update
	if(host_update_conn_param_req){
		host_update_conn_proc();
	}
#endif

	return 0;
}


/**
 * @brief     BLE main loop
 * @param[in]  none.
 * @return     none.
 */
void main_loop (void)
{
	main_idle_loop();
	if (main_service)
	{
		main_service ();
		main_service = 0;
	}
}


#endif


