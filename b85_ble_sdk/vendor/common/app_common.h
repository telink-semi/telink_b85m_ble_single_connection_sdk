/********************************************************************************************************
 * @file    app_common.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#ifndef APP_COMMON_H_
#define APP_COMMON_H_


#include "vendor/common/user_config.h"


/**
 * @brief	system clock definition
 * 			16M by default, user can set it in app_config.h
 */
#ifndef	CLOCK_SYS_CLOCK_HZ
#define CLOCK_SYS_CLOCK_HZ                                 16000000
#endif

#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	#define SYS_CLK_TYPE  									SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	#define SYS_CLK_TYPE  									SYS_CLK_24M_Crystal
#elif(CLOCK_SYS_CLOCK_HZ == 32000000)
	#define SYS_CLK_TYPE  									SYS_CLK_32M_Crystal
#elif(CLOCK_SYS_CLOCK_HZ == 48000000)
	#define SYS_CLK_TYPE  									SYS_CLK_48M_Crystal
#else
	#error "unsupported system clock !"
#endif


/**
 * @brief
 */
enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};









/**
 * @brief  connection device information declaration
 * 		   for Central(master) project only
 */
typedef struct
{
	u16 conn_handle;
	u8 conn_state;
	u8 mac_adrType;
	u8 mac_addr[6];
}dev_info_t;

extern dev_info_t 	cur_conn_device;



/**
 * @brief		this function is used to set deepsleep retention SRAM size for all application project.
 * 			    select DEEPSLEEP_MODE_RET_SRAM_LOW16K or DEEPSLEEP_MODE_RET_SRAM_LOW32K according to retention data size used.
 * @param[in]	none
 * @return      none
 */
void blc_app_setDeepsleepRetentionSramSize(void);




/**
 * @brief		this function is used to Check if any Stack(Controller & Host) Initialization error after all BLE initialization done!!!
 * 			    attention that code will stuck in "while(1)" if any error detected in initialization, user need to find what error happens and then fix it.
 * @param[in]	none
 * @return      none
 */
void blc_app_checkControllerHostInitialization(void);


/**
 * @brief      This function is check If IRK(local IRK or peer IRK) valid
 * @param[in]  pIrk - pointer to IRK(local IRK or peer IRK)
 * @return     1: IRK valid
 * 			   0: IRK invalid
 */
bool blc_app_isIrkValid(u8* pIrk);

#endif /* APP_COMMON_H_ */
