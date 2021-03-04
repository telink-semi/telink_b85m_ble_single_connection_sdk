/********************************************************************************************************
 * @file	hids.h
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
#pragma once

#include "tl_common.h"
//#include "../att.h"

/** @addtogroup  TELINK_BLE_STACK TELINK BLE Stack
 *  @{
 */

/** @addtogroup  SERVICE_MODULE Service
 *  @{
 */


/** @addtogroup  HIDS_Module Hids
 *  @{
 */
 
/** @addtogroup  Hids_Constant Hids Constants
 *  @{
 */

/**
 *  @brief  Definition for Characteristics UUID
 */

/** @addtogroup hids_uuid Hids Charactersitc UUID
 * @{
 */

#define CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT          0x2A22    //!< HID Boot Keyboard Input Report
#define CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT         0x2A32    //!< HID Boot Keyboard Output Report
#define CHARACTERISTIC_UUID_HID_BOOT_MOUSE_INPUT        0x2A33    //!< HID Boot Mouse Input Report
#define CHARACTERISTIC_UUID_HID_INFORMATION             0x2A4A    //!< HID Information
#define CHARACTERISTIC_UUID_HID_REPORT_MAP              0x2A4B    //!< HID Report Map
#define CHARACTERISTIC_UUID_HID_CONTROL_POINT           0x2A4C    //!< HID Control Point
#define CHARACTERISTIC_UUID_HID_REPORT                  0x2A4D    //!< HID Report
#define CHARACTERISTIC_UUID_HID_PROTOCOL_MODE           0x2A4E    //!< HID Protocol Mode

/** @} end of group hids_uuid */


/** @addtogroup  HID_REPORT_ID Hid Report Id
 *  @{
 */

/**
 *  @brief  HID Report ID
 */
 
#define HID_REPORT_ID_KEYBOARD_INPUT                    1   //!< Keyboard input report ID
#define HID_REPORT_ID_CONSUME_CONTROL_INPUT             2   //!< Consumer Control input report ID
#define HID_REPORT_ID_MOUSE_INPUT                       3   //!< Mouse input report ID
#define HID_REPORT_ID_GAMEPAD_INPUT						4   //!< Gamepad  input report ID
#define HID_REPORT_ID_LED_OUT                           0   //!< LED output report ID
#define HID_REPORT_ID_FEATURE                           0   //!< Feature report ID

#define HID_REPORT_ID_CTRL_VOICE                        9

#define HID_REPORT_ID_AUDIO_FIRST_INPUT                 10//250
#define HID_REPORT_ID_AUDIO_SECND_INPUT                 11//251
#define HID_REPORT_ID_AUDIO_THIRD_INPUT                 12//247

/** @} end of group HID_REPORT_ID */


/** @addtogroup  HID_REPORT_TYPE Hid Report Type
 *  @{
 */

/**
 *  @brief  HID Report type
 */

#define HID_REPORT_TYPE_INPUT                           1
#define HID_REPORT_TYPE_OUTPUT                          2
#define HID_REPORT_TYPE_FEATURE                         3

/** @} end of group HID_REPORT_TYPE */


/** @addtogroup  HID_PROTOCOL_MODE Hid Protocol Mode
 *  @{
 */

/**
 *  @brief  Definition for HID protocol mode
 */
#define HID_PROTOCOL_MODE_BOOT                          0
#define HID_PROTOCOL_MODE_REPORT                        1
#define DFLT_HID_PROTOCOL_MODE                          HID_PROTOCOL_MODE_REPORT

/** @} end of group HID_PROTOCOL_MODE */


/** @addtogroup  HID_INFOR_FLAGS Hid Information Flags
 *  @{
 */
/**
 *  @brief  Definition for HID information flags
 */
#define HID_FLAGS_REMOTE_WAKE                           0x01      // RemoteWake
#define HID_FLAGS_NORMALLY_CONNECTABLE                  0x02      // NormallyConnectable

/** @} end of group HID_INFOR_FLAGS */

#define HID_KEYCODE_CC_RELEASE                          0x00
#define HID_KEYCODE_CC_VOL_UP                           0x01
#define HID_KEYCODE_CC_VOL_DN                           0x02


/** @} end of group Hids_Constant */




/** @addtogroup  Hids_Callbacks Hids Callbacks
 *  @{
 */

/**
 *  @brief  Definition argutment type of report change callback function
 */
typedef struct {
    u8 id;
    u8 type;
    u8 len;
    u8 value[1];
} reportChange_t;

/**
 *  @brief  Definition argutment type of CCC change callback function
 */
typedef struct {
    u8 id;
    u8 type;
    u8 value;
} clientCharCfgChange_t;

/**
 *  @brief  Definition client characterist configuration of report changed callback function type for user application
 */
typedef void (*hids_clientCharCfgChangeCb_t)(clientCharCfgChange_t* cccVal);

/**
 *  @brief  Definition report value changed callback function type for user application
 */
typedef void (*hids_reportChangeCb_t)(reportChange_t* reportVal);

/**
 *  @brief  Definition report value changed callback function type for user application
 */
typedef void (*hids_protoModeChangeCb_t)(u32 protoMode);

/**
 *  @brief  Definition report value changed callback function type for user application
 */
typedef void (*hids_ctrlPointChangeCb_t)(u32 ctrlPoint);

/**
 *  @brief  Definition for foundation command callbacks.
 */
typedef struct {
    hids_reportChangeCb_t         reportChangeCbFunc;     //!< report value changed callback function
    hids_protoModeChangeCb_t      protoModeChangeCbFunc;  //!< protocol mode value changed callback function
    hids_ctrlPointChangeCb_t      ctrlPointChangeCbFunc;  //!< control point value changed callback function
    hids_clientCharCfgChangeCb_t  cccChangedCbFunc;       //!< ccc of report changed callback function
} hids_callbacks_t;

/** @} end of group Hids_Callbacks */


 /** @addtogroup  HIDS_Variables  Hids Variables
 *  @{
 */
 


/** @} end of group HIDS_Variables */


/** @addtogroup  HIDS_Functions Hids APIs
 *  @{
 */



/** @} end of group HIDS_Functions */

/** @} end of group HIDS_Module */

/** @} end of group SERVICE_MODULE */

/** @} end of group TELINK_BLE_STACK */
