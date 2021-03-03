/********************************************************************************************************
 * @file	ble_common.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
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
#ifndef BLE_COMMON_H
#define BLE_COMMON_H

#include "ble_config.h"
#include "ble_stack.h"
#include "tl_common.h"


/**
 *  @brief  Definition for Link Layer Feature Support
 */
#define LL_FEATURE_SIZE                                      8
#define LL_FEATURE_MASK_LL_ENCRYPTION                        (0x00000001)   //core_4.0
#define LL_FEATURE_MASK_CONNECTION_PARA_REQUEST_PROCEDURE  	 (0x00000002)	//core_4.1
#define LL_FEATURE_MASK_EXTENDED_REJECT_INDICATION           (0x00000004)	//core_4.1
#define LL_FEATURE_MASK_SLAVE_INITIATED_FEATURES_EXCHANGE    (0x00000008)	//core_4.1
#define LL_FEATURE_MASK_LE_PING                              (0x00000010)   //core_4.1
#define LL_FEATURE_MASK_LE_DATA_LENGTH_EXTENSION             (0x00000020)	//core_4.2
#define LL_FEATURE_MASK_LL_PRIVACY                           (0x00000040)	//core_4.2
#define LL_FEATURE_MASK_EXTENDED_SCANNER_FILTER_POLICIES     (0x00000080)   //core_4.2
#define LL_FEATURE_MASK_LE_2M_PHY         					 (0x00000100)	//core_5.0
#define LL_FEATURE_MASK_STABLE_MODULATION_INDEX_TX 			 (0x00000200)	//core_5.0
#define LL_FEATURE_MASK_STABLE_MODULATION_INDEX_RX 			 (0x00000400)	//core_5.0
#define LL_FEATURE_MASK_LE_CODED_PHY     					 (0x00000800)	//core_5.0
#define LL_FEATURE_MASK_LE_EXTENDED_ADVERTISING          	 (0x00001000)	//core_5.0
#define LL_FEATURE_MASK_LE_PERIODIC_ADVERTISING     		 (0x00002000)	//core_5.0
#define LL_FEATURE_MASK_CHANNEL_SELECTION_ALGORITHM2         (0x00004000)	//core_5.0
#define LL_FEATURE_MASK_LE_POWER_CLASS_1 					 (0x00008000)	//core_5.0
#define LL_FEATURE_MASK_MIN_USED_OF_USED_CHANNELS   	     (0x00010000)	//core_5.0

/////////////////////////////////////////////////////////////////////////////

#define         VENDOR_ID                       0x0211
#define         VENDOR_ID_HI_B                  U16_HI(VENDOR_ID)
#define         VENDOR_ID_LO_B                  U16_LO(VENDOR_ID)

#define			BLUETOOTH_VER_4_0				6
#define			BLUETOOTH_VER_4_1				7
#define			BLUETOOTH_VER_4_2				8
#define			BLUETOOTH_VER_5_0				9

#ifndef 		BLUETOOTH_VER
#define			BLUETOOTH_VER					BLUETOOTH_VER_5_0
#endif


#if (BLUETOOTH_VER == BLUETOOTH_VER_4_2)
	#define			BLUETOOTH_VER_SUBVER			0x22BB
#elif (BLUETOOTH_VER == BLUETOOTH_VER_5_0)
	#define			BLUETOOTH_VER_SUBVER			0x1C1C
#else
	#define			BLUETOOTH_VER_SUBVER			0x4103
#endif




#if (BLUETOOTH_VER == BLUETOOTH_VER_4_0)
	#define LL_FEATURE_ENABLE_LE_ENCRYPTION								1
	#define LL_CMD_MAX						   							LL_REJECT_IND

#elif (BLUETOOTH_VER == BLUETOOTH_VER_4_1)
	#define LL_FEATURE_ENABLE_LE_ENCRYPTION								1
	#define	LL_FEATURE_ENABLE_EXTENDED_REJECT_INDICATION				1
	#define	LL_FEATURE_ENABLE_SLAVE_INITIATED_FEATURES_EXCHANGE			1
	#define	LL_FEATURE_ENABLE_LE_PING									1

	#define LL_CMD_MAX						   							LL_PING_RSP

