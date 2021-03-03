/********************************************************************************************************
 * @file     blt_soft_timer.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2015
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
#ifndef BLT_SOFT_TIMER_H_
#define BLT_SOFT_TIMER_H_



#include "vendor/common/user_config.h"

//user define
#ifndef BLT_SOFTWARE_TIMER_ENABLE
#define BLT_SOFTWARE_TIMER_ENABLE					0   //enable or disable
#endif


#define 	MAX_TIMER_NUM							4   //timer max number


#define		MAINLOOP_ENTRY							0
#define 	CALLBACK_ENTRY							1



//if t1 < t2  return 1
#define		TIME_COMPARE_SMALL(t1,t2)   ( (u32)((t2) - (t1)) < BIT(30)  )

// if t1 > t2 return 1
#define		TIME_COMPARE_BIG(t1,t2)   ( (u32)((t1) - (t2)) < BIT(30)  )


#define		BLT_TIMER_SAFE_MARGIN_PRE	  (SYSTEM_TIMER_TICK_1US<<7)  //128 us
#define		BLT_TIMER_SAFE_MARGIN_POST	  (SYSTEM_TIMER_TICK_1S<<2)   // 4S

/**
 * @brief		This function is used to check the current time is what the timer expects or not
 * @param[in]	t - the time is expired for setting
 * @param[in]   now - Current system clock time
 * @return		0 - The current time isn't what the timer expects
 * 				1 - The current time is what the timer expects
 */
static int inline blt_is_timer_expired(u32 t, u32 now) {
	return ((u32)(now + BLT_TIMER_SAFE_MARGIN_PRE - t) < BLT_TIMER_SAFE_MARGIN_POST);
}


/**
 * @brief	callback function for software timer task
 */
typedef int (*blt_timer_callback_t)(void);




typedef struct blt_time_event_t {
	blt_timer_callback_t    cb;
	u32                     t;
	u32                     interval;
} blt_time_event_t;


// timer table managemnt
typedef struct blt_soft_timer_t {
	blt_time_event_t	timer[MAX_TIMER_NUM];  //timer0 - timer3
	u8					currentNum;  //total valid timer num
} blt_soft_timer_t;




//////////////////////// USER  INTERFACE ///////////////////////////////////
//return 0 means Fail, others OK
/**
 * @brief		This function is used to add new software timer task
 * @param[in]	func - callback function for software timer task
 * @param[in]	interval_us - the interval for software timer task
 * @return      0 - timer task is full, add fail
 * 				1 - create successfully
 */
int 	blt_soft_timer_add(blt_timer_callback_t func, u32 interval_us);

/**
 * @brief		This function is used to delete timer tasks
 * @param[in]	func - callback function for software timer task
 * @return      0 - delete fail
 * 				1 - delete successfully
 */
int 	blt_soft_timer_delete(blt_timer_callback_t func);




//////////////////////// SOFT TIMER MANAGEMENT  INTERFACE ///////////////////////////////////

/**
 * @brief		This function is used to register the call back for pm_appWakeupLowPowerCb
 * @param[in]	none
 * @return      none
 */
void 	blt_soft_timer_init(void);

/**
 * @brief		This function is used to manage software timer tasks
 * @param[in]	type - the type for trigger
 * @return      none
 */
void  	blt_soft_timer_process(int type);

/**
 * @brief		Timer tasks are originally ordered. When deleting, it will
 * 				be overwritten forward, so the order will not be destroyed
 * 				and there is no need to reorder
 * @param[in]	index - the index for some software timer task
 * @return      0 - delete fail
 * 				other - delete successfully
 */
int 	blt_soft_timer_delete_by_index(u8 index);

/**
 * @brief		This function is used to check the current time is what the timer expects or not
 * @param[in]	e - callback function for software timer task
 * @return		none
 */
int is_timer_expired(blt_timer_callback_t *e);


#endif /* BLT_SOFT_TIMER_H_ */
