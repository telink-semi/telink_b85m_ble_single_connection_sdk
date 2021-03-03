/********************************************************************************************************
 * @file	ll_scan.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
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


#include "ll_whitelist.h"



#define			BLS_FLAG_SCAN_ENABLE					BIT(0)


#define			BLS_FLAG_SCAN_IN_ADV_MODE				BIT(5)
#define			BLS_FLAG_SCAN_IN_SLAVE_MODE				BIT(6)



//scan_enable_t
typedef enum {
	BLC_SCAN_DISABLE = 0x00,
	BLC_SCAN_ENABLE  = 0x01,
} scan_en_t;


//duplicate_filter_enable_t
typedef enum {
	DUP_FILTER_DISABLE = 0x00,
	DUP_FILTER_ENABLE  = 0x01,
} dupFilter_en_t;


typedef enum {
	SCAN_TYPE_PASSIVE = 0x00,
	SCAN_TYPE_ACTIVE  = 0x01,
} scan_type_t;



// Advertise report event type
typedef enum {
	ADV_REPORT_EVENT_TYPE_ADV_IND = 0x00,
	ADV_REPORT_EVENT_TYPE_DIRECT_IND = 0x01,
	ADV_REPORT_EVENT_TYPE_SCAN_IND = 0x02,
	ADV_REPORT_EVENT_TYPE_NONCONN_IND = 0x03,
	ADV_REPORT_EVENT_TYPE_SCAN_RSP = 0x04,
} advReportEventType_t;



typedef struct {
	u8		scan_en;
	u8		scan_type;
	u8		scan_filterPolicy;
	u8		filter_dup;

	u8		scanDevice_num;
	u8		scanRspDevice_num;
	u8		scan_extension_mask;
	u8 		rsvd;
//	s8		T_SCAN_REQ_INTVL;



	//u32		scan_interval;
}st_ll_scan_t;



extern 			rf_packet_scan_req_t	pkt_scan_req;


typedef int (*ll_procScanPkt_callback_t)(u8 *, u8 *, u32);
typedef int (*ll_procScanDat_callback_t)(u8 *);

extern ll_procScanDat_callback_t  blc_ll_procScanDatCb;
extern ll_procScanPkt_callback_t  blc_ll_procScanPktCb;





/******************************* User Interface  ************************************/
void 		blc_ll_initScanning_module(u8 *public_adr);


ble_sts_t 	blc_ll_setScanParameter (scan_type_t scan_type, u16 scan_interval, u16 scan_window, own_addr_type_t  ownAddrType, scan_fp_type_t scanFilter_policy);
ble_sts_t 	blc_ll_setScanEnable (scan_en_t scan_enable, dupFilter_en_t filter_duplicate);



ble_sts_t    blc_ll_addScanningInAdvState(void);
ble_sts_t    blc_ll_removeScanningFromAdvState(void);
ble_sts_t    blc_ll_addScanningInConnSlaveRole(void);
ble_sts_t    blc_ll_removeScanningFromConnSLaveRole(void);



/************************* Stack Interface, user can not use!!! ***************************/
int			blc_ll_filterAdvDevice (u8 type, u8 * mac);
int 		blc_ll_addScanRspDevice(u8 type, u8 *mac);
bool 		blc_ll_isScanRspReceived(u8 type, u8 *mac);
void 		blc_ll_clearScanRspDevice(void);


int  		blc_ll_procScanPkt(u8 *raw_pkt, u8 *new_pkt, u32 tick_now);
int  		blc_ll_procScanData(u8 *raw_pkt);

void 		blc_ll_switchScanChannel (int, int);


#endif /* LL_SCAN_H_ */
