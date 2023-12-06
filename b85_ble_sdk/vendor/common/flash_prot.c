/********************************************************************************************************
 * @file	flash_prot.c
 *
 * @brief	This is the source file for BLE SDK
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "flash_prot.h"


_attribute_ble_data_retention_	_attribute_aligned_(4)	flash_prot_t	blc_flashProt;

/* this pointer is used only by stack, when OTA write or erase flash, will give a callback to application layer */
_attribute_ble_data_retention_	_attribute_aligned_(4)	flash_prot_op_callback_t 		flash_prot_op_cb = NULL;


#if (APP_FLASH_PROTECTION_ENABLE)

_attribute_ble_data_retention_	_attribute_aligned_(4)	flash_lock_t 				flash_lock_mid = NULL;
_attribute_ble_data_retention_	_attribute_aligned_(4)	flash_unlock_t 				flash_unlock_mid = NULL;
_attribute_ble_data_retention_	_attribute_aligned_(4)	flash_get_lock_status_t 	flash_get_lock_status_mid = NULL;


_attribute_ble_data_retention_	u16	flash_unlock_status; //record flash unlock status for different flash type


/**
 * @brief      this function is used to register callback for stack, when OTA write or erase flash, send event to application layer
 * @param[in]  cb - flash protection operation callback function
 * @return     none
 */
void blc_appRegisterStackFlashOperationCallback(flash_prot_op_callback_t cb)
{
	flash_prot_op_cb = cb;
}


#if (!MCU_SUPPORT_MULTI_PRIORITY_IRQ)
/**
 * @brief 		this function is used to replace Flash driver API "flash_write_status", solving BLE connection issue on MCU do not support multiple priority IRQ
 * @param[in]  	type	- the type of status.8 bit or 16 bit.
 * @param[in]  	data	- the value of status.
 * @return 		none.
 */
void flash_write_status(flash_status_typedef_e type , unsigned short data)
{
	/* If MCU do not support multiple priority IRQ, Flash write status duration influencing BLE RF IRQ, then lead to BLE data error
	 * use  "blc ll_write_flash_status" to solve the issue, SDK internal will find idle timing to write safely. */
	blc_ll_write_flash_status(type, data);
}
#endif


/**
 * @brief      this function is used to initialize flash protection.Block size of lock is a sample, user can change it according to bin size.
 * 				 This function only shows the flash supported by TELINK testing. If user use other flash, please follow the code below.
 * @param[in]  none
 * @return     none
 */
void flash_protection_init(void)
{
	if(!blc_flash_mid){
		blc_flash_mid = flash_read_mid();
	}

	/* According to the flash mid, execute the corresponding lock flash API. */
	switch(blc_flash_mid)
	{
		#if (FLASH_ZB25WD40B_SUPPORT_EN)
			case MID13325E:
				flash_lock_mid = (flash_lock_t)flash_lock_mid13325e;
				flash_unlock_mid = flash_unlock_mid13325e;
				flash_get_lock_status_mid = (flash_get_lock_status_t)flash_get_lock_block_mid13325e;
				flash_unlock_status = FLASH_LOCK_NONE_MID13325E;
				break;
		#endif

		#if (FLASH_ZB25WD80B_SUPPORT_EN)
			case MID14325E:
				flash_lock_mid = (flash_lock_t)flash_lock_mid14325e;
				flash_unlock_mid = flash_unlock_mid14325e;
				flash_get_lock_status_mid = (flash_get_lock_status_t)flash_get_lock_block_mid14325e;
				flash_unlock_status = FLASH_LOCK_NONE_MID14325E;
				break;
		#endif


		#if (FLASH_GD25LD40C_SUPPORT_EN || FLASH_GD25LD40E_SUPPORT_EN)
			case MID1360C8:
				flash_lock_mid = (flash_lock_t)flash_lock_mid1360c8;
				flash_unlock_mid = flash_unlock_mid1360c8;
				flash_get_lock_status_mid = (flash_get_lock_status_t)flash_get_lock_block_mid1360c8;
				flash_unlock_status = FLASH_LOCK_NONE_MID1360C8;
				break;
		#endif

		#if (FLASH_GD25LD80C_SUPPORT_EN || FLASH_GD25LD80E_SUPPORT_EN)
			case MID1460C8:
				flash_lock_mid = (flash_lock_t)flash_lock_mid1460c8;
				flash_unlock_mid = flash_unlock_mid1460c8;
				flash_get_lock_status_mid = (flash_get_lock_status_t)flash_get_lock_block_mid1460c8;
				flash_unlock_status = FLASH_LOCK_NONE_MID1460C8;
				break;
		#endif

		default:
			break;
	}
}

