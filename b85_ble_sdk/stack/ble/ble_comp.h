/********************************************************************************************************
 * @file	ble_comp.h
 *
 * @brief	This is the header file for BLE SDK
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

#ifndef BLE_COMP_H_
#define BLE_COMP_H_


/* for some API name in old version SDK compatible with new version SDK */

///////////////////////////////////controller//////////////////////////////////////
#define blt_ll_exchangeDataLength						blc_ll_exchangeDataLength
#define blc_ll_modifyBluetoothVersion					blc_contr_setBluetoothVersion
#define blc_pm_modefy_brx_early_set                  blc_pm_modify_brx_early_set
#define blc_ll_set_CustomedAdvScanAccessCode         blc_ll_set_CustomizedAdvScanAccessCode

#define	ll_resolvingList_reset							blc_ll_clearResolvingList
#define	ll_resolvingList_add							blc_ll_addDeviceToResolvingList
#define	ll_resolvingList_setAddrResolutionEnable		blc_ll_setAddressResolutionEnable

#define	ll_whiteList_reset								blc_ll_clearWhiteList
#define	ll_whiteList_add								blc_ll_addDeviceToWhiteList
#define	ll_whiteList_delete								blc_ll_removeDeviceFromWhiteList

#define blc_smp_setPeerAddrResSupportFlg
#define blc_ll_resolvListInit()
#define ll_resolvingList_setResolvablePrivateAddrTimer

/////////////////////////////////////L2CAP/////////////////////////////////////////////
#define blc_l2cap_reg_att_cli_hander					blc_l2cap_reg_att_cli_handler

////////////////////////////////////host//////////////////////////////////////
#define	blt_att_setEffectiveMtuSize						blc_att_setEffectiveMtuSize
#define blt_att_resetEffectiveMtuSize					blc_att_resetEffectiveMtuSize
#define blc_gatt_pushWriteComand						blc_gatt_pushWriteCommand

#define blm_smp_configParingSecurityInfoStorageAddr		blm_smp_configPairingSecurityInfoStorageAddr
#define	blc_smp_setSecurityParamters					blc_smp_setSecurityParameters

////////////////////////////////////ota//////////////////////////////////////
#define bls_ota_set_fwSize_and_fwBootAddr				blc_ota_setFirmwareSizeAndBootAddress
#define	bls_ota_registerStartCmdCb						blc_ota_registerOtaStartCmdCb
#define	bls_ota_registerVersionReqCb					blc_ota_registerOtaFirmwareVersionReqCb
#define	bls_ota_registerResultIndicateCb				blc_ota_registerOtaResultIndicationCb
#define bls_ota_setTimeout(tm_us)						blc_ota_setOtaProcessTimeout( (tm_us)/1000000 )







/* for some macro name in old version SDK compatible with new version SDK */

///////////////////////////////////controller//////////////////////////////////////
#define BLT_EV_FLAG_RX_DATA_ABANDOM                  BLT_EV_FLAG_RX_DATA_ABANDON
/////////////////////////////////////L2CAP/////////////////////////////////////////////

////////////////////////////////////host//////////////////////////////////////

#define ATT_RX_MTU_SIZE_MAX								ATT_MTU_MAX_SDK_DFT_BUF

#define GATT_ERR_SERVICE_DISCOVERY_TIEMOUT				GATT_ERR_SERVICE_DISCOVERY_TIMEOUT
#define GAP_EVT_SMP_TK_DISPALY							GAP_EVT_SMP_TK_DISPLAY
#define GAP_EVT_MASK_SMP_TK_DISPALY						GAP_EVT_MASK_SMP_TK_DISPLAY
#define GAP_EVT_MASK_SMP_PARING_BEAGIN                  GAP_EVT_MASK_SMP_PAIRING_BEGIN
#define GAP_EVT_MASK_SMP_PARING_SUCCESS                 GAP_EVT_MASK_SMP_PAIRING_SUCCESS
#define GAP_EVT_MASK_SMP_PARING_FAIL                    GAP_EVT_MASK_SMP_PAIRING_FAIL
#define GAP_EVT_SMP_PARING_BEAGIN		                GAP_EVT_SMP_PAIRING_BEGIN	// Refer to SMP message sequence and event chart above
#define GAP_EVT_SMP_PARING_SUCCESS			            GAP_EVT_SMP_PAIRING_SUCCESS	// Refer to SMP message sequence and event chart above
#define GAP_EVT_SMP_PARING_FAIL			                GAP_EVT_SMP_PAIRING_FAIL

#define IO_CAPABLITY_DISPLAY_ONLY						IO_CAPABILITY_DISPLAY_ONLY
#define IO_CAPABLITY_DISPLAY_YESNO						IO_CAPABILITY_DISPLAY_YESNO
#define IO_CAPABLITY_KEYBOARD_ONLY						IO_CAPABILITY_KEYBOARD_ONLY
#define IO_CAPABLITY_NO_IN_NO_OUT						IO_CAPABILITY_NO_IN_NO_OUT
#define	IO_CAPABLITY_KEYBOARD_DISPLAY					IO_CAPABILITY_KEYBOARD_DISPLAY

