/********************************************************************************************************
 * @file	flash_prot.h
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
#ifndef VENDOR_COMMON_FLASH_PROT_H_
#define VENDOR_COMMON_FLASH_PROT_H_


#ifndef APP_FLASH_PROTECTION_ENABLE
#define APP_FLASH_PROTECTION_ENABLE					0   //enable or disable
#endif

#ifndef APP_FLASH_PROT_LOG_EN
#define APP_FLASH_PROT_LOG_EN						0
#endif


/**
 * @brief    application lock block definition
 *   Each kind of IC have multiple flash supported, e.g. now (2023.05) B92 have 5 kind of flash supported.
 *   Different flash, especially form different vendor(such as PUYA and GD), do not use all same lock block size.
 *   If use want protect certain range of flash, they must consider different kind of flash supported lock block size.
 *   Here, we just put a few of block size to show how to use flash protection, for typically multiple boot device with firmware size smaller
 *   than 448K(for 1M capacity flash, leave 64K for system data or user data. if user need more data area, should change flash lock size)
 *   If you want use more specific flash protection area, please add by yourself.
 */
typedef enum{
	FLASH_LOCK_LOW_256K			=	2,		//000000h-03FFFFh

	/* attention: for 512K capacity flash, can not lock all, should leave some upper sector for system data and user data */
	FLASH_LOCK_LOW_512K			=	3,		//000000h-07FFFFh

	/* attention: for 1M capacity flash, can not lock all, should leave some upper sector for system data and user data */
	FLASH_LOCK_LOW_1M			=   4,
}flash_app_lock_e;



typedef struct{
	u8	init_err;

}flash_prot_t;
extern flash_prot_t	blc_flashProt;



typedef void  (*flash_lock_t)(unsigned int);
typedef void  (*flash_unlock_t)(void);

typedef unsigned short  (*flash_get_lock_status_t)(void);


typedef void  (*flash_prot_op_callback_t)(u8, u32, u32);
extern	flash_prot_op_callback_t 		flash_prot_op_cb;








/*
 * @brief flash operation event
 *    Here just list all type appeared in SDK sample, including APP initialization and OTA/SMP operating flash
 *    If you have more flash write or erase action for your application, should add more type and process them
 *
 */

/* application layer event, initialization, lock flash */
#define FLASH_OP_EVT_APP_INITIALIZATION					 	1



/* stack layer event, OTA initialization, clear old firmware begin, may need unlock flash */
#define FLASH_OP_EVT_STACK_OTA_CLEAR_OLD_FW_BEGIN			10
/* stack layer event, OTA initialization, clear old firmware end , may need restore locking flash */
#define FLASH_OP_EVT_STACK_OTA_CLEAR_OLD_FW_END				11



/* stack layer event, OTA initialization, write new firmware begin, may need unlock flash */
#define FLASH_OP_EVT_STACK_OTA_WRITE_NEW_FW_BEGIN			12
/* stack layer event, OTA initialization, write new firmware begin, may need restore locking flash */
#define FLASH_OP_EVT_STACK_OTA_WRITE_NEW_FW_END				13





/**
 * @brief      this function is used to initialize flash protection.
 * @param[in]  none
 * @return     none
 */
void flash_protection_init(void);


/**
 * @brief      this function is used to register callback for stack, when OTA write or erase flash, send event to application layer
 * @param[in]  cb - flash protection operation callback function
 * @return     none
 */
void blc_appRegisterStackFlashOperationCallback(flash_prot_op_callback_t cb);

/**
 * @brief 		This function serves to set the protection of the flash.
 * @param[in]   flash_lock_cmd - flash lock block command for different flash type
 * 				e.g. for P25Q16SU, command is selected from "mid156085_lock_block_e"
 * @return 		none.
 */
void flash_lock(unsigned int flash_lock_cmd);

/**
 * @brief 		This function serves to release flash protection.
 * @param[in]   none.
 * @return 		none.
 */
void flash_unlock(void);

/**
 * @brief      this function is used to change application lock block definition to flash lock block definition according to flash type used
 * 			   attention: only processed a few lock block size for SDK demo, if you have more lock block size, please
 * 			              add more code by yourself
 * @param[in]  app_lock_block - application lock block definition
 * @return     flash_lock_block_size - The size of the lock block size of flash.
 */
u16 flash_change_app_lock_block_to_flash_lock_block(flash_app_lock_e app_lock_block);

#endif /* VENDOR_COMMON_FLASH_PROT_H_ */
