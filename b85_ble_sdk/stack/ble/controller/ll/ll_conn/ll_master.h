/********************************************************************************************************
 * @file	ll_master.h
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
#ifndef LL_MASTER_H_
#define LL_MASTER_H_



/******************************* User Interface  ************************************/
#ifndef	BLM_CONN_ENC_REFRESH
#define BLM_CONN_ENC_REFRESH	BIT(10)
#endif


/**
 * @brief      for user to initialize ACL connection master role.
 * @param	   none
 * @return     none
 */
void		blc_ll_initMasterRoleSingleConn_module(void);


/**
 * @brief      for user to check if RF Machine is busy.
 * @param	   none
 * @return     status. 1:  busy
 * 					   other: not busy
 */
bool		blm_ll_isRfStateMachineBusy(void);


/**
 * @brief      for user to start disconnection process in master role .
 * @param[in]  handle - connect handle
 * @param[in]  reason - terminate reason
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blm_ll_disconnect (u16 handle, u8 reason);


/**
 * @brief      for user to start update prarameter process in master role .
 * @param[in]  connHandle - connect handle
 * @param[in]  conn_min - minimum connection interval
 * @param[in]  conn_max - maximum connection interval
 * @param[in]  conn_latency - connection latency
 * @param[in]  timeout - connection timeout
 * @param[in]  ce_min - not supported
 * @param[in]  ce_max - not supported
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blm_ll_updateConnection (u16 connHandle,
							  u16 conn_min, u16 conn_max, u16 conn_latency, u16 timeout,
							  u16 ce_min, u16 ce_max );


/**
 * @brief      for user to start update channel map process in master role .
 * @param[in]  handle - connect handle
 * @param[in]  map - pointer of channel map
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blm_ll_setHostChannel (u16 handle, u8 * map);


/**
 * @brief      l2cap layer processing function in master role.
 * @param[in]  conn - connect handle
 * @param[in]  raw_pkt - pointer of packet
 * @return     address of l2cap layer packet
 */
rf_packet_l2cap_t *		blm_l2cap_packet_pack (u16 conn, u8 * raw_pkt);


/**
 * @brief      for user to read remote feature in master role .
 * @param[in]  handle - connect handle
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blm_ll_readRemoteFeature (u16 handle);



#endif /* LL_MASTER_H_ */
