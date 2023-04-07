/********************************************************************************************************
 * @file	app_audio.c
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
#include "app_config.h"
#include "application/audio/tl_audio.h"
#include "application/audio/audio_config.h"
#include "stack/ble/ble.h"
#include "app.h"
#include "app_att.h"
#include "app_audio.h"
#include "vendor/common/blt_led.h"
#include "battery_check.h"

_attribute_data_retention_	u8		ui_mic_enable = 0;
_attribute_data_retention_	u8 		key_voice_press = 0;
_attribute_data_retention_	int     ui_mtu_size_exchange_req = 0;

#if (BLE_AUDIO_ENABLE)

#if BLE_DMIC_ENABLE
/**
 * @brief      the func serve to init dmic
 * @param[in]  none
 * @return     none
 */
	void dmic_gpio_reset (void)
	{
		gpio_set_func(GPIO_DMIC_BIAS, AS_GPIO);
		gpio_set_input_en(GPIO_DMIC_BIAS, 0);
		gpio_set_output_en(GPIO_DMIC_BIAS, 1);
		gpio_write(GPIO_DMIC_BIAS, 0);

		gpio_set_func(GPIO_DMIC_DI, AS_GPIO);
		gpio_set_input_en(GPIO_DMIC_DI, 1);
		gpio_set_output_en(GPIO_DMIC_DI, 0);
		gpio_setup_up_down_resistor(GPIO_DMIC_DI, PM_PIN_PULLDOWN_100K);

		gpio_set_func(GPIO_DMIC_CK, AS_GPIO);
		gpio_set_input_en(GPIO_DMIC_CK, 0);
		gpio_set_output_en(GPIO_DMIC_CK, 1);
		gpio_write(GPIO_DMIC_CK, 0);
	}
#else

	/**
	 * @brief      the func serve to init amic
	 * @param[in]  none
	 * @return     none
	 */
	void amic_gpio_reset (void)
	{
		gpio_set_func(GPIO_AMIC_BIAS, AS_GPIO);
		gpio_set_input_en(GPIO_AMIC_BIAS, 0);
		gpio_set_output_en(GPIO_AMIC_BIAS, 1);
		gpio_write(GPIO_AMIC_BIAS, 0);

		gpio_set_func(GPIO_AMIC_SP, AS_GPIO);
		gpio_set_input_en(GPIO_AMIC_SP, 0);
		gpio_set_output_en(GPIO_AMIC_SP, 1);
		gpio_write(GPIO_AMIC_SP, 0);

#if (MCU_CORE_TYPE == MCU_CORE_825x)
		gpio_set_func(GPIO_AMIC_SN, AS_GPIO);
		gpio_set_input_en(GPIO_AMIC_SN, 0);
		gpio_set_output_en(GPIO_AMIC_SN, 1);
		gpio_write(GPIO_AMIC_SN, 0);
#endif

	}
#endif

#if (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_GATT_TLEINK)					//GATT Telink

u32 	key_voice_pressTick = 0;
u32 	audio_stick = 0;
_attribute_data_retention_	u8  audio_start = 0;

extern u8		buffer_mic_pkt_wptr;
extern u8		buffer_mic_pkt_rptr;

/**
 * @brief      for open the audio and mtu size exchange
 * @param[in]  en   0:close the micphone  1:open the micphone
 * @return     none
 */
void ui_enable_mic (int en)
{
	ui_mic_enable = en;

#if BLE_DMIC_ENABLE
	//DMIC Bias output
	gpio_set_output_en (GPIO_DMIC_BIAS, en);
	gpio_write (GPIO_DMIC_BIAS, en);
#else
	//AMIC Bias output
	gpio_set_output_en (GPIO_AMIC_BIAS, en);
	#if (MCU_CORE_TYPE == MCU_CORE_827x)
		gpio_set_data_strength (GPIO_AMIC_BIAS, en);
	#endif
	gpio_write (GPIO_AMIC_BIAS, en);
#endif
	#if (BLT_APP_LED_ENABLE)
		extern const led_cfg_t led_cfg[];
		device_led_setup(led_cfg[en ? 1 : 2]);
	#endif

	if(en){  //audio on

		///////////////////// AUDIO initialization///////////////////
		//buffer_mic set must before audio_init !!!
		audio_config_mic_buf (  (u16*)buffer_mic, TL_MIC_BUFFER_SIZE);
		buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
		#if (BLE_DMIC_ENABLE)  //Dmic config
			gpio_set_func(GPIO_DMIC_DI, AS_DMIC);
			gpio_set_func(GPIO_DMIC_CK, AS_DMIC);
			gpio_set_input_en(GPIO_DMIC_DI, 1);
			audio_dmic_init(AUDIO_16K);

		#else
			#if (MCU_CORE_TYPE == MCU_CORE_827x)
				//Amic config
				audio_set_mute_pga(0);  ////enable audio need follow this step: 1 enable bias; 2 disable mute_pga;
				gpio_set_output_en(GPIO_AMIC_SP, 0);
				audio_amic_init(AUDIO_16K);							  //3 init; 4 delay about 17ms; 5 enable mute_pga.
			#elif (MCU_CORE_TYPE == MCU_CORE_825x)
				gpio_set_output_en(GPIO_AMIC_SP, 0);
				gpio_set_output_en(GPIO_AMIC_SN, 0);
				audio_amic_init(AUDIO_16K);
			#endif
		#endif

		#if (IIR_FILTER_ENABLE)
			//only used for debugging EQ Filter parameters, removed after mass production
			extern void filter_setting();
			filter_setting();
		#endif
	}
	else{  //audio off
		#if BLE_DMIC_ENABLE
			dmic_gpio_reset();
		#else
			#if (MCU_CORE_TYPE == MCU_CORE_827x)
				audio_codec_and_pga_disable();	//power off codec and pga
			#elif (MCU_CORE_TYPE == MCU_CORE_825x)
				adc_power_on_sar_adc(0);   //power off sar adc
			#endif
				amic_gpio_reset();
		#endif
		buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
	}
	#if ((!BLE_DMIC_ENABLE) && (BATT_CHECK_ENABLE))
		battery_set_detect_enable(!en);
	#endif

}


