/********************************************************************************************************
 * @file	battery_check.h
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
#ifndef BATTERY_CHECK_H_
#define BATTERY_CHECK_H_

#define MANNUAL_MODE_GET_ADC_SAMPLE_RESULT  0
#define VBAT_LEAKAGE_PROTECT_EN				1
#define VBAT_DEEP_THRES_MV				2000   // 2000 mV low battery alarm
#define VBAT_SUSPEND_THRES_MV			1800   // 1800 mV low battery alarm


/**
 * @brief      This function enable battery detect
 * @param[in]  en - 1: enable;  0: disable.
 * @return     none.
 */
void battery_set_detect_enable (int en);

/**
 * @brief      This function get enable state of battery detect
 * @param[in]  none.
 * @return     0: Battery detect is disable 	 1:Battery detect is enable.
 */
int  battery_get_detect_enable (void);

#if(VBAT_LEAKAGE_PROTECT_EN)
/**
 * @brief		This function serves to set the protect power level of the chip.
 * @param[in]	threshold_deep_vol_mv    - the power-level that the chip enter deepsleep.
 * @param[in]	threshold_suspend_vol_mv - the power-level that the chip enter suspend.
 * @return      none
 */
_attribute_ram_code_ int app_battery_power_check(u16 threshold_deep_vol_mv, u16 threshold_suspend_vol_mv);
#else
/**
 * @brief		This function serves to set the warning-light power level of the program.
 * @param[in]	alram_vol_mv    - the warning-light power level.
 * @return      none
 */
int app_battery_power_check(u16 alram_vol_mv);
#endif

/**
 * @brief		This function is the initialization of the battery check.
 * @param[in]	none.
 * @return      none.
 */
void user_init_battery_power_check(void);


#endif /* APP_BATTDET_H_ */
