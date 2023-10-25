/********************************************************************************************************
 * @file	ll_scan.h
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
#ifndef LL_SCAN_H_
#define LL_SCAN_H_





/**
 * @brief      for user to initialize scanning module
 * @param	   none
 * @return     none
 */
void 		blc_ll_initScanning_module(u8 *public_adr);


/**
 * @brief      This function is used to set the scan parameters
 * @param[in]  scan_type - passive Scanning or active scanning.
 * @param[in]  scan_interval - time interval from when the Controller started its last LE scan until it begins the subsequent LE scan
 * @param[in]  scan_window - The duration of the LE scan.
 * @param[in]  ownAddrType - Own_Address_Type
 * @param[in]  scan_fp - Scanning_Filter_Policy
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_ll_setScanParameter (scan_type_t scan_type, u16 scan_interval, u16 scan_window, own_addr_type_t  ownAddrType, scan_fp_type_t scan_fp);


/**
 * @brief	   enable or disable legacy scanning.
 * @param[in]  scan_enable
 * @param[in]  filter_duplicate - controls whether the Link Layer should filter out
 * 								  duplicate advertising reports (Filtering_Enabled) to the Host,
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_ll_setScanEnable (scan_en_t scan_enable, dupFilter_en_t filter_duplicate);


/**
 * @brief      This function is used to add scan state in advertise state of slave role.
 * @param[in]  none.
 * @return     Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t    blc_ll_addScanningInAdvState(void);


/**
 * @brief      This function is used to remove scan state in advertise state of slave role.
 * @param[in]  none.
 * @return      Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t    blc_ll_removeScanningFromAdvState(void);


/**
 * @brief      This function is used to add scan state in connect state of slave role.
 * @param[in]  none.
 * @return     Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t    blc_ll_addScanningInConnSlaveRole(void);


/**
 * @brief      This function is used to remove scan state in connect state of slave role.
 * @param[in]  none.
 * @return      Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t    blc_ll_removeScanningFromConnSLaveRole(void);


/**
 * @brief      This function is used to determine whether scan request is sent for all advertising.
 * @param[in]  scan_req_filter_enable - Scan request filter enable.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t   blc_ll_scanReq_filter_en(u8 scan_req_filter_enable);

/**
 * @brief      This function is used to set Scan channel.
 * @param[in]  scan_channelMap - channel map
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t bls_ll_setScanChannelMap(adv_chn_map_t scan_channelMap);



#endif /* LL_SCAN_H_ */