/**
 * @brief      this function is used to define what to do when voice key is pressed
 * @param[in]  none
 * @return     none
 */
void key_voice_is_press(void)
{
	if(ui_mic_enable){  //if voice on, voice off
		ui_enable_mic (0);
	}
	else{ //if voice not on, mark voice key press tick
		key_voice_press = 1;
		key_voice_pressTick = clock_time();
	}
}

/**
 * @brief      this function is used to define what to do when voice key is released
 * @param[in]  none
 * @return     none
 */
void key_voice_is_release(void)
{

}

/**
 * @brief      this function is used to check audio state
 * @param[in]  none
 * @return     none
 */
void audio_state_check(void)
{
	 //long press voice 1 second
	if(key_voice_press && !ui_mic_enable && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && \
		clock_time_exceed(key_voice_pressTick, 1000000)){

		voice_press_proc();
	}
}

/**
 * @brief      for open the audio and mtu size exchange
 * @param[in]  none
 * @return     none
 */
void voice_press_proc(void)
{
	key_voice_press = 0;
	audio_stick = clock_time()|1;
	ui_enable_mic (1);
	if(ui_mtu_size_exchange_req && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
		ui_mtu_size_exchange_req = 0;
		blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, 0x009e);
	}
}

/**
 * @brief      audio task in loop for encode and transmit encode data
 * @param[in]  none
 * @return     none
 */
void task_audio (void)
{
	static u32 audioProcTick = 0;
	if(clock_time_exceed(audioProcTick, 500)){
		audioProcTick = clock_time();
	}
	else{
		return;
	}

	///////////////////////////////////////////////////////////////
	proc_mic_encoder ();

	//////////////////////////////////////////////////////////////////
	if (blc_ll_getTxFifoNumber() < 9)
	{
		int *p = mic_encoder_data_buffer ();
		if (p)					//around 3.2 ms @16MHz clock
		{
			if( BLE_SUCCESS == blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,AUDIO_MIC_INPUT_DP_H, (u8*)p, ADPCM_PACKET_LEN) ){
				mic_encoder_data_read_ok();
			}
		}
	}
}

/**
 * @brief      This function serves to Request ConnParamUpdate
 * @param[in]  none
 * @return     none
 */
void blc_checkConnParamUpdate(void)
{
	extern u32 interval_update_tick;

	if(	 interval_update_tick && clock_time_exceed(interval_update_tick,5*1000*1000) && \
		 blc_ll_getCurrentState() == BLS_LINK_STATE_CONN &&  bls_ll_getConnectionInterval()!= 8 )
	{
		interval_update_tick = clock_time() | 1;
		bls_l2cap_requestConnParamUpdate (CONN_INTERVAL_10MS, CONN_INTERVAL_10MS, 99, CONN_TIMEOUT_4S);
	}
}

/**
 * @brief      audio proc in main loop
 * @param[in]  none
 * @return     none
 */
void proc_audio(void){
	//blc_checkConnParamUpdate();
	if(ui_mic_enable){
		#if (MCU_CORE_TYPE == MCU_CORE_827x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 17*1000))){// for 8278
			if(!audio_start)
			{
				audio_set_mute_pga(1);
			}
		#elif (MCU_CORE_TYPE == MCU_CORE_825x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 1*1000))){// for 8258
		#endif
			audio_start = 1;
			task_audio();
		}
	}
	else{
		audio_start = 0;
	}
}

#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_GATT_GOOGLE) 				// GATT GOOGLE

#include "application/audio/gl_audio.h"
_attribute_data_retention_ u32 	key_voice_pressTick = 0;

extern u8		buffer_mic_pkt_wptr;
extern u8		buffer_mic_pkt_rptr;
extern u16		app_audio_sync_serial;
extern u32		app_audio_timer;

_attribute_data_retention_ u8	audio_send_index = 0;
_attribute_data_retention_ u32	audio_stick = 0;
_attribute_data_retention_ u8	audio_start = 0;

/**
 * @brief      for open the audio and mtu size exchange
 * @param[in]  en   0:close the micphone  1:open the micphone
 * @return     none
 */
