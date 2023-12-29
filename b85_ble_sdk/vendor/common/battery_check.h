/********************************************************************************************************
 * @file    battery_check.h
 *
 * @brief   This is the header file for BLE SDK
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
#ifndef BATTERY_CHECK_H_
#define BATTERY_CHECK_H_


/**
 * @brief	Application battery check enable control. Default disable, use can enable it in app_conifg.h.
 	 	 	User must check battery voltage on mass production application to prevent abnormal writing or erasing Flash at a low voltage !!!
 */
#ifndef APP_BATT_CHECK_ENABLE
#define APP_BATT_CHECK_ENABLE								0
#endif


#if (APP_BATT_CHECK_ENABLE)



/**
 * @brief	Battery Voltage log enable
 */
#ifndef		APP_BATT_CHECK_LOG_EN
#define		APP_BATT_CHECK_LOG_EN								0
#endif



#define VBAT_DEEP_THRES_MV				2000   // 2000 mV low battery alarm


extern u8 adc_hw_initialized;

/**
 * @brief      This function is used to clear adc setting flag
 * @param      none
 * @return     none.
 */
static inline void battery_clear_adc_setting_flag (void)
{
	adc_hw_initialized = 0;
}


/**
 * @brief      This function is used to enable battery detect
 * @param[in]  en - 1: enable;  0: disable.
 * @return     none.
 */
void		   battery_set_detect_enable (int en);

/**
 * @brief      This function is used to get enable state of battery detect
 * @param[in]  none.
 * @return     0: Battery detect is disable 	 1:Battery detect is enable.
 */
int  		   battery_get_detect_enable (void);

/**
 * @brief		vbat detect initialization
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void adc_vbat_detect_init(void);

/**
 * @brief	   This is battery check function
 * @param[in]  alram_vol_mv - input battery value
 * @return     0: fail, 1: success
 */
int 		   app_battery_power_check(u16 alram_vol_mv);

#endif


#endif /* APP_BATTDET_H_ */
