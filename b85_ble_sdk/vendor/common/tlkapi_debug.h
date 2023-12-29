/********************************************************************************************************
 * @file    tlkapi_debug.h
 *
 * @brief   This is the header file for BLE SDK
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
#ifndef VENDOR_COMMON_TLKAPI_DEBUG_H_
#define VENDOR_COMMON_TLKAPI_DEBUG_H_



#ifndef UART_PRINT_DEBUG_ENABLE
#define UART_PRINT_DEBUG_ENABLE                    			0
#endif



/**
 * @brief	GPIO simulate UART, support 1000000 baud rate only, user don not change !!!
 */
#ifndef PRINT_BAUD_RATE
#define PRINT_BAUD_RATE       	1000000
#endif






/**
 * @brief	Debug log control data structure, user do not need to pay attention to these
 */
typedef struct{
	u8	dbg_en;
	u8 rsdv[3];
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
 * @param[in]	format - the string will be printed
 * @return
 */
int  tlk_printf(const char *format, ...);



/**
 * @brief	user do not need to pay attention to this API below, and do not use them in application.
 * @param[in]	str - character string
 * @param[in]	pData - pointer of data
 * @param[in]	data_len - length of data
 * @return
 */
void tlkapi_send_str_data (char *str, u8 *pData, u32 data_len);


/**
 * @brief   	print log with GPIO simulate UART or USB
 * @param[in]	en - print log enable, 1: enable;  0: disable
 * @param[in]	fmt - the string will be printed
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







#endif /* VENDOR_COMMON_TLKAPI_DEBUG_H_ */