void ui_enable_mic (int en)
{
	ui_mic_enable = en;

#if BLE_DMIC_ENABLE
	//DMIC Bias output
	gpio_set_output_en (GPIO_DMIC_BIAS, en);
	gpio_write (GPIO_DMIC_BIAS, en);
#else
	//AMIC Bias output
	gpio_set_output_en (GPIO_AMIC_BIAS, en);
	#if (MCU_CORE_TYPE == MCU_CORE_827x)
		gpio_set_data_strength (GPIO_AMIC_BIAS, en);
	#endif
	gpio_write (GPIO_AMIC_BIAS, en);
#endif

#if (BLT_APP_LED_ENABLE)
	extern const led_cfg_t led_cfg[];
	device_led_setup(led_cfg[en ? 1 : 2]);
#endif

if(en){  //audio on
	app_audio_sync_serial = 0;
	bls_pm_setManualLatency(0);
	bls_pm_setSuspendMask(SUSPEND_DISABLE);
	app_audio_timer = clock_time() | 1;
	audio_stick = clock_time() | 1;
	extern u16	adpcm_serial_num;
	adpcm_serial_num = 0;
	audio_send_index = 0;

	///////////////////// AUDIO initialization///////////////////
	//buffer_mic set must before audio_init !!!
	audio_config_mic_buf ( (u16*)buffer_mic, TL_MIC_BUFFER_SIZE);
	buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
	#if (BLE_DMIC_ENABLE)  //Dmic config
		gpio_set_func(GPIO_DMIC_DI, AS_DMIC);
		gpio_set_func(GPIO_DMIC_CK, AS_DMIC);
		gpio_set_input_en(GPIO_DMIC_DI, 1);
		audio_dmic_init(AUDIO_16K);

	#else  //Amic config
		#if (MCU_CORE_TYPE == MCU_CORE_827x)
			//Amic config
			audio_set_mute_pga(0);  ////enable audio need follow this step: 1 enable bias; 2 disable mute_pga;
			gpio_set_output_en(GPIO_AMIC_SP, 0);
			audio_amic_init(AUDIO_16K);							  //3 init; 4 delay about 17ms; 5 enable mute_pga.
		#elif (MCU_CORE_TYPE == MCU_CORE_825x)
			gpio_set_output_en(GPIO_AMIC_SP, 0);
			gpio_set_output_en(GPIO_AMIC_SN, 0);
			audio_amic_init(AUDIO_16K);
		#endif
	#endif
		extern	u32 	latest_user_event_tick;
		latest_user_event_tick = clock_time() | 1;
	#if (IIR_FILTER_ENABLE)
		//only used for debugging EQ Filter parameters, removed after mass production
		extern void filter_setting();
		filter_setting();
	#endif
}
else{  //audio off
	#if BLE_DMIC_ENABLE
		dmic_gpio_reset();
	#else
		#if (MCU_CORE_TYPE == MCU_CORE_827x)
			audio_codec_and_pga_disable();	//power off codec and pga
		#elif (MCU_CORE_TYPE == MCU_CORE_825x)
			adc_power_on_sar_adc(0);   //power off sar adc
		#endif
			amic_gpio_reset();
	#endif
	audio_stick = 0;
	app_audio_timer = 0;
	audio_start = 0;
	buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
}
#if ((!BLE_DMIC_ENABLE) && (BATT_CHECK_ENABLE))
	battery_set_detect_enable(!en);
#endif

}

/**
 * @brief      this function is used to check audio state
 * @param[in]  none
 * @return     none
 */
void audio_state_check(void)
{

}

/**
 * @brief      this function is used to define what to do when voice key is pressed
 * @param[in]  none
 * @return     none
 */
void key_voice_is_press(void)
{
	if(!ui_mic_enable && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){

		if(app_audio_key_start(1) == APP_AUDIO_ENABLE){
			ui_enable_mic(1);
		}
	}
}

/**
 * @brief      this function is used to define what to do when voice key is released
 * @param[in]  none
 * @return     none
 */
void key_voice_is_release(void)
{
	extern u8 app_audio_key_flags;
	extern u8 htt_audio_model_key_press_flags;

	if(app_audio_key_flags & APP_AUDIO_KEY_FLAG_PRESS || htt_audio_model_key_press_flags)
	{
		app_audio_key_flags &= ~APP_AUDIO_KEY_FLAG_PRESS;

		if(ui_mic_enable){
			if(app_audio_key_start(0) == APP_AUDIO_DISABLE){
				ui_enable_mic(0);
			}
		}
	}
}

/**
 * @brief      audio task in loop for encode and transmit encode data
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_ void task_audio (void)
{
	static u32 audioProcTick = 0;
	if(clock_time_exceed(audioProcTick, 500)){
		audioProcTick = clock_time();
	}
	else{
		return;
	}

	///////////////////////////////////////////////////////////////

	if(app_audio_timeout_proc()){
		return;
	}
	proc_mic_encoder ();
	u8 audio_send_length;
	//////////////////////////////////////////////////////////////////
	if (blc_ll_getTxFifoNumber() < 7)
	{
		int *p = mic_encoder_data_buffer ();
		if(p)
		{
			for(u8 i=0; i<7; i++)
			{
				if(audio_send_index < 6)
				{
					audio_send_length = 20;
				}
				else if(audio_send_index == 6)
				{
					audio_send_length = 14;
				}
				else
				{
					audio_send_length = 0;
				}
				if(BLE_SUCCESS == blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,AUDIO_GOOGLE_RX_DP_H, (u8*)p+audio_send_index*20, audio_send_length))
				{
					audio_send_index++;
				}
				else
				{
					return ;
				}
				if(audio_send_index == 7)
				{
					audio_send_index = 0;
					mic_encoder_data_read_ok();
				}
			}
		}
	}
}

/**
 * @brief      audio protocol in main loop for audio process work normal
 * @param[in]  none
 * @return     none
 */
