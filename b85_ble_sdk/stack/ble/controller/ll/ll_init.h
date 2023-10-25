/********************************************************************************************************
 * @file	ll_init.h
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
#ifndef LL_INIT_H_
#define LL_INIT_H_



/**
 * @brief      for user to initialize legacy initiating module
 * 			   notice that only one module can be selected between legacy initiating module and extended initiating module
 * @param	   none
 * @return     none
 */
void	 	blc_ll_initInitiating_module(void);



/**
 * @brief      This function is used to create an ACL connection to a connectable advertiser.
 * @param[in]  scan_interval - recommendations from the Host on how frequently (LE_Scan_Interval) the Controller should scan.
 * @param[in]  scan_window - recommendations from the Host on how long (LE_Scan_Window) the Controller should scan.
 * @param[in]  filter_policy - used to determine whether the White List is used.
 * @param[in]  adr_type - indicates the type of address used in the connectable advertisement sent by the peer.
 * @param[in]  *mac - indicates the Peer's Public Device Address.
 * @param[in]  own_adr_type - indicates the type of address being used in the connection request packets.
 * @param[in]  conn_min - the minimum allowed connection interval.
 * @param[in]  conn_max - the maximum allowed connection interval.
 * @param[in]  conn_latency - he maximum allowed connection latency
 * @param[in]  timeout - defines the link supervision timeout for the connection.
 * @param[in]  ce_min - informative parameters providing the Controller with the expected minimum length of the connection events.
 * @param[in]  ce_max - informative parameters providing the Controller with the expected maximum length of the connection events.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_ll_createConnection( scan_inter_t scanInter, scan_wind_t scanWindow, init_fp_t initiator_fp, u8 peerAdrType,    u8 *peerAddr, own_addr_type_t ownAdrType,
									 conn_inter_t conn_min,  conn_inter_t conn_max,  u16 conn_latency,       conn_tm_t timeout, u16 ce_min,   u16 ce_max );


/**
 * @brief      this function is used to to cancel create connection process .
 * @param	   none
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_ll_createConnectionCancel (void);


/**
 * @brief      this function is used to set create connection process timeout.
 * @param	   timeout_ms - timeout threshold
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t   blc_ll_setCreateConnectionTimeout (u32 timeout_ms);



#endif /* LL_INIT_H_ */
