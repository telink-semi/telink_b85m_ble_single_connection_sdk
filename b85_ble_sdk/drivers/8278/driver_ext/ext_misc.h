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
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef DRIVERS_8278_EXT_MISC_H_
#define DRIVERS_8278_EXT_MISC_H_
#include "../register.h"


/******************************* rf start **********************************************************************/
/**
 * @brief     This function serves to set BLE mode of RF.
 * @return	  none.
 */
void rf_drv_ble_init(void);

#define		RF_POWER_P3dBm					RF_POWER_P3p13dBm
#define		RF_POWER_P0dBm					RF_POWER_N0p28dBm

/******************************* rf end  **********************************************************************/


/******************************* watchdog_start ***********************************************************************/

#define WATCHDOG_TIMEOUT_COEFF	18		//  check register definition, 0x622

#define WATCHDOG_DISABLE	( reg_tmr_ctrl &= ~FLD_TMR_WD_EN )

/******************************** watchdog_end  **********************************************************************/


/******************************* stimer_start ******************************************************************/
//#define reg_system_tick_irq 		REG_ADDR32(0x744)//reg_system_tick_irq_level

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





/******************************* dma_start ******************************************************************/

/**
 * @brief	ACL RX Data buffer length = maxRxOct + 21, then 16 Byte align
 *			maxRxOct + 21 = 4(DMA_len) + 2(BLE header) + maxRxOct + 4(MIC) + 3(CRC) + 8(ExtraInfo)
			RX buffer size must be be 16*n, due to MCU design
 */
//actually +21.The purpose of +22 is to deal with extreme situations. Due to DMA design,at least one byte buffer can not be unusable.

#define		TLK_RF_RX_EXT_LEN		(22)	//4(DMA_len) + 2(BLE header) + ISORxOct + 4(MIC) + 3(CRC) + 8(ExtraInfo)
#define 	CAL_LL_ACL_RX_FIFO_SIZE(maxRxOct)	(((maxRxOct + TLK_RF_RX_EXT_LEN) + 15) / 16 *16)


/**
 * @brief	ACL TX Data buffer length = maxTxOct + 10, then 4 Byte align
 *			maxTxOct + 10 = 4(DMA_len) + 2(BLE header) + maxTxOct + 4(MIC)
			TX buffer size must be be 4*n, due to MCU design
 */

#define     TLK_RF_TX_EXT_LEN		(10)	//10 = 4(DMA_len) + 2(BLE header) + 4(MIC)
#define 	CAL_LL_ACL_TX_FIFO_SIZE(maxTxOct)	(((maxTxOct + TLK_RF_TX_EXT_LEN) + 3) / 4 *4)

/******************************* dma_end ******************************************************************/



#endif
