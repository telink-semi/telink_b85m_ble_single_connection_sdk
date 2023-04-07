/********************************************************************************************************
 * @file	app_audio.h
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
#ifndef APP_AUDIO_H_
#define APP_AUDIO_H_

/**
 * @brief		usb_endpoints_irq_handler
 * @param[in]	none
 * @return      none
 */
void  	usb_endpoints_irq_handler (void);

/**
 * @brief		call this function to process when attHandle equal to AUDIO_HANDLE_MIC
 * @param[in]	conn - connect handle
 * @param[in]	p - Pointer point to l2cap data packet.
 * @return      none
 */
void	att_mic (u16 conn, u8 *p);

/**
 * @brief		audio proc in main loop
 * @param[in]	none
 * @return      none
 */
void 	proc_audio (void);

/**
 * @brief		reset mic_packet,reset audio id and writer pointer and read pointer
 * @param[in]	none
 * @return      none
 */
void 	mic_packet_reset(void);

/**
 * @brief		push_mic_packet
 * @param[in]	p - Pointer point to l2cap data packet
 * @return      none
 */
void 	push_mic_packet(unsigned char *p);

/**
 * @brief		copy packet data to defined buffer to process
 * @param[in]	data - Pointer point to l2cap data packet
 * @param[in]	length - the data length
 * @return      none
 */
void    app_audio_data(u8 * data, u16 length);

/**
 * @brief		usb_report_hid_mic
 * @param[in]	data - Pointer point to l2cap data packet
 * @param[in]	report_id - the data packet of report id
 * @return      0 - usb is busy and forbidden report hid mic
 *              1 - usb allow to report hid mic
 */
unsigned char 	usb_report_hid_mic(u8* data, u8 report_id);

#endif /* APP_AUDIO_H_ */