/**
 * @brief      this function is used to change application lock block definition to flash lock block definition according to flash type used
 * 			   attention: only processed a few lock block size for SDK demo, if you have more lock block size, please
 * 			              add more code by yourself
 * @param[in]  app_lock_block - application lock block definition
 * @return     flash_lock_block_size - The size of the lock block size of flash.
 */
u16 flash_change_app_lock_block_to_flash_lock_block(flash_app_lock_e app_lock_block)
{
	u32 flash_lock_block_size = 0;

	switch(blc_flash_mid)
	{
		#if (FLASH_ZB25WD40B_SUPPORT_EN) //512K capacity
			case MID13325E:
				if(app_lock_block == FLASH_LOCK_LOW_256K){
					flash_lock_block_size = FLASH_LOCK_LOW_256K_MID13325E;
					tlkapi_printf(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] flash lock low 256K block!\n");
				}
				else if(app_lock_block == FLASH_LOCK_LOW_512K){
					/* attention 1: use can change this value according to application
					 * attention 2: can not lock stack SMP data storage area
					 * attention 3: firmware size under protection is not 512K, user should calculate
					 * demo code: choose 448K, leave at 64K for system data(SMP storage data & calibration data & MAC address) and user data,
					 *            now firmware size under protection is 448K - 256K = 192K
					 * if this demo can not meet your requirement, you should change !!!*/
					flash_lock_block_size = FLASH_LOCK_LOW_448K_MID13325E;
					tlkapi_printf(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] flash lock low 448K block!\n");
				}
				else{
					blc_flashProt.init_err = 1; //can not use LOCK LOW 1M for 512K capacity flash
				}
				break;
		#endif

		#if (FLASH_ZB25WD80B_SUPPORT_EN) //1M capacity
			case MID14325E:
				if(app_lock_block == FLASH_LOCK_LOW_256K || app_lock_block == FLASH_LOCK_LOW_512K){
					/* attention that :This flash type, minimum lock size is 768K, do not support 256K or other value
					 * demo code will lock 768K when user set OTA 128K or 256K as multiple boot address,
					 * system data(SMP storage data & calibration data & MAC address) is OK;
					 * user data must be stored in flash address bigger than 768K !!!
					 * if this demo code lock area do not meet your requirement, you can change it !!!*/
					flash_lock_block_size = FLASH_LOCK_LOW_768K_MID14325E;
					tlkapi_printf(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] flash lock low 768K block!\n");
				}
				else if(app_lock_block == FLASH_LOCK_LOW_1M){
					/* attention 1: use can change this value according to application
					 * attention 2: can not lock stack SMP data storage area
					 * attention 3: firmware size under protection is not 1M, user should calculate
					 * demo code: choose 960K, leave 64K for system data(SMP storage data & calibration data & MAC address) and user data,
					 * 			  now firmware size under protection is 960K - 512K = 448K
					 * if this demo can not meet your requirement, you should change !!! */
					flash_lock_block_size = FLASH_LOCK_LOW_960K_MID14325E;
					tlkapi_printf(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] flash lock low 960K block!\n");
				}
				break;
		#endif


		#if (FLASH_GD25LD40C_SUPPORT_EN || FLASH_GD25LD40E_SUPPORT_EN) //512K capacity
			case MID1360C8:
				if(app_lock_block == FLASH_LOCK_LOW_256K){
					flash_lock_block_size = FLASH_LOCK_LOW_256K_MID1360C8;
					tlkapi_printf(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] flash lock low 256K block!\n");
				}
				else if(app_lock_block == FLASH_LOCK_LOW_512K){
					/* attention 1: use can change this value according to application
					 * attention 2: can not lock stack SMP data storage area
					 * attention 3: firmware size under protection is not 512K, user should calculate
					 * demo code: choose 448K, leave at 64K for system data(SMP storage data & calibration data & MAC address) and user data,
					 *            now firmware size under protection is 448K - 256K = 192K
					 * if this demo can not meet your requirement, you should change !!!*/
					flash_lock_block_size = FLASH_LOCK_LOW_448K_MID1360C8;
					tlkapi_printf(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] flash lock low 448K block!\n");
				}
				else{
					blc_flashProt.init_err = 1; //can not use LOCK LOW 1M for 512K capacity flash
				}
				break;
		#endif

		#if (FLASH_GD25LD80C_SUPPORT_EN || FLASH_GD25LD80E_SUPPORT_EN) //1M capacity
			case MID1460C8:
				if(app_lock_block == FLASH_LOCK_LOW_256K || app_lock_block == FLASH_LOCK_LOW_512K){
					/* attention that :This flash type, minimum lock size is 768K, do not support 256K or other value
					 * demo code will lock 768K when user set OTA 128K or 256K as multiple boot address,
					 * system data(SMP storage data & calibration data & MAC address) is OK;
					 * user data must be stored in flash address bigger than 768K !!!
					 * if this demo code lock area do not meet your requirement, you can change it !!!*/
					flash_lock_block_size = FLASH_LOCK_LOW_768K_MID1460C8;
					tlkapi_printf(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] flash lock low 768K block!\n");
				}
				else if(app_lock_block == FLASH_LOCK_LOW_1M){
					/* attention 1: use can change this value according to application
					 * attention 2: can not lock stack SMP data storage area
					 * attention 3: firmware size under protection is not 1M, user should calculate
					 * demo code: choose 960K, leave 64K for system data(SMP storage data & calibration data & MAC address) and user data,
					 * 			  now firmware size under protection is 960K - 512K = 448K
					 * if this demo can not meet your requirement, you should change !!! */
					flash_lock_block_size = FLASH_LOCK_LOW_960K_MID1460C8;
					tlkapi_printf(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] flash lock low 960K block!\n");
				}
				break;
		#endif

		default:
			break;
	}

	return flash_lock_block_size;
}

