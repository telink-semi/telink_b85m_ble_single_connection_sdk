/********************************************************************************************************
 * @file	app_phytest.c
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
#include "stack/ble/ble.h"
#include "app.h"
#include "app_att.h"
#include "../common/blt_led.h"

#if (BLE_PHYTEST_MODE != PHYTEST_MODE_DISABLE )
/*----------------------------------------------*
 *	HCI TX FIFO  = 2 Bytes LEN + n Bytes Data.	*
 *												*
 *	T_txdata_buf = 4 Bytes LEN + n Bytes Data.	*
 *												*
 *	HCI_TXFIFO_SIZE = 2 + n.					*
 *												*
 *	UART_DATA_LEN = n.							*
 *												*
 *	UART_DATA_LEN = HCI_TXFIFO_SIZE - 2.		*
 * ---------------------------------------------*/
#define HCI_RXFIFO_SIZE		20
#define HCI_RXFIFO_NUM		2

#define HCI_TXFIFO_SIZE		20
#define HCI_TXFIFO_NUM		2

#define UART_DATA_LEN    	(HCI_TXFIFO_SIZE - 2)      // data max 252
typedef struct{
	unsigned int len;        // data max 252
	unsigned char data[UART_DATA_LEN];
}uart_data_t;

MYFIFO_INIT(hci_rx_fifo, HCI_RXFIFO_SIZE, HCI_RXFIFO_NUM);
MYFIFO_INIT(hci_tx_fifo, HCI_TXFIFO_SIZE, HCI_TXFIFO_NUM);






#if (BLE_PHYTEST_MODE == PHYTEST_MODE_OVER_HCI_WITH_UART)
/**
 * @brief		this function is used to process rx uart data.
 * @param[in]	none
 * @return      0 is ok
 */
	int rx_from_uart_cb (void)
	{
		if(my_fifo_get(&hci_rx_fifo) == 0)
		{
			return 0;
		}

		u8* p = my_fifo_get(&hci_rx_fifo);
		u32 rx_len = p[0]; //usually <= 255 so 1 byte should be sufficient

		if (rx_len)
		{
			blc_hci_handler(&p[4], rx_len - 4);
			my_fifo_pop(&hci_rx_fifo);
		}

		return 0;




	}

	/**
	 * @brief		this function is used to process tx uart data.
	 * @param[in]	none
	 * @return      0 is ok
	 */
	int tx_to_uart_cb (void)
	{
		uart_data_t T_txdata_buf;
		static u32 uart_tx_tick = 0;

		u8 *p = my_fifo_get (&hci_tx_fifo);

		#if (ADD_DELAY_FOR_UART_DATA)
			if (p && !uart_tx_is_busy () && clock_time_exceed(uart_tx_tick, 30000))
		#else
			if (p && !uart_tx_is_busy ())
		#endif
		{
			memcpy(&T_txdata_buf.data, p + 2, p[0]+p[1]*256);
			T_txdata_buf.len = p[0]+p[1]*256 ;

			uart_dma_send((unsigned char*)&T_txdata_buf);
			my_fifo_pop (&hci_tx_fifo);
			uart_tx_tick = clock_time();
		}
		return 0;
	}
#endif




_attribute_ram_code_ void irq_phyTest_handler(void)
{
#if(FEATURE_TEST_MODE == TEST_BLE_PHY)
	unsigned char uart_dma_irqsrc;
	//1. UART irq
	uart_dma_irqsrc = dma_chn_irq_status_get();///in function,interrupt flag have already been cleared,so need not to clear DMA interrupt flag here
	if(uart_dma_irqsrc & FLD_DMA_CHN_UART_RX)
	{
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);
		u8* w = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
		if(w[0]!=0)
		{
			my_fifo_next(&hci_rx_fifo);
			u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
			reg_dma0_addr = (u16)((u32)p);
		}
	}
	if(uart_dma_irqsrc & FLD_DMA_CHN_UART_TX){
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);
	}
#endif
}



extern const led_cfg_t led_cfg[];

void app_trigger_phytest_mode(void)
{
	static u8 phyTestFlag = 0;
	if(!phyTestFlag && blc_ll_getCurrentState() != BLS_LINK_STATE_CONN){  //can not enter phytest in connection state
		phyTestFlag = 1;
		device_led_setup(led_cfg[4]);  //LED_SHINE_FAST: 4


		//adjust some rf parameters here if needed
		write_reg8(0x402, 0x2b);   //must: adjust rf packet preamble for BQB
		blc_phy_setPhyTestEnable( BLC_PHYTEST_ENABLE );
	}

}






void app_phytest_init(void)
{
	blc_phy_initPhyTest_module();
	blc_phy_preamble_length_set(11);

	#if(BLE_PHYTEST_MODE == PHYTEST_MODE_THROUGH_2_WIRE_UART || BLE_PHYTEST_MODE == PHYTEST_MODE_OVER_HCI_WITH_UART)  //uart
		uart_gpio_set(UART_TX_PB1, UART_RX_PB0);
		uart_reset();
	#endif

	uart_recbuff_init((unsigned short*)hci_rx_fifo_b, hci_rx_fifo.size);


	#if (CLOCK_SYS_CLOCK_HZ == 16000000)
		uart_init(9,13,PARITY_NONE, STOP_BIT_ONE); //baud rate: 115200
	#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
		uart_init(12,15,PARITY_NONE, STOP_BIT_ONE); //baud rate: 115200
	#endif


	uart_dma_enable(1,1);

	irq_set_mask(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1);   	//uart Rx/Tx dma irq enable
	uart_irq_enable(1,0);

	#if	(BLE_PHYTEST_MODE == PHYTEST_MODE_THROUGH_2_WIRE_UART)
		blc_register_hci_handler (phy_test_2_wire_rx_from_uart, phy_test_2_wire_tx_to_uart);
	#elif(BLE_PHYTEST_MODE == PHYTEST_MODE_OVER_HCI_WITH_UART)
		blc_register_hci_handler (rx_from_uart_cb, tx_to_uart_cb);		//default handler
	#endif

}








#endif  //end of  BLE_PHYTEST_MODE != PHYTEST_MODE_DISABLE
