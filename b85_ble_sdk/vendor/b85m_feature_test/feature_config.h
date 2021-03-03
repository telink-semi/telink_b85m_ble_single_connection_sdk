/********************************************************************************************************
 * @file     feature_config.h
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
#ifndef FEATURE_CONFIG_H_
#define FEATURE_CONFIG_H_



/////////////////// TEST FEATURE SELECTION /////////////////////////////////
//ble link layer test
#define	TEST_ADVERTISING_ONLY							1
#define TEST_SCANNING_ONLY								2
#define TEST_ADVERTISING_IN_CONN_SLAVE_ROLE				3
#define TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE        4
#define TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE	5



//power test
#define TEST_POWER_ADV									10

//smp test
#define TEST_SMP_SECURITY								20 //If testing SECURITY, such as Passkey Entry or Numric_Comparison, we use the remote control board for testing

//gatt secure test
#define TEST_GATT_SECURITY								21 //If testing SECURITY, such as Passkey Entry or Numric_Comparison, we use the remote control board for testing

//slave data length exchange test
#define TEST_SDATA_LENGTH_EXTENSION						22

//master data length exchange test
#define TEST_MDATA_LENGTH_EXTENSION						23

 //phy test
#define TEST_BLE_PHY									32		// BQB PHY_TEST demo


#define TEST_EXTENDED_ADVERTISING						40		//  Extended ADV demo

#define TEST_2M_CODED_PHY_EXT_ADV						50		//  2M/Coded PHY used on Extended ADV

#define TEST_2M_CODED_PHY_CONNECTION					60		//  2M/Coded PHY used on Legacy_ADV and Connection

#define	TEST_OTA_BIG_PDU								70


//some simple function test
#define TEST_USER_BLT_SOFT_TIMER						100
#define TEST_WHITELIST									110
#define	TEST_CSA2										120



#define TEST_FEATURE_BACKUP								200




#define FEATURE_TEST_MODE								TEST_FEATURE_BACKUP






#endif /* FEATURE_CONFIG_H_ */
