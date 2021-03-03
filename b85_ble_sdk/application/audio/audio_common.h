/********************************************************************************************************
 * @file     audio_common.h
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

#ifndef AUDIO_COMMON_H_
#define AUDIO_COMMON_H_

#define		TL_AUDIO_MASK_SBC_MODE							(0x00000001)
#define		TL_AUDIO_MASK_MSBC_MODE							(0x00000002)
#define		TL_AUDIO_MASK_ADPCM_MODE						(0x00000004)

#define		TL_AUDIO_MASK_HID_SERVICE_CHANNEL				(0x00000100)
#define		TL_AUDIO_MASK_GATT_SERVICE_TELINK_CHANNEL		(0x00000200)
#define		TL_AUDIO_MASK_GATT_SERVICE_GOOGLE_CHANNEL		(0x00000400)

#define		TL_AUDIO_MASK_DONGLE_TO_STB						(0x00010000)

#define		RCU_PROJECT										(0x01000000)
#define		DONGLE_PROJECT									(0x02000000)

#define		AUDIO_DISABLE									0

//RCU Audio Mode

#define		TL_AUDIO_RCU_ADPCM_GATT_TLEINK					(RCU_PROJECT | TL_AUDIO_MASK_ADPCM_MODE | TL_AUDIO_MASK_GATT_SERVICE_TELINK_CHANNEL)

#define		TL_AUDIO_RCU_ADPCM_GATT_GOOGLE					(RCU_PROJECT | TL_AUDIO_MASK_ADPCM_MODE | TL_AUDIO_MASK_GATT_SERVICE_GOOGLE_CHANNEL)

#define		TL_AUDIO_RCU_ADPCM_HID							(RCU_PROJECT | TL_AUDIO_MASK_ADPCM_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL)

#define		TL_AUDIO_RCU_SBC_HID							(RCU_PROJECT | TL_AUDIO_MASK_SBC_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL)

#define		TL_AUDIO_RCU_ADPCM_HID_DONGLE_TO_STB			(RCU_PROJECT | TL_AUDIO_MASK_ADPCM_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL | TL_AUDIO_MASK_DONGLE_TO_STB)

#define		TL_AUDIO_RCU_SBC_HID_DONGLE_TO_STB				(RCU_PROJECT | TL_AUDIO_MASK_SBC_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL | TL_AUDIO_MASK_DONGLE_TO_STB)

#define		TL_AUDIO_RCU_MSBC_HID							(RCU_PROJECT | TL_AUDIO_MASK_MSBC_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL)

//Dongle Audio Mode

#define		TL_AUDIO_DONGLE_ADPCM_GATT_TELINK				(DONGLE_PROJECT | TL_AUDIO_MASK_ADPCM_MODE | TL_AUDIO_MASK_GATT_SERVICE_TELINK_CHANNEL)

#define		TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE				(DONGLE_PROJECT | TL_AUDIO_MASK_ADPCM_MODE | TL_AUDIO_MASK_GATT_SERVICE_GOOGLE_CHANNEL)

#define		TL_AUDIO_DONGLE_ADPCM_HID						(DONGLE_PROJECT | TL_AUDIO_MASK_ADPCM_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL)

#define		TL_AUDIO_DONGLE_SBC_HID							(DONGLE_PROJECT | TL_AUDIO_MASK_SBC_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL)

#define		TL_AUDIO_DONGLE_ADPCM_HID_DONGLE_TO_STB			(DONGLE_PROJECT | TL_AUDIO_MASK_ADPCM_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL | TL_AUDIO_MASK_DONGLE_TO_STB)

#define		TL_AUDIO_DONGLE_SBC_HID_DONGLE_TO_STB			(DONGLE_PROJECT | TL_AUDIO_MASK_SBC_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL | TL_AUDIO_MASK_DONGLE_TO_STB)

#define		TL_AUDIO_DONGLE_MSBC_HID						(DONGLE_PROJECT | TL_AUDIO_MASK_MSBC_MODE | TL_AUDIO_MASK_HID_SERVICE_CHANNEL)






#endif /* AUDIO_COMMON_H_ */
