/********************************************************************************************************
 * @file	ext_misc.h
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
#ifndef DRIVERS_8258_EXT_MISC_H_
#define DRIVERS_8258_EXT_MISC_H_






/******************************* rf start **********************************************************************/
/**
 * @brief     This function serves to set BLE mode of RF.
 * @return	  none.
 */
void rf_drv_ble_init();

#define		RF_POWER_P3dBm					RF_POWER_P3p01dBm
#define		RF_POWER_P0dBm					RF_POWER_P0p04dBm
/******************************* rf end  **********************************************************************/



/******************************* watchdog_start ***********************************************************************/

#define WATCHDOG_TIMEOUT_COEFF	18		//  check register definition, 0x622

#define WATCHDOG_DISABLE	( reg_tmr_ctrl &= ~FLD_TMR_WD_EN )

/******************************** watchdog_end  **********************************************************************/



/******************************* stimer_start ******************************************************************/

/**
 * @brief   system Timer : 16Mhz, Constant
 */
enum{
	SYSTEM_TIMER_TICK_1US 		= 16,
	SYSTEM_TIMER_TICK_1MS 		= 16000,
	SYSTEM_TIMER_TICK_1S 		= 16000000,

	SYSTEM_TIMER_TICK_625US  	= 10000,  //625*16
	SYSTEM_TIMER_TICK_1250US 	= 20000,  //1250*16
};



/******************************* stimer_end ********************************************************************/

#endif








