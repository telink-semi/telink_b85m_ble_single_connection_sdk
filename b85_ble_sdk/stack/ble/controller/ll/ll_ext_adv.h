/********************************************************************************************************
 * @file	ll_ext_adv.h
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
#ifndef LL_ADV_EXT_H_
#define LL_ADV_EXT_H_


#include <stack/ble/ble_common.h>

#include "stack/ble/hci/hci_cmd.h"

#include "tl_common.h"
#include "stack/ble/controller/phy/phy.h"

#include "ll_stack.h"


#define	INVALID_ADVHD_FLAG										0xFF


#define AUX_ADV_FEA			BIT(0)
#define PER_ADV_FEA			BIT(1)





/* Advertising_Handle */
#define ADV_SID_0												0x00
#define ADV_SID_1												0x01
#define ADV_SID_2												0x02
#define ADV_SID_3												0x03



typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;
	u8	advA[6];			//address
	u8	data[31];			//0-31 byte
}rf_pkt_pri_adv_t;
#define MAX_LENGTH_PRIMARY_ADV_PKT				44   //sizeof(rf_pkt_pri_adv_t) = 43






typedef struct{
	u8 chn_ind		:6;
	u8 ca			:1;
	u8 off_unit		:1;
	u16 aux_off		:13;
	u16 aux_phy		:3;
} aux_ptr_t;







//NOTE: this data structure must 4 bytes aligned
typedef struct
{
    u8		adv_handle;
    u8 		extAdv_en;
    u8 		adv_chn_mask;
    u8		adv_chn_num;

	u8 		own_addr_type;
	u8 		peer_addr_type;
    u8 		pri_phy;
    u8 		sec_phy;


    u8 		max_ext_adv_evt;
    u8 		run_ext_adv_evt;
    u8		unfinish_advData;
    u8		unfinish_scanRsp;


	u8		adv_filterPolicy;
    u8 		scan_req_noti_en;
    u8 		coding_ind;					//s2 or s8
    u8		param_update_flag;


	u8		with_aux_adv_ind;   //ADV_EXT_IND  with AUX_ADV_IND
	u8		with_aux_chain_ind;
	u8 		rand_adr_flg;
    u8 		adv_sid;


	u16     adv_did; 	// BIT<11:0>
	u16 	evt_props;
	u16		advInt_use;
	u16		send_dataLen;
    u16 	maxLen_advData;			//for each ADV sets, this value can be different to save SRAM
    u16 	curLen_advData;
    u16 	maxLen_scanRsp;			//for each ADV sets, this value can be different to save SRAM
    u16 	curLen_scanRsp;

    u16		send_dataLenBackup;
    u16		rsvd_16_1;


	u32 	adv_duration_tick;
	u32 	adv_begin_tick;				//24
    u32		adv_event_tick;

	u8*		dat_extAdv;
	u8*		dat_scanRsp;                //Scan response data.
	rf_pkt_pri_adv_t*		primary_adv;
	rf_pkt_ext_adv_t*		secondary_adv;

	u8 		rand_adr[6];
	u8 		peer_addr[6];
}ll_ext_adv_t;


#define ADV_SET_PARAM_LENGTH				(sizeof(ll_ext_adv_t))   //sizeof(ll_ext_adv_t) =  ,  must 4 byte aligned










typedef struct
{
    u8 		maxNum_advSets;
    u8		useNum_advSets;
    u8		last_advSet;
    u8		last_advHand;


    u8		T_SCAN_RSP_INTVL;
    u8 		custom_aux_chn;
    u8 		T_AUX_RSP_INTVL; //settle aux_scan_rsp/aux_conn_rsp's IFS 150s
    u8 		rsvd3;

    u32		rand_delay;

}ll_adv_mng_t;












/******************************************** User Interface  ********************************************************************/
//initialization
void 		blc_ll_initExtendedAdvertising_module(	u8 *pAdvCtrl, u8 *pPriAdv,int num_sets);

void 		blc_ll_initExtSecondaryAdvPacketBuffer(u8 *pSecAdv, int sec_adv_buf_len);

