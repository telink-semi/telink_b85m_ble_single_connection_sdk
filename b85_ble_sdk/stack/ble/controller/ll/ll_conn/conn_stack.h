/********************************************************************************************************
 * @file	conn_stack.h
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
#ifndef STACK_BLE_LL_LL_CONN_CONN_STACK_H_
#define STACK_BLE_LL_LL_CONN_CONN_STACK_H_

#include "stack/ble/ble_stack.h"
#include "stack/ble/ble_format.h"
#include "stack/ble/controller/phy/phy_stack.h"
#include "algorithm/aes_ccm/aes_ccm.h"


/******************************* ll_conn start ************************************************************************/




#define 		DATA_LENGTH_REQ_PENDING							1
#define			DATA_LENGTH_REQ_DONE							2
#define			BLE_STACK_USED_TX_FIFIO_NUM						2
/******************************* ll_conn end **************************************************************************/



















/******************************* ll_slave start ************************************************************************/


#define					SLAVE_LL_ENC_OFF						0
#define					SLAVE_LL_ENC_REQ						1
#define					SLAVE_LL_ENC_RSP_T						2
#define					SLAVE_LL_ENC_START_REQ_T				3
#define					SLAVE_LL_ENC_START_RSP					4
#define					SLAVE_LL_ENC_START_RSP_T				5
#define					SLAVE_LL_ENC_PAUSE_REQ					6
#define					SLAVE_LL_ENC_PAUSE_RSP_T				7
#define					SLAVE_LL_ENC_PAUSE_RSP					8
#define					SLAVE_LL_REJECT_IND_T					9


#define 				TRY_FIX_ERR_BY_ADD_BRX_WAIT				1



typedef struct {
	u8		time_update_st;
	u8 		last_rf_len;
	u8		remoteFeatureReq;
	u8 		long_suspend;

	u8		interval_level;
	u8		ll_recentAvgRSSI;
	u8		conn_sn_master;
	u8		master_not_ack_slaveAckUpReq;

	u8		conn_rcvd_ack_pkt;
	u8		conn_new_param;
	u8		conn_winsize_next;
	u8		rsvd1;

#if (LL_FEATURE_ENABLE_LL_PRIVACY)
	u8	    conn_peer_addr_type; //host event use addr type: 0,1,2,3
	u8      rsvd2;
	u8		conn_peer_addr[6];   //host event use addr
#endif

	u8 		conn_master_terminate;
	u8		conn_terminate_reason;
	u8 		conn_slave_terminate;
	u8		conn_terminate_pending;   // terminate_pending = master_terminate || slave_terminate


	u16		conn_update_inst_diff;
	u16		connHandle;
	u16     conn_offset_next;
	u16		conn_interval_next; //standard value,  not * 1.25ms
	u16		conn_latency_next;
	u16		conn_timeout_next;  //standard value,  not *10ms


	u32		conn_access_code_revert;
	u32		connExpectTime;
	int		conn_interval_adjust;
	u32		conn_timeout;
	u32		conn_interval;
	u32		conn_latency;
	u32		conn_duration;

	u32 	ll_remoteFeature; 	 //feature mask <0:31>,  not only one for BLE master, use connHandle to identify
	//u32   ll_remoteFeature_2;  //feature mask <32:63> for future use

	u32		conn_start_tick;

	int		conn_tolerance_time;

	u32		tick_1st_rx;
	u32		conn_brx_tick;



	u32 	conn_slaveTerminate_time;


	u32		conn_pkt_rcvd;
	u32		conn_pkt_rcvd_no;
	u8 *	conn_pkt_dec_pending;
	int		conn_enc_dec_busy;
	int		conn_stop_brx;

	u32		conn_fsm_timeout;   // unit:  uS

#if	(BQB_5P0_TEST_ENABLE)
	u32		conn_establish_pending_timeout;;
	u32		conn_establish_pending_flag;
#endif


} st_ll_conn_slave_t;




extern _attribute_aligned_(4) st_ll_conn_slave_t		bltc;
#define 	IS_LL_CONNECTION_VALID(handle)  ( bltc.connHandle == (handle) && bltc.connHandle != BLE_INVALID_CONNECTION_HANDLE )





bool		bls_ll_pushTxFifo (int handle, u8 *p);
ble_sts_t  	bls_hci_reset(void);

ble_sts_t	bls_hci_receiveHostACLData(u16 connHandle, u8 PB_Flag, u8 BC_Flag, u8 *pData );
ble_sts_t 	bls_hci_le_getRemoteSupportedFeatures(u16 connHandle);

void		blt_push_fifo_hold (u8 *p);
void 		blt_terminate_proc(void);



extern ble_crypt_para_t 	blc_cyrpt_para;
static inline int  bls_ll_isConnectionEncrypted(u16 connHandle)
{
	return blc_cyrpt_para.enable;
}

#if (TRY_FIX_ERR_BY_ADD_BRX_WAIT)
	extern int CONN_SHORT_TOLERANCE_TIME;
	static inline void blc_pm_modefy_brx_early_set(int us)
	{
		CONN_SHORT_TOLERANCE_TIME = us * SYSTEM_TIMER_TICK_1US;
	}

	static inline int blc_pm_get_brx_early_time(void)
	{
		return CONN_SHORT_TOLERANCE_TIME;
	}
#endif





/******************************* ll_slave end **************************************************************************/












/******************************* ll_master start ***********************************************************************/



/******************************* ll_master end *************************************************************************/














/******************************* ll_conn_phy start *********************************************************************/



/******************************* ll_conn_phy end ***********************************************************************/






/******************************* ll_conn_csa start *********************************************************************/

//See the Core_v5.0(Vol 6/Part B/4.5.8, "Data Channel Index Selection") for more information.
typedef enum {
	CHANNAL_SELECTION_ALGORITHM_1      	=	0x00,
	CHANNAL_SELECTION_ALGORITHM_2      	=	0x01,
} channel_algorithm_t;




typedef u8 (*ll_chn_index_calc_callback_t)(u8*, u16, u16);
extern ll_chn_index_calc_callback_t	ll_chn_index_calc_cb;



u8	blc_calc_remapping_table(u8 chm[5]);

u8	blc_ll_channel_index_calc_csa2(u8 chm[5], u16 event_cntr, u16 channel_id);


/******************************* ll_conn_csa end ***********************************************************************/





#endif /* STACK_BLE_LL_LL_CONN_CONN_STACK_H_ */
