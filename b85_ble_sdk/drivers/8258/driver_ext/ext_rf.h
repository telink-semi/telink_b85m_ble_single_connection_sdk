/********************************************************************************************************
 * @file	ext_rf.h
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
#ifndef DRIVERS_8278_EXT_RF_H_
#define DRIVERS_8278_EXT_RF_H_

#define			STOP_RF_STATE_MACHINE						( REG_ADDR8(0xf00) = 0x80 )//Todo:need check by sunwei
#define			REG_BB_LL_BASE_ADDR							0xf00//Todo:need check by sunwei
enum{
	FLD_RF_R_CMD                 =	BIT_RNG(0,3),
	FLD_RF_R_STOP                =	0,
	FLD_RF_R_BTX                 =	1,
	FLD_RF_R_BRX                 =	2,
	FLD_RF_R_PTX                 =	3,
	FLD_RF_R_PRX                 =	4,
	FLD_RF_R_STX                 =	5,
	FLD_RF_R_SRX                 =	6,
	FLD_RF_R_STR                 =	7,
	FLD_RF_R_SRT                 =	8,
	FLD_RF_R_CMD_TRIG            =  BIT(7),
};

/**
 * @brief   This function serves to triggle accesscode in coded Phy mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_trigle_codedPhy_accesscode(void)
{
	WRITE_REG8(0x405, REG_ADDR8(0x405) | BIT(7)); //reg_rf_acc_len |= FLD_RF_LR_ACC_TRIG;
}

static inline void rf_set_dma_tx_addr(unsigned int src_addr)//Todo:need check by sunwei
{
	reg_dma3_addr = (unsigned short)(src_addr);
}


unsigned int cpu_stall_WakeUp_By_RF_SystemTick(int WakeupSrc, unsigned short rf_mask, unsigned int tick);


#define		rf_receiving_flag				is_rf_receiving_pkt ///static inline
#define		rf_tx_settle_adjust				tx_settle_adjust    ///static inline

#define 	RF_TX_PAKET_DMA_LEN(len)		(len)

#define     RF_BLE_RF_PAYLOAD_LENGTH_OK		RF_BLE_PACKET_LENGTH_OK
#define     RF_BLE_RF_PACKET_CRC_OK			RF_BLE_PACKET_CRC_OK

#define 	reg_rf_ll_cmd_schedule 			REG_ADDR32(0xf18)
#define 	reg_rf_ll_cmd					REG_ADDR8(0xf00)
#define     reg_rf_ll_rest_pid        		REG_ADDR8(0xf01)

#endif