void 		blc_ll_initExtAdvDataBuffer(u8 *pExtAdvData, int max_len_advData);								//set AdvData buffer for all adv_set
void 		blc_ll_initExtAdvDataBuffer_by_advHandle(u8 *pExtAdvData, u8 advHandle, int max_len_advData);  //set AdvData buffer for specific adv_set

void 		blc_ll_initExtScanRspDataBuffer(u8 *pScanRspData, int max_len_scanRspData);
void 		blc_ll_initExtScanRspDataBuffer_by_advHandle(u8 *pScanRspData,  u8 advHandle, int max_len_scanRspData);



//Set Extended ADV parameters
ble_sts_t	blc_ll_setAdvRandomAddr(u8 advHandle, u8* rand_addr);


ble_sts_t 	blc_ll_setExtAdvParam(  adv_handle_t advHandle, 		advEvtProp_type_t adv_evt_prop, u32 pri_advIntervalMin, 		u32 pri_advIntervalMax,
									u8 pri_advChnMap,	 			own_addr_type_t ownAddrType, 	u8 peerAddrType, 			u8  *peerAddr,
									adv_fp_type_t advFilterPolicy,  tx_power_t adv_tx_pow,			le_phy_type_t pri_adv_phy, 	u8 sec_adv_max_skip,
									le_phy_type_t sec_adv_phy, 	 	u8 adv_sid, 					u8 scan_req_noti_en);
ble_sts_t	blc_ll_setExtAdvData	(u8 advHandle, data_oper_t operation, data_fragm_t fragment_prefer, u8 adv_dataLen, 	u8 *advdata);
ble_sts_t 	blc_ll_setExtScanRspData(u8 advHandle, data_oper_t operation, data_fragm_t fragment_prefer, u8 scanRsp_dataLen, u8 *scanRspData);
ble_sts_t 	blc_ll_setExtAdvEnable_1(u32 extAdv_en, u8 sets_num, u8 advHandle, 	 u16 duration, 	  u8 max_extAdvEvt);
ble_sts_t	blc_ll_setExtAdvEnable_n(u32 extAdv_en, u8 sets_num, u8 *pData);


ble_sts_t	blc_ll_removeAdvSet(u8 advHandle);
ble_sts_t	blc_ll_clearAdvSets(void);


// if Coded PHY is used, this API set default S2/S8 mode for Extended ADV
ble_sts_t	blc_ll_setDefaultExtAdvCodingIndication(u8 advHandle, le_ci_prefer_t prefer_CI);


void        blc_ll_setAuxAdvChnIdxByCustomers(u8 aux_chn);
void		blc_ll_setMaxAdvDelay_for_AdvEvent(u8 max_delay_ms);    //unit: mS, only 8/4/2/1/0  available


/****************************************** Stack Interface, user can not use!!! *************************************************/
ble_sts_t 	blc_hci_le_setExtAdvParam( hci_le_setExtAdvParam_cmdParam_t *para, u8 *pTxPower);
ble_sts_t 	blc_hci_le_setExtAdvEnable(u8 extAdv_en, u8 sets_num, u8 *pData);

u16 	  	blc_ll_readMaxAdvDataLength(void);
u8		  	blc_ll_readNumberOfSupportedAdvSets(void);


int  		blt_ext_adv_proc(void);
int  		blt_send_adv2(void);
int 		blt_send_legacy_adv(void);
int 		blt_send_extend_adv(void);
void 		blt_send_extend_no_aux_adv(void);
int 		blt_send_aux_adv(void);
ble_sts_t	blt_ll_clearAdvSets(void);
void 		blt_clearAdvSetsParam(ll_ext_adv_t		*pEadv);
u8			blt_ll_searchExistingAdvSet(u8 advHandle);
u8 			blt_ll_searchAvailableAdvSet(u8 advHandle);
void		blt_ll_updateAdvState(void);
ble_sts_t   blt_ll_enableExtAdv(int adv_en);
void 		blt_ll_procAuxConnectReq(u8 * prx);
int  		blt_ll_updateAdvPacket(void);
void		blt_ll_reset_ext_adv(void);

#endif /* LL_ADV_EXT_H_ */
