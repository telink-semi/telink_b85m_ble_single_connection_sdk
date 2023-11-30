/********************************************************************************************************
 * @file	mcu_config.h
 *
 * @brief	This is the header file for B85
 *
 * @author	Driver Group
 * @date	May 8,2018
 *
 * @par		Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef DRIVERS_B85_EXT_DRIVER_MCU_CONFIG_H_
#define DRIVERS_B85_EXT_DRIVER_MCU_CONFIG_H_

/*
 * attention: user can not change any code in this file,
 * 			  these are hardware feature !!!
 */


/*
	Flash Type	uid CMD			MID		Company
	ZB25WD40B	0x4b		0x13325E	ZB
	ZB25WD80B	0x4b		0x14325E	ZB
	GD25LD40C	0x4b		0x1360C8	GD
	GD25LD40E	0x4b		0x1360C8	GD
	GD25LD80C	0x4b(AN)	0x1460C8	GD
	GD25LD80E	0x4b(AN)	0x1460C8	GD
 */
#define FLASH_ZB25WD40B_SUPPORT_EN					1
#define FLASH_ZB25WD80B_SUPPORT_EN					1
#define FLASH_GD25LD40C_SUPPORT_EN					1
#define FLASH_GD25LD40E_SUPPORT_EN					1
#define FLASH_GD25LD80C_SUPPORT_EN					1
#define FLASH_GD25LD80E_SUPPORT_EN					1



/**
 * @brief	this MCU do not support multiple priority interrupt, do not change this macro !!!
 */
#define	MCU_SUPPORT_MULTI_PRIORITY_IRQ							0


/**
 * @brief	this MCU do not support Hardware secure boot, do not change this macro !!!
 */
#define HARDWARE_SECURE_BOOT_SUPPORT_EN							0


/**
 * @brief	this MCU do not support Hardware firmware encryption, do not change this macro !!!
 */
#define HARDWARE_FIRMWARE_ENCRYPTION_SUPPORT_EN					0



#endif