void proc_audio(void){

	if(ui_mic_enable){
		#if (MCU_CORE_TYPE == MCU_CORE_827x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 17*1000))){// for 8278
			if(!audio_start)
			{
				audio_set_mute_pga(1);
			}
		#elif (MCU_CORE_TYPE == MCU_CORE_825x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 1*1000))){// for 8258
		#endif
			audio_start = 1;
			task_audio();
		}
	}
	else{
		audio_start = 0;
	}

}

#elif ((TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_HID) || (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_HID_DONGLE_TO_STB))	//HID Service,ADPCM

u8		audio_start_status = 0;
u8		audio_end_status = 0;
u8 		audio_send_idx = 0;
u32 	key_voice_pressTick = 0;
u32 	audio_stick = 0;
u32 	audio_end_tick = 0;

_attribute_data_retention_	u8  audio_start = 0;
_attribute_data_retention_	u8  audio_bt_status = 0;

extern u8		buffer_mic_pkt_wptr;
extern u8		buffer_mic_pkt_rptr;

/**
 * @brief      for open the audio and mtu size exchange
 * @param[in]  en   0:close the micphone  1:open the micphone
 * @return     none
 */
void ui_enable_mic (int en)
{
	ui_mic_enable = en;

#if BLE_DMIC_ENABLE
	//DMIC Bias output
	gpio_set_output_en (GPIO_DMIC_BIAS, en);
	gpio_write (GPIO_DMIC_BIAS, en);
#else
	//AMIC Bias output
	gpio_set_output_en (GPIO_AMIC_BIAS, en);
	#if (MCU_CORE_TYPE == MCU_CORE_827x)
		gpio_set_data_strength (GPIO_AMIC_BIAS, en);
	#endif
	gpio_write (GPIO_AMIC_BIAS, en);
#endif
	#if (BLT_APP_LED_ENABLE)
		extern const led_cfg_t led_cfg[];
		device_led_setup(led_cfg[en ? 1 : 2]);
	#endif

	if(en){  //audio on
		audio_mic_param_init();
		audio_send_idx = 0;
		audio_end_tick = 0;
		///////////////////// AUDIO initialization///////////////////
		//buffer_mic set must before audio_init !!!
		audio_config_mic_buf (  (u16*)buffer_mic, TL_MIC_BUFFER_SIZE);
		buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
		#if (BLE_DMIC_ENABLE)  //Dmic config
			gpio_set_func(GPIO_DMIC_DI, AS_DMIC);
			gpio_set_func(GPIO_DMIC_CK, AS_DMIC);
			gpio_set_input_en(GPIO_DMIC_DI, 1);
			audio_dmic_init(AUDIO_16K);

		#else  //Amic config
			#if (MCU_CORE_TYPE == MCU_CORE_827x)
				//Amic config
				audio_set_mute_pga(0);  ////enable audio need follow this step: 1 enable bias; 2 disable mute_pga;
				gpio_set_output_en(GPIO_AMIC_SP, 0);
				audio_amic_init(AUDIO_16K);							  //3 init; 4 delay about 17ms; 5 enable mute_pga.
			#elif (MCU_CORE_TYPE == MCU_CORE_825x)
				gpio_set_output_en(GPIO_AMIC_SP, 0);
				gpio_set_output_en(GPIO_AMIC_SN, 0);
				audio_amic_init(AUDIO_16K);
			#endif
		#endif

		#if (IIR_FILTER_ENABLE)
			//only used for debugging EQ Filter parameters, removed after mass production
			extern void filter_setting();
			filter_setting();
		#endif
	}
	else{  //audio off
		#if BLE_DMIC_ENABLE
			dmic_gpio_reset();
		#else
			#if (MCU_CORE_TYPE == MCU_CORE_827x)
				audio_codec_and_pga_disable();	//power off codec and pga
			#elif (MCU_CORE_TYPE == MCU_CORE_825x)
				adc_power_on_sar_adc(0);   //power off sar adc
			#endif
				amic_gpio_reset();
		#endif
		buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
		audio_start_status = 0;
		audio_end_status = 0;
		audio_stick = 0;
		audio_end_tick = 0;
		audio_bt_status = APP_AUDIO_BT_CLOSE;
	}
	#if ((!BLE_DMIC_ENABLE) && (BATT_CHECK_ENABLE))
		battery_set_detect_enable(!en);
	#endif

}

/**
 * @brief      this function is used to define what to do when voice key is pressed
 * @param[in]  none
 * @return     none
 */
void key_voice_is_press(void)
{
	if(!key_voice_press)
	{
		key_voice_pressTick = clock_time() | 1;//first press tick
		key_voice_press = 1;
		if(!ui_mic_enable && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN)
		{
			if(audio_start_status == 0)
			{
				u8 value[20]={0x99, 0x99, 0x99, 0x21, };// AC_SEARCH
				if(blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
					audio_start_status = 2; //push notify fail
				}
				else{
					audio_start_status = 1; //push notify success
				}
			}
		}
	}
}

/**
 * @brief      this function is used to define what to do when voice key is released
 * @param[in]  none
 * @return     none
 */
void key_voice_is_release(void)
{
	if(key_voice_press)
	{
		key_voice_pressTick = 0;
		key_voice_press = 0;
		if(ui_mic_enable && (audio_end_status == 0)){
			u8 value[20]={0x99, 0x99, 0x99, 0x24, };// AC_SEARCH
			if (blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
				audio_end_status = 2;	//push notify fail
			}
			else{
				audio_end_status = 1;	//push notify success
			}
			audio_end_tick = clock_time() | 1;
		}
	}
}