#define PARING_FAIL_REASON_PASSKEY_ENTRY				PAIRING_FAIL_REASON_PASSKEY_ENTRY
#define PARING_FAIL_REASON_OOB_NOT_AVAILABLE			PAIRING_FAIL_REASON_OOB_NOT_AVAILABLE
#define PARING_FAIL_REASON_AUTH_REQUIRE					PAIRING_FAIL_REASON_AUTH_REQUIRE
#define PARING_FAIL_REASON_CONFIRM_FAILED				PAIRING_FAIL_REASON_CONFIRM_FAILED
#define PARING_FAIL_REASON_PARING_NOT_SUPPORTED			PAIRING_FAIL_REASON_PAIRING_NOT_SUPPORTED
#define PARING_FAIL_REASON_ENCRYPT_KEY_SIZE				PAIRING_FAIL_REASON_ENCRYPT_KEY_SIZE
#define PARING_FAIL_REASON_CMD_NOT_SUPPORT				PAIRING_FAIL_REASON_CMD_NOT_SUPPORT
#define PARING_FAIL_REASON_UNSPECIFIED_REASON			PAIRING_FAIL_REASON_UNSPECIFIED_REASON
#define PARING_FAIL_REASON_REPEATED_ATTEMPT				PAIRING_FAIL_REASON_REPEATED_ATTEMPT
#define PARING_FAIL_REASON_INVAILD_PARAMETER			PAIRING_FAIL_REASON_INVAILD_PARAMETER
#define PARING_FAIL_REASON_DHKEY_CHECK_FAIL				PAIRING_FAIL_REASON_DHKEY_CHECK_FAIL
#define PARING_FAIL_REASON_NUMUERIC_FAILED				PAIRING_FAIL_REASON_NUMUERIC_FAILED
#define PARING_FAIL_REASON_BREDR_PARING					PAIRING_FAIL_REASON_BREDR_PAIRING
#define PARING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW		PAIRING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW
#define PARING_FAIL_REASON_PARING_TIEMOUT				PAIRING_FAIL_REASON_PAIRING_TIEMOUT
#define PARING_FAIL_REASON_CONN_DISCONNECT				PAIRING_FAIL_REASON_CONN_DISCONNECT
#define PARING_FAIL_REASON_SUPPORT_NC_ONLY				PAIRING_FAIL_REASON_SUPPORT_NC_ONLY

#define DATA_FRAGM_ALLOWED			DATA_FRAGMENT_ALLOWED

/* for some data structure name in old version SDK compatible with new version SDK */
#define gap_smp_paringSuccessEvt_t						gap_smp_pairingSuccessEvt_t


#define adv_para_t                       hci_le_setAdvParam_cmdParam_t
#define data_fragm_t                    data_fragment_t

#define event_disconnection_t         hci_disconnectionCompleteEvt_t

typedef struct {
	u8	subcode;
	u8	status;
	u16	handle;
	u8	role;
	u8	peer_adr_type;
	u8	mac[6];
	u16	interval;
	u16	latency;
	u16	timeout;
	u8	accuracy;
} event_connection_complete_t;			//20-byte

typedef struct {
	u8	subcode;
	u8	status;
	u16	handle;
	u16	interval;
	u16	latency;
	u16	timeout;
} event_connection_update_t;			//20-byte

extern void att_req_read (u8 *p, u16 attHandle);
extern void att_req_read_blob (u8 *p, u16 attHandle, u16 offset);
extern void att_req_read_multi (u8 *p, u16* h, u8 n);
extern void att_req_write (u8 *p, u16 attHandle, u8 *buf, int len);
extern void att_req_signed_write_cmd (u8 *p, u16 attHandle, u8 *pd, int n, u8 *sign);
extern void att_req_prep_write (u8 *p, u16 attHandle, u8 *pd, u16 offset, int len);
extern void att_req_write_cmd (u8 *p, u16 attHandle, u8 *buf, int len);
extern void att_req_read_by_type (u8 *p, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);
extern void att_req_read_by_group_type (u8 *p, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);
extern void att_req_find_info(u8 *p, u16 start_attHandle, u16 end_attHandle);
extern void att_req_find_by_type (u8 *p, u16 start_attHandle, u16 end_attHandle, u8 *uuid, u8* attr_value, int len);

extern ble_sts_t  	bls_att_pushNotifyData (u16 attHandle, u8 *p, int len);
extern ble_sts_t	bls_att_pushIndicateData (u16 attHandle, u8 *p, int len);


#endif /* BLE_COMP_H_ */
