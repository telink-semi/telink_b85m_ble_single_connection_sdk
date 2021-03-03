/********************************************************************************************************
 * @file     app.c
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2018
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app.h"
#include "../default_att.h"

#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"


#if (FEATURE_TEST_MODE == TEST_POWER_ADV)


#define		MY_RF_POWER_INDEX					RF_POWER_P0dBm




#define RX_FIFO_SIZE	64
#define RX_FIFO_NUM		8

#define TX_FIFO_SIZE	40
#define TX_FIFO_NUM		8



_attribute_data_retention_  u8 		 	blt_rxfifo_b[RX_FIFO_SIZE * RX_FIFO_NUM] = {0};
_attribute_data_retention_	my_fifo_t	blt_rxfifo = {
												RX_FIFO_SIZE,
												RX_FIFO_NUM,
												0,
												0,
												blt_rxfifo_b,};


_attribute_data_retention_  u8 		 	blt_txfifo_b[TX_FIFO_SIZE * TX_FIFO_NUM] = {0};
_attribute_data_retention_	my_fifo_t	blt_txfifo = {
												TX_FIFO_SIZE,
												TX_FIFO_NUM,
												0,
												0,
												blt_txfifo_b,};



/**
 * @brief	Adv Packet data
 */
const u8	tbl_advData[] = {
	 0x08, 0x09, 'f', 'e', 'a', 't', 'u', 'r', 'e',
	 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
	 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
	 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
};
/**
 * @brief	Scan Response Packet data
 */
const u8	tbl_scanRsp [] = {
	 0x08, 0x09, 'f', 'e', 'a', 't', 'u', 'r', 'e',
};


_attribute_data_retention_	int device_in_connection_state;



/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void	user_set_rf_power (u8 e, u8 *p, int n)
{
	rf_set_power_level_index (MY_RF_POWER_INDEX);
}


/**
 * @brief      power management code for application
 * @param	   none
 * @return     none
 */
_attribute_ram_code_
void blt_pm_proc(void)
{
#if(FEATURE_PM_ENABLE)
	#if (FEATURE_DEEPSLEEP_RETENTION_ENABLE)
		bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
	#else
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	#endif
#endif  //end of FEATURE_PM_ENABLE
}






/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void user_init_normal(void)
{
	//random number generator must be initiated here( in the beginning of user_init_nromal)
	//when deepSleep retention wakeUp, no need initialize again
	random_generator_init();  //this is must



////////////////// BLE stack initialization ////////////////////////////////////
	u8  mac_public[6];
	u8  mac_random_static[6];
	//for 512K Flash, flash_sector_mac_address equals to 0x76000
	//for 1M  Flash, flash_sector_mac_address equals to 0xFF000
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);


	////// Controller Initialization  //////////
	blc_ll_initBasicMCU();                      //mandatory
	blc_ll_initStandby_module(mac_public);		//mandatory



	//when debugging, if long time deepSleep retention or suspend happens quickly after power on, it will make "ResetMCU" very hard, so add some time here
	sleep_us(2000000);  //only for debug




	blc_ll_initAdvertising_module(mac_public); 	//adv module: 		 mandatory for BLE slave,


/******************************************************************************************************
 * Here are just some ADV power example
 * The actual measured power is affected by several ADV parameters, such as:
 * 1. ADV data length: long ADV data means bigger power
 *
 * 2. ADV type:   non_connectable undirected: ADV power is small, cause only data sending involved, no
 *                                           need receiving any packet from master
 *                connectable ADV: must try to receive scan_req/scan_conn from master after sending adv
 *                                           data, so power is bigger.
 *                                               And if needing send scan_rsp to master's scan_req,
 *                                           power will increase. Here we can use whiteList to disable scan_rsp.
 *											     With connectable ADV, user should test power under a clean
 *											 and shielded environment to avoid receiving scan_req/conn_req
 *
 * 3. ADV power index: We use 0dBm in examples, higher power index will cause poser to increase
 *
 * 4. ADV interval: Bigger adv interval lead to smaller power, cause more timing for suspend/deepSleep retention
 *
 * 5. ADV channel: Power with 3 channel is bigger than power with 1 or 2 channel
 *
 *
 * If you want test ADV power with different ADV parameters from our examples, you should modify these
 *      parameters in code, and re_test by yourself.
 *****************************************************************************************************/

	//set to special ADV channel can avoid master's scan_req to get a very clean power,
	// but remember that special channel ADV packet can not be scanned by BLE master and captured by BLE sniffer
//	blc_ll_setAdvCustomedChannel(33,34,35);