/**
 * @brief      this function is used to check audio state
 * @param[in]  none
 * @return     none
 */
void audio_state_check(void)
{
	if (audio_start_status == 2){//press
		u8 value[20]={0x99, 0x99, 0x99, 0x21, };// AC_SEARCH
		if(!blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
			audio_start_status = 2; //push notify success
		}
	}
	if(audio_end_status == 2){//release
		u8 value[20]={0x99, 0x99, 0x99, 0x24, };// AC_SEARCH
		if (!blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
			audio_end_status = 0;	//push notify success
		}
	}
}

/**
 * @brief      this function is call back function of audio measurement from server to client
 * @param[in]  p:data pointer.
 * @return     will always return 0
 */
int server2client_auido_proc(void* p)
{
	rf_packet_att_data_t *pw = (rf_packet_att_data_t *)p;

	if (pw->dat[0] == APP_AUDIO_BT_OPEN){  //audio start
		if(!ui_mic_enable){
			audio_bt_status = APP_AUDIO_BT_OPEN;
		}

	}
	else if(pw->dat[0] == APP_AUDIO_BT_CLOSE){
		audio_bt_status = APP_AUDIO_BT_CLOSE;
	}
	return 0;
}

/**
 * @brief      audio task in loop for encode and transmit encode data
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_ void task_audio (void)
{
	static u32 audioProcTick = 0;
	if(clock_time_exceed(audioProcTick, 500)){
		audioProcTick = clock_time();
	}
	else{
		return;
	}

	///////////////////////////////////////////////////////////////


	proc_mic_encoder ();

	//////////////////////////////////////////////////////////////////
	if (blc_ll_getTxFifoNumber() < 8 + audio_send_idx)//8 + audio_send_idx
	{
		int *p = mic_encoder_data_buffer ();
		if (p)					//around 3.2 ms @16MHz clock
		{
			u8 i =0;
			u8 handle;
			u8 *buff;
			for(i=0;i<12;i++){
				handle = HID_AUDIO_REPORT_INPUT_FIRST_DP_H + (audio_send_idx%3)*4;//+ (audio_send_idx%3)*4
				buff = ((u8*)(p))+20*audio_send_idx;
				if(BLE_SUCCESS == blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,handle, buff, 20)){
					audio_send_idx++;
				}
				if(audio_send_idx == 6){
					audio_send_idx = 0;
					buffer_mic_pkt_rptr++;
					return;
				}
			}
		}
	}
}

/**
 * @brief      audio proc in main loop
 * @param[in]  none
 * @return     none
 */
void proc_audio(void){
	if(!ui_mic_enable && key_voice_pressTick && clock_time_exceed(key_voice_pressTick,3*1000*1000)){
		key_voice_pressTick = 0;
		audio_start_status = 0;
		audio_end_status = 0;
		return;
	}
	if(ui_mic_enable && audio_stick && clock_time_exceed(audio_stick, 100*1000*1000)){
		ui_enable_mic (0);
		return;
	}
	if(!key_voice_press && ui_mic_enable && audio_end_tick && clock_time_exceed(audio_end_tick, 200*1000)){
		ui_enable_mic (0);
		return;
	}
	if((!ui_mic_enable) && audio_bt_status == APP_AUDIO_BT_OPEN){
		audio_stick = clock_time()|1;
		ui_enable_mic (1);
		if(ui_mtu_size_exchange_req && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
			ui_mtu_size_exchange_req = 0;
			blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, 0x009e);
		}
	}
	if(ui_mic_enable){
		#if (MCU_CORE_TYPE == MCU_CORE_827x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 17*1000))){// for 8278
			if(!audio_start)
			{
				audio_set_mute_pga(1);
			}
		#elif (MCU_CORE_TYPE == MCU_CORE_825x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 1*1000))){// for 8258
		#endif
			audio_start = 1;
			task_audio();
		}
	}
	else{
		audio_start = 0;
	}

}

#elif ((TL_AUDIO_MODE == TL_AUDIO_RCU_SBC_HID) || (TL_AUDIO_MODE == TL_AUDIO_RCU_SBC_HID_DONGLE_TO_STB))	//HID Service, SBC


u8		audio_start_status = 0;
u8		audio_end_status = 0;
u8		audio_send_idx = 0;

u32 	key_voice_pressTick = 0;
u32 	audio_stick = 0;
u32		audio_end_tick = 0;

_attribute_data_retention_	u8  audio_start = 0;
_attribute_data_retention_	u8  audio_bt_status = 0;

extern u8		buffer_mic_pkt_wptr;
extern u8		buffer_mic_pkt_rptr;

/**
 * @brief      for open the audio and mtu size exchange
 * @param[in]  en   0:close the micphone  1:open the micphone
 * @return     none
 */
