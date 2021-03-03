/********************************************************************************************************
 * @file     battery_check.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
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

#ifndef BATTERY_CHECK_H_
#define BATTERY_CHECK_H_

#define MANNUAL_MODE_GET_ADC_SAMPLE_RESULT  0
#define VBAT_LEAKAGE_PROTECT_EN				1
#define VBAT_DEEP_THRES_MV				2000   // 2000 mV low battery alarm
#define VBAT_SUSPEND_THRES_MV			1800   // 1800 mV low battery alarm



void battery_set_detect_enable (int en);
int  battery_get_detect_enable (void);

#if(VBAT_LEAKAGE_PROTECT_EN)
_attribute_ram_code_ int app_battery_power_check(u16 threshold_deep_vol_mv, u16 threshold_suspend_vol_mv);
#else
int app_battery_power_check(u16 alram_vol_mv);
#endif


void user_init_battery_power_check(void);


#endif /* APP_BATTDET_H_ */
