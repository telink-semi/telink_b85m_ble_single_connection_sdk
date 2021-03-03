/********************************************************************************************************
 * @file	 button.c
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     Sep. 18, 2018
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "application/usbstd/usbkeycode.h"

#include "app.h"
#include "app_att.h"


#if (UI_BUTTON_ENABLE)



/////////////////////////////////////////////////////////////////////
	#define MAX_BTN_SIZE			2
	#define BTN_VALID_LEVEL			0

	#define USER_BTN_1				0x01
	#define USER_BTN_2				0x02

	_attribute_data_retention_	u32 ctrl_btn[] = {SW1_GPIO, SW2_GPIO};
	_attribute_data_retention_	u8 btn_map[MAX_BTN_SIZE] = {USER_BTN_1, USER_BTN_2};



	_attribute_data_retention_	int button_not_released = 0;

	_attribute_data_retention_	static int button0_press_flag;
	_attribute_data_retention_	static u32 button0_press_tick;
	_attribute_data_retention_	static int button1_press_flag;
	_attribute_data_retention_	static u32 button1_press_tick;

	_attribute_data_retention_	static int consumer_report = 0;

	typedef	struct{
		u8 	cnt;				//count button num
		u8 	btn_press;
		u8 	keycode[MAX_BTN_SIZE];			//6 btn
	}vc_data_t;
	_attribute_data_retention_	vc_data_t vc_event;

	typedef struct{
		u8  btn_history[4];		//vc history btn save
		u8  btn_filter_last;
		u8	btn_not_release;
		u8 	btn_new;					//new btn  flag
	}btn_status_t;
	_attribute_data_retention_	btn_status_t 	btn_status;


	u8 btn_debounce_filter(u8 *btn_v)
	{
		u8 change = 0;

		for(int i=3; i>0; i--){
			btn_status.btn_history[i] = btn_status.btn_history[i-1];
		}
		btn_status.btn_history[0] = *btn_v;

		if(  btn_status.btn_history[0] == btn_status.btn_history[1] && btn_status.btn_history[1] == btn_status.btn_history[2] && \
			btn_status.btn_history[0] != btn_status.btn_filter_last ){
			change = 1;

			btn_status.btn_filter_last = btn_status.btn_history[0];
		}

		return change;
	}

	u8 vc_detect_button(int read_key)
	{
		u8 btn_changed, i;
		memset(&vc_event,0,sizeof(vc_data_t));			//clear vc_event
		//vc_event.btn_press = 0;

		for(i=0; i<MAX_BTN_SIZE; i++){
			if(BTN_VALID_LEVEL != !gpio_read(ctrl_btn[i])){
				vc_event.btn_press |= BIT(i);
			}
		}

		btn_changed = btn_debounce_filter(&vc_event.btn_press);


		if(btn_changed && read_key){
			for(i=0; i<MAX_BTN_SIZE; i++){
				if(vc_event.btn_press & BIT(i)){
					vc_event.keycode[vc_event.cnt++] = btn_map[i];
				}
			}

			return 1;
		}

		return 0;
	}

	/**
	 * @brief		this function is used to detect if button pressed or released.
	 * @param[in]	e - event type when this function is triggered by LinkLayer event
	 * @param[in]	p - event callback data pointer for when this function is triggered by LinkLayer event
	 * @param[in]	n - event callback data length when this function is triggered by LinkLayer event
	 * @return      none
	 */
	void proc_button (u8 e, u8 *p, int n)
	{
		int det_key = vc_detect_button (1);

		if (det_key)  //key change: press or release
		{
			extern u32 latest_user_event_tick;
			latest_user_event_tick = clock_time();

			u8 key0 = vc_event.keycode[0];
//			u8 key1 = vc_event.keycode[1];

			button_not_released = 1;

			if(vc_event.cnt == 2)  //two key press
			{

			}
			else if(vc_event.cnt == 1) //one key press
			{
				if(key0 == USER_BTN_1)
				{
					button0_press_flag = 1;
					button0_press_tick = clock_time();

					//send "Vol+"
					u16 consumer_key = MKEY_VOL_UP;
					blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
					consumer_report = 1;

				}
				else if(key0 == USER_BTN_2)
				{
					button1_press_flag = 1;
					button1_press_tick = clock_time();

					//send "Vol-"
					u16 consumer_key = MKEY_VOL_DN;
					blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
					consumer_report = 1;
				}
			}
			else{  //release
				button_not_released = 0;

				button0_press_flag = 0;
				button1_press_flag = 0;

				//send release Vol+/Vol-
				if(consumer_report){
					consumer_report = 0;
					u16 consumer_key = 0;
					blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
				}
			}

		}


	}
#endif   //end of UI_BUTTON_ENABLE
