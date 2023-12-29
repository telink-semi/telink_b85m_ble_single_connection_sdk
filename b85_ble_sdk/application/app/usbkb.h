/********************************************************************************************************
 * @file    usbkb.h
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
#include <application/usbstd/HIDClassCommon.h>
#include <application/usbstd/HIDReportData.h>
#include "tl_common.h"
#include "drivers.h"
//#include "../common/types.h"
//#include "../common/static_assert.h"
//#include "../mcu/clock_i.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define DAT_TYPE_KB			1
#define DAT_TYPE_MOUSE		2

#define USB_FIFO_NUM		4
#define USB_FIFO_SIZE		8

extern u8 usb_fifo[USB_FIFO_NUM][USB_FIFO_SIZE];
extern u8 usb_ff_rptr;
extern u8 usb_ff_wptr;



#define KEYBOARD_REPORT_KEY_MAX     6
typedef struct {
    u8 Modifier; /**< Keyboard modifier byte, indicating pressed modifier keys (a combination of
                       *   \c HID_KEYBOARD_MODIFIER_* masks).
                       */
    u8 Reserved; /**< Reserved for OEM use, always set to 0. */
    u8 KeyCode[KEYBOARD_REPORT_KEY_MAX]; /**< Key codes of the currently pressed keys. */
} usbkb_hid_report_t;

int usbkb_hid_report_normal(u8 ctrl_key, u8 *keycode);

int usb_hid_report_fifo_proc(void);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

