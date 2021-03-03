/********************************************************************************************************
 * @file	ll_stack.h
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
#ifndef STACK_BLE_LL_LL_STACK_H_
#define STACK_BLE_LL_LL_STACK_H_


#include "stack/ble/ble_format.h"
#include "stack/ble/hci/hci_cmd.h"



/******************************* ll start *************************************************************************/



typedef struct {
	u8		save_flg;
	u8		sn_nesn;
	u8		blt_dma_tx_rptr;

	// to solve the problem: ble flow control is managed by software replace of hardware,
	//						software can resolve HW CRC error,HW kick away txFIFo data mistakenly.
	//this variable must clear to 0, when IC wake up from deep(deep ,deepretention),because of IC wake up from deep(deep ,deepretention),
	//both hardware reg_dma_tx_rptr and reg_dma_tx_wptr will become 0, moreover IC can't enter deep until data in tx fifo have send over.
	u8 		blt_dma_tx_wptr;

#if (DEEP_RET_ENTRY_CONDITIONS_OPTIMIZATION_EN)
	u8		dma_tx_rcvry_dat_depth;
	u8 		dma_tx_data_depth;

	u16		dma_tx_ptr[8];//dma tx buffer ptr rcd
#endif

}bb_sts_t;

extern _attribute_aligned_(4) bb_sts_t	blt_bb;
static inline void	blt_save_snnesn()
{
#if (MCU_CORE_TYPE == MCU_CORE_9518)
								// SN									NESN
	blt_bb.sn_nesn = ((REG_ADDR8(0x80140a22) & BIT(0)) << 4) | ((REG_ADDR8(0x80140a23) & BIT(4)) << 1);
#elif (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
								// SN									NESN
	blt_bb.sn_nesn = ((REG_ADDR8(0xf22) & BIT(0)) << 4) | ((REG_ADDR8(0xf23) & BIT(4)) << 1);
#endif
}

static inline void	blt_restore_snnesn()
{
	reg_rf_ll_ctrl_1 &= ~(FLD_RF_BRX_SN_INIT | FLD_RF_BRX_NESN_INIT);
	reg_rf_ll_ctrl_1 |= blt_bb.sn_nesn;
}

/////////////////////// DMA Tx fifo rptr /////////////////////////////////////////////
static inline void	blt_save_dma_tx_rptr()
{
#if (MCU_CORE_TYPE == MCU_CORE_9518)
	//TX Fifo: 0x100501[0:4] means rptr, 0 ~ 31
	blt_bb.blt_dma_tx_rptr = reg_dma_tx_rptr & FLD_DMA_RPTR_MASK;  //
#elif (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	//TX Fifo: 0xc2a[0:3] means rptr
	blt_bb.blt_dma_tx_rptr = reg_dma_tx_rptr & 0x0f;
#endif
}

static inline void	blt_restore_dma_tx_rptr()
{
#if (MCU_CORE_TYPE == MCU_CORE_9518)
	//pay attention: FLD_DMA_RPTR_SET
	reg_dma_tx_rptr = (FLD_DMA_RPTR_SET | blt_bb.blt_dma_tx_rptr);//restore tx_rptr
#elif (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	//0xc2a[6] rptr set
	reg_dma_tx_rptr = (BIT(6) | blt_bb.blt_dma_tx_rptr);//restore tx_rptr
#endif
}
/******************************* ll end *************************************************************************/



















/******************************* ll_adv start ***********************************************************************/



/******************************* ll_adv end *************************************************************************/














/******************************* ll_scan start ***********************************************************************/


/******************************* ll_scan end *************************************************************************/





/******************************* ll_init start ***********************************************************************/



/******************************* ll_init end *************************************************************************/






/******************************* ll_pm start ***********************************************************************/


/******************************* ll_pm end *************************************************************************/





/******************************* ll_whitelist start ***********************************************************************/


/******************************* ll_whitelist end *************************************************************************/














/******************************* ll_ext_adv start ***********************************************************************/






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
}rf_pkt_pri_scanrsp_t;




typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	u8	data[253];   //Extended Header + AdvData
}rf_pkt_ext_adv_t;
#define MAX_LENGTH_SECOND_ADV_PKT				264   //sizeof(rf_pkt_ext_adv_t) = 261




/******************************************************  ADV_EXT_IND ************************************************************
												AdvA  TargetA  ADI	  Aux	Sync	Tx   ACAD	AdvData			Structure
																  	  Ptr  	Info   Power
Non-Connectable
Non-Scannable   Undirected without AUX		  	 M		 X		X	   X	 X		X	   X	 X			rf_pkt_adv_ext_ind_1

Non-Connectable
Non-Scannable   Undirected with    AUX			 X 	 	 X		M	   M	 X		X	   X	 X			rf_pkt_adv_ext_ind_2

Non-Connectable
Non-Scannable	Directed   without AUX			 M		 M		X	   X	 X		X	   X	 X			rf_pkt_adv_ext_ind_1

Non-Connectable
Non-Scannable	Directed   with	   AUX			 X	 	 X		M	   M	 X		X	   X	 X			rf_pkt_adv_ext_ind_2


Connectable 	Undirected					 	 X		 X		M	   M	 X		X	   X	 X			rf_pkt_adv_ext_ind_2

Connectable 	Directed					 	 X		 X		M	   M	 X		X	   X	 X			rf_pkt_adv_ext_ind_2

Scannable 		Undirected				     	 X		 X		M	   M	 X		X	   X	 X			rf_pkt_adv_ext_ind_2

Scannable 		Directed					 	 X		 X		M	   M	 X		X	   X	 X			rf_pkt_adv_ext_ind_2
*********************************************************************************************************************************/
typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	u8	advA[6];
	u8	targetA[6];
}rf_pkt_adv_ext_ind_1;


typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	//ADI
	u16	did	:12;
	u16	sid : 4;

	//AuxPrt
	u8  chn_index    :6;
	u8  ca 		 	:1;
	u8  offset_unit	:1;
	u16 aux_offset  :13;
	u16	aux_phy		:3;

}rf_pkt_adv_ext_ind_2;




/******************************************************  AUX_ADV_IND ************************************************************
												AdvA  TargetA  ADI	  Aux	Sync	Tx   ACAD	AdvData
																  	  Ptr  	Info   Power
Non-Connectable
Non-Scannable   Undirected with    AUX			 M 	 	 X		M	   O	 X		X	   X	 O			rf_pkt_aux_adv_ind_1

Non-Connectable
Non-Scannable	Directed   with	   AUX			 M	 	 M		M	   O	 X		X	   X	 O			rf_pkt_aux_adv_ind_2

Connectable 	Undirected					 	 M		 X		M	   X	 X		X	   X	 O			rf_pkt_aux_adv_ind_1

Connectable 	Directed					 	 M		 M		M	   X	 X		X	   X	 O			rf_pkt_aux_adv_ind_2

Scannable 		Undirected				     	 M		 X		M	   X	 X		X	   X	 X			rf_pkt_aux_adv_ind_1

Scannable 		Directed					 	 M		 M		M	   X	 X		X	   X	 X			rf_pkt_aux_adv_ind_2
*********************************************************************************************************************************/
typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	u8	advA[6];

	//ADI
	u16	did	:12;
	u16	sid : 4;

	u8	dat[1];   // AuxPtr/AdvData
}rf_pkt_aux_adv_ind_1;


typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	u8	advA[6];
	u8	targetA[6];

	//ADI
	u16	did	:12;
	u16	sid : 4;

	u8	dat[1]; // AuxPtr/AdvData
}rf_pkt_aux_adv_ind_2;





typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	u8	advA[6];

	//ADI
	u16	did	:12;
	u16	sid : 4;

	//AuxPrt
	u8  chn_index    :6;
	u8  ca 		 	:1;
	u8  offset_unit	:1;
	u16 aux_offset  :13;
	u16	aux_phy		:3;

	u8	dat[1];   // AdvData
}rf_pkt_aux_adv_ind_3;




typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	u8	advA[6];
	u8	targetA[6];

	//ADI
	u16	did	:12;
	u16	sid : 4;

	//AuxPrt
	u8  chn_index    :6;
	u8  ca 		 	:1;
	u8  offset_unit	:1;
	u16 aux_offset  :13;
	u16	aux_phy		:3;

	u8	dat[1]; // AdvData
}rf_pkt_aux_adv_ind_4;



/******************************************************  AUX_SCAN_RSP ***********************************************************

												AdvA  TargetA  ADI	  Aux	Sync	Tx   ACAD	AdvData
																  	  Ptr  	Info   Power

								  	  	  	  	  M		 X		X	   O	 X		X	   X	 M
*********************************************************************************************************************************/
typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	u8	advA[6];
	u8	dat[1];   // AuxPtr/AdvData
}rf_pkt_aux_scan_rsp_t;

#define MAX_ADVDATA_NUM_AUX_SCANRSP									247   // 255 - 8( Extended Header Length(1) + Extended Header Flag(1) + AdvA(6) ) = 247




/******************************************************  AUX_CHAIN_IND ***********************************************************

												AdvA  TargetA  ADI	  Aux	Sync	Tx   ACAD	AdvData
																  	  Ptr  	Info   Power

		Chained data:							  X		 X		C3	   O	 X		O	   X	 O

														    			||
																		||
														   	   	   	   \||/
																		\/

Chained data after AUX_ADV_IND  :				 X		 X		M	   O	 X		X	   X	 O			rf_pkt_aux_chain_ind_1

Chained data after AUX_SCAN_RSP :				 X		 X		X	   O	 X		X	   X	 O			rf_pkt_aux_chain_ind_2
*********************************************************************************************************************************/
typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;


	//ADI
	u16	did	:12;
	u16	sid : 4;

	u8	dat[1]; // AuxPrt/AdvData
}rf_pkt_aux_chain_ind_1;

#define MAX_ADVDATA_NUM_AUX_CHAIN_IND_1									251   // 255 - 4( Extended Header Length(1) + Extended Header Flag(1) + ADI(2) ) = 251








/******************************************************  AUX_CONNECT_RSP *********************************************************

												AdvA  TargetA  ADI	  Aux	Sync	Tx   ACAD	AdvData
																  	  Ptr  	Info   Power

								  	  	  	  	  M		 M		X	   X	 X		X	   X	 X
*********************************************************************************************************************************/
typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	ext_hdr_len		:6;
	u8	adv_mode		:2;

	u8	ext_hdr_flg;

	u8	advA[6];
	u8	targetA[6];
}rf_pkt_aux_conn_rsp_t;




/****************************************************  SCAN_REQ / AUX_SCAN_REQ **************************************************

*********************************************************************************************************************************/
typedef struct{
	u32 dma_len;

	u8 type   :4;
	u8 rfu1   :1;
	u8 chan_sel:1;
	u8 txAddr :1;
	u8 rxAddr :1;

	u8  rf_len;

	u8	scanA[6];
	u8	advA[6];
}rf_pkt_ext_scan_req_t;






/********************* Macro & Enumeration variables for Stack, user can not use!!!!  **********************/



#define	EXT_ADV_PDU_AUXPTR_OFFSET_UNITS_30_US					0
#define	EXT_ADV_PDU_AUXPTR_OFFSET_UNITS_300_US					1

#define	EXT_ADV_PDU_AUXPTR_CA_51_500_PPM						0
#define	EXT_ADV_PDU_AUXPTR_CA_0_50_PPM							1




#define TLK_T_MAFS													302  // T_MAFS >= 300 uS, add some margin
#define TLK_T_MAFS_30US_NUM											10

#define	TX_TX_DELAY_US												120 // TX settle(74 uS) + 5  preamble(40uS) + some other baseband cost,   and be 30*N













/***********************************************************************************************************************/





#endif /* STACK_BLE_LL_LL_STACK_H_ */
