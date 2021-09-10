/********************************************************************************************************
 * @file	ll_resolvlist.h
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
#ifndef LL_RESOLVLIST_H_
#define LL_RESOLVLIST_H_


#include <stack/ble/ble_common.h>



/******************************* Macro & Enumeration & Structure Definition for Stack Begin, user can not use!!!!  *****/






/******************************* Macro & Enumeration & Structure Definition for Stack End ******************************/






/******************************* Macro & Enumeration variables for User Begin ******************************************/


/******************************* Macro & Enumeration variables for User End ********************************************/







/******************************* User Interface  Begin *****************************************************************/

/**
 * @brief		for user to add device to resolving list
 * @param[i]	peerIdAddrType - peer device address type
 * @param[i]	peerIdAddr - peer device address
 * @param[i]	peer_irk - peer device IRK
 * @param[i]	local_irk - peer device local IRK
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_add(u8 peerIdAddrType, u8 *peerIdAddr, u8 *peer_irk, u8 *local_irk);

/**
 * @brief		for user to delete device to resolving list
 * @param[i]	peerIdAddrType - peer device address type
 * @param[i]	peerIdAddr - peer device address
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_delete(u8 peerIdAddrType, u8 *peerIdAddr);

/**
 * @brief		for user to reset resolving list
 * @param[i]	none
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_reset(void);

/**
 * @brief		for user to get peer resolving list number
 * @param[i]	Size - get resolve list
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_getSize(u8 *Size);

/**
 * @brief		for user to get peer device address .
 * @param[i]	peerIdAddrType - peer device address type
 * @param[i]	peerIdAddr - peer device address
 * @param[i]	peerResolvableAddr - peer device address
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_getPeerResolvableAddr (u8 peerIdAddrType, u8* peerIdAddr, u8* peerResolvableAddr); //not available now

/**
 * @brief		for user to get local device Resolvable address .
 * @param[i]	peerIdAddrType - peer device address type
 * @param[i]	peerIdAddr - peer device address
 * @param[i]	LocalResolvableAddr - local device address
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_getLocalResolvableAddr(u8 peerIdAddrType, u8* peerIdAddr, u8* LocalResolvableAddr); //not available now

/**
 * @brief		for user to set Resolution en
 * @param[i]	resolutionEn - enable 1 stand for enable.
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_setAddrResolutionEnable (u8 resolutionEn);

/**
 * @brief		for user to set Resolution en
 * @param[i]	timeout_s - enable 1 stand for enable.
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_setResolvablePrivateAddrTimer (u16 timeout_s);   //not available now

/**
 * @brief		for user to set privacy mode .
 * @param[i]	peerIdAddrType - peer device address type
 * @param[i]	peerIdAddr - peer device address
 * @param[i]	privMode - mode
 * @return  	statue : BLE_SUCCESS OK else : fail.
 */
ble_sts_t  		ll_resolvingList_setPrivcyMode(u8 peerIdAddrType, u8* peerIdAddr, u8 privMode);

/**
 * @brief		for user to set privacy mode .
 * @param		none.
 * @return  	time : timeout value.
 */
u16				blc_ll_resolvGetRpaTmo(void);

/**
 * @brief		for user to resolve a address by rpa .
 * @param[in]	rpa - rpa address.
 * @return  	i : index in resolve table.
 */
int				blc_ll_resolvPeerRpaResolvedAny(const u8* rpa);

/**
 * @brief		for user to set rpa by index .
 * @param[in]	idx - index in table.
 * @param[out]	rpa - rpa buffer address.
 * @return  	none.
 */
void			blc_ll_resolvSetPeerRpaByIdx(u8 idx, u8 *rpa);

/**
 * @brief		for user to set local rpa by index .
 * @param[in]	idx - index in table.
 * @param[out]	rpa - rpa buffer address.
 * @return  	none.
 */
void			blc_ll_resolvSetLocalRpaByIdx(u8 idx, u8 *rpa);

/**
 * @brief		for user to set privacy mode .
 * @param[i]	peerIdAddrType - peer device address type
 * @param[i]	peerIdAddr - peer device address
 * @param[i]	rpa - rpa store buffer
 * @param[i]	local - 0:peer RPA  1:local RPA
 * @return  	1 : get in table 0: not in table
 */
bool			blc_ll_resolvGetRpaByAddr(u8* peerIdAddr, u8 peerIdAddrType, u8* rpa, u8 local);

/**
 * @brief 	Resolvable a Private Address
 * @param[in]	irk  - The IRKs are stored in little endian format
 * @param[in]	rpa  - The addr are stored in little endian format
 * @return      1: Address resolution succeeded; 0: Address resolution failed
 * */
bool			blc_ll_resolvIsAddrResolved(const u8* irk, const u8* rpa);

/**
 * @brief 		Is resolve address from resolve list enable.
 * @param		none
 * @return  	1: enable; 0: not enable
 * */
bool			blc_ll_resolvIsAddrRlEnabled(void);

/**
 * @brief 		init resolve list.
 * @param		none
 * @return  	none
 * */
void			blc_ll_resolvListInit(void);
/******************************* User Interface  End  ******************************************************************/





#endif /* LL_RESOLVLIST_H_ */
