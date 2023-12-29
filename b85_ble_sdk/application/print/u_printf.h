/********************************************************************************************************
 * @file    u_printf.h
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
#pragma once

#include <stdarg.h>

#if (UART_PRINT_DEBUG_ENABLE || USB_PRINT_DEBUG_ENABLE)//print info by a gpio or usb printer
	int  u_printf(const char *fmt, ...);
	int  u_sprintf(char* s, const char *fmt, ...);
	void tl_array_printf(const char *fmt, unsigned char*data, unsigned int len);
	void u_array_printf(unsigned char*data, unsigned int len);

	#define printf	 		u_printf
	#define sprintf	 		u_sprintf
    #define array_printf	u_array_printf
#else
	#define printf
	#define sprintf
	#define array_printf
#endif




int v_printf(const char *format, va_list args);
