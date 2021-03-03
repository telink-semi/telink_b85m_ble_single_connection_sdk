/********************************************************************************************************
 * @file	hw_ecc.c
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
#include <algorithm/ecc/hw_ecc.h>
#include "stack/ble/ble_config.h"
#include "drivers.h"




#if  ((MCU_CORE_TYPE == MCU_CORE_827x) || (MCU_CORE_TYPE == MCU_CORE_9518))

//ECDH return code
enum ECDH_RET_CODE
{
	ECDH_SUCCESS = PKE_SUCCESS,
	ECDH_POINTOR_NULL = PKE_SUCCESS+0x60,
	ECDH_INVALID_INPUT,
	ECDH_INVALID_POINT,
	ECDH_RNG_NULL,
};

typedef enum
{
	TRNG_ERROR,
	TRNG_SUCCESS = 1,
}pke_trng_ret_code_e;


unsigned int secp256r1_p[8]    = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
unsigned int secp256r1_p_h[8]  = {0x00000003,0x00000000,0xFFFFFFFF,0xFFFFFFFB,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFD,0x00000004};
unsigned int secp256r1_p_n1[1] = {1};
unsigned int secp256r1_a[8]    = {0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
unsigned int secp256r1_b[8]    = {0x27D2604B,0x3BCE3C3E,0xCC53B0F6,0x651D06B0,0x769886BC,0xB3EBBD55,0xAA3A93E7,0x5AC635D8};
unsigned int secp256r1_Gx[8]   = {0xD898C296,0xF4A13945,0x2DEB33A0,0x77037D81,0x63A440F2,0xF8BCE6E5,0xE12C4247,0x6B17D1F2};
unsigned int secp256r1_Gy[8]   = {0x37BF51F5,0xCBB64068,0x6B315ECE,0x2BCE3357,0x7C0F9E16,0x8EE7EB4A,0xFE1A7F9B,0x4FE342E2};
unsigned int secp256r1_n[8]    = {0xFC632551,0xF3B9CAC2,0xA7179E84,0xBCE6FAAD,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0xFFFFFFFF};
unsigned int secp256r1_n_h[8]  = {0xBE79EEA2,0x83244C95,0x49BD6FA6,0x4699799C,0x2B6BEC59,0x2845B239,0xF3D95620,0x66E12D94};
unsigned int secp256r1_n_n1[1] = {0xEE00BC4F};

eccp_curve_t secp256r1[1]={
{	256,
	256,
	secp256r1_p,
	secp256r1_p_h,
	secp256r1_p_n1,
	secp256r1_a,
	secp256r1_b,
	secp256r1_Gx,
	secp256r1_Gy,
	secp256r1_n,
	secp256r1_n_h,
	secp256r1_n_n1,}
};

static hECC_rng_func g_rng_function = NULL;

void hwECC_set_rng(hECC_rng_func rng_func) {
    g_rng_function = rng_func;
}


/**
 * @brief		This function is used to determine whether the array is all 0s.
 * @param[in]	data	- the buffer data.
 * @param[in]	len		- the length of data.
 * @return		1: all 0, 0: not all 0.
 */
static int ismemzero4(void *data, unsigned int len){
	int *p = (int*)data;
	len = len >> 2;
	for(unsigned int i = 0; i < len; ++i){
		if(*p){
			return 0;
		}
		++p;
	}
	return 1;
}


/**
 * @brief		get ECCP key pair(the key pair could be used in ECDH).
 * @param[out]	public_key	- public key, big--endian.
 * @param[out]	private_key	- private key, big--endian.
 * @return		PKE_SUCCESS(success), other(error).
 */
