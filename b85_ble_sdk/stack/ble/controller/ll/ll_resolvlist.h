/********************************************************************************************************
 * @file	ll_resolvlist.h
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
#ifndef LL_RESOLVLIST_H_
#define LL_RESOLVLIST_H_


#include <stack/ble/ble_common.h>



/******************************* Macro & Enumeration & Structure Definition for Stack Begin, user can not use!!!!  *****/






/******************************* Macro & Enumeration & Structure Definition for Stack End ******************************/






/******************************* Macro & Enumeration variables for User Begin ******************************************/


/******************************* Macro & Enumeration variables for User End ********************************************/







/******************************* User Interface  Begin *****************************************************************/
ble_sts_t  		ll_resolvingList_add(u8 peerIdAddrType, u8 *peerIdAddr, u8 *peer_irk, u8 *local_irk);
ble_sts_t  		ll_resolvingList_delete(u8 peerIdAddrType, u8 *peerIdAddr);
ble_sts_t  		ll_resolvingList_reset(void);
ble_sts_t  		ll_resolvingList_getSize(u8 *Size);
ble_sts_t  		ll_resolvingList_getPeerResolvableAddr (u8 peerIdAddrType, u8* peerIdAddr, u8* peerResolvableAddr); //not available now
ble_sts_t  		ll_resolvingList_getLocalResolvableAddr(u8 peerIdAddrType, u8* peerIdAddr, u8* LocalResolvableAddr); //not available now
ble_sts_t  		ll_resolvingList_setAddrResolutionEnable (u8 resolutionEn);
ble_sts_t  		ll_resolvingList_setResolvablePrivateAddrTimer (u16 timeout_s);   //not available now
ble_sts_t  		ll_resolvingList_setPrivcyMode(u8 peerIdAddrType, u8* peerIdAddr, u8 privMode);

u16				blc_ll_resolvGetRpaTmo(void);
int				blc_ll_resolvPeerRpaResolvedAny(const u8* rpa);
//void			blc_ll_resolvGetRpaByRlEntry(ll_resolv_list_t* rl, u8* addr, u8 local);
void			blc_ll_resolvSetPeerRpaByIdx(u8 idx, u8 *rpa);
void			blc_ll_resolvSetLocalRpaByIdx(u8 idx, u8 *rpa);
bool			blc_ll_resolvGetRpaByAddr(u8* peerIdAddr, u8 peerIdAddrType, u8* rpa, u8 local);
bool			blc_ll_resolvIsAddrResolved(const u8* irk, const u8* rpa);
bool			blc_ll_resolvIsAddrRlEnabled(void);
void			blc_ll_resolvListInit(void);
/******************************* User Interface  End  ******************************************************************/





#endif /* LL_RESOLVLIST_H_ */