/**
 * @brief      this function is used to lock flash.
 * @param[in]  flash_lock_block - flash lock block, different value for different flash type
 * @return     none
 */
void flash_lock(unsigned int flash_lock_cmd)
{
	if(blc_flashProt.init_err){
		return;
	}

	u16 cur_lock_status = flash_get_lock_status_mid();

	if(cur_lock_status == flash_lock_cmd){ //lock status is what we want, no need lock again

	}
	else{ //unlocked or locked block size is not what we want

		if(cur_lock_status != flash_unlock_status){ //locked block size is not what we want, need unlock first
			for(int i = 0; i < 3; i++){ //Unlock flash up to 3 times to prevent failure.
				flash_unlock_mid();
				cur_lock_status = flash_get_lock_status_mid();

				if(cur_lock_status == flash_unlock_status){ //unlock success
					break;
				}
			}
		}

		for(int i = 0; i < 3; i++) //Lock flash up to 3 times to prevent failure.
		{
			flash_lock_mid(flash_lock_cmd);
			cur_lock_status = flash_get_lock_status_mid();
			if(cur_lock_status == flash_lock_cmd){  //lock OK
				break;
			}
		}
	}
}

/**
 * @brief      this function is used to unlock flash.
 * @param[in]  none
 * @return     none
 */
void flash_unlock(void)
{
	if(blc_flashProt.init_err){
		return;
	}

	u16 cur_lock_status = flash_get_lock_status_mid();

	if(cur_lock_status != flash_unlock_status){ //not in lock status
		for(int i = 0; i < 3; i++){ //Unlock flash up to 3 times to prevent failure.
			flash_unlock_mid();
			cur_lock_status = flash_get_lock_status_mid();

			if(cur_lock_status == flash_unlock_status){ //unlock success
				break;
			}
		}
	}
}



#endif
