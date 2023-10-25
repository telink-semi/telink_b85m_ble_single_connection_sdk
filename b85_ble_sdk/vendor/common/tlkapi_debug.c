/********************************************************************************************************
 * @file	tlkapi_debug.c
 *
 * @brief	This is the source file for BLE SDK
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
#include "tl_common.h"
#include "drivers.h"

#include <stdarg.h>

_attribute_ble_data_retention_ tlk_dbg_t tlkDbgCtl;
extern int u_printf(const char *format, ...);


#include "tlkapi_debug.h"
void tlkapi_debug_init(void)
{
	#if (PRINT_BAUD_RATE != 1000000)
		#error "GPIO simulate UART, support 1000000 baud rate only!!!*"
	#endif
	tlkDbgCtl.dbg_en = 1;
}

/**
 * @brief   	Send debug log to log FIFO, printf mode
 *				attention: here just send log to FIFO, can not output immediately, wait for "tlkapi debug_handler" to output log.
 * @param[in]	format -
 * @return
 */

int tlk_printf(const char *format, ...)
{
	if(tlkDbgCtl.dbg_en){
		va_list args;
		va_start( args, format );
		v_printf(format,args);
		va_end( args );
	}
	return 0;
}

/**
 * @brief   Send debug log to log FIFO, character string and data mixed mode.
 *			attention: here just send log to FIFO, can not output immediately, wait for "tlkapi debug_handler" to output log.
 * @param[in]	str - character string
 * @param[in]	pData - pointer of data
 * @param[in]	len - length of data
 * @return		none
 */

void tlkapi_send_str_data (char *str, u8 *pData, u32 data_len)
{
	if(tlkDbgCtl.dbg_en){

		/* user can change this size if "data_len" bigger than 32 */
		#define TLKAPI_DEBUG_DATA_MAX_LEN    32
		unsigned char hex[] = "0123456789abcdef";
		unsigned char temp_str[TLKAPI_DEBUG_DATA_MAX_LEN * 3 + 1];
		const u8 *b = pData;
		u8 i;

		u8 len = min(data_len, TLKAPI_DEBUG_DATA_MAX_LEN);

		for (i = 0; i < len; i++) {
			temp_str[i*3] = ' ';
			temp_str[i * 3 + 1]     = hex[b[i] >> 4];
			temp_str[i * 3 + 2] =  hex[b[i] & 0xf];
		}

		temp_str[i * 3] = '\0';


		u_printf(str, temp_str);
	}
}
