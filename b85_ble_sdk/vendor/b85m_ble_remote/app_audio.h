/********************************************************************************************************
 * @file	 app_audio.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
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

#define APP_AUDIO_BT_OPEN    0x01
#define APP_AUDIO_BT_CLOSE   0x00
#define APP_AUDIO_BT_CONFIG  0x02

extern 	unsigned int 		key_voice_pressTick;
extern	unsigned char		ui_mic_enable;
extern	unsigned char 		key_voice_press;
extern	int     			ui_mtu_size_exchange_req;

void dmic_gpio_reset (void);
void amic_gpio_reset (void);
void ui_enable_mic (int en);
void voice_press_proc(void);
void task_audio (void);
void blc_checkConnParamUpdate(void);
void proc_audio(void);
int server2client_auido_proc(void* p);

void audio_state_check(void);
void key_voice_is_press(void);
void key_voice_is_release(void);

#endif /* APP_AUDIO_H_ */
