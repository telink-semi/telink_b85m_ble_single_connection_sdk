/********************************************************************************************************
 * @file	phy_test.h
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
#ifndef PHY_TEST_H_
#define PHY_TEST_H_

#include "tl_common.h"



/**
 * @brief	PHY test mode
 */
#ifndef			PHYTEST_MODE_DISABLE
#define			PHYTEST_MODE_DISABLE					0
#endif

#ifndef			PHYTEST_MODE_THROUGH_2_WIRE_UART
#define 		PHYTEST_MODE_THROUGH_2_WIRE_UART		1   //Direct Test Mode through a 2-wire UART interface
#endif

#ifndef			PHYTEST_MODE_OVER_HCI_WITH_USB
#define 		PHYTEST_MODE_OVER_HCI_WITH_USB			2   //Direct Test Mode over HCI(UART hardware interface)
#endif

#ifndef			PHYTEST_MODE_OVER_HCI_WITH_UART
#define 		PHYTEST_MODE_OVER_HCI_WITH_UART			3   //Direct Test Mode over HCI(USB  hardware interface)
#endif




/**
 * @brief	PHY test mode enableor disable
 */
#define 		BLC_PHYTEST_DISABLE						0
#define 		BLC_PHYTEST_ENABLE						1



#define			PHY_CMD_SETUP							0
#define			PHY_CMD_RX								1
#define			PHY_CMD_TX								2
#define			PHY_CMD_END								3


#define 		PKT_TYPE_PRBS9 							0
#define 		PKT_TYPE_0X0F 							1
#define 		PKT_TYPE_0X55 							2
#define 		PKT_TYPE_0XFF 							3

#define			PKT_TYPE_HCI_PRBS9						0
#define			PKT_TYPE_HCI_0X0F						1
#define			PKT_TYPE_HCI_0X55 						2
#define			PKT_TYPE_HCI_PRBS15						3
#define			PKT_TYPE_HCI_0XFF 						4
#define			PKT_TYPE_HCI_0X00						5
#define			PKT_TYPE_HCI_0XF0						6
#define			PKT_TYPE_HCI_0XAA						7
enum{
	PHY_EVENT_STATUS	 = 0,
	PHY_EVENT_PKT_REPORT = 0x8000,
};

enum{
	PHY_STATUS_SUCCESS 	 = 0,
	PHY_STATUS_FAIL 	 = 0x0001,
};





typedef struct {
	u8 cmd;
	u8 tx_start;
	u16 pkts;

	u32 tick_tx;
}phy_data_t;








/******************************* User Interface  ************************************/
void 	  blc_phy_initPhyTest_module(void);

ble_sts_t blc_phy_setPhyTestEnable (u8 en);
bool 	  blc_phy_isPhyTestEnable(void);


//user for phy test 2 wire uart mode
int 	 phy_test_2_wire_rx_from_uart (void);
int 	 phy_test_2_wire_tx_to_uart (void);



/************************* Stack Interface, user can not use!!! ***************************/

int 	  blc_phy_test_main_loop(void);

int 	  blc_phytest_cmd_handler (u8 *p, int n);

ble_sts_t blc_phy_setReceiverTest (u8 rx_chn);
ble_sts_t blc_phy_setTransmitterTest (u8 tx_chn, u8 length, u8 pkt_type);
ble_sts_t blc_phy_setPhyTestEnd(u8 *pkt_num);

ble_sts_t blc_phy_reset(void);
void blc_phy_preamble_length_set(unsigned char len);
void phy_test_driver_init(RF_ModeTypeDef rf_mode);





#endif /* PHY_TEST_H_ */
