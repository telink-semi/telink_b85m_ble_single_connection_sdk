/********************************************************************************************************
 * @file	string.h
 *
 * @brief	This is the header file for B85
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
#ifndef COMMON_STRING_H_
#define COMMON_STRING_H_



void *  memset(void * d, int c, unsigned int  n);
void *  memcpy(void * des_ptr, const void * src_ptr, unsigned int);

// do not return void*,  otherwise, we must use a variable to store the dest porinter, that is not performance
void   	memcpy4(void * dest, const void * src, unsigned int);

int		memcmp(const void *_s1, const void *_s2, unsigned int _n);


#endif /* COMMON_STRING_H_ */
