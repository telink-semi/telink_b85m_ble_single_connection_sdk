/********************************************************************************************************
 * @file    adpcm.h
 *
 * @brief   This is the header file for B85
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef ADPCM_H_
#define ADPCM_H_

/**
 * @brief     This function serves to compress the data through the ADPCM algorithm
 * @param[in] ps - The first address of the memory that points to the data before compression
 * @param[in] len - len of the data before compression
 * @param[in] pds - The first address of the memory that points to the data after compression
 * @param[in] start - start flag, it should be 1 if you want to use this API to compress data
 * @return    none
 */
void mic_to_adpcm_split (signed short *ps, int len, signed short *pds, int start);

/**
 * @brief     This function serves to decompress the data through the ADPCM algorithm
 * @param[in] ps - The first address of the memory that points to the data to be decompressed
 * @param[in] pd - The first address of the memory that points to the data compressed and restored
 * @param[in] len - len of the data before compression
 * @return    none
 */
void adpcm_to_pcm (signed short *ps, signed short *pd, int len);

#endif /* ADPCM_H_ */
