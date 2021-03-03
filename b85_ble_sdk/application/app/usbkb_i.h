/********************************************************************************************************
 * @file     usbkb_i.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/

#pragma once

#include "usbkb.h"
#include "../usbstd/usbhw.h"
#include "../usbstd/usbhw_i.h"
#include "../../common/config/user_config.h"

/** HID class report descriptor. This is a special descriptor constructed with values from the
 *  USBIF HID class specification to describe the reports and capabilities of the HID device. This
 *  descriptor is parsed by the host and its contents used to determine what data (and in what encoding)
 *  the device will send, and what it may be sent back from the host. Refer to the HID specification for
 *  more details on HID report descriptors.
 */
static const USB_Descriptor_HIDReport_Datatype_t keyboard_report_desc[] = {
    HID_DESCRIPTOR_KEYBOARD(KEYBOARD_REPORT_KEY_MAX),
};

static inline u8* usbkb_get_report_desc(void) {
	return (u8*) (keyboard_report_desc);
}

static inline u16 usbkb_get_report_desc_size(void) {
	return sizeof(keyboard_report_desc);
}

#if (AUDIO_HOGP)
static const USB_Descriptor_HIDReport_Datatype_t audio_hogp_report_desc[] = {

	    0x06, 0x01, 0xFF,  // Usage Page (Vendor Defined 0xFF01)
	    0x09, 0x02,        // Usage (tmp usage)
	    0xA1, 0x02,        //   Collection (Logical)

	    0x85, 0x02,        //   Report ID (2)
	    0x09, 0x14,        //   Usage (tmp usage)
	    0x75, 0x08,        //   Report Size (8)
	    0x95, 0x14,        //   Report Count (20)
	    0x15, 0x80,        //   Logical Minimum (128)
	    0x25, 0x7F,        //   Logical Maximum (127)
	    0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)

	    0x85, 0x0a,        //   Report ID (10)
	    0x09, 0x14,        //   Usage (tmp usage)
	    0x75, 0x08,        //   Report Size (8)
	    0x95, 0x14,        //   Report Count (20)
	    0x15, 0x80,        //   Logical Minimum (128)
	    0x25, 0x7F,        //   Logical Maximum (127)
	    0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)

	    0x85, 0x0b,        //   Report ID (11)
	    0x09, 0x14,        //   Usage (tmp usage)
	    0x75, 0x08,        //   Report Size (8)
	    0x95, 0x14,        //   Report Count (20)
	    0x15, 0x80,        //   Logical Minimum (128)
	    0x25, 0x7F,        //   Logical Maximum (127)
	    0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)


	    0x85, 0x0c,        //   Report ID (12)
	    0x09, 0x14,        //   Usage (tmp usage)
	    0x75, 0x08,        //   Report Size (8)
	    0x95, 0x14,        //   Report Count (20)
	    0x15, 0x80,        //   Logical Minimum (128)
	    0x25, 0x7F,        //   Logical Maximum (127)
	    0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)

	    0x85, 0x04,        //   Report ID (4)
	    0x09, 0x04,        //   Usage (tmp usage)
	    0x75, 0x08,        //   Report Size (8)
	    0x95, 0x01,        //   Report Count (1)
	    0x91, 0x02,        //   Output

	    0xC0,              // End Collection
};
static const USB_Descriptor_HIDReport_Datatype_t vendor_report_desc[] = {

		0x06, 0x01, 0xFF,  // Usage Page (Vendor Defined 0xFF01)
		0x09, 0x02, 	   // Usage (tmp usage)
		0xA1, 0x02, 	   //	Collection (Logical)
		0x85, 0x03, 	   //	Report ID (3)
		0x09, 0x14, 	   //	Usage (tmp usage)
		0x75, 0x08, 	   //	Report Size (8)
		0x95, 0x20, 	   //	Report Count (20)
		0x15, 0x80, 	   //	Logical Minimum (128)
		0x25, 0x7F, 	   //	Logical Maximum (127)
		0x81, 0x22, 	   //	Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)

		0x85, 0x04, 	   //	Report ID (4)
		0x09, 0x04, 	   //	Usage (tmp usage)
		0x75, 0x08, 	   //	Report Size (8)
		0x95, 0x20, 	   //	Report Count (1)
		0x91, 0x02, 	   //	Output

		0xC0,			   // End Collection

};

static inline u8* usbaudio_hogp_get_report_desc(void) {
	return (u8*) (audio_hogp_report_desc);
}
static inline u16 usbaudio_hogp_get_report_desc_size(void) {
	return sizeof(audio_hogp_report_desc);
}

static inline u8* usb_vendor_get_report_desc(void) {
	return (u8*) (vendor_report_desc);
}
static inline u16 usb_vendor_get_report_desc_size(void) {
	return sizeof(vendor_report_desc);
}
#endif
