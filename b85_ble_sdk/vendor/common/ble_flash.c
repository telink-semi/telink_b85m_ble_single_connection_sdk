/********************************************************************************************************
 * @file	ble_flash.c
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
#include "ble_flash.h"
#include "stack/ble/ble.h"


/* global variable for some flash sector address, initial value use 512K flash.
 * attention that they will be changed in function "blc readFlashSize_autoConfigCustomFlashSector"  */
_attribute_data_retention_	unsigned int flash_sector_mac_address = CFG_ADR_MAC_512K_FLASH;
_attribute_data_retention_	unsigned int flash_sector_calibration = CFG_ADR_CALIBRATION_512K_FLASH;
_attribute_data_retention_	unsigned int flash_sector_smp_storage = FLASH_ADR_SMP_PAIRING_512K_FLASH; //for slave only

_attribute_data_retention_	unsigned int flash_sector_master_pairing = FLASH_ADR_MASTER_PAIRING_512K; //for master only

_attribute_data_retention_	unsigned int  blc_flash_mid = 0;
_attribute_data_retention_	unsigned int  blc_flash_vendor = 0;
_attribute_data_retention_	unsigned char blc_flash_capacity;


/**
 * @brief   BLE parameters on flash
 */
typedef struct{
	u8		cap_frqoffset_en;
	u8		cap_frqoffset_value;

	u8		vddf_calib_en;
	u8		vddf_calib_value0;
	u8		vddf_calib_value1;
}nv_para_t;

_attribute_data_retention_ 	nv_para_t 				blc_nvParam;


/**
 * @brief		This function is used to read flash mid, get flash vendor, and set flash capacity
 * @param[in]	none
 * @return      none
 */
void blc_flash_read_mid_get_vendor_set_capacity(void)
{
	/*
		Flash Type	uid CMD			MID		Company
		ZB25WD40B	0x4b		0x13325E	ZB
		ZB25WD80B	0x4b		0x14325E	ZB
		GD25LD40C	0x4b		0x1360C8	GD
		GD25LD80C	0x4b(AN)	0x1460C8	GD
		GD25LE80C	0x4b		0x1460C8	GD
		GD25LQ80C	0x4b		0x1460C8	GD
		MD25D40D	0x4b(AN)	0x134051	GD
		P25D40L		0x4b		0x136085	PUYA
		P25Q80U     0x4b        0x146085    PUYA
	 */
	/* attention: blc_flash_mid/blc_flash_vendor/blc_flash_capacity will be used by application and stack later
	 * so do not change code here */
	blc_flash_mid = flash_read_mid();
	blc_flash_vendor = flash_get_vendor(blc_flash_mid);
	blc_flash_capacity = ((blc_flash_mid & 0x00ff0000)>>16);

}


/**
 * @brief		This function can automatically recognize the flash size,
 * 				and the system selects different customized sector according
 * 				to different sizes.
 * @param[in]	none
 * @return      none
 */
void blc_readFlashSize_autoConfigCustomFlashSector(void)
{
	blc_flash_read_mid_get_vendor_set_capacity();

#if (FLASH_ZB25WD40B_SUPPORT_EN || FLASH_GD25LD40C_SUPPORT_EN || FLASH_GD25LD40E_SUPPORT_EN)	//512K
	if(blc_flash_capacity == FLASH_SIZE_512K){
		flash_sector_mac_address = CFG_ADR_MAC_512K_FLASH;
		flash_sector_calibration = CFG_ADR_CALIBRATION_512K_FLASH;
		flash_sector_smp_storage = FLASH_ADR_SMP_PAIRING_512K_FLASH;
		flash_sector_master_pairing = FLASH_ADR_MASTER_PAIRING_512K;
		tlkapi_printf(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] 512K Flash, MAC on 0x%x\n", flash_sector_mac_address);
	}
#endif
#if (FLASH_ZB25WD80B_SUPPORT_EN || FLASH_GD25LD80C_SUPPORT_EN || FLASH_GD25LD80E_SUPPORT_EN)		//1M
	else if(blc_flash_capacity == FLASH_SIZE_1M){
		flash_sector_mac_address = CFG_ADR_MAC_1M_FLASH;
		flash_sector_calibration = CFG_ADR_CALIBRATION_1M_FLASH;
		flash_sector_smp_storage = FLASH_ADR_SMP_PAIRING_1M_FLASH;
		flash_sector_master_pairing = FLASH_ADR_MASTER_PAIRING_1M;
		tlkapi_printf(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] 1M Flash, MAC on 0x%x\n", flash_sector_mac_address);
	}
#endif
	else{
		//This SDK do not support flash size other than 512K/1M
		//If code stop here, please check your Flash
		while(1);
	}

}