void ui_enable_mic (int en)
{
	ui_mic_enable = en;

#if BLE_DMIC_ENABLE
	//DMIC Bias output
	gpio_set_output_en (GPIO_DMIC_BIAS, en);
	gpio_write (GPIO_DMIC_BIAS, en);
#else
	//AMIC Bias output
	gpio_set_output_en (GPIO_AMIC_BIAS, en);
	#if (MCU_CORE_TYPE == MCU_CORE_827x)
		gpio_set_data_strength (GPIO_AMIC_BIAS, en);
	#endif
	gpio_write (GPIO_AMIC_BIAS, en);
#endif
	#if (BLT_APP_LED_ENABLE)
		extern const led_cfg_t led_cfg[];
		device_led_setup(led_cfg[en ? 1 : 2]);
	#endif

	if(en){  //audio on
		audio_mic_param_init();
		audio_send_idx = 0;
		audio_end_tick = 0;
		///////////////////// AUDIO initialization///////////////////
		//buffer_mic set must before audio_init !!!
		audio_config_mic_buf (  (u16*)buffer_mic, TL_MIC_BUFFER_SIZE);
		buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
		#if (BLE_DMIC_ENABLE)  //Dmic config
			gpio_set_func(GPIO_DMIC_DI, AS_DMIC);
			gpio_set_func(GPIO_DMIC_CK, AS_DMIC);
			gpio_set_input_en(GPIO_DMIC_DI, 1);
			audio_dmic_init(AUDIO_16K);

		#else  //Amic config
			#if (MCU_CORE_TYPE == MCU_CORE_827x)
				//Amic config
				audio_set_mute_pga(0);  ////enable audio need follow this step: 1 enable bias; 2 disable mute_pga;
				gpio_set_output_en(GPIO_AMIC_SP, 0);
				audio_amic_init(AUDIO_16K);							  //3 init; 4 delay about 17ms; 5 enable mute_pga.
			#elif (MCU_CORE_TYPE == MCU_CORE_825x)
				gpio_set_output_en(GPIO_AMIC_SP, 0);
				gpio_set_output_en(GPIO_AMIC_SN, 0);
				audio_amic_init(AUDIO_16K);
			#endif
		#endif

		#if (IIR_FILTER_ENABLE)
			//only used for debugging EQ Filter parameters, removed after mass production
			extern void filter_setting();
			filter_setting();
		#endif
	}
	else{  //audio off
		#if BLE_DMIC_ENABLE
			dmic_gpio_reset();
		#else
			#if (MCU_CORE_TYPE == MCU_CORE_827x)
				audio_codec_and_pga_disable();	//power off codec and pga
			#elif (MCU_CORE_TYPE == MCU_CORE_825x)
				adc_power_on_sar_adc(0);   //power off sar adc
			#endif
				amic_gpio_reset();
		#endif
		buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
		audio_start_status = 0;
		audio_end_status = 0;
		audio_stick = 0;
		audio_end_tick = 0;
		audio_bt_status = APP_AUDIO_BT_CLOSE;
	}
	#if ((!BLE_DMIC_ENABLE) && (BATT_CHECK_ENABLE))
		battery_set_detect_enable(!en);
	#endif

}

/**
 * @brief      this function is used to define what to do when voice key is pressed
 * @param[in]  none
 * @return     none
 */
void key_voice_is_press(void)
{

	if(!ui_mic_enable && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
		if(audio_start_status == 0){

			u8 value[20]={0x99, 0x99, 0x99, 0x31, };// AC_SEARCH
			if(blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
				audio_start_status = 2; //push notify fail
			}
			else{
				audio_start_status = 1; //push notify success
			}
		}
	}

}

/**
 * @brief      this function is used to define what to do when voice key is released
 * @param[in]  none
 * @return     none
 */
void key_voice_is_release(void)
{
	if(ui_mic_enable && (audio_end_status == 0)){
		u8 value[20]={0x99, 0x99, 0x99, 0x34, };// AC_SEARCH
		if (blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
			audio_end_status = 2;	//push notify fail
		}
		else{
			audio_end_status = 1;	//push notify success
		}
		audio_end_tick = clock_time() | 1;
	}
}

/**
 * @brief      this function is used to check audio state
 * @param[in]  none
 * @return     none
 */
void audio_state_check(void)
{
	if (audio_start_status == 2){//press
		u8 value[20]={0x99, 0x99, 0x99, 0x31, };// AC_SEARCH
		if(!blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
			audio_start_status = 2; //push notify success
		}
	}
	if(audio_end_status == 2){//release
		u8 value[20]={0x99, 0x99, 0x99, 0x34, };// AC_SEARCH
		if (!blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
			audio_end_status = 0;	//push notify success
		}
	}
}

/**
 * @brief      this function is call back function of audio measurement from server to client
 * @param[in]  p:data pointer.
 * @return     will always return 0
 */
int server2client_auido_proc(void* p)
{
	rf_packet_att_data_t *pw = (rf_packet_att_data_t *)p;

	if (pw->dat[0] == APP_AUDIO_BT_OPEN){  //audio start
		if(!ui_mic_enable){
			audio_bt_status = APP_AUDIO_BT_OPEN;
		}

	}
	else if(pw->dat[0] == APP_AUDIO_BT_CLOSE){
		audio_bt_status = APP_AUDIO_BT_CLOSE;
	}
	return 0;
}