#elif (BLUETOOTH_VER == BLUETOOTH_VER_4_2)

	#define LL_FEATURE_ENABLE_LE_ENCRYPTION								1
	#define	LL_FEATURE_ENABLE_EXTENDED_REJECT_INDICATION				1
	#define	LL_FEATURE_ENABLE_SLAVE_INITIATED_FEATURES_EXCHANGE			1
	#define	LL_FEATURE_ENABLE_LE_PING									1
	#define	LL_FEATURE_ENABLE_LE_DATA_LENGTH_EXTENSION					BLE_CORE42_DATA_LENGTH_EXTENSION_ENABLE
	#define	LL_FEATURE_ENABLE_LL_PRIVACY								0

	#define LL_CMD_MAX						  							LL_LENGTH_RSP

#elif (BLUETOOTH_VER == BLUETOOTH_VER_5_0)

	#define LL_FEATURE_ENABLE_LE_ENCRYPTION								1
	#define	LL_FEATURE_ENABLE_EXTENDED_REJECT_INDICATION				1
	#define	LL_FEATURE_ENABLE_SLAVE_INITIATED_FEATURES_EXCHANGE			1
	#define	LL_FEATURE_ENABLE_LE_PING									1
	#define	LL_FEATURE_ENABLE_LE_DATA_LENGTH_EXTENSION					BLE_CORE42_DATA_LENGTH_EXTENSION_ENABLE
	#define	LL_FEATURE_ENABLE_LL_PRIVACY								0

	#define	LL_FEATURE_ENABLE_LE_2M_PHY									LL_FEATURE_SUPPORT_LE_2M_PHY
	#define	LL_FEATURE_ENABLE_LE_CODED_PHY								LL_FEATURE_SUPPORT_LE_CODED_PHY
	#define	LL_FEATURE_ENABLE_LE_EXTENDED_ADVERTISING					LL_FEATURE_SUPPORT_LE_EXTENDED_ADVERTISING
	#define	LL_FEATURE_ENABLE_LE_PERIODIC_ADVERTISING					LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING
	#define	LL_FEATURE_ENABLE_CHANNEL_SELECTION_ALGORITHM2				LL_FEATURE_SUPPORT_CHANNEL_SELECTION_ALGORITHM2


	#define	LL_FEATURE_ENABLE_LE_EXTENDED_SCAN							0 //vendor define

	#define LL_CMD_MAX						   							LL_MIN_USED_CHN_IND
#else


#endif


#ifndef		 LL_FEATURE_ENABLE_LE_ENCRYPTION
#define		 LL_FEATURE_ENABLE_LE_ENCRYPTION							0
#endif

#ifndef		 LL_FEATURE_ENABLE_CONNECTION_PARA_REQUEST_PROCEDURE
#define		 LL_FEATURE_ENABLE_CONNECTION_PARA_REQUEST_PROCEDURE		0
#endif

#ifndef		 LL_FEATURE_ENABLE_EXTENDED_REJECT_INDICATION
#define		 LL_FEATURE_ENABLE_EXTENDED_REJECT_INDICATION				0
#endif

#ifndef		 LL_FEATURE_ENABLE_SLAVE_INITIATED_FEATURES_EXCHANGE
#define		 LL_FEATURE_ENABLE_SLAVE_INITIATED_FEATURES_EXCHANGE		0
#endif

#ifndef		 LL_FEATURE_ENABLE_LE_PING
#define		 LL_FEATURE_ENABLE_LE_PING									0
#endif

#ifndef		 LL_FEATURE_ENABLE_LE_DATA_LENGTH_EXTENSION
#define		 LL_FEATURE_ENABLE_LE_DATA_LENGTH_EXTENSION					0
#endif

#ifndef		 LL_FEATURE_ENABLE_LL_PRIVACY
#define		 LL_FEATURE_ENABLE_LL_PRIVACY								0
#endif

#ifndef		 LL_FEATURE_ENABLE_EXTENDED_SCANNER_FILTER_POLICIES
#define		 LL_FEATURE_ENABLE_EXTENDED_SCANNER_FILTER_POLICIES			0
#endif

#ifndef		 LL_FEATURE_ENABLE_LE_2M_PHY
#define		 LL_FEATURE_ENABLE_LE_2M_PHY								0
#endif

