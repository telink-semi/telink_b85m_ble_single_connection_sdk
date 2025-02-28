/********************************************************************************************************
 * @file    ext_calibration.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver Group
 * @date    May 8,2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "ext_calibration.h"
#include "drivers.h"
#include "vendor/common/ble_flash.h"


/**
 * @brief      This function is used to calib ADC 1.2V vref.
 */
int user_calib_adc_vref(unsigned char * adc_vref_calib_value_rd) {
	/********************************************************************************************
		There have three kind of calibration value of ADC 1.2V vref in flash,and one calibration value in Efuse.
		Three kind of ADC calibration value in flash are adc_vbat_calib_vref(used for internal voltage sample)
		and two-point calibration adc_gpio_calib_vref(used for gpio voltage sample)
		and adc_gpio_calib_vref(used for gpio voltage sample).The ADC calibration value in Efuse is
		adc_vbat_calib_vref(used for internal voltage sample).
		The calibration value of efuse has a total of 8 bits, all of which are calibration values..
		The priority of adc_gpio_calib_vref is: two-point calib from Flash > one-point calib from Flash > Default(1175mV).
		The priority of adc_vbat_calib_vref is: Flash > Efuse >Default(1175mV).
	********************************************************************************************/
	unsigned short gpio_calib_vref = 0;
	signed char gpio_calib_vref_offset = 0;
	unsigned short vbat_calib_vref = 0;

	/****** Check the two-point gpio calibration value whether is exist ********/
	if((adc_vref_calib_value_rd[4]  >= 0) && (adc_vref_calib_value_rd[4] <= 0x7f) && (((adc_vref_calib_value_rd[6] << 8) + adc_vref_calib_value_rd[5]) >= 47) && (((adc_vref_calib_value_rd[6] << 8) + adc_vref_calib_value_rd[5]) <= 300)){
		/****** Method of calculating two-point gpio calibration Flash_gain and Flash_offset value: ********/
		/****** Vref = [(Seven_Byte << 8) + Six_Byte + 1000]mv ********/
		/****** offset = [Five_Byte - 20] mv. ********/
		gpio_calib_vref = (adc_vref_calib_value_rd[6] << 8) + adc_vref_calib_value_rd[5] + 1000;
		gpio_calib_vref_offset = adc_vref_calib_value_rd[4] - 20;
		adc_set_gpio_calib_vref(gpio_calib_vref);
		adc_set_gpio_two_point_calib_offset(gpio_calib_vref_offset);
	}
	else{
		/****** If flash do not exist the two-point gpio calibration value,use the one-point gpio calibration value ********/
		/****** Method of calculating one-point gpio calibration Flash_gpio_Vref value: ********/
		/****** Vref = [1175 +First_Byte-255+Second_Byte] mV = [920 + First_Byte + Second_Byte] mV ********/
		gpio_calib_vref = 920 + adc_vref_calib_value_rd[0] + adc_vref_calib_value_rd[1];
		/****** Check the one-point calibration value whether is correct ********/
		if ((gpio_calib_vref >= 1047) && (gpio_calib_vref <= 1302))
		{
			adc_set_gpio_calib_vref(gpio_calib_vref);
		}
	}
	/****** Method of calculating calibration Flash_vbat_Vref value: ********/
	/****** Vref = [1175 +First_Byte-255+Second_Byte] mV = [920 + First_Byte + Second_Byte] mV ********/
	vbat_calib_vref = 920 + adc_vref_calib_value_rd[2] + adc_vref_calib_value_rd[3];
	/****** If flash do not exist the vbat one-point calibration value or the value is incorrect,use the Efuse vbat calibration value ********/
	if ((vbat_calib_vref < 1047) || (vbat_calib_vref > 1302))
	{
		vbat_calib_vref = efuse_get_adc_calib_value();
		if(0 != vbat_calib_vref)
		{
			adc_set_vbat_calib_vref(vbat_calib_vref);
			return 1;
		}
	}
	else
	{
		adc_set_vbat_calib_vref(vbat_calib_vref);
		return 1;
	}
	return 0;
}





/**
 * @brief		This function serves to set flash voltage vdd_f.TO ensure the vdd_f is enough to supply the flash,need to calibration the vdd_f.
 * @param[in]	voltage - the vdd_f need to set.The voltage is range from 0 to 0x07.
 * @return		none.
 */
_attribute_ram_code_ void flash_set_vdd_f(Flash_VoltageDef voltage_ldo, Flash_VoltageDef voltage_dcdc)
{
	/*
	 * Because the DCDC and LDO mode just use one in the same time,So just one setting in below with the vdd_f is valid.
	 * confirmed with haitao.gu, add by yi.bao.
	 */
	analog_write(0x09, ((analog_read(0x09) & 0x8f) | ((voltage_ldo & 0x07) << 4)));    	//ldo mode flash ldo
	analog_write(0x0c, ((analog_read(0x0c) & 0xf8) | (voltage_dcdc & 0x07)));			//dcdc mode flash ldo
}




/**
 * @brief		This function serves to calibration the flash voltage(VDD_F),if the flash has the calib_value,we will use it,either will
 * 				trim vdd_f to 1.95V(2b'111 the max) if the flash is zb.
 */
int user_calib_vdd_f(unsigned char * calib_value) {
	if(0 == (calib_value[0] & 0xf8) && 0 == (calib_value[1] & 0xf8))
	{
		flash_set_vdd_f(calib_value[1], calib_value[0]);
		return 1;
	}
	else
	{
		/* special design: for ZBIT Flash, set 1.95V/1.9V if no calibration value write on flash */
		if(blc_flash_vendor == FLASH_ETOX_ZB)
		{
			flash_set_vdd_f(FLASH_VOLTAGE_1V95, FLASH_VOLTAGE_1V9);
			calib_value[1] = FLASH_VOLTAGE_1V95;  //update calib_value[1], for upper layer use
			calib_value[0] = FLASH_VOLTAGE_1V9;   //update calib_value[0], for upper layer use
			return 1;
		}
	}
	return 0;
}
