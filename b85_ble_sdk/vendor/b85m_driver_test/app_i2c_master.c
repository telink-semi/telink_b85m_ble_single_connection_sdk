/********************************************************************************************************
 * @file	app_i2c_master.c
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
/*
 * i2c_master.c
 *
 *  Created on: 2018-6-15
 *      Author: Administrator
 */
#include "app_config.h"
#include "tl_common.h"
#include "drivers.h"



#if(DRIVER_TEST_MODE == TEST_IIC && I2C_DEMO_SELECT == I2C_DEMO_MASTER)





/**************************************************************************************
   8258 Demo Code Config
   1. select i2c demo:  master or slave( config it in app_config.h )
   2. if i2c slave demo,  select slave working mode: mapping mode or dma mode
   3. if i2c master demo, select the peer device i2c slave address mode:
                826x/8255 mapping mode/dma mode or other not telink's device
            here selecting peer slave device mode is for master sending address byte length

 *************************************************************************************/


#if(I2C_DEMO_SELECT == I2C_DEMO_MASTER)
	//826x: 8261/8266/8267/8269
	//825x: 8255/8258
    //82xx: 8261/8266/8267/8269 and 8255/8258

	#define     SLAVE_TELINK_825x_DMA_MODE      1 //slave  825x dma mode, master send 3 byte sram address(0x40000~0x4ffff)
	#define     SLAVE_TELINK_82xx_MAPPING_MODE  2 //slave  826x/825x  mapping mode, master no need send address(address length is 0)
	#define     SLAVE_TELINK_826x_DMA_MODE      3 //slave  826x dma mode, master send 2 byte sram address(0x8000~0xffff)
	#define     SLAVE_OTHER_I2C_DEVICE			4 //slave  not telink mcu, address length may be 1 or 2 or others.


	#define		PEER_SLAVE_DEVICE				SLAVE_TELINK_825x_DMA_MODE
#endif

/**************************************************************************************
 *
 *************************************************************************************/
/*************** i2c master data buffer *****************/
//write buff
volatile unsigned char i2c_master_tx_buff[DBG_DATA_LEN] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
//read buff
volatile unsigned char i2c_master_rx_buff[DBG_DATA_LEN] = {0};

unsigned char master_rx_buff_debug[DBG_DATA_LEN *DBG_DATA_NUM];   //store i2c master reading data here to debug
int master_rx_index = 0;




/**
 * @brief      Initialize the i2c master test
 * @param[in]  none
 * @return     none
 */
void i2c_master_test_init(void)
{

	//I2C pin set
#if(MCU_CORE_TYPE == MCU_CORE_827x)
	i2c_gpio_set(I2C_GPIO_SDA_C0,I2C_GPIO_SCL_C1);  	//SDA/CK : C0/C1
#elif (MCU_CORE_TYPE == MCU_CORE_825x)
	i2c_gpio_set(I2C_GPIO_GROUP_C0C1);  	//SDA/CK : C0/C1
#endif

	//slave device id 0x5C(write) 0x5D(read)
	//i2c clock 200K, only master need set i2c clock
	i2c_master_init(0x5C, (unsigned char)(CLOCK_SYS_CLOCK_HZ/(4*200000)) );

}

/**
 * @brief		Main loop of i2c master test
 * @param[in]	none
 * @return      none
 */
void i2c_master_mainloop(void)
{

	WaitMs(1000);   //1 S


	#if (PEER_SLAVE_DEVICE == SLAVE_TELINK_825x_DMA_MODE)
		i2c_master_tx_buff[0] += 1;
		//825x slave dma mode, sram address(0x40000~0x4FFFF) length should be 3 byte
		i2c_write_series(SLAVE_DMA_MODE_ADDR_WRITE, 3, (unsigned char *)i2c_master_tx_buff, DBG_DATA_LEN);
		i2c_read_series(SLAVE_DMA_MODE_ADDR_READ,  3, (unsigned char *)i2c_master_rx_buff, DBG_DATA_LEN);

		/*********** copy the data read by i2c master from slave for debug  ****************/
		memcpy( (unsigned char *)(master_rx_buff_debug + master_rx_index*DBG_DATA_LEN), (unsigned char *)i2c_master_rx_buff, DBG_DATA_LEN);
		master_rx_index ++;
		if(master_rx_index>=DBG_DATA_NUM){
			master_rx_index = 0;
		}

	#elif (PEER_SLAVE_DEVICE == SLAVE_TELINK_82xx_MAPPING_MODE)
		i2c_master_tx_buff[0] += 1;
		//slave mapping mode, no need send address information, so address length should be 0
		i2c_write_series(0, 0, (unsigned char*)i2c_master_tx_buff, DBG_DATA_LEN);
		i2c_read_series(0, 0, (unsigned char*)i2c_master_rx_buff, DBG_DATA_LEN);

		/*********** copy the data read by i2c master from slave for debug  ****************/
		memcpy((unsigned char*)(master_rx_buff_debug + master_rx_index*DBG_DATA_LEN), (unsigned char*)i2c_master_rx_buff, DBG_DATA_LEN);
		master_rx_index ++;
		if(master_rx_index>=DBG_DATA_NUM){
			master_rx_index = 0;
		}

	#elif (PEER_SLAVE_DEVICE == SLAVE_TELINK_826x_DMA_MODE)

		need  add code here

	#elif (PEER_SLAVE_DEVICE == SLAVE_OTHER_I2C_DEVICE)

		need  add code here

	#endif

}



/**
 * @brief		this function is used to process i2c irq
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void app_i2c_test_irq_proc(void)
{

}



#endif  //end of I2C_DEMO_SELECT == I2C_DEMO_MASTER