#ifndef		 LL_FEATURE_ENABLE_STABLE_MODULATION_INDEX_TX
#define		 LL_FEATURE_ENABLE_STABLE_MODULATION_INDEX_TX				0
#endif

#ifndef		 LL_FEATURE_ENABLE_STABLE_MODULATION_INDEX_RX
#define		 LL_FEATURE_ENABLE_STABLE_MODULATION_INDEX_RX				0
#endif

#ifndef		 LL_FEATURE_ENABLE_LE_CODED_PHY
#define		 LL_FEATURE_ENABLE_LE_CODED_PHY								0
#endif

#ifndef		 LL_FEATURE_ENABLE_LE_EXTENDED_ADVERTISING
#define		 LL_FEATURE_ENABLE_LE_EXTENDED_ADVERTISING					0
#endif

#ifndef		 LL_FEATURE_ENABLE_LE_PERIODIC_ADVERTISING
#define		 LL_FEATURE_ENABLE_LE_PERIODIC_ADVERTISING					0
#endif

#ifndef		 LL_FEATURE_ENABLE_CHANNEL_SELECTION_ALGORITHM2
#define		 LL_FEATURE_ENABLE_CHANNEL_SELECTION_ALGORITHM2				0
#endif

#ifndef		 LL_FEATURE_ENABLE_LE_POWER_CLASS_1
#define		 LL_FEATURE_ENABLE_LE_POWER_CLASS_1							0
#endif

#ifndef		 LL_FEATURE_ENABLE_MIN_USED_OF_USED_CHANNELS
#define		 LL_FEATURE_ENABLE_MIN_USED_OF_USED_CHANNELS				0
#endif




//BIT<0:31>
#if 1

// feature below is conFiged by application layer
// LL_FEATURE_ENABLE_LE_2M_PHY
// LL_FEATURE_ENABLE_LE_CODED_PHY
// LL_FEATURE_ENABLE_LE_EXTENDED_ADVERTISING
// LL_FEATURE_ENABLE_CHANNEL_SELECTION_ALGORITHM2

#define LL_FEATURE_MASK_BASE0											(	LL_FEATURE_ENABLE_LE_ENCRYPTION 					<<0		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_CONNECTION_PARA_REQUEST_PROCEDURE <<1		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_EXTENDED_REJECT_INDICATION 	   	<<2		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_SLAVE_INITIATED_FEATURES_EXCHANGE <<3		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_PING 						   	<<4		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_DATA_LENGTH_EXTENSION 		   	<<5		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LL_PRIVACY 					   	<<6		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_EXTENDED_SCANNER_FILTER_POLICIES  <<7		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_STABLE_MODULATION_INDEX_TX 	   	<<9		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_STABLE_MODULATION_INDEX_RX  	   	<<10	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_PERIODIC_ADVERTISING 		   	<<13	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_POWER_CLASS_1 				   	<<15	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_MIN_USED_OF_USED_CHANNELS 		<<16 	)
#else
#define LL_FEATURE_MASK_0												(	LL_FEATURE_ENABLE_LE_ENCRYPTION 					<<0		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_CONNECTION_PARA_REQUEST_PROCEDURE <<1		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_EXTENDED_REJECT_INDICATION 	   	<<2		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_SLAVE_INITIATED_FEATURES_EXCHANGE <<3		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_PING 						   	<<4		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_DATA_LENGTH_EXTENSION 		   	<<5		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LL_PRIVACY 					   	<<6		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_EXTENDED_SCANNER_FILTER_POLICIES  <<7		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_2M_PHY 						<<8		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_STABLE_MODULATION_INDEX_TX 	   	<<9		|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_STABLE_MODULATION_INDEX_RX  	   	<<10	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_CODED_PHY 					   	<<11	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_EXTENDED_ADVERTISING  		   	<<12	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_PERIODIC_ADVERTISING 		   	<<13	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_CHANNEL_SELECTION_ALGORITHM2 	   	<<14	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_LE_POWER_CLASS_1 				   	<<15	|  \
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	LL_FEATURE_ENABLE_MIN_USED_OF_USED_CHANNELS 		<<16 	)

#endif


