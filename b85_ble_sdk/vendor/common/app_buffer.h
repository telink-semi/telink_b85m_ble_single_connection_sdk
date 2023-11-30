/********************************************************************************************************
 * @file	app_buffer.h
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

#ifndef APP_BUFFER_H_
#define APP_BUFFER_H_

#include "vendor/common/user_config.h"
//#include "stack/ble/host/l2cap/l2cap.h"




/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ************************************************/
/**
 * @brief	connMaxRxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 */
#ifndef ACL_CONN_MAX_RX_OCTETS
#define ACL_CONN_MAX_RX_OCTETS			27	//user set value
#endif

/**
 * @brief	connMaxTxOctets
 * refer to BLE SPEC: Vol 6, Part B, "4.5.10 Data PDU length management"
 * 					  Vol 6, Part B, "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 *
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 */
#ifndef ACL_CONN_MAX_TX_OCTETS
#define ACL_CONN_MAX_TX_OCTETS			27	//user set value
#endif


/**
 * @brief	ACL RX buffer size & number
 *  		ACL RX buffer is shared by all connections to hold LinkLayer RF RX data.
 * usage limitation for ACL_RX_FIFO_SIZE:
 * 1. must use CAL_LL_ACL_RX_FIFO_SIZE to calculate, user can not change !!!
 *
 * usage limitation for ACL_RX_FIFO_NUM:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 4, 8, 16
 */
#define ACL_RX_FIFO_SIZE				CAL_LL_ACL_RX_FIFO_SIZE(ACL_CONN_MAX_RX_OCTETS)  //user can not change !!!
#define ACL_RX_FIFO_NUM					8	//user set value

/**
 * @brief	ACL TX buffer size & number
 *  		ACL Central TX buffer is shared by all central connections to hold LinkLayer RF TX data.
*			ACL Peripheral TX buffer is shared by all peripheral connections to hold LinkLayer RF TX data.
 * usage limitation for ACL_xxx_TX_FIFO_SIZE:
 * 1. must use CAL_LL_ACL_TX_FIFO_SIZE to calculate, user can not change !!!
 *
 * usage limitation for ACL_xxx_TX_FIFO_NUM:
 * 1. must be: (2^n), (power of 2)
 * 2. at least 8; recommended value: 8, 16, 32; other value not allowed.
 */
#define ACL_TX_FIFO_SIZE				CAL_LL_ACL_TX_FIFO_SIZE(ACL_CONN_MAX_TX_OCTETS)	//user can not change !!!
#define ACL_TX_FIFO_NUM					8	//user set value

/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***************************************************/






/**
 * @brief	MTU MTU size
 * refer to BLE SPEC: Vol 3, Part F, "3.2.8 Exchanging MTU size" & "3.4.2 MTU exchange"; Vol 3, Part G, "4.3.1 Exchange MTU"
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
