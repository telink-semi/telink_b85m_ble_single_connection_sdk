/********************************************************************************************************
 * @file    app_buffer.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef APP_BUFFER_H_
#define APP_BUFFER_H_

#include "vendor/common/user_config.h"




/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ************************************************/


/* if user set ACL_CONN_MAX_RX_OCTETS in app_config.h, then RX_FIFO_SIZE will auto calculated here;
 * otherwise, user should define it in their application code */
#ifdef ACL_CONN_MAX_RX_OCTETS

/**
 * @brief	ACL RX buffer size & number
 *  		ACL RX buffer is shared by all connections to hold LinkLayer RF RX data.
 * usage limitation for RX_FIFO_SIZE:
 * 1. must use CAL_LL_ACL_RX_BUF_SIZE to calculate, user can not change !!!
 */
#define RX_FIFO_SIZE				CAL_LL_ACL_RX_BUF_SIZE(ACL_CONN_MAX_RX_OCTETS)  //user can not change !!!

#endif



/* if user set ACL_CONN_MAX_TX_OCTETS in app_config.h, then TX_FIFO_SIZE will auto calculated here;
 * otherwise, user should define it in their application code */
#ifdef ACL_CONN_MAX_TX_OCTETS
/**
 * @brief	ACL TX buffer size & number
 *  		ACL Central TX buffer is shared by all central connections to hold LinkLayer RF TX data.
*			ACL Peripheral TX buffer is shared by all peripheral connections to hold LinkLayer RF TX data.
 * usage limitation for ACL_xxx_TX_FIFO_SIZE:
 * 1. must use CAL_LL_ACL_TX_BUF_SIZE to calculate, user can not change !!!
 */
#define TX_FIFO_SIZE				CAL_LL_ACL_TX_BUF_SIZE(ACL_CONN_MAX_TX_OCTETS)	//user can not change !!!


#endif


/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***************************************************/






/**
 * @brief	MTU MTU size
 * refer to BLE Core Specification: Vol 3, Part F, "3.2.8 Exchanging MTU size" & "3.4.2 MTU exchange"; Vol 3, Part G, "4.3.1 Exchange MTU"
 * usage limitation:
 * 1. must equal to or greater than 23(default MTU)
 * 2. if support LE Secure Connections, must equal to or bigger than 65
 */
#ifndef MTU_SIZE_SETTING
#define MTU_SIZE_SETTING					23  //user set value, user can change it in app_config.h
#endif



/*
 * if MTU no greater than "ATT_MTU_MAX_SDK_DFT_BUF", default L2CAP RX & TX buffer(stack inside) is enough,
 * if MTU greater than "ATT_MTU_MAX_SDK_DFT_BUF", default L2CAP RX & TX buffer is not enough,
	 	 user must allocate new buffer and set it
 */
#if (MTU_SIZE_SETTING > ATT_MTU_MAX_SDK_DFT_BUF)
/**
 * @brief	ACL connection L2CAP RX & TX data Buffer
 * L2CAP RX buffer is used in stack, to hold split l2cap packet data sent by peer device, and will combine
 *                 them to one complete packet when last sub_packet come, then use for upper layer.
 * ACL_L2CAP_BUFF_SIZE: can use CAL_L2CAP_BUFF_SIZE to calculate
 *  1. should be greater than or equal to (MTU_SIZE_SETTING + 6)
 *  2. should be be an integer multiple of 4 (4 Byte align)
 */
#define	ACL_L2CAP_BUFF_SIZE					CAL_L2CAP_BUFF_SIZE(MTU_SIZE_SETTING)
#define	ACL_L2CAP_BUFF_SIZE					CAL_L2CAP_BUFF_SIZE(MTU_SIZE_SETTING)

extern	u8 app_l2cap_rx_fifo[];
extern	u8 app_l2cap_tx_fifo[];

#endif






#endif /* APP_BUFFER_H_ */