//BIT<32:63>
// feature below is conFiged by application layer or HCI command
// <32> :	LL_FEATURE_ENABLE_ISOCHRONOUS_CHANNELS
#define LL_FEATURE_MASK_BASE1											0



extern u32 LL_FEATURE_MASK_0;
extern u32 LL_FEATURE_MASK_1;



#define LL_FEATURE_BYTE_0												U32_BYTE0(LL_FEATURE_MASK_0)
#define LL_FEATURE_BYTE_1												U32_BYTE1(LL_FEATURE_MASK_0)
#define LL_FEATURE_BYTE_2												U32_BYTE2(LL_FEATURE_MASK_0)
#define LL_FEATURE_BYTE_3												U32_BYTE3(LL_FEATURE_MASK_0)
#define LL_FEATURE_BYTE_4												U32_BYTE0(LL_FEATURE_MASK_1)
#define LL_FEATURE_BYTE_5												U32_BYTE1(LL_FEATURE_MASK_1)
#define LL_FEATURE_BYTE_6												U32_BYTE2(LL_FEATURE_MASK_1)
#define LL_FEATURE_BYTE_7												U32_BYTE3(LL_FEATURE_MASK_1)






///////////////// ADV Interval/Scan Interval/Scan Window/Connection Interval  /////////////////
#define 		ADV_INTERVAL_3_125MS                        5
#define 		ADV_INTERVAL_3_75MS                         6
#define 		ADV_INTERVAL_10MS                           16
#define 		ADV_INTERVAL_15MS                           24
#define 		ADV_INTERVAL_20MS                           32
#define 		ADV_INTERVAL_25MS                           40
#define 		ADV_INTERVAL_30MS                           48
#define 		ADV_INTERVAL_35MS                           56
#define 		ADV_INTERVAL_40MS                           64
#define 		ADV_INTERVAL_45MS                           72
#define 		ADV_INTERVAL_50MS                           80
#define 		ADV_INTERVAL_55MS                           88
#define 		ADV_INTERVAL_60MS                           96
#define 		ADV_INTERVAL_70MS                           112
#define 		ADV_INTERVAL_80MS                           128
#define 		ADV_INTERVAL_90MS							144
#define 		ADV_INTERVAL_100MS                          160
#define 		ADV_INTERVAL_105MS                          168
#define 		ADV_INTERVAL_200MS                          320
#define 		ADV_INTERVAL_205MS                          328
#define 		ADV_INTERVAL_300MS                          480
#define 		ADV_INTERVAL_305MS                          488
#define 		ADV_INTERVAL_400MS                          640
#define 		ADV_INTERVAL_405MS                          648
#define 		ADV_INTERVAL_500MS                          800
#define 		ADV_INTERVAL_505MS                          808

#define 		ADV_INTERVAL_1S                          	1600
#define 		ADV_INTERVAL_1S5                          	2400
#define 		ADV_INTERVAL_2S                          	3200
#define 		ADV_INTERVAL_1_28_S                         0x0800
#define 		ADV_INTERVAL_10_24S                         16384



#define 		SCAN_INTERVAL_10MS                          16
#define 		SCAN_INTERVAL_30MS                          48
#define 		SCAN_INTERVAL_50MS                          80
#define 		SCAN_INTERVAL_60MS                          96
#define 		SCAN_INTERVAL_90MS                          144
#define 		SCAN_INTERVAL_100MS                         160
#define 		SCAN_INTERVAL_200MS                         320
#define 		SCAN_INTERVAL_300MS                         480
#define 		SCAN_INTERVAL_400MS                         640
#define 		SCAN_INTERVAL_500MS                         800


#define 		CONN_INTERVAL_7P5MS                          6
#define 		CONN_INTERVAL_10MS                           8
#define 		CONN_INTERVAL_11P25MS                        9
#define 		CONN_INTERVAL_15MS                           12
#define 		CONN_INTERVAL_16P25MS                        13
#define 		CONN_INTERVAL_18P75MS                        15
#define 		CONN_INTERVAL_20MS                           16
#define 		CONN_INTERVAL_30MS                           24
#define 		CONN_INTERVAL_31P25MS                        25
#define 		CONN_INTERVAL_38P75MS                        31
#define 		CONN_INTERVAL_40MS                           32
#define 		CONN_INTERVAL_48P75MS                        39
#define 		CONN_INTERVAL_50MS                           40
#define 		CONN_INTERVAL_75MS                           60
#define 		CONN_INTERVAL_100MS                          80


