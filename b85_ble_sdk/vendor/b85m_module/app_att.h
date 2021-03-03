/********************************************************************************************************
 * @file     app_att.h
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

#ifndef APP_ATT_H_
#define APP_ATT_H_



///////////////////////////////////// ATT  HANDLER define ///////////////////////////////////////
typedef enum
{
	ATT_H_START = 0,


	//// Gap ////
	/**********************************************************************************************/
	GenericAccess_PS_H, 					//UUID: 2800, 	VALUE: uuid 1800
	GenericAccess_DeviceName_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	GenericAccess_DeviceName_DP_H,			//UUID: 2A00,   VALUE: device name
	GenericAccess_Appearance_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	GenericAccess_Appearance_DP_H,			//UUID: 2A01,	VALUE: appearance
	CONN_PARAM_CD_H,						//UUID: 2803, 	VALUE:  			Prop: Read
	CONN_PARAM_DP_H,						//UUID: 2A04,   VALUE: connParameter


	//// gatt ////
	/**********************************************************************************************/
	GenericAttribute_PS_H,					//UUID: 2800, 	VALUE: uuid 1801
	GenericAttribute_ServiceChanged_CD_H,	//UUID: 2803, 	VALUE:  			Prop: Indicate
	GenericAttribute_ServiceChanged_DP_H,   //UUID:	2A05,	VALUE: service change
	GenericAttribute_ServiceChanged_CCB_H,	//UUID: 2902,	VALUE: serviceChangeCCC


	//// device information ////
	/**********************************************************************************************/
	DeviceInformation_PS_H,					 //UUID: 2800, 	VALUE: uuid 180A
	DeviceInformation_pnpID_CD_H,			 //UUID: 2803, 	VALUE:  			Prop: Read
	DeviceInformation_pnpID_DP_H,			 //UUID: 2A50,	VALUE: PnPtrs

	//// SPP ////
	/**********************************************************************************************/
	SPP_PS_H, 							 //UUID: 2800, 	VALUE: telink spp service uuid

	//server to client
	SPP_SERVER_TO_CLIENT_CD_H,		     //UUID: 2803, 	VALUE:  			Prop: read | Notify
	SPP_SERVER_TO_CLIENT_DP_H,			 //UUID: telink spp s2c uuid,  VALUE: SppDataServer2ClientData
	SPP_SERVER_TO_CLIENT_CCB_H,			 //UUID: 2902, 	VALUE: SppDataServer2ClientDataCCC
	SPP_SERVER_TO_CLIENT_DESC_H,		 //UUID: 2901, 	VALUE: TelinkSPPS2CDescriptor

	//client to server
	SPP_CLIENT_TO_SERVER_CD_H,		     //UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp
	SPP_CLIENT_TO_SERVER_DP_H,			 //UUID: telink spp c2s uuid,  VALUE: SppDataClient2ServerData
	SPP_CLIENT_TO_SERVER_DESC_H,		 //UUID: 2901, 	VALUE: TelinkSPPC2SDescriptor

	//// Ota ////
	/**********************************************************************************************/
	OTA_PS_H, 								//UUID: 2800, 	VALUE: telink ota service uuid
	OTA_CMD_OUT_CD_H,						//UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp | Notify
	OTA_CMD_OUT_DP_H,						//UUID: telink ota uuid,  VALUE: otaData
	OTA_CMD_INPUT_CCB_H,					//UUID: 2902, 	VALUE: otaDataCCC
	OTA_CMD_OUT_DESC_H,						//UUID: 2901, 	VALUE: otaName


	ATT_END_H,

}ATT_HANDLE;


/**
 * @brief      Initialize the attribute table
 * @param[in]  none
 * @return     none
 */
void my_att_init(void);

#endif /* APP_ATT_H_ */
