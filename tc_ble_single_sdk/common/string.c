/********************************************************************************************************
 * @file    string.c
 *
 * @brief   This is the source file for B85
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "types.h"
#include "string.h"
#include "common/assert.h"
#include "tl_common.h"

unsigned int strlen(const char *str) {
	unsigned int len = 0;
	if (str != NULL) {
		while (*str++) {
			len++;
		}
	}
	return len;
}

int memcmp(const void * m1, const void *m2, u32 len) {
	u8 *st1 = (u8 *) m1;
	u8 *st2 = (u8 *) m2;

	while(len--){
		if(*st1 != *st2){
			return (*st1 - *st2);
		}
		st1++;
		st2++;
	}
	return 0;
}



void bcopy(register char * src, register char * dest, int len) {
	if (dest < src)
		while (len--)
			*dest++ = *src++;
	else {
		char *lasts = src + (len - 1);
		char *lastd = dest + (len - 1);
		while (len--)
			*(char *) lastd-- = *(char *) lasts--;
	}
}


void * memset(void * dest, int val, unsigned int len) {
	register unsigned char *ptr = (unsigned char*) dest;
	while (len-- > 0)
		*ptr++ = (unsigned char)val;
	return dest;
}

void * memcpy(void * out, const void * in, unsigned int length) {
	bcopy((char *) in, (char *) out, (int) length);
	return out;
}

// for performance, assume length % 4 == 0,  and no memory overlapped
void memcpy4(void * d, const void * s, unsigned int length){
	int* dst = (int*)d;
	int* src = (int*)s;
	assert((((int)dst) >> 2) << 2 == ((int)dst));			// address must alighn to 4
	assert((((int)src) >> 2) << 2 == ((int)src));			// address must alighn to 4
	assert((length >> 2) << 2 == length);					// length % 4 == 0
	assert(( ((char*)dst) + length <= (const char*)src) || (((const char*)src) + length <= (char*)dst));	//  no overlapped
	unsigned int len = length >> 2;
	while(len --){
		*dst++ = *src++;
	}
}

/**
 * @brief		This function is used to determine whether the array is all 0s.
 * @param[in]	data	- the buffer data.
 * @param[in]	len		- the length of data.
 * @return		1: all 0, 0: not all 0.
 */
int ismemzero4(void *data, unsigned int len){
	int *p = (int*)data;
	len = len >> 2;
	for(int i = 0; i < len; ++i){
		if(*p){
			return 0;
		}
		++p;
	}
	return 1;
}