#define 		CONN_TIMEOUT_500MS							 50
#define 		CONN_TIMEOUT_1S							 	 100
#define 		CONN_TIMEOUT_2S							 	 200
#define 		CONN_TIMEOUT_3S							 	 300
#define 		CONN_TIMEOUT_4S							 	 400
#define 		CONN_TIMEOUT_5S							 	 500
#define 		CONN_TIMEOUT_6S							 	 600
#define 		CONN_TIMEOUT_7S							 	 700
#define 		CONN_TIMEOUT_8S							 	 800
#define 		CONN_TIMEOUT_9S							 	 900
#define 		CONN_TIMEOUT_10S							 1000
#define 		CONN_TIMEOUT_20S							 2000


/*********************************************************************
 * ENUMS
 */
typedef enum {
    BLE_SUCCESS = 0,

//// HCI Status, See the Core_v5.0(Vol 2/Part D/1.3 "list of Error Codes") for more information)
    HCI_ERR_UNKNOWN_HCI_CMD                                        = 0x01,
    HCI_ERR_UNKNOWN_CONN_ID                                        = 0x02,
    HCI_ERR_HW_FAILURE                                             = 0x03,
    HCI_ERR_PAGE_TIMEOUT                                           = 0x04,
    HCI_ERR_AUTH_FAILURE                                           = 0x05,
    HCI_ERR_PIN_KEY_MISSING                                        = 0x06,
    HCI_ERR_MEM_CAP_EXCEEDED                                       = 0x07,
    HCI_ERR_CONN_TIMEOUT                                           = 0x08,
    HCI_ERR_CONN_LIMIT_EXCEEDED                                    = 0x09,
    HCI_ERR_SYNCH_CONN_LIMIT_EXCEEDED                              = 0x0A,
    HCI_ERR_ACL_CONN_ALREADY_EXISTS                                = 0x0B,
    HCI_ERR_CMD_DISALLOWED                                         = 0x0C,
    HCI_ERR_CONN_REJ_LIMITED_RESOURCES                             = 0x0D,
    HCI_ERR_CONN_REJECTED_SECURITY_REASONS                         = 0x0E,
    HCI_ERR_CONN_REJECTED_UNACCEPTABLE_BDADDR                      = 0x0F,
    HCI_ERR_CONN_ACCEPT_TIMEOUT_EXCEEDED                           = 0x10,
    HCI_ERR_UNSUPPORTED_FEATURE_PARAM_VALUE                        = 0x11,
    HCI_ERR_INVALID_HCI_CMD_PARAMS                                 = 0x12,
    HCI_ERR_REMOTE_USER_TERM_CONN                                  = 0x13,
    HCI_ERR_REMOTE_DEVICE_TERM_CONN_LOW_RESOURCES                  = 0x14,
    HCI_ERR_REMOTE_DEVICE_TERM_CONN_POWER_OFF                      = 0x15,
    HCI_ERR_CONN_TERM_BY_LOCAL_HOST                                = 0x16,
    HCI_ERR_REPEATED_ATTEMPTS                                      = 0x17,
    HCI_ERR_PAIRING_NOT_ALLOWED                                    = 0x18,
    HCI_ERR_UNKNOWN_LMP_PDU                                        = 0x19,
    HCI_ERR_UNSUPPORTED_REMOTE_FEATURE                             = 0x1A,
    HCI_ERR_SCO_OFFSET_REJ                                         = 0x1B,
    HCI_ERR_SCO_INTERVAL_REJ                                       = 0x1C,
    HCI_ERR_SCO_AIR_MODE_REJ                                       = 0x1D,
    HCI_ERR_INVALID_LMP_PARAMS                                     = 0x1E,
    HCI_ERR_UNSPECIFIED_ERROR                                      = 0x1F,
    HCI_ERR_UNSUPPORTED_LMP_PARAM_VAL                              = 0x20,
    HCI_ERR_ROLE_CHANGE_NOT_ALLOWED                                = 0x21,
    HCI_ERR_LMP_LL_RESP_TIMEOUT                                    = 0x22,
    HCI_ERR_LMP_ERR_TRANSACTION_COLLISION                          = 0x23,
    HCI_ERR_LMP_PDU_NOT_ALLOWED                                    = 0x24,
    HCI_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE                            = 0x25,
    HCI_ERR_LINK_KEY_CAN_NOT_BE_CHANGED                            = 0x26,
    HCI_ERR_REQ_QOS_NOT_SUPPORTED                                  = 0x27,
    HCI_ERR_INSTANT_PASSED                                         = 0x28,
    HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                    = 0x29,
    HCI_ERR_DIFFERENT_TRANSACTION_COLLISION                        = 0x2A,
    HCI_ERR_RESERVED1                                              = 0x2B,
    HCI_ERR_QOS_UNACCEPTABLE_PARAM                                 = 0x2C,
    HCI_ERR_QOS_REJ                                                = 0x2D,
    HCI_ERR_CHAN_ASSESSMENT_NOT_SUPPORTED                          = 0x2E,
    HCI_ERR_INSUFFICIENT_SECURITY                                  = 0x2F,
    HCI_ERR_PARAM_OUT_OF_MANDATORY_RANGE                           = 0x30,
    HCI_ERR_RESERVED2                                              = 0x31,
    HCI_ERR_ROLE_SWITCH_PENDING                                    = 0x32,
    HCI_ERR_RESERVED3                                              = 0x33,
    HCI_ERR_RESERVED_SLOT_VIOLATION                                = 0x34,
    HCI_ERR_ROLE_SWITCH_FAILED                                     = 0x35,
    HCI_ERR_EXTENDED_INQUIRY_RESP_TOO_LARGE                        = 0x36,
    HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST                   = 0x37,
    HCI_ERR_HOST_BUSY_PAIRING                                      = 0x38,
    HCI_ERR_CONN_REJ_NO_SUITABLE_CHAN_FOUND                        = 0x39,
    HCI_ERR_CONTROLLER_BUSY                                        = 0x3A,
    HCI_ERR_UNACCEPTABLE_CONN_INTERVAL                             = 0x3B,
    HCI_ERR_ADVERTISING_TIMEOUT                                    = 0x3C,
    HCI_ERR_CONN_TERM_MIC_FAILURE                                  = 0x3D,
    HCI_ERR_CONN_FAILED_TO_ESTABLISH                               = 0x3E,
    HCI_ERR_MAC_CONN_FAILED                                        = 0x3F,
    HCI_ERR_COARSE_CLOCK_ADJUSTMENT_REJECT						   = 0x40,
    HCI_ERR_TYPE0_SUBMAP_NOT_DEFINED							   = 0x41,
	HCI_ERR_UNKNOWN_ADV_IDENTIFIER								   = 0x42,
    HCI_ERR_LIMIT_REACHED										   = 0x43,
    HCI_ERR_OP_CANCELLED_BY_HOST								   = 0x44,
    




///////////////////////// TELINK define status /////////////////////////////

    //LL status
	LL_ERR_CONNECTION_NOT_ESTABLISH 							   = 0x80,
	LL_ERR_TX_FIFO_NOT_ENOUGH,
	LL_ERR_ENCRYPTION_BUSY,
	LL_ERR_CURRENT_STATE_NOT_SUPPORTED_THIS_CMD,

	//L2CAP status
    L2CAP_ERR_INVALID_PARAMETER 								   = 0x90,

    //SMP status
	SMP_ERR_INVALID_PARAMETER 									   = 0xA0,
	SMP_ERR_PAIRING_BUSY,

	//GATT status
	GATT_ERR_INVALID_PARAMETER 									   = 0xB0,
	GATT_ERR_PREVIOUS_INDICATE_DATA_HAS_NOT_CONFIRMED,
	GATT_ERR_SERVICE_DISCOVERY_TIEMOUT,
	GATT_ERR_NOTIFY_INDICATION_NOT_PERMITTED,
	GATT_ERR_DATA_PENDING_DUE_TO_SERVICE_DISCOVERY_BUSY,
	GATT_ERR_DATA_LENGTH_EXCEED_MTU_SIZE,

	//GAP status
	GAP_ERR_INVALID_PARAMETER 								   	   = 0xC0,

	//Service status
	SERVICE_ERR_INVALID_PARAMETER 								   = 0xD0,

} ble_sts_t;






