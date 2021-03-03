/********************************************************************************************************
 * @file     blt_common.h
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
