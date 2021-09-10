/********************************************************************************************************
 * @file	app_config.h
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
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#define FIRMWARES_SIGNATURE_ENABLE          0
#define FirmwareEncryptionKey               0x77180 //512k flash: 0x77180~0x7718F


/////////////////// TEST FEATURE SELECTION /////////////////////////////////

#define	TEST_HW_TIMER									1



#define	TEST_GPIO_IRQ									10



#define	TEST_UART										20


#define TEST_IIC										30


#define TEST_SPI										40


#define TEST_ADC										50


#define TEST_PWM										60


#define TEST_LOW_POWER									70


#define TEST_RF_EMI										80



#define DRIVER_TEST_MODE								TEST_LOW_POWER



#if (DRIVER_TEST_MODE == TEST_ADC)

	#define BATT_CHECK_ENABLE    0

#elif (DRIVER_TEST_MODE == TEST_IIC)
	/**************************************************************************************
	   8258 Demo Code Config
	   1. select i2c demo:  master or slave( app_config.h )
	   2. if i2c slave demo,  select slave working mode: mapping mode or dma mode
	   3. if i2c master demo, select the peer device i2c slave address mode:
					826x/8255 mapping mode/dma mode or other not telink's device

	 *************************************************************************************/
	#define  	I2C_DEMO_MASTER        	1   //i2c master demo
	#define     I2C_DEMO_SLAVE			2   //i2c master demo


	//select i2c demo here(master or slave)
	#define     I2C_DEMO_SELECT 		I2C_DEMO_MASTER



	#define 	DBG_DATA_LEN					16
	#define 	DBG_DATA_NUM					16


	#define 	SLAVE_DMA_MODE_ADDR_WRITE		0x44a00  //i2c master write data to  0x4A000
	#define 	SLAVE_DMA_MODE_ADDR_READ		0x4A100  //i2c master read data from 0x4A100

#elif (DRIVER_TEST_MODE == TEST_SPI)

	#define SPI_MASTER_MODE	1   //spi use master mode
	#define SPI_SLAVE_MODE	2   //spi use slave mode
	#define SPI_MODE		SPI_SLAVE_MODE

	#define SLAVE_REGADDR1  0x04
	#define SLAVE_REGADDR2  0x80
	#define SLAVE_REGADDR3  0x00
	#define SPI_READ_CMD  	0x80// spi read command
	#define SPI_WRITE_CMD 	0x00// spi write command
	#define DBG_DATA_LEN    16

#endif




/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	16000000

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};




/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		500  //ms







#define DEBUG_GPIO_ENABLE							1

#if(DEBUG_GPIO_ENABLE)
	//define debug GPIO here according to your hardware
	#define GPIO_CHN0							GPIO_PB0
	#define GPIO_CHN1							GPIO_PB1
	#define GPIO_CHN2							GPIO_PB4
	#define GPIO_CHN3							GPIO_PB5

	#define PB0_OUTPUT_ENABLE					1
	#define PB1_OUTPUT_ENABLE					1
	#define PB4_OUTPUT_ENABLE					1
	#define PB5_OUTPUT_ENABLE					1
#endif  //end of DEBUG_GPIO_ENABLE





#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
