/********************************************************************************************************
 * @file     config.h
 *
 * @brief    This is the header file for TLSR8258
 *
 * @author	 BLE Group
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma once


#define	CHIP_TYPE_825x  	1
#define CHIP_TYPE_827x      2
#define	CHIP_TYPE_9518		5

#ifndef CHIP_TYPE
#define	CHIP_TYPE 			CHIP_TYPE_825x
#endif


#define	MCU_CORE_825x 		1
#define	MCU_CORE_8258 		1
#define MCU_CORE_827x       2
#define MCU_CORE_8278       2

#define	MCU_CORE_9518 		5

#if(CHIP_TYPE == CHIP_TYPE_825x)
	#define MCU_CORE_TYPE	MCU_CORE_825x
#elif(CHIP_TYPE == CHIP_TYPE_827x)
	#define MCU_CORE_TYPE	MCU_CORE_827x
#else
	#define MCU_CORE_TYPE	1000
#endif