// l2cap pb flag type
#define L2CAP_FRIST_PKT_H2C              0x00
#define L2CAP_CONTINUING_PKT             0x01
#define L2CAP_FIRST_PKT_C2H              0x02



/******************************************** Link Layer **************************************************************/


/////////////////////////////// BLE  MAC ADDRESS //////////////////////////////////////////////
#define BLE_ADDR_PUBLIC                  0
#define BLE_ADDR_RANDOM                  1
#define BLE_ADDR_INVALID                 0xff
#define BLE_ADDR_LEN                     6


//Definition for BLE Common Address Type
/*
 *
 *				  |--public  ..................................................... BLE_DEVICE_ADDRESS_PUBLIC
 *                |
 * Address Type --|		      |-- random static  ................................. BLE_DEVICE_ADDRESS_RANDOM_STATIC
 *           	  |           |
 *    			  |--random --|
 * 			   				  |				       |-- non_resolvable private  ... BLE_DEVICE_ADDRESS_NON_RESOLVABLE_PRIVATE
 * 			 				  |-- random private --|
 *           					                   |-- resolvable private  ....... BLE_DEVICE_ADDRESS_RESOLVABLE_PRIVATE
 *
 */

#define	BLE_DEVICE_ADDRESS_PUBLIC							1
#define BLE_DEVICE_ADDRESS_RANDOM_STATIC					2
#define BLE_DEVICE_ADDRESS_NON_RESOLVABLE_PRIVATE			3
#define BLE_DEVICE_ADDRESS_RESOLVABLE_PRIVATE				4