/*
 *Kite: 	VVWWXX38C1A4YYZZ
 *Vulture:  VVWWXXD119C4YYZZ
 * public_mac:
 * 				Kite 	: VVWWXX 38C1A4
* 				Vulture : VVWWXX D119C4
 * random_static_mac: VVWWXXYYZZ C0
 */
/**
 * @brief		This function is used to initialize the MAC address
 * @param[in]	flash_addr - flash address for MAC address
 * @param[in]	mac_public - public address
 * @param[in]	mac_random_static - random static MAC address
 * @return      none
 */
void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static)
{
	if(flash_sector_mac_address == 0){
		return;
	}


	u8 mac_read[8];
	flash_read_page(flash_addr, 8, mac_read);

	u8 value_rand[5];
	generateRandomNum(5, value_rand);

	u8 ff_six_byte[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	if ( memcmp(mac_read, ff_six_byte, 6) ) {
		memcpy(mac_public, mac_read, 6);  //copy public address from flash
	}
	else{  //no public address on flash
		mac_public[0] = value_rand[0];
		mac_public[1] = value_rand[1];
		mac_public[2] = value_rand[2];

		#if(MCU_CORE_TYPE == MCU_CORE_825x)
			mac_public[3] = 0x38;             //company id: 0xA4C138
			mac_public[4] = 0xC1;
			mac_public[5] = 0xA4;
		#elif(MCU_CORE_TYPE == MCU_CORE_827x)
			mac_public[3] = 0xD1;             //company id: 0xC119D1
			mac_public[4] = 0x19;
			mac_public[5] = 0xC4;
		#endif


		flash_write_page (flash_addr, 6, mac_public);
	}





	mac_random_static[0] = mac_public[0];
	mac_random_static[1] = mac_public[1];
	mac_random_static[2] = mac_public[2];
	mac_random_static[5] = 0xC0; 			//for random static

	u16 high_2_byte = (mac_read[6] | mac_read[7]<<8);
	if(high_2_byte != 0xFFFF){
		memcpy( (u8 *)(mac_random_static + 3), (u8 *)(mac_read + 6), 2);
	}
	else{
		mac_random_static[3] = value_rand[3];
		mac_random_static[4] = value_rand[4];

		flash_write_page (flash_addr + 6, 2, (u8 *)(mac_random_static + 3) );
	}
}

/**
 * @brief		load customized parameters (from Flash) when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void blc_app_loadCustomizedParameters_normal(void)
{
	/* read and set RF frequency offset capacitance calibration value from Flash */
	if(!blt_miscParam.ext_cap_en){
		if(flash_sector_calibration){
			u8 cap_frqoft = *(unsigned char*) (flash_sector_calibration + CALIB_OFFSET_CAP_INFO);
			if( cap_frqoft != 0xff ){
				blc_nvParam.cap_frqoffset_en = 1;
				blc_nvParam.cap_frqoffset_value = cap_frqoft;
				rf_update_internal_cap(blc_nvParam.cap_frqoffset_value);
			}
		}
	}

	/* read and set VDD_F calibration value from Flash */
	unsigned char calib_value[2] = {0};
	flash_read_page(flash_sector_calibration + CALIB_OFFSET_FLASH_VREF, 2, calib_value);
	if(user_calib_vdd_f(calib_value)){ //set calibration value success
		/* attention that "calib_value" may be changed in "user calib_vdd_f", and that's what we want */
		blc_nvParam.vddf_calib_en = 1;
		blc_nvParam.vddf_calib_value0 = calib_value[0];
		#if(MCU_CORE_TYPE == MCU_CORE_827x)
			blc_nvParam.vddf_calib_value1 = calib_value[1];
		#endif
	}

	/* read and set ADC V_reference calibration value from Flash */
	unsigned char adc_vref_calib_value_rd[7] = {0};
	flash_read_page(flash_sector_calibration + CALIB_OFFSET_ADC_VREF, 7, adc_vref_calib_value_rd);
	user_calib_adc_vref(adc_vref_calib_value_rd);
}

/**
 * @brief		load customized parameters (from SRAM) MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void blc_app_loadCustomizedParameters_deepRetn(void)
{
	/* set RF frequency offset capacitance calibration value with stored variables */
	if(blc_nvParam.cap_frqoffset_en) {
		rf_update_internal_cap(blc_nvParam.cap_frqoffset_value);
	}

	/* set VDD_F calibration value with stored variables*/
	if(blc_nvParam.vddf_calib_en) {
		#if(MCU_CORE_TYPE == MCU_CORE_825x)
			flash_set_vdd_f(blc_nvParam.vddf_calib_value0);
		#elif(MCU_CORE_TYPE == MCU_CORE_827x)
			flash_set_vdd_f(blc_nvParam.vddf_calib_value1, blc_nvParam.vddf_calib_value0);
		#endif
	}


	/* ADC V_reference calibration value is stored in variables on retention area, so no need set again */
}

