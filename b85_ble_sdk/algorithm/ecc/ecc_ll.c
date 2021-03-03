/********************************************************************************************************
 * @file	ecc_ll.c
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/controller/ble_controller.h"

#include "algorithm/ecc/sw_ecc.h"
#include "algorithm/ecc/hw_ecc.h"
#include "algorithm/ecc/ecc_ll.h"
#include "algorithm/aes_ccm/aes_ccm.h"


//ECC implementation method selection
#define		ECC_HW_IMPLEMEMTATION		0
#define		ECC_SW_IMPLEMEMTATION		1


#if  (MCU_CORE_TYPE == MCU_CORE_827x || MCU_CORE_TYPE == MCU_CORE_9518)
#define		ECC_METHOD_SELECTION		ECC_HW_IMPLEMEMTATION //Eagle/Vulture use hw ECC, Kite use sw ECC
#elif(MCU_CORE_TYPE == MCU_CORE_825x)
#define		ECC_METHOD_SELECTION		ECC_SW_IMPLEMEMTATION //Eagle/Vulture use hw ECC, Kite use sw ECC
#endif


#if (ECC_METHOD_SELECTION == ECC_SW_IMPLEMEMTATION)
#define		ecc_ll_set_rng				swECC_set_rng
#define		ecc_ll_make_key				swECC_make_key
#define		ecc_ll_gen_dhkey			swECC_shared_secret
#elif (ECC_METHOD_SELECTION == ECC_HW_IMPLEMEMTATION)
#define		ecc_ll_set_rng				hwECC_set_rng
#define		ecc_ll_make_key				hwECC_make_key
#define		ecc_ll_gen_dhkey			hwECC_shared_secret
#endif


/* Refer to <<Core 4.2>> Vol 3. Part H 2.3.5.6.1
 * SMP test dhkey. Only one side (initiator or responder) needs to set SC debug mode in order for debug
 * equipment to be able to determine the LTK and, therefore, be able to monitor the encrypted connection.
 * */
const u8 blt_ecc_dbg_priv_key[32] = { //SKb :Private key of the response device, big--endian
	//Private key
	0x3f, 0x49, 0xf6, 0xd4, 0xa3, 0xc5, 0x5f, 0x38, 0x74, 0xc9, 0xb3, 0xe3, 0xd2, 0x10, 0x3f, 0x50,
	0x4a, 0xff, 0x60, 0x7b, 0xeb, 0x40, 0xb7, 0x99, 0x58, 0x99, 0xb8, 0xa6, 0xcd, 0x3c, 0x1a, 0xbd,
};

const u8 blt_ecc_dbg_pub_key[64] = { //PKb :Public key of response device, big--endian
	//Public key (X):
	0x20, 0xb0, 0x03, 0xd2, 0xf2, 0x97, 0xbe, 0x2c, 0x5e, 0x2c, 0x83, 0xa7, 0xe9, 0xf9, 0xa5, 0xb9,
	0xef, 0xf4, 0x91, 0x11, 0xac, 0xf4, 0xfd, 0xdb, 0xcc, 0x03, 0x01, 0x48, 0x0e, 0x35, 0x9d, 0xe6,
	//Public key (Y):
	0xdc, 0x80, 0x9c, 0x49, 0x65, 0x2a, 0xeb, 0x6d, 0x63, 0x32, 0x9a, 0xbf, 0x5a, 0x52, 0x15, 0x5c,
	0x76, 0x63, 0x45, 0xc2, 0x8f, 0xed, 0x30, 0x24, 0x74, 0x1c, 0x8e, 0xd0, 0x15, 0x89, 0xd2, 0x8b,
};


/**
 * @brief		This function is used to provide random number generator for ECC calculation
 * @param[out]  dest: The address where the random number is stored
 * @param[in]   size: Output random number size, unit byte
 * @return		1:  success
 */
static int blt_ecc_gen_rand(unsigned char *dest, unsigned int size)
{
	unsigned int randNums = 0;
	/* if len is odd */
	for (int i=0; i<size; i++ ) {
		if( (i & 3) == 0 ){
			randNums = rand();
		}

		dest[i] = randNums & 0xff;
		randNums >>=8;
	}

    return 1;
}


/**
 * @brief		This function is used to register the random number function needed for ECC calculation
 * @param		none
 * @return		none
 */
void blt_ecc_init(void)
{
	ecc_ll_set_rng(blt_ecc_gen_rand);
}


/**
 * @brief		This function is used to generate an ECDH public-private key pairs
 * @param[out]  pub[64]:  output ecdh public key, big--endian
 * @param[out]  priv[64]: output ecdh private key, big--endian
 * @param[in]   use_dbg_key: 0: Non-debug key , others: debug key
 * @return		1: success
 *              0: failure
 */
int blt_ecc_gen_key_pair(unsigned char pub[64], unsigned char priv[32], bool use_dbg_key)
{
	//distribute private/public key pairs
	if(use_dbg_key){
		smemcpy(priv, (u8*)blt_ecc_dbg_priv_key, 32);
		smemcpy(pub , (u8*)blt_ecc_dbg_pub_key,	 64);
	}
	else{
		do{
			//DBG_C HN4_TOGGLE;  //32M: hw take 45ms
			if(!ecc_ll_make_key(pub, priv)){
				return 0; //check swECC_make_key() failed
			}
			//DBG_C HN4_TOGGLE;
		/* Make sure generated key isn't debug key. */
		}while (memcmp(priv, blt_ecc_dbg_priv_key, 32) == 0);
	}

    return 1;
}


/**
 * @brief		This function is used to calculate DHKEY based on the peer public key and own private key
 * @param[in]   peer_pub_key[64]: peer public key, big--endian
 * @param[in]   own_priv_key[32]: own private key, big--endian
 * @param[out]  out_dhkey[32]: dhkey key, big--endian
 * @return		1:  success
 *              0: failure
 */
int blt_ecc_gen_dhkey(const unsigned char peer_pub[64], const unsigned char own_priv[32], unsigned char out_dhkey[32])
{
	//DBG_C HN4_TOGGLE;  //32M: hw take 45ms
    if (!ecc_ll_gen_dhkey(peer_pub, own_priv, out_dhkey)) {
        return 0;
    }
    //DBG_C HN4_TOGGLE;

    return 1;
}




