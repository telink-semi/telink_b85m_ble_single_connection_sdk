/********************************************************************************************************
 * @file	debug.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/

#ifndef STACK_BLE_DEBUG_H_
#define STACK_BLE_DEBUG_H_
/**
 *  @brief stack log
 */
typedef enum {
	STK_LOG_DISABLE		 		= 0,

	STK_LOG_LL_CMD		 		= BIT(0),

	STK_LOG_LL_RX		 		= BIT(11),
	STK_LOG_LL_TX		 		= BIT(12),

	STK_LOG_SMP_RX				= BIT(14),
	STK_LOG_SMP_TX				= BIT(15),

	STK_LOG_ATT_RX		 		= BIT(20),
	STK_LOG_ATT_TX		 		= BIT(21),

	STK_LOG_OTA_FLOW			= BIT(24),
	STK_LOG_OTA_DATA			= BIT(25),

	STK_LOG_ALL		 			= 0xFFFFFFFF,
}stk_log_msk_t;

extern u32 stkLog_mask;


/**
 * @brief      for user to configure which type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_enableStackLog(stk_log_msk_t mask);

/**
 * @brief      for user to add some type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_addStackLog(stk_log_msk_t mask);

/**
 * @brief      for user to remove some type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_removeStackLog(stk_log_msk_t mask);


#endif /* STACK_BLE_DEBUG_H_ */
