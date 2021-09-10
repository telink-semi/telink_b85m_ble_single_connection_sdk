/********************************************************************************************************
 * @file	app_spi_master.c
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


#if (DRIVER_TEST_MODE == TEST_SPI)




/**************************************************************************************
   8258 Demo Code Config
   1. select spi demo:  master or slave( config it in app_config.h )
   2. if spi slave demo,  select slave working mode: dma mode
   3. if spi master demo, select the peer device spi slave address mode:
 *************************************************************************************/
    #define     SLAVE_TELINK_825x_DMA_MODE      1 //slave  825x dma mode, master send 3 byte sram address(0x40000~0x4ffff)
	#define     SLAVE_TELINK_826x_DMA_MODE      2 //slave  826x dma mode, master send 2 byte sram address(0x8000~0xffff)
	#define		PEER_SLAVE_DEVICE				SLAVE_TELINK_825x_DMA_MODE
    #define     SPI_CS_PIN				        GPIO_PD6//SPI CS pin

#if (PEER_SLAVE_DEVICE == SLAVE_TELINK_825x_DMA_MODE)
    unsigned char slaveRegAddr_WriteCMD[] = {SLAVE_REGADDR1,SLAVE_REGADDR2,SLAVE_REGADDR3,SPI_WRITE_CMD};
	unsigned char slaveRegAddr_ReadCMD[]  = {SLAVE_REGADDR1,SLAVE_REGADDR2,SLAVE_REGADDR3,SPI_READ_CMD};
#elif(PEER_SLAVE_DEVICE == SLAVE_TELINK_826x_DMA_MODE)
    unsigned char slaveRegAddr_WriteCMD[] = {SLAVE_REGADDR2,SLAVE_REGADDR3,SPI_WRITE_CMD};
    unsigned char slaveRegAddr_ReadCMD[]  = {SLAVE_REGADDR2,SLAVE_REGADDR3,SPI_READ_CMD};
#endif
//write buff
volatile unsigned char spi_master_tx_buff[DBG_DATA_LEN]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
//read buff
volatile unsigned char spi_master_rx__buff[DBG_DATA_LEN]={0x00};
/**
 * @brief      Initialize the spi master test
 * @param[in]  none
 * @return     none
 */
void spi_master_test_init(void)
{
	//spi clock 500K, only master need set i2c clock
	spi_master_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*500000)-1),SPI_MODE0);          //div_clock. spi_clk = sys_clk/((div_clk+1)*2),mode select
#if(MCU_CORE_TYPE == MCU_CORE_827x)
	spi_master_gpio_set(SPI_GPIO_SCL_A4,SPI_GPIO_CS_D6,SPI_GPIO_SDO_A2,SPI_GPIO_SDI_A3);    //master mode ：spi pin set
#elif(MCU_CORE_TYPE == MCU_CORE_825x)
	spi_master_gpio_set(SPI_GPIO_GROUP_A2A3A4D6);    //master mode £ºspi pin set
#endif
}
/**
 * @brief		Main loop of spi master test
 * @param[in]	none
 * @return      none
 */
void spi_master_mainloop(void)
{
	WaitMs(1000);   //1S
	spi_master_tx_buff[0] += 1;
	spi_master_tx_buff[0] &= 0xff;
#if (PEER_SLAVE_DEVICE == SLAVE_TELINK_825x_DMA_MODE)
    spi_write(slaveRegAddr_WriteCMD, 4,(unsigned char*)spi_master_tx_buff, DBG_DATA_LEN,SPI_CS_PIN);// pls refer to the datasheet for the write and read format of spi.
	spi_read( slaveRegAddr_ReadCMD , 4,(unsigned char*)spi_master_rx__buff,DBG_DATA_LEN,SPI_CS_PIN);
#elif (PEER_SLAVE_DEVICE == SLAVE_TELINK_826x_DMA_MODE)
	spi_write(slaveRegAddr_WriteCMD, 3,(unsigned char*)spi_master_tx_buff, DBG_DATA_LEN,SPI_CS_PIN);// pls refer to the datasheet for the write and read format of spi.
    spi_read( slaveRegAddr_ReadCMD , 3,(unsigned char*)spi_master_rx__buff,DBG_DATA_LEN,SPI_CS_PIN);
#endif
}




#endif //end of DRIVER_TEST_MODE == TEST_SPI
