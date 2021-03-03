/********************************************************************************************************
 * @file	 main.c
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

#include "tl_common.h"
#include "drivers.h"
#include "rc_ir_learn.h"
#include "stack/ble/ble.h"
#include "app.h"
#include "battery_check.h"


extern void deep_wakeup_proc(void);

extern void rc_ir_irq_prc(void);


/**
 * @brief   IRQ handler
 * @param   none.
 * @return  none.
 */
_attribute_ram_code_ void irq_handler(void)
{
#if (REMOTE_IR_ENABLE)
	rc_ir_irq_prc();
#endif

#if (REMOTE_IR_LEARN_ENABLE)
	u32 src = reg_irq_src;
	if ((src & IR_LEARN_INTERRUPT_MASK))
	{
		ir_learn_irq_handler();
	}
#endif

	irq_blt_sdk_handler ();

#if (BLE_PHYTEST_MODE != PHYTEST_MODE_DISABLE)
	extern void irq_phyTest_handler(void);
	irq_phyTest_handler();
#endif

}




/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main (void)    //must run in ramcode
{

	DBG_CHN0_LOW;   //debug

	blc_pm_select_internal_32k_crystal();

	#if(MCU_CORE_TYPE == MCU_CORE_825x)
		cpu_wakeup_init();
	#elif(MCU_CORE_TYPE == MCU_CORE_827x)
		cpu_wakeup_init(LDO_MODE,EXTERNAL_XTAL_24M);
	#endif

	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init( !deepRetWakeUp );  //analog resistance will keep available in deepSleep mode, so no need initialize again

	clock_init(SYS_CLK_TYPE);

	if(!deepRetWakeUp){//read flash size
		user_init_battery_power_check(); //battery check must do before flash code
		blc_readFlashSize_autoConfigCustomFlashSector();
		#if FIRMWARE_CHECK_ENABLE
			//Execution time is in ms.such as:48k fw,16M crystal clock,need about 290ms.
			if(flash_fw_check(0xffffffff)){ //retrun 0, flash fw crc check ok. retrun 1, flash fw crc check fail
				while(1);				    //Users can process according to the actual application.
			}
		#endif
	}

	blc_app_loadCustomizedParameters();  //load customized freq_offset cap value

	if( deepRetWakeUp ){
		user_init_deepRetn ();
	}
	else{
		#if FIRMWARES_SIGNATURE_ENABLE
			blt_firmware_signature_check();
		#endif
		user_init_normal ();
	}


    irq_enable();



	while (1) {
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		main_loop ();
	}

}

