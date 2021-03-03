/********************************************************************************************************
 * @file     app_audio.h
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     2020-5-13
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

#ifndef APP_AUDIO_H_
#define APP_AUDIO_H_



void  	usb_endpoints_irq_handler (void);
void	att_mic (u16 conn, u8 *p);
void 	proc_audio (void);
void 	mic_packet_reset(void);
void 	push_mic_packet(unsigned char *p);
void    app_audio_data(u8 * data, u16 length);

unsigned char 	usb_report_hid_mic(u8* data, u8 report_id);

#endif /* APP_AUDIO_H_ */
