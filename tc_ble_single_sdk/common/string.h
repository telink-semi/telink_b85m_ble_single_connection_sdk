/********************************************************************************************************
 * @file    string.h
 *
 * @brief   This is the header file for B85
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
#ifndef COMMON_STRING_H_
#define COMMON_STRING_H_

#ifndef NULL
#define NULL  0
#endif

void *  memset(void * d, int c, unsigned int  n);
void *  memcpy(void * des_ptr, const void * src_ptr, unsigned int);

// do not return void*,  otherwise, we must use a variable to store the dest porinter, that is not performance
void   	memcpy4(void * dest, const void * src, unsigned int);

int		memcmp(const void *_s1, const void *_s2, unsigned int _n);

/**
 * @brief		This function is used to determine whether the array is all 0s.
 * @param[in]	data	- the buffer data.
 * @param[in]	len		- the length of data.
 * @return		1: all 0, 0: not all 0.
 */
int ismemzero4(void *data, unsigned int len);
#endif /* COMMON_STRING_H_ */
