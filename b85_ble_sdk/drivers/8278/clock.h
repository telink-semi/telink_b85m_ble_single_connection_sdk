/********************************************************************************************************
 * @file	clock.h
 *
 * @brief	This is the header file for B87
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once


#include "compiler.h"
#include "register.h"


#define 	_ASM_NOP_			asm("tnop")

/**
 * @brief  instruction delay.
 */
#define		CLOCK_DLY_1_CYC    _ASM_NOP_
#define		CLOCK_DLY_2_CYC    _ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_3_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_4_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_5_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_6_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_7_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_8_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_9_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_10_CYC   _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_

/**
 * @brief select RC as system clock
 */
#ifndef SYSCLK_RC_CLOCK_EN
#define SYSCLK_RC_CLOCK_EN			0
#endif

extern unsigned char system_clk_type;


/**
 * @brief 	external XTAL type,
 * 			0- 24M , 1-48M
 */
#define  CLK_48M_XTAL_EN   			0

#define  VULTURE_A0   				0
/**
 * @brief 	Power type for different application
 */
typedef enum{
	LDO_MODE 		=0x40,	//LDO mode
	DCDC_MODE		=0x43,	//DCDC mode (16pin is not suported this mode.)
	DCDC_LDO_MODE	=0x41,	//DCDC_LDO mode (synchronize mode,Use the asynchronize 
								//mode with DCDC_LDO may cause the current abnormal(A0 version))
}POWER_MODE_TypeDef;
/**
 * @brief 	crystal for different application
 */
typedef enum{
	EXTERNAL_XTAL_24M	= 0,			//Use an external 24M crystal and internal capacitors.
	EXTERNAL_XTAL_EXTERNAL_CAP_24M	= 2,//Use an external 24M crystal and external capacitors.
}XTAL_TypeDef;
/**
 * @brief system clock type.
 */
typedef enum{
	SYS_CLK_4M_Crystal  = 0x4C,
	SYS_CLK_6M_Crystal  = 0x48,
	SYS_CLK_8M_Crystal  = 0x46,
	SYS_CLK_12M_Crystal = 0x44,
	SYS_CLK_16M_Crystal = 0x43,
	SYS_CLK_24M_Crystal = 0x42,
	SYS_CLK_32M_Crystal = 0x60,
	SYS_CLK_48M_Crystal = 0x20,

	SYS_CLK_RC_THRES = 0x10,

	SYS_CLK_24M_RC 	 = 0x00,
//	SYS_CLK_32M_RC 	 = 0x01,
//	SYS_CLK_48M_RC 	 = 0x02,
}SYS_CLK_TypeDef;

/**
 * @brief 32K clock type.
 */

typedef enum{
	CLK_32K_RC   =0,
	CLK_32K_XTAL =1,
}CLK_32K_TypeDef;

/**
 * @brief       This function to select the system clock source.
 * @param[in]   SYS_CLK - the clock source of the system clock.
 * @note		Do not switch the clock during the DMA sending and receiving process;
 * 			    because during the clock switching process, the system clock will be
 * 			    suspended for a period of time, which may cause data loss.
 */
void clock_init(SYS_CLK_TypeDef SYS_CLK);

/**
 * @brief       This function to get the system clock source.
 * @param[in]   none
 * @return      system clock source
 */

static inline int clock_get_system_clk(void)
{
	return system_clk_type;
}

/**
 * @brief   This function serves to set 32k clock source.
 * @param   variable of 32k type.
 * @return  none.
 */
void clock_32k_init (CLK_32K_TypeDef src);

/**
 * @brief     This function performs to select 24M as the system clock source.
 * @param[in] none.
 * @return    none.
 */
void rc_24m_cal (void);

/**
 * @brief     This function performs to select 48M RC as the system clock source.
 * @param[in] none.
 * @return    none.
 */
void rc_48m_cal (void);

/**
 * @brief     This function performs to select 32K as the system clock source.
 * @param[in] none.
 * @return    none.
 */
void rc_32k_cal (void);

/**
 * @brief     This function performs to select 24M/2 RC as source of DMIC.
 * @param[in] source clock to provide DMIC.
 * @return    none.
 */
void dmic_prob_48M_rc(void);