#define IS_PUBLIC_ADDR(Type, Addr)  					( (Type)==BLE_ADDR_PUBLIC) )
#define IS_RANDOM_STATIC_ADDR(Type, Addr)  				( (Type)==BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0xC0 )
#define IS_NON_RESOLVABLE_PRIVATE_ADDR(Type, Addr)  	( (Type)==BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0x00 )
#define IS_RESOLVABLE_PRIVATE_ADDR(Type, Addr)  		( (Type)==BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0x40 )




typedef enum{
	OWN_ADDRESS_PUBLIC = 0,
	OWN_ADDRESS_RANDOM = 1,
	OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC = 2,
	OWN_ADDRESS_RESOLVE_PRIVATE_RANDOM = 3,
}own_addr_type_t;
/******************************************** ATT ***************************************************************/

/**
 *  @brief  Definition for Attribute protocol PDUs
 */
typedef enum{
	ATT_OP_ERROR_RSP					= 0x01,
	ATT_OP_EXCHANGE_MTU_REQ				= 0x02,
	ATT_OP_EXCHANGE_MTU_RSP				= 0x03,
	ATT_OP_FIND_INFORMATION_REQ			= 0x04,		ATT_OP_FIND_INFO_REQ = 0x04,
	ATT_OP_FIND_INFORMATION_RSP			= 0x05,		ATT_OP_FIND_INFO_RSP = 0x05,
	ATT_OP_FIND_BY_TYPE_VALUE_REQ		= 0x06,
	ATT_OP_FIND_BY_TYPE_VALUE_RSP		= 0x07,
	ATT_OP_READ_BY_TYPE_REQ				= 0x08,
	ATT_OP_READ_BY_TYPE_RSP				= 0x09,
	ATT_OP_READ_REQ						= 0x0A,
	ATT_OP_READ_RSP						= 0x0B,
	ATT_OP_READ_BLOB_REQ				= 0x0C,
	ATT_OP_READ_BLOB_RSP				= 0x0D,
	ATT_OP_READ_MULTIPLE_REQ			= 0x0E,		ATT_OP_READ_MULTI_REQ = 0x0E,
	ATT_OP_READ_MULTIPLE_RSP			= 0x0F,
	ATT_OP_READ_BY_GROUP_TYPE_REQ		= 0x10,
	ATT_OP_READ_BY_GROUP_TYPE_RSP		= 0x11,
	ATT_OP_WRITE_REQ					= 0x12,
	ATT_OP_WRITE_RSP					= 0x13,
	ATT_OP_PREPARE_WRITE_REQ			= 0x16,
	ATT_OP_PREPARE_WRITE_RSP			= 0x17,
	ATT_OP_EXECUTE_WRITE_REQ			= 0x18,
	ATT_OP_EXECUTE_WRITE_RSP			= 0x19,

	ATT_OP_HANDLE_VALUE_NTF				= 0x1B,		ATT_OP_HANDLE_VALUE_NOTI = 0x1B,
	ATT_OP_HANDLE_VALUE_IND				= 0x1D,
	ATT_OP_HANDLE_VALUE_CFM				= 0x1E,

	ATT_OP_READ_MULTIPLE_VARIABLE_REQ	= 0x20,	//core_5.2
	ATT_OP_READ_MULTIPLE_VARIABLE_RSP	= 0x21, //core_5.2
	ATT_OP_MULTIPLE_HANDLE_VALUE_NTF	= 0x23, //core_5.2

	ATT_OP_WRITE_CMD					= 0x52,
	ATT_OP_SIGNED_WRITE_CMD				= 0xD2,
}att_pdu_type;