/**
 * @brief      audio task in loop for encode and transmit encode data
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_ void task_audio (void)
{
	static u32 audioProcTick = 0;
	if(clock_time_exceed(audioProcTick, 500)){
		audioProcTick = clock_time();
	}
	else{
		return;
	}

	///////////////////////////////////////////////////////////////


	proc_mic_encoder ();

	//////////////////////////////////////////////////////////////////
	if (blc_ll_getTxFifoNumber() < 12)//8 + audio_send_idx
	{
		int *p = mic_encoder_data_buffer ();
		if (p)					//around 3.2 ms @16MHz clock
		{
			u8 i =0;
			u8 handle;
			for(i=0;i<3;i++){
				handle = HID_AUDIO_REPORT_INPUT_FIRST_DP_H + (audio_send_idx%3)*4;//+ (audio_send_idx%3)*4
				if(BLE_SUCCESS == blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,handle, (u8*)p +3, 20)){
					audio_send_idx++;
					buffer_mic_pkt_rptr++;
					if(audio_send_idx == 3){
						audio_send_idx = 0;
					}
					return;
				}
			}
		}
	}
}

/**
 * @brief      audio protocol in main loop for audio process work normal
 * @param[in]  none
 * @return     none
 */
void proc_audio(void){
	if(!ui_mic_enable && key_voice_pressTick && clock_time_exceed(key_voice_pressTick,3*1000*1000)){
		key_voice_pressTick = 0;
		audio_start_status = 0;
		audio_end_status = 0;
		return;
	}
	if(ui_mic_enable && audio_stick && clock_time_exceed(audio_stick, 100*1000*1000)){
		ui_enable_mic (0);
		return;
	}
	if(!key_voice_press && ui_mic_enable && audio_end_tick && clock_time_exceed(audio_end_tick, 200*1000)){
		ui_enable_mic (0);
		return;
	}
	if((!ui_mic_enable) && audio_bt_status == APP_AUDIO_BT_OPEN){
		audio_stick = clock_time()|1;
		ui_enable_mic (1);
		if(ui_mtu_size_exchange_req && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
			ui_mtu_size_exchange_req = 0;
			blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, 0x009e);
		}
	}
	if(ui_mic_enable){
		#if (MCU_CORE_TYPE == MCU_CORE_827x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 17*1000))){// for 8278
			if(!audio_start)
			{
				audio_set_mute_pga(1);
			}
		#elif (MCU_CORE_TYPE == MCU_CORE_825x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 1*1000))){// for 8258
		#endif
			audio_start = 1;
			task_audio();
		}
	}
	else{
		audio_start = 0;
	}

}

#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_MSBC_HID)					//HID Service, MSBC


u8		audio_start_status = 0;
u8		audio_end_status = 0;
u8		audio_send_idx = 0;

u32		audio_end_tick = 0;
u32 	audio_stick = 0;
u32 	key_voice_pressTick = 0;

_attribute_data_retention_	u8  audio_start = 0;
_attribute_data_retention_	u8  audio_bt_status = 0;

extern u8		buffer_mic_pkt_wptr;
extern u8		buffer_mic_pkt_rptr;

/**
 * @brief      for open the audio and mtu size exchange
 * @param[in]  en   0:close the micphone  1:open the micphone
 * @return     none
 */
void ui_enable_mic (int en)
{
	ui_mic_enable = en;

#if BLE_DMIC_ENABLE
	//DMIC Bias output
	gpio_set_output_en (GPIO_DMIC_BIAS, en);
	gpio_write (GPIO_DMIC_BIAS, en);
#else
	//AMIC Bias output
	gpio_set_output_en (GPIO_AMIC_BIAS, en);
	#if (MCU_CORE_TYPE == MCU_CORE_827x)
		gpio_set_data_strength (GPIO_AMIC_BIAS, en);
	#endif
	gpio_write (GPIO_AMIC_BIAS, en);
#endif
	#if (BLT_APP_LED_ENABLE)
		extern const led_cfg_t led_cfg[];
		device_led_setup(led_cfg[en ? 1 : 2]);
	#endif

	if(en){  //audio on
		audio_mic_param_init();
		audio_send_idx = 0;
		audio_end_tick = 0;
		///////////////////// AUDIO initialization///////////////////
		//buffer_mic set must before audio_init !!!
		audio_config_mic_buf (  (u16*)buffer_mic, TL_MIC_BUFFER_SIZE);
		buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
		#if (BLE_DMIC_ENABLE)  //Dmic config
			gpio_set_func(GPIO_DMIC_DI, AS_DMIC);
			gpio_set_func(GPIO_DMIC_CK, AS_DMIC);
			gpio_set_input_en(GPIO_DMIC_DI, 1);
			audio_dmic_init(AUDIO_16K);

		#else  //Amic config
			#if (MCU_CORE_TYPE == MCU_CORE_827x)
				//Amic config
				audio_set_mute_pga(0);  ////enable audio need follow this step: 1 enable bias; 2 disable mute_pga;
				gpio_set_output_en(GPIO_AMIC_SP, 0);
				audio_amic_init(AUDIO_16K);							  //3 init; 4 delay about 17ms; 5 enable mute_pga.
			#elif (MCU_CORE_TYPE == MCU_CORE_825x)
				gpio_set_output_en(GPIO_AMIC_SP, 0);
				gpio_set_output_en(GPIO_AMIC_SN, 0);
				audio_amic_init(AUDIO_16K);
			#endif
		#endif

		#if (IIR_FILTER_ENABLE)
			//only used for debugging EQ Filter parameters, removed after mass production
			extern void filter_setting();
			filter_setting();
		#endif
	}
	else{  //audio off
		#if BLE_DMIC_ENABLE
			dmic_gpio_reset();
		#else
			#if (MCU_CORE_TYPE == MCU_CORE_827x)
				audio_codec_and_pga_disable();	//power off codec and pga
			#elif (MCU_CORE_TYPE == MCU_CORE_825x)
				adc_power_on_sar_adc(0);   //power off sar adc
			#endif
				amic_gpio_reset();
		#endif
		buffer_mic_pkt_rptr = buffer_mic_pkt_wptr = 0;
		audio_start_status = 0;
		audio_end_status = 0;
		audio_stick = 0;
		audio_end_tick = 0;
		audio_bt_status = APP_AUDIO_BT_CLOSE;
	}
	#if ((!BLE_DMIC_ENABLE) && (BATT_CHECK_ENABLE))
		battery_set_detect_enable(!en);
	#endif

}

