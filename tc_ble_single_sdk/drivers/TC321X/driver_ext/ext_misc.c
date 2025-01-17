/********************************************************************************************************
 * @file    ext_misc.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver Group
 * @date    May 8,2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "drivers/TC321X/driver.h"
#include "ext_misc.h"
/*********************************************************** STIMER START********************************************/
_attribute_ram_code_ void systimer_set_irq_capture(unsigned int sys_timer_tick)//8278 for save time,need to place in ram.
{
	if( (unsigned int)( sys_timer_tick  - (reg_system_tick + 7*sys_tick_per_us )) > BIT(30) )//63us - 75us in flash / 3.5us in ram
	{
		reg_system_tick_irq = reg_system_tick + 12*sys_tick_per_us; ///320 = 0x140, so not &0xfffffff8
	}
	else{
		reg_system_tick_irq = sys_timer_tick & 0xfffffff8;
	}
}
/*********************************************************** STIMER END********************************************/
