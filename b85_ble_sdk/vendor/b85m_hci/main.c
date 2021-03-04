/********************************************************************************************************
 * @file	main.c
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

extern my_fifo_t hci_rx_fifo;
extern void user_init_normal(void);
extern void main_loop (void);


/**
 * @brief   IRQ handler
 * @param   none.
 * @return  none.
 */
_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler ();

#if (HCI_ACCESS==HCI_USE_UART)
	unsigned char irqS = dma_chn_irq_status_get();
    if(irqS & FLD_DMA_CHN_UART_RX)	//rx
    {
    	dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);

    	u8* w = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
    	if(w[0]!=0)
    	{
    		my_fifo_next(&hci_rx_fifo);
    		u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
    		reg_dma_uart_rx_addr = (u16)((u32)p);  //switch uart RX dma address
    	}
    }

    if(irqS & FLD_DMA_CHN_UART_TX)	//tx
    {
    	dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);
    }
#endif
}


/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main (void)    //must run in ramcode
{
#if (BLE_MODULE_PM_ENABLE)
	blc_pm_select_internal_32k_crystal();
#endif

#if(MCU_CORE_TYPE == MCU_CORE_825x)
	cpu_wakeup_init();
#elif(MCU_CORE_TYPE == MCU_CORE_827x)
	cpu_wakeup_init(LDO_MODE,EXTERNAL_XTAL_24M);
#endif

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

	clock_init(SYS_CLK_TYPE);

	//read flash size
	blc_readFlashSize_autoConfigCustomFlashSector();

	blc_app_loadCustomizedParameters();  //load customized freq_offset cap value


#if	(PM_DEEPSLEEP_RETENTION_ENABLE)
	if( pm_is_MCU_deepRetentionWakeup() ){
		user_init_deepRetn ();
	}
	else
#endif
	{
		user_init_normal ();
	}

    irq_enable();

	while (1) {
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		main_loop ();
	}
}
