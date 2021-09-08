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

#define APP_AUDIO_BT_OPEN    0x01
#define APP_AUDIO_BT_CLOSE   0x00
#define APP_AUDIO_BT_CONFIG  0x02

extern 	unsigned int 		key_voice_pressTick;
extern	unsigned char		ui_mic_enable;
extern	unsigned char 		key_voice_press;
extern	int     			ui_mtu_size_exchange_req;

/**
 * @brief      the func serve to init dmic
 * @param[in]  none
 * @return     none
 */
void dmic_gpio_reset (void);

/**
 * @brief      the func serve to init amic
 * @param[in]  none
 * @return     none
 */
void amic_gpio_reset (void);

/**
 * @brief      for open the audio and mtu size exchange
 * @param[in]  en   0:close the micphone  1:open the micphone
 * @return     none
 */
void ui_enable_mic (int en);

/**
 * @brief      for open the audio and mtu size exchange
 * @param[in]  none
 * @return     none
 */
void voice_press_proc(void);

/**
 * @brief      audio task in loop for encode and transmit encode data
 * @param[in]  none
 * @return     none
 */
void task_audio (void);

/**
 * @brief      This function serves to Request ConnParamUpdate
 * @param[in]  none
 * @return     none
 */
void blc_checkConnParamUpdate(void);

/**
 * @brief      audio proc in main loop
 * @param[in]  none
 * @return     none
 */
void proc_audio(void);

/**
 * @brief      this function is call back function of audio measurement from server to client
 * @param[in]  p:data pointer.
 * @return     will always return 0
 */
int server2client_auido_proc(void* p);

/**
 * @brief      this function is used to check audio state
 * @param[in]  none
 * @return     none
 */
void audio_state_check(void);

/**
 * @brief      this function is used to define what to do when voice key is pressed
 * @param[in]  none
 * @return     none
 */
void key_voice_is_press(void);

/**
 * @brief      this function is used to define what to do when voice key is released
 * @param[in]  none
 * @return     none
 */
void key_voice_is_release(void);

#endif /* APP_AUDIO_H_ */
