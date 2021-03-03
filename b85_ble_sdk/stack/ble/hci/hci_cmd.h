/********************************************************************************************************
 * @file	hci_cmd.h
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

#ifndef HCI_CMD_H_
#define HCI_CMD_H_


/* Advertising_Channel_Map */
typedef enum{
	BLT_ENABLE_ADV_37	=		BIT(0),
	BLT_ENABLE_ADV_38	=		BIT(1),
	BLT_ENABLE_ADV_39	=		BIT(2),
	BLT_ENABLE_ADV_ALL	=		(BLT_ENABLE_ADV_37 | BLT_ENABLE_ADV_38 | BLT_ENABLE_ADV_39),
}adv_chn_map_t;
typedef enum {
	 TX_POWER_0dBm  = 0,
	 TX_POWER_1dBm  = 1,
	 TX_POWER_2dBm 	= 2,
	 TX_POWER_3dBm 	= 3,
	 TX_POWER_4dBm 	= 4,
	 TX_POWER_5dBm 	= 5,
	 TX_POWER_6dBm 	= 6,
	 TX_POWER_7dBm 	= 7,
	 TX_POWER_8dBm 	= 8,
	 TX_POWER_9dBm  = 9,
	 TX_POWER_10dBm = 10,
} tx_power_t;


/**
 *  @brief   "Operation" in "LE Set Extended Advertising Data Command" and "LE Set Extended Scan Response Data Command"
 */
typedef enum {
	DATA_OPER_INTER      	=	0x00,
	DATA_OPER_FIRST      	=	0x01,
	DATA_OPER_LAST       	=	0x02,
	DATA_OPER_COMPLETE   	=	0x03,
	DATA_OPER_UNCHANGEED	=  	0x04,
} data_oper_t;




/**
 *  @brief   "Fragment_Preference" in "LE Set Extended Advertising Data Command" and "LE Set Extended Scan Response Data Command"
 */
typedef enum {
	DATA_FRAGM_ALLOWED			      	=	0x00,
	DATA_FRAGM_NONE_OR_MINIMIZE      	=	0x01,
} data_fragm_t;















/**
 *  @brief  Return Parameters for "HCI LE Read PHY Command"
 */
typedef struct {
	u8         status;
	u8         handle[2];
	u8         tx_phy;
	u8         rx_phy;
} hci_le_readPhyCmd_retParam_t;




/**
 *  @brief  Command Parameters for "HCI LE Set PHY Command"
 */

typedef struct {
	u16 connHandle;
	u8 	all_phys;
	u8 	tx_phys;
	u8 	rx_phys;
	u16 phy_options;
} hci_le_setPhyCmd_param_t;







/**
 *  @brief  Command Parameters Definition for "HCI LE Set Extended Advertising Parameters Command"
 */
typedef struct {
	u8		adv_handle;
    u16		advEvt_props;
    u8	 	pri_advIntMin[3];
	u8		pri_advIntMax[3];
    u8 		pri_advChnMap;
    u8		ownAddrType;
    u8 		peerAddrType;
    u8  	peerAddr[6];
    u8		advFilterPolicy;
    u8 		adv_tx_pow;
    u8		pri_adv_phy;
    u8		sec_adv_max_skip;
    u8		sec_adv_phy;
    u8		adv_sid;
    u8		scan_req_noti_en;
}hci_le_setExtAdvParam_cmdParam_t;

/* Advertising_Handle */
typedef enum{
	ADV_HANDLE0		= 0x00,
	ADV_HANDLE1		= 0x01,
	ADV_HANDLE2		= 0x02,
}adv_handle_t;



/* Advertising Event Properties mask*/
typedef enum{
	ADVEVT_PROP_MASK_CONNECTABLE						=	BIT(0),
	ADVEVT_PROP_MASK_SCANNABLE     						=	BIT(1),
	ADVEVT_PROP_MASK_DIRECTED  							=   BIT(2),
	ADVEVT_PROP_MASK_HD_DIRECTED     					=	BIT(3),
	ADVEVT_PROP_MASK_LEGACY          					=	BIT(4),
	ADVEVT_PROP_MASK_ANON_ADV       		 			=	BIT(5),
	ADVEVT_PROP_MASK_INC_TX_PWR      					=	BIT(6),
}advEvtProp_mask_t;


