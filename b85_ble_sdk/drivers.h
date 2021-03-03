/********************************************************************************************************
 * @file     drivers.h
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

#include "config.h"

#if(MCU_CORE_TYPE == MCU_CORE_825x)
	#include "drivers/8258/driver_8258.h"
	#include "drivers/8258/driver_ext/ext_rf.h"
	#include "drivers/8258/driver_ext/ext_pm.h"
	#include "drivers/8258/driver_ext/ext_misc.h"
#elif(MCU_CORE_TYPE == MCU_CORE_827x)
	#include "drivers/8278/driver_8278.h"
	#include "drivers/8278/driver_ext/ext_rf.h"
	#include "drivers/8278/driver_ext/ext_pm.h"
	#include "drivers/8278/driver_ext/ext_misc.h"
#else
	#error MCU core typr error !
#endif