#if 0   // connectable undirected ADV
	//ADV data length: 12 byte
	u8 tbl_advData[12] = {
		 0x08, 0x09, 't', 'e', 's', 't', 'a', 'd', 'v',
		 0x02, 0x01, 0x05,
		};
	u8	tbl_scanRsp [] = {
			 0x08, 0x09, 'T', 'E', 'S', 'T', 'A', 'D', 'V',	//scan name
		};

	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));


	// ADV data length:	12 byte
	// ADV type: 		connectable undirected ADV
	// ADV power index: 0 dBm
	// ADV interval: 	1S
	// ADV channel: 	1 channel
	// test result: 	8258 : 9 uA; 8278 : 14uA;
//	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_1S, ADV_INTERVAL_1S, \
									ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_37, ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_WL);  //no scan, no connect

	// ADV data length:	12 byte
	// ADV type: 		connectable undirected ADV
	// ADV power index: 0 dBm
	// ADV interval: 	1S
	// ADV channel: 	3 channel
	// test result: 	15 uA
	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_1S, ADV_INTERVAL_1S, \
									ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_WL);  //no scan, no connect

	// ADV data length:	12 byte
	// ADV type: 		connectable undirected ADV
	// ADV power index: 0 dBm
	// ADV interval: 	500 mS
	// ADV channel: 	3 channel
	// test result: 	8258 : 30 uA; 8278 : 50uA;
//	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_500MS, ADV_INTERVAL_500MS, \
									ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_WL);  //no scan, no connect

	// ADV data length:	12 byte
	// ADV type: 		connectable undirected ADV
	// ADV power index: 0 dBm
	// ADV interval: 	30 mS
	// ADV channel: 	3 channel
	// test result: 	430 uA
//	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_30MS, \
									ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_WL);  //no scan, no connect


#else  // non_connectable undirected ADV, no need scanRsp

	#if 1 	//ADV data length: 16 byte
		u8 tbl_advData[] = {
			 14, 0x09, 't', 'e', 's', 't', 'a', 'd', 'v', '8', '9', 'A', 'B', 'C', 'D', 'E',
			};
	#else  	//ADV data length: max 31 byte
		u8 tbl_advData[] = {
			 30, 0x09, 't', 'e', 's', 't', 'a', 'd', 'v', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D'
		};
	#endif


	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );


	// ADV data length: 16 byte
	// ADV type: non_connectable undirected ADV
	// ADV power index: 0 dBm
	// ADV interval: 1S
	// ADV channel: 3 channel
	// test result: 11 uA (if ADV data length change to 31 byte, test result: 14 uA)
	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_1S, ADV_INTERVAL_1S, \
									ADV_TYPE_NONCONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_NONE);


	// ADV data length: 16 byte
	// ADV type: non_connectable undirected ADV
	// ADV power index: 0 dBm
	// ADV interval: 1.5S
	// ADV channel: 3 channel
	// test result: 8 uA (if ADV data length change to 31 byte, test result: 11 uA)
//	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_1S5, ADV_INTERVAL_1S5, \
									ADV_TYPE_NONCONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_NONE);


	// ADV data length: 16 byte
	// ADV type: non_connectable undirected ADV
	// ADV power index: 0 dBm
	// ADV interval: 2S
	// ADV channel: 3 channel
	// test result: 6 uA (if ADV data length change to 31 byte, test result: 7 uA)
//	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_2S, ADV_INTERVAL_2S, \
									ADV_TYPE_NONCONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
#endif


	if(status != BLE_SUCCESS){  //adv setting err
		  //debug
		while(1);
	}

	bls_ll_setAdvEnable(1);  //adv enable


	user_set_rf_power(0, 0, 0);



#if(FEATURE_PM_ENABLE)
	blc_ll_initPowerManagement_module();

	#if (FEATURE_DEEPSLEEP_RETENTION_ENABLE)
		bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
		blc_pm_setDeepsleepRetentionThreshold(50, 50);
		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(200);
	#else
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	#endif

	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_EXIT, &user_set_rf_power);
#else
	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif
}



/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void user_init_deepRetn(void)
{
#if (FEATURE_DEEPSLEEP_RETENTION_ENABLE)

	blc_ll_initBasicMCU();   //mandatory
	user_set_rf_power(0, 0, 0);

	blc_ll_recoverDeepRetention();

	irq_enable();

	DBG_CHN0_HIGH;    //debug
#endif
}




/**
 * @brief     BLE main loop
 * @param[in]  none.
 * @return     none.
 */
void main_loop (void)
{

	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();


	////////////////////////////////////// UI entry /////////////////////////////////


	////////////////////////////////////// PM Process /////////////////////////////////
	blt_pm_proc();
}


#endif  //end of (FEATURE_TEST_MODE == ...)