unsigned char hwECC_make_key(unsigned char public_key[hECC_BYTES*2], unsigned char private_key[hECC_BYTES])
{
	eccp_curve_t *curve = secp256r1;

	unsigned int tmpLen;
 	unsigned int nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
 	unsigned int nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);
 	unsigned int pByteLen = GET_BYTE_LEN(curve->eccp_p_bitLen);
 	unsigned int k[PKE_OPERAND_MAX_WORD_LEN] = {0};
 	unsigned int x[PKE_OPERAND_MAX_WORD_LEN];
 	unsigned int y[PKE_OPERAND_MAX_WORD_LEN];
 	unsigned char ret;


 	ECCP_GETKEY_LOOP:

	if(g_rng_function == NULL)
	{
		return 0; //ECDH_RNG_NULL;
	}

	ret = g_rng_function((unsigned char *)k, nByteLen);

 	if(TRNG_SUCCESS != ret)
 	{
 		return 0; //ECDH_RNG_NULL;
 	}

 	//make sure k has the same bit length as n
 	tmpLen = (curve->eccp_n_bitLen)&0x1F;
 	if(tmpLen)
 	{
 		k[nWordLen-1] &= (1<<(tmpLen))-1;
 	}

 	//make sure k in [1, n-1]
 	if(ismemzero4(k, nWordLen<<2))
 	{
 		goto ECCP_GETKEY_LOOP;
 	}
 	if(big_integer_compare(k, nWordLen, curve->eccp_n, nWordLen) >= 0)
 	{
 		goto ECCP_GETKEY_LOOP;
 	}
 	//get public_key
 	ret = pke_eccp_point_mul(curve, k, curve->eccp_Gx, curve->eccp_Gy, x, y);
 	if(PKE_SUCCESS != ret)
 	{
 		return 0; //Q=[k]P Failed
 	}

 	//to big-end
 	swapX((unsigned char *)k, private_key, nByteLen);
 	swapX((unsigned char *)x, public_key, pByteLen);
 	swapX((unsigned char *)y, public_key + pByteLen, pByteLen);

 	return 1; //PKE_SUCCESS;
}


/**
 * @brief		ECDH compute key.
 * @param[in]	local_prikey	- local private key, big--endian.
 * @param[in]	public_key		- peer public key, big--endian.
 * @param[out]	dhkey			- output dhkey, big--endian..
 * @Return		0(success); other(error).
 */
unsigned char hwECC_shared_secret(const unsigned char public_key[hECC_BYTES*2],  const unsigned char private_key[hECC_BYTES], unsigned char secret[hECC_BYTES])
{
	unsigned int k[ECC_MAX_WORD_LEN] = {0};
	unsigned int Px[ECC_MAX_WORD_LEN] = {0};
	unsigned int Py[ECC_MAX_WORD_LEN] = {0};
	unsigned int byteLen, wordLen;
	unsigned char ret;

	eccp_curve_t *curve = secp256r1;

	if(0 == private_key || 0 == public_key || 0 == secret)
	{
		return 0; //ECDH_POINTOR_NULL;
	}

	byteLen = (curve->eccp_n_bitLen+7)/8;
	wordLen = (curve->eccp_n_bitLen+31)/32;

	//make sure private key is in [1, n-1]
	swapX((unsigned char *)private_key, (unsigned char *)k, byteLen);

	if(ismemzero4(k, wordLen<<2))
	{
		return 0; //ECDH_INVALID_INPUT;
	}
	if(big_integer_compare(k, wordLen, curve->eccp_n, wordLen) >= 0)
	{
		return 0;// ECDH_INVALID_INPUT;
	}

	//check public key
	swapX(public_key, (unsigned char *)Px, byteLen);
	swapX(public_key+byteLen, (unsigned char *)Py, byteLen);
	ret = pke_eccp_point_verify(curve, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return 0; //ECDH_INVALID_POINT
	}

	ret = pke_eccp_point_mul(curve, k, Px, Py, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return 0; //Q=[k]P Failed
	}

	swapX((unsigned char *)Px, secret, hECC_BYTES);

	return 1; //ECDH_SUCCESS;
}

#endif


