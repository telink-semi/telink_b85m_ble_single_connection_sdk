/********************************************************************************************************
 * @file     sbc.h
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     2020-5-13
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef SBC_H_
#define SBC_H_

#include "common/types.h"

typedef signed long long int64_t;
typedef signed int 	int32_t;
typedef signed int 	ssize_t;
typedef signed short int16_t;
typedef signed char int8_t;
typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef int32_t sbc_fixed_t;

uint64_t mul64(uint64_t x, uint32_t y);
uint64_t div64(uint64_t x, uint16_t y);
uint64_t shift_left64(uint64_t x, uint8_t bits);

void sbcenc_reset(void);
uint32_t sbc_enc(const uint8_t* buf, uint16_t len, uint8_t *outbuf, uint32_t outbuf_len, uint32_t* out_len);
size_t sbc_decode(const void *input, size_t input_len,void *output, size_t output_len, size_t *written);
/* Returns the output block size in bytes */
size_t sbc_get_frame_length(int type);

/* Returns the input block size in bytes */
size_t sbc_get_codesize(int type);

void msbc_init_ctx();
void msbc_release_ctx();
#endif /* SBC_H_ */
