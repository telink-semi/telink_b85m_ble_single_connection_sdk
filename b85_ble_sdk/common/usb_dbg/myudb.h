/********************************************************************************************************
 * @file	myudb.h
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
#ifndef		__MYUDB_H__
#define		__MYUDB_H__
#pragma once


#define my_usb_init(id, p_print)
#define	usb_send_str(s)
#define	usb_send_data(p,n)
#define my_dump_str_data(en,s,p,n)
#define my_dump_str_u32s(en,s,d0,d1,d2,d3)
#define my_uart_send_str_data
#define	my_uart_send_str_int
#define	myudb_usb_handle_irq()



#define	log_hw_ref()

// 4-byte sync word: 00 00 00 00
#define	log_sync(en)
//4-byte (001_id-5bits) id0: timestamp align with hardware gpio output; id1-31: user define
#define	log_tick(en,id)

//1-byte (000_id-5bits)
#if (MCU_CORE_TYPE == MCU_CORE_9518)
	#define	log_event_irq(en,id)
#elif(MCU_CORE_TYPE == MCU_CORE_827x || MCU_CORE_TYPE == MCU_CORE_825x )
	#define	log_event_irq(id)
#endif

//1-byte (01x_id-5bits) 1-bit data: id0 & id1 reserved for hardware
#define	log_task(en,id,b)

//2-byte (10-id-6bits) 8-bit data
#define	log_b8(en,id,d)

//3-byte (11-id-6bits) 16-bit data
#define	log_b16(en,id,d)




#define	log_tick_irq(en,id)
#define	log_tick_irq_2(en,id,t)




#define	log_task_irq(en,id,b)

#define	log_task_begin_irq(en,id)
#define	log_task_end_irq(en,id)

#define	log_task_begin_irq_2(en,id,t)
#define	log_task_end_irq_2(en,id,t)



#define	log_b8_irq(en,id,d)

#define	log_b16_irq(en,id,d)

#define	log_event(id)


#endif