/**
 * @brief      this function is used to define what to do when voice key is pressed
 * @param[in]  none
 * @return     none
 */
void key_voice_is_press(void)
{

	if(!ui_mic_enable && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
		if(audio_start_status == 0){

			u8 value[20]={0x99, 0x99, 0x99, 0x31, };// AC_SEARCH
			if(blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
				audio_start_status = 2; //push notify fail
			}
			else{
				audio_start_status = 1; //push notify success
			}
		}
	}
}

/**
 * @brief      this function is used to define what to do when voice key is released
 * @param[in]  none
 * @return     none
 */
void key_voice_is_release(void)
{
	if(ui_mic_enable && (audio_end_status == 0)){
		u8 value[20]={0x99, 0x99, 0x99, 0x34, };// AC_SEARCH
		if (blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
			audio_end_status = 2;	//push notify fail
		}
		else{
			audio_end_status = 1;	//push notify success
		}
		audio_end_tick = clock_time() | 1;
	}
}

/**
 * @brief      this function is used to check audio state
 * @param[in]  none
 * @return     none
 */
void audio_state_check(void)
{
	if (audio_start_status == 2){//press
		u8 value[20]={0x99, 0x99, 0x99, 0x31, };// AC_SEARCH
		if(!blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
			audio_start_status = 2; //push notify success
		}
	}
	if(audio_end_status == 2){//release
		u8 value[20]={0x99, 0x99, 0x99, 0x34, };// AC_SEARCH
		if (!blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,HID_CONSUME_REPORT_INPUT_DP_H, value, 20)){
			audio_end_status = 0;	//push notify success
		}
	}
}

/**
 * @brief      this function is call back function of audio measurement from server to client
 * @param[in]  p:data pointer.
 * @return     will always return 0
 */
int server2client_auido_proc(void* p)
{
	rf_packet_att_data_t *pw = (rf_packet_att_data_t *)p;

	if (pw->dat[0] == APP_AUDIO_BT_OPEN){  //audio start
		if(!ui_mic_enable){
			audio_bt_status = APP_AUDIO_BT_OPEN;
		}

	}
	else if(pw->dat[0] == APP_AUDIO_BT_CLOSE){
		audio_bt_status = APP_AUDIO_BT_CLOSE;
	}
	return 0;
}

/**
 * @brief      audio task in loop for encode and transmit encode data
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_ void task_audio (void)
{
	///////////////////////////////////////////////////////////////
	proc_mic_encoder ();

	//////////////////////////////////////////////////////////////////
	if (blc_ll_getTxFifoNumber() < 12)//8 + audio_send_idx
	{
		int *p = mic_encoder_data_buffer ();
		if (p)					//around 3.2 ms @16MHz clock
		{
			u8 i =0;
			u8 handle;
			for(i=0;i<3;i++){
				handle = HID_AUDIO_REPORT_INPUT_FIRST_DP_H + (audio_send_idx%3)*4;//+ (audio_send_idx%3)*4
				if(BLE_SUCCESS == blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,handle, (u8*)p, ADPCM_PACKET_LEN)){
					audio_send_idx++;
					buffer_mic_pkt_rptr++;
					if(audio_send_idx == 3){
						audio_send_idx = 0;
					}
					return;
				}
			}
		}
	}
}

/**
 * @brief      audio proc in main loop
 * @param[in]  none
 * @return     none
 */
void proc_audio(void){
	if(!ui_mic_enable && key_voice_pressTick && clock_time_exceed(key_voice_pressTick,3*1000*1000)){
		key_voice_pressTick = 0;
		audio_start_status = 0;
		audio_end_status = 0;
		return;
	}
	if(ui_mic_enable && audio_stick && clock_time_exceed(audio_stick, 100*1000*1000)){
		ui_enable_mic (0);
		return;
	}
	if(!key_voice_press && ui_mic_enable && audio_end_tick && clock_time_exceed(audio_end_tick, 200*1000)){
		ui_enable_mic (0);
		return;
	}
	if((!ui_mic_enable) && audio_bt_status == APP_AUDIO_BT_OPEN){
		audio_stick = clock_time()|1;
		ui_enable_mic (1);
		if(ui_mtu_size_exchange_req && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
			ui_mtu_size_exchange_req = 0;
			blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, 0x009e);
		}
	}
	if(ui_mic_enable){
		#if (MCU_CORE_TYPE == MCU_CORE_827x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 17*1000))){// for 8278
			if(!audio_start)
			{
				audio_set_mute_pga(1);
			}
		#elif (MCU_CORE_TYPE == MCU_CORE_825x)
		if(audio_start || (audio_stick && clock_time_exceed(audio_stick, 1*1000))){// for 8258
		#endif
			audio_start = 1;
			task_audio();
		}
	}
	else{
		audio_start = 0;
	}

}
#else

#endif
#endif

