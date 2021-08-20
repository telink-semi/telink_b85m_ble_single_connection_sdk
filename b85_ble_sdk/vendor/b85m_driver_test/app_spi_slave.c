/********************************************************************************************************
 * @file     app_spi_slave.c 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     May. 12, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/
#include "app_config.h"
#include "tl_common.h"
#include "drivers.h"


#if (DRIVER_TEST_MODE == TEST_SPI)
/**
 * @brief      Initialize the spi slave test
 * @param[in]  none
 * @return     none
 */
void spi_slave_test_init(void)
{
	spi_slave_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*500000)-1),SPI_MODE0);           //slave mode init
#if(MCU_CORE_TYPE == MCU_CORE_827x)
    spi_slave_gpio_set(SPI_GPIO_SCL_A4,SPI_GPIO_CS_D6,SPI_GPIO_SDO_A2,SPI_GPIO_SDI_A3);      //slave mode ：A2A3A4D6 spi pin set
#elif(MCU_CORE_TYPE == MCU_CORE_825x)
    spi_slave_gpio_set(SPI_GPIO_GROUP_A2A3A4D6);      //slave mode £ºspi pin set
#endif
}
#endif
