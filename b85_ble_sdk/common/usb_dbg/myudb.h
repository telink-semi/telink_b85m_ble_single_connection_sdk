/********************************************************************************************************
 * @file	myudb.h
 *
 * @brief	This is the header file for B85
 *
 * @author	BLE GROUP
 * @date	06,2020
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
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
