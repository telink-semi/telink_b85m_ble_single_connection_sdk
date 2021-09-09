/********************************************************************************************************
 * @file	rc_ir_learn.h
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
#ifndef RC_IR_LEARN_H_
#define RC_IR_LEARN_H_

	//ir control GPIO and irq define
	#define 		GPIO_IR_CONTROL			 GPIO_PA1
	#define         GPIO_IR_OUT              GPIO_PB3
	#define         GPIO_IR_LEARN_IN         GPIO_PA2

	#define 		IR_LEARN_INTERRUPT_MASK  (FLD_IRQ_GPIO_EN)

	/************************************************************************************************
	* defineIR_LEARN_FREQUENCY_RANGE_MODE :														*
	* Choose IR Learn frequency range that device supported.										*
    * defineIR_LEARN_MAX_FREQUENCY: Max carrier frequency that device support to do IR learn		*
	* defineIR_LEARN_MIN_FREQUENCY: Min carrier frequency that device support to do IR learn		*
	************************************************************************************************/

	#define			IR_LEARN_SET_PARAMETER_MODE	USER_DEFINE

	#define			USER_DEFINE	1
	#define			DEFAULT		2


	#if(IR_LEARN_SET_PARAMETER_MODE == USER_DEFINE)

		/************************************************************************************************
		* defineIR_LEARN_FREQUENCY_RANGE_MODE :														*
		* Choose IR Learn frequency range that device supported											*
		* defineIR_LEARN_MAX_FREQUENCY: Max carrier frequency that device support to do IR learn		*
		* defineIR_LEARN_MIN_FREQUENCY: Min carrier frequency that device support to do IR learn		*
		************************************************************************************************/

		#define			IR_LEARN_MAX_FREQUENCY			40000
		#define			IR_LEARN_MIN_FREQUENCY			30000

		#define			IR_LEARN_CARRIER_MIN_CYCLE		16000000/IR_LEARN_MAX_FREQUENCY
		#define			IR_LEARN_CARRIER_MIN_HIGH_TICK	IR_LEARN_CARRIER_MIN_CYCLE/3
		#define			IR_LEARN_CARRIER_MAX_CYCLE		16000000/IR_LEARN_MIN_FREQUENCY
		#define			IR_LEARN_CARRIER_MAX_HIGH_TICK	IR_LEARN_CARRIER_MAX_CYCLE/3

		/************************************************************************************************
		* definePARAMETER SETTING :																	*
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
		* defineIR_LEARN_FREQUENCY_RANGE_MODE :														*
		* Choose IR Learn frequency range that device supported.										*
		* defineIR_LEARN_MAX_FREQUENCY: Max carrier frequency that device support to do IR learn		*
		* defineIR_LEARN_MIN_FREQUENCY: Min carrier frequency that device support to do IR learn		*
		************************************************************************************************/

		#define			IR_LEARN_MAX_FREQUENCY			40000
		#define			IR_LEARN_MIN_FREQUENCY			30000

		#define			IR_LEARN_CARRIER_MIN_CYCLE		16000000/IR_LEARN_MAX_FREQUENCY
		#define			IR_LEARN_CARRIER_MIN_HIGH_TICK	IR_LEARN_CARRIER_MIN_CYCLE/3
		#define			IR_LEARN_CARRIER_MAX_CYCLE		16000000/IR_LEARN_MIN_FREQUENCY
		#define			IR_LEARN_CARRIER_MAX_HIGH_TICK	IR_LEARN_CARRIER_MAX_CYCLE/3

		/************************************************************************************************
		* definePARAMETER SETTING :																	*
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

/**
 * @brief		IR learn send init,set pwn and irq related.
 * @param		none
 * @return      none.
 */
void ir_learn_start(void);

/**
 * @brief		Stop IR learn process.
 * @param		none
 * @return      none.
 */
void ir_learn_stop(void);

/**
 * @brief		Copy necessary parameter to send_buffer to buffer.
 * @param		none
 * @return		none
 */
void ir_learn_copy_result(ir_learn_send_t* send_buffer);

/**
 * @brief		IR learn process in irq
 *              learning high frequency carrier wave.
 * @param		none
 * @return      none.
 */
void ir_learn_irq_handler(void);

/**
 * @brief		Copy necessary parameter to send_buffer to buffer.
 * @param		none
 * @return		none
 */
void ir_learn_send(ir_learn_send_t* send_buffer);

/**
 * @brief		IR learn send init,set pwn and irq related.
 * @param		none
 * @return      none.
 */
void ir_learn_send_init(void);

/**
 * @brief		IR learn deal process,better to use it every loop.
 * @param		none
 * @return		none
 */
void ir_learn_detect(void);

/**
 * @brief		ir learn init algorithm ,set related GPIO function and
 * 			    irq related.
 * @param[in]	none.
 * @return      none.
 */
void ir_learn_init(void);

/**
 * @brief		Stop IR learn process.
 * @param		none
 * @return      0 	 : ir learn success
 * 				1 	 : ir learn is doing or disable
 * 				else : ir learn fail ,return fail reason,
 * 					   match enum ir_learn_states
 */
unsigned char get_ir_learn_state(void);

#endif /* RC_IR_LEARN_H_ */
