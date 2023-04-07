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
#include "application/usbstd/usb.h"

#if (FEATURE_TEST_MODE == TEST_LL_PRIVACY_MASTER)

MYFIFO_INIT(blt_rxfifo, 64, 16);
MYFIFO_INIT(blt_txfifo, 40, 8);



u8	local_addr_type = OWN_ADDRESS_PUBLIC;

#if	LL_FEATURE_ENABLE_LL_PRIVACY

	smp_master_param_save_t  dev_msg;
	/**
	 * @brief      callback function of Host Event
	 * @param[in]  h - Host Event type
	 * @param[in]  para - data pointer of event
	 * @param[in]  n - data length of event
	 * @return     0
	 */
	int app_host_event_callback (u32 h, u8 *para, int n)
	{
		u8 event = h & 0xFF;

		switch(event)
		{
			case GAP_EVT_SMP_PAIRING_SUCCESS:
			{
				gap_smp_paringSuccessEvt_t* p = (gap_smp_paringSuccessEvt_t*)para;

				if(p->bonding_result){

					dev_char_info_t *pt = &cur_conn_device;		//get current device mac
					u8	index = 0;
					if(IS_RESOLVABLE_PRIVATE_ADDR(pt->mac_adrType,pt->mac_addr))	//resolvable
					{
						u8 index_rl = blt_ll_searchAddr_in_ResolvingList_index(pt->mac_adrType,pt->mac_addr);
						if( index_rl )
						{
							index = tbl_bond_slave_search(ll_resolvingList_tbl.rlList[index_rl-1].rlIdAddrType,ll_resolvingList_tbl.rlList[index_rl-1].rlIdAddr);
						}
					}
					else
					{
						index = tbl_bond_slave_search(pt->mac_adrType,pt->mac_addr);
					}


					extern bond_slave_t  tbl_bondSlave;  //slave mac bond table
					u32 device_add = tbl_bondSlave.bond_flash_idx[index-1];

					printf("bond number after Conn = %x\n",device_add);

					//flash_read_data
					flash_read_page(device_add,sizeof(smp_master_param_save_t),(unsigned char *)(&dev_msg) );

					ll_resolvingList_setAddrResolutionEnable(0);

					u8 reason = ll_resolvingList_add(dev_msg.adr_type,&dev_msg.address[0],&dev_msg.peer_irk[0],&dev_msg.local_irk[0]);

					printf("Conn :");
					printf("peer adr: ");
					printf("%x %x %x %x %x %x\n",ll_resolvingList_tbl.rlList[0].rlPeerRpa[0],ll_resolvingList_tbl.rlList[0].rlPeerRpa[1],	\
												 ll_resolvingList_tbl.rlList[0].rlPeerRpa[2],ll_resolvingList_tbl.rlList[0].rlPeerRpa[3],	\
												 ll_resolvingList_tbl.rlList[0].rlPeerRpa[4],ll_resolvingList_tbl.rlList[0].rlPeerRpa[5]);

					printf("local adr: ");
					printf("%x %x %x %x %x %x\n",ll_resolvingList_tbl.rlList[0].rlLocalRpa[0],ll_resolvingList_tbl.rlList[0].rlLocalRpa[1],	\
												 ll_resolvingList_tbl.rlList[0].rlLocalRpa[2],ll_resolvingList_tbl.rlList[0].rlLocalRpa[3],	\
												 ll_resolvingList_tbl.rlList[0].rlLocalRpa[4],ll_resolvingList_tbl.rlList[0].rlLocalRpa[5]);

					ll_resolvingList_setAddrResolutionEnable(1);
					ll_resolvingList_setResolvablePrivateAddrTimer(15);

					printf("reason:%x , blm:%d\n",reason,blm_create_connection);

					printf("En:%d",ll_resolvingList_tbl.rpaTmoCtrl.rpaTmrEn);
					printf("Tick:%x",ll_resolvingList_tbl.rpaTmoCtrl.rpaTmr1sChkTick);

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


	//set USB ID
	REG_ADDR8(0x74) = 0x62;
	REG_ADDR16(0x7e) = 0x08d0;
	REG_ADDR8(0x74) = 0x00;

	//////////////// config USB ISO IN/OUT interrupt /////////////////
	reg_usb_mask = BIT(7);			//audio in interrupt enable
	reg_irq_mask |= FLD_IRQ_IRQ4_EN;
	reg_usb_ep6_buf_addr = 0x80;
	reg_usb_ep7_buf_addr = 0x60;
	reg_usb_ep_max_size = (256 >> 3);

	usb_dp_pullup_en (1);  //open USB enum




///////////// BLE stack Initialization ////////////////
	u8  mac_public[6];
	u8  mac_random_static[6];
	//for 512K Flash, flash_sector_mac_address equals to 0x76000
	//for 1M  Flash, flash_sector_mac_address equals to 0xFF000
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);


	////// Controller Initialization  //////////
	blc_ll_initBasicMCU();
	blc_ll_initStandby_module(mac_public);				//mandatory
	blc_ll_initScanning_module(mac_public); 			//scan module: 		 mandatory for BLE master,
	blc_ll_initInitiating_module();						//initiate module: 	 mandatory for BLE master,
	blc_ll_initConnection_module();						//connection module  mandatory for BLE slave/master
	blc_ll_initMasterRoleSingleConn_module();			//master module: 	 mandatory for BLE master,

	rf_set_power_level_index (RF_POWER_P3dBm);

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



	blm_smp_configParingSecurityInfoStorageAddr(FLASH_ADR_PARING);
	blm_smp_registerSmpFinishCb(app_host_smp_finish);

	blc_smp_central_init();

	//SMP trigger by master
	blm_host_smp_setSecurityTrigger(MASTER_TRIGGER_SMP_FIRST_PAIRING | MASTER_TRIGGER_SMP_AUTO_CONNECT);


	extern int host_att_register_idle_func (void *p);
	host_att_register_idle_func (main_idle_loop);


#if	LL_FEATURE_ENABLE_LL_PRIVACY

	blc_ll_resolvListInit();

	blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_SUCCESS );
	blc_gap_registerHostEventHandler( app_host_event_callback );

	extern u8 tbl_get_bond_slave_num(void);
	u8	bond_number = tbl_get_bond_slave_num();
	if(bond_number != 0)	//No bondind device
	{
		extern bond_slave_t  tbl_bondSlave;  //slave mac bond table
		u32 device_add = tbl_bondSlave.bond_flash_idx[bond_number-1];

		printf("bond number addr not 0 , is %x\n",device_add);

		//read bonding device
		flash_read_page(device_add,sizeof(smp_master_param_save_t),(unsigned char *)(&dev_msg) );

		//add bonding message to resolve list
		ll_resolvingList_add(dev_msg.adr_type,&dev_msg.address[0],&dev_msg.peer_irk[0],&dev_msg.local_irk[0]);

#if	LOCAL_ADDR_RPA_EN
		local_addr_type = OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC;

		ll_resolvingList_setAddrResolutionEnable(1);
		ll_resolvingList_setResolvablePrivateAddrTimer(15);
#else
		local_addr_type = OWN_ADDRESS_PUBLIC;
#endif
	}


#endif

	//set scan parameter and scan enable

	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,	\
											local_addr_type, SCAN_FP_ALLOW_ADV_ANY);
	blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);


}



extern u8 send_scan_flag;
extern void usb_handle_irq(void);

int main_idle_loop (void)
{

	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();

	///////////////////////////////////// proc usb cmd from host /////////////////////
	usb_handle_irq();

	/////////////////////////////////////// HCI ///////////////////////////////////////
	blc_hci_proc ();

#if (UI_BUTTON_ENABLE)
	static u8 button_detect_en = 0;
	if(!button_detect_en && clock_time_exceed(0, 1000000)){// proc button 1 second later after power on
		button_detect_en = 1;
	}
	static u32 button_detect_tick = 0;
	if(button_detect_en && clock_time_exceed(button_detect_tick, 5000))
	{
		button_detect_tick = clock_time();
		proc_button();  //button triggers pair & unpair  and OTA
	}
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


