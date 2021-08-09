/********************************************************************************************************
 * @file	s7816.c
 *
 * @brief	This is the source file for B85
 *
 * @author	Driver Group
 * @date	May 8,2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "s7816.h"
#include "uart.h"
#include "irq.h"
#include "dma.h"
#include "timer.h"

/**
 * @brief      	This function is used to initiate 7816 module of MCU
 * @param[in]  	Div	-set the divider of clock of 7816 module:
 * 				7816clk = sysclk/(0x7b[6:4]*2),	0x7b[7]:enable 7816clk
 * 				7816clk:  0x40-2Mhz   0x20-4Mhz
 * 				baudrate: 0x40-16194  0x20-32388
 * @return     	none
 */
void s7816_set_clk(unsigned char Div)
{
	//---Set 7816 CLK divider
    //caution:7816 clk module only controlled by the highest bit of 7816 clk mode
	reg_7816_clk_div&=0x0f;
	reg_7816_clk_div|=(unsigned char)Div;
}

/**
 * @brief      	This function is used to set address and size of buffer 7816 module of MCU
 * @param[in]  	*RecvAddr		-set the address of buffer to receive data
 * @param[in]  	RecvBufLen		-set the length of buffer to receive data
 * @return     	none
 */
void s7816_set_rx_buf(unsigned short *RecvAddr, unsigned short RecvBufLen){
    unsigned char bufLen;
    unsigned int addr;
	//Set DMA begin address and the size of buffer needed

    addr = (unsigned int) RecvAddr;
    bufLen = RecvBufLen / 16;

    reg_dma0_addr = addr; 				//set receive buffer address(L,M)
    reg_dma0_addrHi = ((addr>>16)&0xff);//set receive buffer address(H)
    reg_dma0_size = bufLen; 			//set receive buffer size

    reg_dma0_mode = FLD_DMA_WR_MEM;
}

/**
 * @brief      	This function is used to initiate 7816 module of MCU
 * @param[in]  	Pin_7816_TRX	-select the I/O 	pin of 7816 module
 * @param[in]	Pin_7816_RST	-select the RST 	pin of 7816 module
 * @param[in]	Pin_7816_VCC	-select the VCC 	pin of 7816 module
 * @return     	none
 */
void s7816_set_pin(S7816_TRx_PinDef Pin_7816_TRX)
{
	//---enable UART 7816 CLK(GPIOA_GP1)
	write_reg8(0x586,read_reg8(0x586)&(~(BIT(1))));				//disable relative GPIO
	unsigned char tmp = read_reg8(0x5a8)| BIT(2);
	write_reg8(0x5a8,tmp&(~BIT(3)));	//enable 7816CLK
	//---enable UART 7816 TRX(Optional)
	switch(Pin_7816_TRX)
	{
		case S7816_TRX_D0://take care the details of every pin when using this pin
			gpio_setup_up_down_resistor(GPIO_PD0, PM_PIN_PULLUP_10K);
			gpio_set_func(GPIO_PD0,AS_UART);
			gpio_set_input_en(GPIO_PD0, 1);
			break;
		case S7816_TRX_D3:
			gpio_setup_up_down_resistor(GPIO_PD3, PM_PIN_PULLUP_10K);
			gpio_set_func(GPIO_PD3,AS_UART);
			gpio_set_input_en(GPIO_PD3, 1);
			break;
		case S7816_TRX_D7:
			gpio_setup_up_down_resistor(GPIO_PD7, PM_PIN_PULLUP_10K);
			gpio_set_func(GPIO_PD7,AS_UART);
			gpio_set_input_en(GPIO_PD7, 1);
			break;
		default:
			break;
	}
}

/**
 * @brief      	This function is used to send data to ID card,after succeeding in getting ATR
 * @param[in]  	*TransAddr	- data is waitting to send
 * @return     	none
 */
unsigned char s7816_dma_send(unsigned char *TransAddr)
{
    unsigned int addr;

	if(reg_uart_status1 & FLD_UART_TX_DONE) {//waiting for finishing an operation of sending

		//send DMA and buffer details
		addr = (unsigned int) TransAddr;

		reg_dma1_addr = addr; //set receive buffer address
		reg_dma1_addrHi = ((addr>>16)&0xff);
		reg_dma_tx_rdy0	 = FLD_DMA_CHN1;

		return 1;
	}
	return 0;
}

/**
 * @brief      	This function is used to transform half duplex mode of 7816
 * @param[in]  	mode	- half_duplex_mode_TX/RX is transformed by setting 0x9b[5]
 * 						0x9b[5]=1:half_duplex_mode_RX;0x9b[5]=0:half_duplex_mode_TX
 * @return     	none
 */
void s7816_set_half_duplex(S7816_Half_Duplex_ModeDef mode)
{
	//---------------------------------------------------------
	if(mode==S7816_RX){
		reg_uart_rx_timeout1|=FLD_UART_P7816_EN;
	}
	else if(mode==S7816_TX)
	{
		reg_uart_rx_timeout1&=~FLD_UART_P7816_EN;
	}
}
