/********************************************************************************************************
 * @file    sdk_version.c
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "sdk_version.h"
/*
 * Release Tool need to change this macro to match the release version,
 * the replace rules is: "$$$B85m_driver_sdk_"#sdk_version_num"$$$", The "#sdk_version_num"
 * will replace with this macro value.
 */
volatile __attribute__((section(".sdk_version"))) unsigned char sdk_version[] = {SDK_VERSION(SDK_VERSION_NUM)};

#if(PATCH_NUM)
volatile __attribute__((section(".sdk_version"))) unsigned char patch_version[] = {PATCH_VERSION(PATCH_NUM)};
#endif

#if(CUSTOM_MAJOR_VERSION||CUSTOM_MINOR_VERSION)
volatile __attribute__((section(".sdk_version"))) unsigned char custom_version[] = {CUSTOM_VERSION(CUSTOM_VERSION_NUM)};
#endif
