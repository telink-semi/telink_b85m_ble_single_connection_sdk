/********************************************************************************************************
 * @file	 rc_ir.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2018
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

#ifndef RC_IR_LEARN_H_
#define RC_IR_LEARN_H_

	//ir control GPIO and irq define
	#define 		GPIO_IR_CONTROL			 GPIO_PA1
	#define         GPIO_IR_OUT              GPIO_PB3
	#define         GPIO_IR_LEARN_IN         GPIO_PA2

	#define 		IR_LEARN_INTERRUPT_MASK  (FLD_IRQ_GPIO_EN)

	/************************************************************************************************
	* ¡¾define¡¿IR_LEARN_FREQUENCY_RANGE_MODE :														*
	* Choose IR Learn frequency range that device supported.										*
    * ¡¾define¡¿IR_LEARN_MAX_FREQUENCY: Max carrier frequency that device support to do IR learn		*
	* ¡¾define¡¿IR_LEARN_MIN_FREQUENCY: Min carrier frequency that device support to do IR learn		*
	************************************************************************************************/

	#define			IR_LEARN_SET_PARAMETER_MODE	USER_DEFINE

	#define			USER_DEFINE	1
	#define			DEFAULT		2


	#if(IR_LEARN_SET_PARAMETER_MODE == USER_DEFINE)

		/************************************************************************************************
		* ¡¾define¡¿IR_LEARN_FREQUENCY_RANGE_MODE :														*
		* Choose IR Learn frequency range that device supported											*
		* ¡¾define¡¿IR_LEARN_MAX_FREQUENCY: Max carrier frequency that device support to do IR learn		*
		* ¡¾define¡¿IR_LEARN_MIN_FREQUENCY: Min carrier frequency that device support to do IR learn		*
		************************************************************************************************/

		#define			IR_LEARN_MAX_FREQUENCY			40000
		#define			IR_LEARN_MIN_FREQUENCY			30000

		#define			IR_LEARN_CARRIER_MIN_CYCLE		16000000/IR_LEARN_MAX_FREQUENCY
		#define			IR_LEARN_CARRIER_MIN_HIGH_TICK	IR_LEARN_CARRIER_MIN_CYCLE/3
		#define			IR_LEARN_CARRIER_MAX_CYCLE		16000000/IR_LEARN_MIN_FREQUENCY
		#define			IR_LEARN_CARRIER_MAX_HIGH_TICK	IR_LEARN_CARRIER_MAX_CYCLE/3

		/************************************************************************************************
		* ¡¾define¡¿PARAMETER SETTING :																	*
		* Related parameter setting to change IR learn result											*
		************************************************************************************************/

		#define			IR_LEARN_INTERVAL_THRESHOLD			(IR_LEARN_CARRIER_MAX_CYCLE*3/2)
		#define			IR_LEARN_END_THRESHOLD	 			(30*CLOCK_SYS_CLOCK_1MS)
		#define			IR_LEARN_OVERTIME_THRESHOLD			10000000						//10s
		#define			IR_CARR_CHECK_CNT		 			10
		#define			CARR_AND_NO_CARR_MIN_NUMBER			15
		#define			MAX_SECTION_NUMBER					100

	#else

		/************************************************************************************************
		* ¡¾define¡¿IR_LEARN_FREQUENCY_RANGE_MODE :														*
		* Choose IR Learn frequency range that device supported.										*
		* ¡¾define¡¿IR_LEARN_MAX_FREQUENCY: Max carrier frequency that device support to do IR learn		*
		* ¡¾define¡¿IR_LEARN_MIN_FREQUENCY: Min carrier frequency that device support to do IR learn		*
		************************************************************************************************/

		#define			IR_LEARN_MAX_FREQUENCY			40000
		#define			IR_LEARN_MIN_FREQUENCY			30000

		#define			IR_LEARN_CARRIER_MIN_CYCLE		16000000/IR_LEARN_MAX_FREQUENCY
		#define			IR_LEARN_CARRIER_MIN_HIGH_TICK	IR_LEARN_CARRIER_MIN_CYCLE/3
		#define			IR_LEARN_CARRIER_MAX_CYCLE		16000000/IR_LEARN_MIN_FREQUENCY
		#define			IR_LEARN_CARRIER_MAX_HIGH_TICK	IR_LEARN_CARRIER_MAX_CYCLE/3

		/************************************************************************************************
		* ¡¾define¡¿PARAMETER SETTING :																	*
		* Related parameter setting to change IR learn result											*
		************************************************************************************************/

		#define			IR_LEARN_INTERVAL_THRESHOLD			(IR_LEARN_CARRIER_MAX_CYCLE*3/2)
		#define			IR_LEARN_END_THRESHOLD	 			(30*CLOCK_SYS_CLOCK_1MS)
		#define			IR_LEARN_OVERTIME_THRESHOLD			10000000						//10s
		#define			IR_CARR_CHECK_CNT		 			10
		#define			CARR_AND_NO_CARR_MIN_NUMBER			15
		#define			MAX_SECTION_NUMBER					100

	#endif

enum {
    IR_LEARN_DISABLE = 0x00,
    IR_LEARN_WAIT_KEY,
    IR_LEARN_KEY,
    IR_LEARN_BEGIN,
    IR_LEARN_SAMPLE_END,
	IR_LEARN_SUCCESS,
    IR_LEARN_FAIL_FIRST_INTERVAL_TOO_LONG,
    IR_LEARN_FAIL_TWO_LONG_NO_CARRIER,
    IR_LEARN_FAIL_WAIT_OVER_TIME,
    IR_LEARN_FAIL_WAVE_NUM_TOO_FEW,
    IR_LEARN_FAIL_FLASH_FULL,
    IR_LEARN_FAIL,
}ir_learn_states;

typedef struct{
	unsigned int   last_trigger_tm_point;
	unsigned int   curr_trigger_tm_point;
	unsigned int   time_interval;
	unsigned int   carr_first_interval;

	unsigned int   carr_switch_start_tm_point;
	unsigned char  carr_check_cnt;
	unsigned char  carr_or_not;
	unsigned char  ir_learn_state;
	unsigned char  ir_learn_for_later_use;
	unsigned int   ir_learn_tick;
	unsigned int   carr_cycle_interval;

	unsigned int   ir_learn_finish_tm;
	unsigned short ir_enter_irq_cnt;
	unsigned short wave_series_cnt;
	unsigned int   wave_series_buf[MAX_SECTION_NUMBER];
}ir_learn_ctrl_t;

typedef struct{
	unsigned int   ir_learn_carrier_cycle;
	unsigned short ir_learn_wave_num;
	unsigned int   ir_lenrn_send_buf[MAX_SECTION_NUMBER];
}ir_learn_send_t;


typedef struct{
    unsigned int   dma_len;        // dma len
    unsigned short data[MAX_SECTION_NUMBER];
    unsigned int   data_num;
}ir_send_dma_data_t;

void ir_learn_start(void);
void ir_learn_stop(void);
void ir_learn_copy_result(ir_learn_send_t* send_buffer);
void ir_learn_irq_handler(void);
void ir_learn_send(ir_learn_send_t* send_buffer);
void ir_learn_send_init(void);
void ir_learn_detect(void);
void ir_learn_init(void);
unsigned char get_ir_learn_state(void);

#endif /* RC_IR_LEARN_H_ */