/******************************************** L2CAP ***************************************************************/

/**
 *  @brief  Definition for L2CAP CID name space for the LE-U
 */
typedef enum{
	L2CAP_CID_NULL				= 0x0000,
	L2CAP_CID_ATTR_PROTOCOL		= 0x0004,
	L2CAP_CID_SIG_CHANNEL		= 0x0005,
	L2CAP_CID_SMP				= 0x0006,
}l2cap_cid_type;

/**
 *  @brief  Definition for L2CAP signal packet formats
 */
typedef enum{
	L2CAP_COMMAND_REJECT_RSP           		= 0x01,
	L2CAP_CONNECTION_REQ                 	= 0x02,
	L2CAP_CONNECTION_RSP                 	= 0x03,
	L2CAP_CONFIGURATION_REQ                	= 0x04,
	L2CAP_CONFIGURATION_RSP           		= 0x05,
	L2CAP_DISCONNECTION_REQ           		= 0x06,
	L2CAP_DISCONNECTION_RSP           		= 0x07,
	L2CAP_ECHO_REQ          		 		= 0x08,
	L2CAP_ECHO_RSP           				= 0x09,
	L2CAP_INFORMATION_REQ           		= 0x0A,
	L2CAP_INFORMATION_RSP           		= 0x0B,
	L2CAP_CREATE_CHANNEL_REQ          		= 0x0C,
	L2CAP_CREATE_CHANNEL_RSP           		= 0x0D,
	L2CAP_MOVE_CHANNEL_REQ           		= 0x0E,
	L2CAP_MOVE_CHANNEL_RSP           		= 0x0F,
	L2CAP_MOVE_CHANNEL_CONFIRMATION_REQ		= 0x10,
	L2CAP_MOVE_CHANNEL_CONFIRMATION_RSP     = 0x11,
	L2CAP_CONNECTION_PARAMETER_UPDATE_REQ	= 0x12,		L2CAP_CMD_CONN_UPD_PARA_REQ		= 0x12,
	L2CAP_CONNECTION_PARAMETER_UPDATE_RSP	= 0x13,		L2CAP_CMD_CONN_UPD_PARA_RESP 	= 0x13,
	L2CAP_LE_CREDIT_BASED_CONNECTION_REQ 	= 0x14,
	L2CAP_LE_CREDIT_BASED_CONNECTION_RSP 	= 0x15,
	L2CAP_FLOW_CONTROL_CREDIT_IND 			= 0x16,
	L2CAP_CREDIT_BASED_CONNECTION_REQ 		= 0x17,	//core_5.2
	L2CAP_CREDIT_BASED_CONNECTION_RSP 		= 0x18,	//core_5.2
	L2CAP_CREDIT_BASED_RECONFIGURE_REQ 		= 0x19,	//core_5.2
	L2CAP_CREDIT_BASED_RECONFIGURE_RSP 		= 0x1A,	//core_5.2
}l2cap_sig_pkt_format;
#endif
