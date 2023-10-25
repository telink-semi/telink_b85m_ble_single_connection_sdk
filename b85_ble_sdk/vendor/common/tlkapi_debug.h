/********************************************************************************************************
 * @file	tlkapi_debug.h
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

#ifndef VENDOR_COMMON_TLKAPI_DEBUG_H_
#define VENDOR_COMMON_TLKAPI_DEBUG_H_




/* GPIO simulate UART, support 1000000 baud rate only, user don not change !!!*/
#ifndef PRINT_BAUD_RATE
#define PRINT_BAUD_RATE       	1000000
#endif






/**
 * @brief	Debug log control data structure, user do not need to pay attention to these
 */
typedef struct{
	u8	dbg_en;

}tlk_dbg_t;
extern tlk_dbg_t tlkDbgCtl;


/**
 * @brief		uart init when MCU power on or wake_up from deepSleep mode
 * @param[in]	None
 * @return		None
 */
void tlkapi_debug_init(void);



/**
 * @brief   	print debug log with GPIO simulate UART
 * @param[in]	format -
 * @return
 */
int  tlk_printf(const char *format, ...);



/**
 * @brief	user do not need to pay attention to this API below, and do not use them in application.
 */
void tlkapi_send_str_data (char *str, u8 *pData, u32 data_len);


/**
 * @brief   	print log with GPIO simulate UART or USB
 * @param[in]	en - print log enable, 1: enable;  0: disable
 * @param[in]	fmt -
 * @return		none
 */
#define tlkapi_printf(en, fmt, ...)									if(en){tlk_printf(fmt, ##__VA_ARGS__);}



/**
 * @brief   	print debug log with GPIO simulate UART or USB, character string and hex data mixed mode.
 * @param[in]	en - print log enable, 1: enable;  0: disable
 * @param[in]	str - character string
 * @param[in]	pData - pointer of data
 * @param[in]	len - length of data
 * @return		none
 */
#define tlkapi_send_string_data(en, str, pData, len)				if(en){tlkapi_send_str_data(str":%s\n", (u8*)pData, len);}







#endif /* VENDOR_COMMOM_TLKAPI_DEBUG_H_ */