#define ADVEVT_PROP_MASK_CONNECTABLE_SCANNABLE					(0x0003)  // ADVEVT_PROP_MASK_CONNECTABLE | ADVEVT_PROP_MASK_SCANNABLE
#define ADVEVT_PROP_MASK_LEGACY_SCANNABLE						(0x0012)  // ADVEVT_PROP_MASK_LEGACY | ADVEVT_PROP_MASK_SCANNABLE
#define	ADVEVT_PROP_MASK_LEGACY_DIRECTED						(0x0014)  // ADVEVT_PROP_MASK_LEGACY | ADVEVT_PROP_MASK_DIRECTED
#define	ADVEVT_PROP_MASK_LEGACY_HD_DIRECTED						(0x0018)  // ADVEVT_PROP_MASK_LEGACY | ADVEVT_PROP_MASK_HD_DIRECTED
#define ADVEVT_PROP_MASK_LEGACY_CONNECTABLE_SCANNABLE			(0x0013)  // ADVEVT_PROP_MASK_LEGACY | ADVEVT_PROP_MASK_CONNECTABLE | ADVEVT_PROP_MASK_SCANNABLE

/* Advertising Event Properties
 * See the Core_v5.0(Vol 2/Part E/7.8.53 & Vol 6/Part B/4.4.2/Table 4.1) for more information
*/
typedef enum{
  ADV_EVT_PROP_LEGACY_CONNECTABLE_SCANNABLE_UNDIRECTED 				       	= 0x0013,		//  0001 0011'b 	ADV_IND
  ADV_EVT_PROP_LEGACY_CONNECTABLE_DIRECTED_LOW_DUTY 				       	= 0x0015,		//  0001 0101'b		ADV_DIRECT_IND(low duty cycle)
  ADV_EVT_PROP_LEGACY_CONNECTABLE_DIRECTED_HIGH_DUTY 	 				    = 0x001D,		//  0001 1101'b		ADV_DIRECT_IND(high duty cycle)
  ADV_EVT_PROP_LEGACY_SCANNABLE_UNDIRECTED 							       	= 0x0012,		//  0001 0010'b		ADV_SCAN_IND
  ADV_EVT_PROP_LEGACY_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED				= 0x0010,		//  0001 0000'b		ADV_NONCONN_IND


  ADV_EVT_PROP_EXTENDED_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED    	    = 0x0000,		//  0000 0000'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
  ADV_EVT_PROP_EXTENDED_CONNECTABLE_UNDIRECTED       				 	  	= 0x0001,		//  0000 0001'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
  ADV_EVT_PROP_EXTENDED_SCANNABLE_UNDIRECTED						        = 0x0002,		//  0000 0010'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
  ADV_EVT_PROP_EXTENDED_NON_CONNECTABLE_NON_SCANNABLE_DIRECTED				= 0x0004,		//  0000 0100'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
  ADV_EVT_PROP_EXTENDED_CONNECTABLE_DIRECTED			       				= 0x0005,		//  0000 0101'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
  ADV_EVT_PROP_EXTENDED_SCANNABLE_DIRECTED								    = 0x0006,		//  0000 0110'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND


  ADV_EVT_PROP_EXTENDED_MASK_ANONYMOUS_ADV 		= 0x0020,  //if this mask on(only extended ADV event can mask it), anonymous advertising
  ADV_EVT_PROP_EXTENDED_MASK_TX_POWER_INCLUDE	= 0x0040,  //if this mask on(only extended ADV event can mask it), TX power include

}advEvtProp_type_t;
ble_sts_t blc_hci_le_setPhy(hci_le_setPhyCmd_param_t* para);


#endif /* HCI_CMD_H_ */
