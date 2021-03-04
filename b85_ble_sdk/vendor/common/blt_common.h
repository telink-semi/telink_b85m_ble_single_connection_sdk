/********************************************************************************************************
 * @file	blt_common.h
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
#ifndef BLT_COMMON_H_
#define BLT_COMMON_H_

#include "drivers.h"


#define			BLMS_STATE_SCAN									BIT(2)
#define			BLMS_STATE_SCAN_POST							BIT(3)



//////////////////////////// Flash  Address Configuration ///////////////////////////////
/**************************** 128 K Flash *****************************/
#ifndef		CFG_ADR_MAC_128K_FLASH
#define		CFG_ADR_MAC_128K_FLASH								0x1F000
#endif

#ifndef		CFG_ADR_CALIBRATION_128K_FLASH
#define		CFG_ADR_CALIBRATION_128K_FLASH						0x1E000
#endif

/**************************** 512 K Flash *****************************/
#ifndef		CFG_ADR_MAC_512K_FLASH
#define		CFG_ADR_MAC_512K_FLASH								0x76000
#endif

#ifndef		CFG_ADR_CALIBRATION_512K_FLASH
#define		CFG_ADR_CALIBRATION_512K_FLASH						0x77000
#endif

/**************************** 1 M Flash *******************************/
#ifndef		CFG_ADR_MAC_1M_FLASH
#define		CFG_ADR_MAC_1M_FLASH		   						0xFF000
#endif

#ifndef		CFG_ADR_CALIBRATION_1M_FLASH
#define		CFG_ADR_CALIBRATION_1M_FLASH						0xFE000
#endif



/** Calibration Information FLash Address Offset of  CFG_ADR_CALIBRATION_xx_FLASH ***/
#define		CALIB_OFFSET_CAP_INFO								0x0
#define		CALIB_OFFSET_TP_INFO								0x40

#define		CALIB_OFFSET_ADC_VREF								0xC0

#define		CALIB_OFFSET_FIRMWARE_SIGNKEY						0x180







extern u32 flash_sector_mac_address;
extern u32 flash_sector_calibration;




/*
 * only 1 can be set
 */
static inline void blc_app_setExternalCrystalCapEnable(u8  en)
{
	blt_miscParam.ext_cap_en = en;

	WriteAnalogReg(0x8a,ReadAnalogReg(0x8a)|0x80);//close internal cap

}



static inline void blc_app_loadCustomizedParameters(void)
{
	 if(!blt_miscParam.ext_cap_en)
	 {
		 //customize freq_offset adjust cap value, if not customized, default ana_81 is 0xd0
		 //for 512K Flash, flash_sector_calibration equals to 0x77000
		 //for 1M  Flash, flash_sector_calibration equals to 0xFE000
		 if(flash_sector_calibration){
			 u8 cap_frqoft = *(unsigned char*) (flash_sector_calibration + CALIB_OFFSET_CAP_INFO);
			 if( cap_frqoft != 0xff ){
				 analog_write(0x8A, (analog_read(0x8A) & 0xc0)|(cap_frqoft & 0x3f));
			 }
		 }
	 }
}


void blc_readFlashSize_autoConfigCustomFlashSector(void);


void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static);




#endif /* BLT_COMMON_H_ */
