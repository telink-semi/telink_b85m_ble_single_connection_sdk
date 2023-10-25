/********************************************************************************************************
 * @file	ext_calibration.h
 *
 * @brief	This is the header file for B85
 *
 * @author	Driver Group
 * @date	May 8,2018
 *
 * @par		Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef DRIVERS_8278_EXT_CALIBRATION_H_
#define DRIVERS_8278_EXT_CALIBRATION_H_

#include "../flash.h"

/**
 * @brief      This function is used to calib ADC 1.2V vref.
 */
int user_calib_adc_vref(unsigned char * adc_vref_calib_value_rd);

/**
 * @brief		This function serves to calibration the flash voltage(VDD_F),if the flash has the calib_value,we will use it,either will
 * 				trim vdd_f to 1.95V(2b'111 the max) if the flash is zb.
 */
int user_calib_vdd_f(unsigned char * calib_value);



/**
 * @brief		This function serves to set flash voltage vdd_f.TO ensure the vdd_f is enough to supply the flash,need to calibration the vdd_f.
 * @param[in]	voltage - the vdd_f need to set.The voltage is range from 0 to 0x07.
 * @return		none.
 */
void flash_set_vdd_f(Flash_VoltageDef voltage_ldo, Flash_VoltageDef voltage_dcdc);


#endif


