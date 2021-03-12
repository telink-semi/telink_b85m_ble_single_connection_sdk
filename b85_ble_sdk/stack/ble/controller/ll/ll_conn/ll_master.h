/********************************************************************************************************
 * @file	ll_master.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	06,2020
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef LL_MASTER_H_
#define LL_MASTER_H_



/******************************* User Interface  ************************************/
void		blc_ll_initMasterRoleSingleConn_module(void);
bool		blm_ll_isRfStateMachineBusy(void);

ble_sts_t	blm_ll_disconnect (u16 handle, u8 reason);
ble_sts_t	blm_ll_updateConnection (u16 connHandle,
							  u16 conn_min, u16 conn_max, u16 conn_latency, u16 timeout,
							  u16 ce_min, u16 ce_max );

ble_sts_t	blm_ll_setHostChannel (u16 handle, u8 * map);


rf_packet_l2cap_t *		blm_l2cap_packet_pack (u16 conn, u8 * raw_pkt);
#if 0
st_ll_conn_master_t *	blm_ll_getConnection (u16 h);
#endif

#endif /* LL_MASTER_H_ */
