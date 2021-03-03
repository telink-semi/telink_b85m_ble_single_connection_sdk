/********************************************************************************************************
 * @file     audio_config.h
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     2020-5-14
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


#include  "application/audio/audio_common.h"
#include  "tl_common.h"


#ifndef		TL_AUDIO_MODE
#define		TL_AUDIO_MODE									AUDIO_DISABLE
#endif

#if (TL_AUDIO_MODE & RCU_PROJECT)						//RCU
	#if (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_GATT_TLEINK)
		#define	ADPCM_PACKET_LEN				128
		#define TL_MIC_ADPCM_UNIT_SIZE			248
		#define	TL_MIC_BUFFER_SIZE				992
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_GATT_GOOGLE)
		#define	ADPCM_PACKET_LEN				136		//(128+6+2)
		#define TL_MIC_ADPCM_UNIT_SIZE			256
		#define	TL_MIC_BUFFER_SIZE				1024
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_HID_DONGLE_TO_STB)
		#define	ADPCM_PACKET_LEN				120
		#define TL_MIC_ADPCM_UNIT_SIZE			240
		#define	TL_MIC_BUFFER_SIZE				960
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_HID)
		#define	ADPCM_PACKET_LEN				120
		#define TL_MIC_ADPCM_UNIT_SIZE			240
		#define	TL_MIC_BUFFER_SIZE				960
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_SBC_HID_DONGLE_TO_STB)
		#define	ADPCM_PACKET_LEN				20
		#define MIC_SHORT_DEC_SIZE				80
		#define	TL_MIC_BUFFER_SIZE				320
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_SBC_HID)
		#define	ADPCM_PACKET_LEN				20
		#define MIC_SHORT_DEC_SIZE				80
		#define	TL_MIC_BUFFER_SIZE				320
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_MSBC_HID)
		#define	ADPCM_PACKET_LEN				57
		#define MIC_SHORT_DEC_SIZE				120
		#define	TL_MIC_BUFFER_SIZE				480
	#else

	#endif

#elif (TL_AUDIO_MODE & DONGLE_PROJECT)					//Dongle

	#if (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_TELINK)
		#define	MIC_ADPCM_FRAME_SIZE		128
		#define	MIC_SHORT_DEC_SIZE			248
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE)
		#define	MIC_ADPCM_FRAME_SIZE		136 		//128+6+2
		#define	MIC_SHORT_DEC_SIZE			256
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_HID_DONGLE_TO_STB)
		#define	MIC_ADPCM_FRAME_SIZE		120
		#define	MIC_SHORT_DEC_SIZE			240
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_HID)
		#define	MIC_ADPCM_FRAME_SIZE		120
		#define	MIC_SHORT_DEC_SIZE			240
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID_DONGLE_TO_STB)
		#define	MIC_ADPCM_FRAME_SIZE		20
		#define	MIC_SHORT_DEC_SIZE			80

	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID)
		#define	MIC_ADPCM_FRAME_SIZE		20
		#define	MIC_SHORT_DEC_SIZE			80
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_MSBC_HID)
		#define	MIC_ADPCM_FRAME_SIZE		57
		#define	MIC_SHORT_DEC_SIZE			120
	#else

	#endif

	#if ((TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID) || (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID_DONGLE_TO_STB) || (TL_AUDIO_MODE == TL_AUDIO_DONGLE_MSBC_HID))
		#define 	MIC_OPEN_FROM_RCU		0x31999999
		#define 	MIC_OPEN_TO_STB			0x32999999
		#define 	MIC_CLOSE_FROM_RCU		0x34999999
		#define 	MIC_CLOSE_TO_STB		0x35999999
	#else
		#define 	MIC_OPEN_FROM_RCU		0x21999999
		#define 	MIC_OPEN_TO_STB			0x22999999
		#define 	MIC_CLOSE_FROM_RCU		0x24999999
		#define 	MIC_CLOSE_TO_STB		0x25999999
	#endif
#else

#endif
