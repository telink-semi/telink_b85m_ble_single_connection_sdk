/********************************************************************************************************
 * @file	app_uart.c
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
#include "app_config.h"
#include "tl_common.h"
#include "drivers.h"


#if (DRIVER_TEST_MODE == TEST_UART)

#define UART_DMA  		1     //uart use dma
#define UART_NDMA  		2     //uart not use dma
#define UART_MODE		UART_NDMA






#define LED1     				GPIO_PD0
#define LED2     				GPIO_PD1
#define LED3     				GPIO_PD2


volatile unsigned char uart_rx_flag=0;
volatile unsigned char uart_dmairq_tx_cnt=0;
volatile unsigned char uart_dmairq_rx_cnt=0;
volatile unsigned int  uart_ndmairq_cnt=0;
volatile unsigned char uart_ndmairq_index=0;

#if (UART_MODE==UART_DMA)

	#define UART_DATA_LEN    12      //data max ?    (UART_DATA_LEN+4) must 16 byte aligned
	typedef struct{
		unsigned int dma_len;        // dma len must be 4 byte
		unsigned char data[UART_DATA_LEN];
	}uart_data_t;

	uart_data_t rec_buff = {0,  {0, } };
	uart_data_t trans_buff = {6,   {0xaa,0xbb,0xcc,0xdd,0xee,0xff, } };
#elif(UART_MODE==UART_NDMA)

	#define rec_buff_Len    16
	#define trans_buff_Len  16

	__attribute__((aligned(4))) unsigned char rec_buff[rec_buff_Len]={0};
	__attribute__((aligned(4))) unsigned char trans_buff[trans_buff_Len] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, \
			                                                                0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00};
#endif



	/**
	 * @brief		Initialization of uart test
	 * @param[in]	none
	 * @return      none
	 */
void app_uart_test_init(void)
{
	WaitMs(2000);  //leave enough time for SWS_reset when power on

	//note: dma addr must be set first before any other uart initialization! (confirmed by sihui)
	uart_recbuff_init( (unsigned char *)&rec_buff, sizeof(rec_buff));

	uart_gpio_set(UART_TX_PB1, UART_RX_PB0);// uart tx/rx pin set

	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset

	//baud rate: 115200
	#if (CLOCK_SYS_CLOCK_HZ == 16000000)
//		uart_init(118, 13, PARITY_NONE, STOP_BIT_ONE);
		uart_init(9, 13, PARITY_NONE, STOP_BIT_ONE);
	#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
		uart_init(249, 9, PARITY_NONE, STOP_BIT_ONE);
	#endif


#if (UART_MODE==UART_DMA)
	uart_dma_enable(1, 1); 	//uart data in hardware buffer moved by dma, so we need enable them first

	irq_set_mask(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1);   	//uart Rx/Tx dma irq enable

	uart_irq_enable(0, 0);  	//uart Rx/Tx irq no need, disable them


#elif(UART_MODE==UART_NDMA)
	uart_dma_enable(0, 0);

	#if(MCU_CORE_TYPE == MCU_CORE_827x)
		irq_disable_type(FLD_IRQ_DMA_EN);
	#elif(MCU_CORE_TYPE == MCU_CORE_825x)
		irq_clr_mask(FLD_IRQ_DMA_EN);
	#endif

	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);

	uart_irq_enable(1,0);   //uart RX irq enable

	uart_ndma_irq_triglevel(1,0);   //set the trig level. 1 indicate one byte will occur interrupt
#endif



	irq_enable();

}



/**
 * @brief		Main loop of uart test
 * @param[in]	none
 * @return      none
 */
void app_uart_test_start(void)
{

	WaitMs(1000);

#if	(UART_MODE==UART_DMA)

		uart_dma_send( (unsigned char*)&rec_buff);
		WaitMs(300);
		uart_dma_send((unsigned char*)&trans_buff);

#elif(UART_MODE==UART_NDMA)
		for(unsigned char i=0;i<trans_buff_Len;i++){
			uart_ndma_send_byte(trans_buff[i]);
		}
		if(uart_rx_flag>0){
			uart_ndmairq_cnt=0; //Clear uart_ndmairq_cnt
			uart_rx_flag=0;
			for(unsigned char i=0;i<trans_buff_Len;i++){
				uart_ndma_send_byte(rec_buff[i]);
			}
		}
#endif

}
















#define rec_buff_Len    16
#define trans_buff_Len    16
volatile unsigned int cnt=0;


/**
 * @brief		this function is used to process uart irq
 * @param[in]	none
 * @return      none
 */
void app_uart_test_irq_proc(void)
{

#if (UART_MODE==UART_DMA)

	unsigned char uart_dma_irqsrc;
	//1. UART irq
	uart_dma_irqsrc = dma_chn_irq_status_get();///in function,interrupt flag have already been cleared,so need not to clear DMA interrupt flag here


	if(uart_dma_irqsrc & FLD_DMA_CHN_UART_RX){
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);

		uart_dmairq_rx_cnt++;

		//Received uart data in rec_buff, user can copy data here

	}
	if(uart_dma_irqsrc & FLD_DMA_CHN_UART_TX){
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);

		uart_dmairq_tx_cnt++;
	}

#elif(UART_MODE==UART_NDMA)

	static unsigned char uart_ndma_irqsrc;
	uart_ndma_irqsrc = uart_ndmairq_get();  ///get the status of uart irq.
	if(uart_ndma_irqsrc){

	//cycle the four registers 0x90 0x91 0x92 0x93,in addition reading will clear the irq.
		if(uart_rx_flag==0){
			rec_buff[uart_ndmairq_cnt++] = reg_uart_data_buf(uart_ndmairq_index);
			uart_ndmairq_index++;
			uart_ndmairq_index &= 0x03;// cycle the four registers 0x90 0x91 0x92 0x93, it must be done like this for the design of SOC.
			if(uart_ndmairq_cnt%16==0&&uart_ndmairq_cnt!=0){
				uart_rx_flag=1;
			}
		}
		else{
			read_reg8(0x90+ uart_ndmairq_index);
			uart_ndmairq_index++;
			uart_ndmairq_index &= 0x03;
		}
	}


#endif

}

#endif
