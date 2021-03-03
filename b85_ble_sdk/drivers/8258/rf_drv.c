/********************************************************************************************************
 * @file     rf_drv.c 
 *
 * @brief    This is the RF file for TLSR8258
 *
 * @author	 Driver Group
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *			 2.add ant mode and rffe api(Jan. 2 2019)
 * @version  A001
 *         
 *******************************************************************************************************/

#include "register.h"
#include "analog.h"
#include "clock.h"
#include "pm.h"
#include "rf_drv.h"
#include "bsp.h"
#include "compiler.h"
#define		RF_MANUAL_AGC_MAX_GAIN			1
#define     REG_RF_ACCESS_CODE_GTH          REG_ADDR8(0x420)
#define 	ACCESS_CODE_BASE_PIPE0    		(0x800408)
#define 	ACCESS_CODE_BASE_PIPE2    		(0x800418)


static RF_StatusTypeDef RF_TRxState = RF_MODE_TX;


_attribute_data_retention_ RF_ModeTypeDef   g_RFMode;

unsigned char g_RFRxPingpongEn;

#define FRE_OFFSET		0
#define MAX_RF_CHANNEL  16

unsigned char txPower_index = 0;

const RF_PowerTypeDef rf_power_Level_list[60] =
{
	 /*VBAT*/
	 RF_POWER_P10p46dBm,
	 RF_POWER_P10p29dBm,
	 RF_POWER_P10p01dBm,
	 RF_POWER_P9p81dBm,
	 RF_POWER_P9p48dBm,
	 RF_POWER_P9p24dBm,
	 RF_POWER_P8p97dBm,
	 RF_POWER_P8p73dBm,
	 RF_POWER_P8p44dBm,
	 RF_POWER_P8p13dBm,
	 RF_POWER_P7p79dBm,
	 RF_POWER_P7p41dBm,
	 RF_POWER_P7p02dBm,
	 RF_POWER_P6p60dBm,
	 RF_POWER_P6p14dBm,
	 RF_POWER_P5p65dBm,
	 RF_POWER_P5p13dBm,
	 RF_POWER_P4p57dBm,
	 RF_POWER_P3p94dBm,
	 RF_POWER_P3p23dBm,
	 /*VANT*/
	 RF_POWER_P3p01dBm,
	 RF_POWER_P2p81dBm,
	 RF_POWER_P2p61dBm,
	 RF_POWER_P2p39dBm,
	 RF_POWER_P1p99dBm,
	 RF_POWER_P1p73dBm,
	 RF_POWER_P1p45dBm,
	 RF_POWER_P1p17dBm,
	 RF_POWER_P0p90dBm,
	 RF_POWER_P0p58dBm,
	 RF_POWER_P0p04dBm,
	 RF_POWER_N0p14dBm,
	 RF_POWER_N0p97dBm,
	 RF_POWER_N1p42dBm,
	 RF_POWER_N1p89dBm,
	 RF_POWER_N2p48dBm,
	 RF_POWER_N3p03dBm,
	 RF_POWER_N3p61dBm,
	 RF_POWER_N4p26dBm,
	 RF_POWER_N5p03dBm,
	 RF_POWER_N5p81dBm,
	 RF_POWER_N6p67dBm,
	 RF_POWER_N7p65dBm,
	 RF_POWER_N8p65dBm,
	 RF_POWER_N9p89dBm,
	 RF_POWER_N11p4dBm,
	 RF_POWER_N13p29dBm,
	 RF_POWER_N15p88dBm,
	 RF_POWER_N19p27dBm,
	 RF_POWER_N25p18dBm,
};


const unsigned char rf_chn_table[MAX_RF_CHANNEL] = {
	FRE_OFFSET+ 5, FRE_OFFSET+ 9, FRE_OFFSET+13, FRE_OFFSET+17,
	FRE_OFFSET+22, FRE_OFFSET+26, FRE_OFFSET+30, FRE_OFFSET+35,
	FRE_OFFSET+40, FRE_OFFSET+45, FRE_OFFSET+50, FRE_OFFSET+55,
	FRE_OFFSET+60, FRE_OFFSET+65, FRE_OFFSET+70, FRE_OFFSET+76,
};

const TBLCMDSET tbl_rf_init[] = {
    {0x12d2, 0x9b,  TCMD_UNDER_BOTH | TCMD_WRITE}, //DCOC_DBG0
    {0x12d3, 0x19,  TCMD_UNDER_BOTH | TCMD_WRITE}, //DCOC_DBG1
    {0x127b, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //BYPASS_FILT_1
    {0x1276, 0x50,  TCMD_UNDER_BOTH | TCMD_WRITE}, //FREQ_CORR_CFG2_0
    {0x1277, 0x73,  TCMD_UNDER_BOTH | TCMD_WRITE}, //FREQ_CORR_CFG2_1
};

const TBLCMDSET  tbl_rf_zigbee_250k[] =
{
    {0x1220, 0x04, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1221, 0x2b, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1222, 0x43, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1223, 0x86, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x122a, 0x90, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1254, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_2M_0
    {0x1255, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_2M_1
    {0x1256, 0x0c,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD2_2M_0
    {0x1257, 0x08,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD2_2M_1
    {0x1258, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_2M_0
    {0x1259, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_2M_1

    {0x400, 0x13, TCMD_UNDER_BOTH | TCMD_WRITE},//{0x400, 0x0a,	TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x420, 0x18, TCMD_UNDER_BOTH | TCMD_WRITE},

    {0x402, 0x46, TCMD_UNDER_BOTH | TCMD_WRITE}, //pre-amble


    {0x404, 0xc0, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x405, 0x04, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x421, 0x23, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x422, 0x04, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x408, 0xa7, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x409, 0x00, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x40a, 0x00, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x40b, 0x00, TCMD_UNDER_BOTH | TCMD_WRITE},
    //AGC table 2M
    {0x460, 0x36, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x46, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x51, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x61, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6d, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x78, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
};

const TBLCMDSET  tbl_rf_1m[] = {

	//these setting below is for AURA RF module BLE 1M mode
	{0x1220, 0x16,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1221, 0x0a,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1222, 0x20,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1223, 0x23,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x124a, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_0
	{0x124b, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_1
	{0x124e, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_0
	{0x124f, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_1
	//8258 script set 0x1f, but 8255 ble  SDK set 0x0f
	//BIT<4>:tx output data mode, 0: xor 32'hcccccccc; 1: xor 32'h66666666
	{0x400, 0x1f,	TCMD_UNDER_BOTH | TCMD_WRITE}, //poweron_dft: 0x1f    	//<4>default,8255 use 0 with error
	{0x401, 0x01,	TCMD_UNDER_BOTH | TCMD_WRITE},	// PN enable
	//core_402, [7:5] trailer_len,  [4:0] new 2m mode preamble length
	//8255 is 0x26, 8258 script is 0x43
	{0x402, 0x46,	TCMD_UNDER_BOTH | TCMD_WRITE},//<4:0>preamble length   	//<7:4>default
	{0x404, 0xf5,	TCMD_UNDER_BOTH | TCMD_WRITE},//BLE header and 1Mbps  BIT<5>: ble_wt(PN) enable
	{0x405, 0x04,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x420, 0x1e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //sync thre bit match
	{0x421, 0xa1,	TCMD_UNDER_BOTH | TCMD_WRITE}, //len_0_en
	{0x430, 0x36,	TCMD_UNDER_BOTH | TCMD_WRITE},//0x3e->0x36 disable tx timestamp, only use rx timestamp

    //AGC table 1M
    {0x460, 0x34, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x4f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x5f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6b, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x76, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
    {0x1276, 0x45,  TCMD_UNDER_BOTH | TCMD_WRITE}, //FREQ_CORR_CFG2_0 //This setting(1276,1277,1279) for dealing ble1m per_floor and sensitivity(1279:00¡ª>08)
    {0x1277, 0x7b,  TCMD_UNDER_BOTH | TCMD_WRITE}, //FREQ_CORR_CFG2_1 //decrease 1db on amazon chip.changed by zhiwei,confirmed by junwen.
    {0x1279, 0x08,  TCMD_UNDER_BOTH | TCMD_WRITE}, //set 1279 bit<3>:1 to deal with the ble_floor unstable (per not around 0 when power big enough sometime)
          												//changed by zhiwei,confirmed by junwen 2019/10/30
};

const TBLCMDSET  tbl_rf_2m_no_pn[] =
{
	{0x1220, 0x04,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1221, 0x2a,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1222, 0x43,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1223, 0x06,	TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1254, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_2M_0
    {0x1255, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_2M_1
    {0x1256, 0x0c,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD2_2M_0
    {0x1257, 0x08,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD2_2M_1
    {0x1258, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_2M_0
    {0x1259, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_2M_1

	{0x400, 0x1f,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x401, 0x00,	TCMD_UNDER_BOTH | TCMD_WRITE},	// PN disable
	{0x402, 0x46,	TCMD_UNDER_BOTH | TCMD_WRITE},//preamble length<4:0>
	{0x404, 0xc5,	TCMD_UNDER_BOTH | TCMD_WRITE},//BLE header and 2Mbps  BIT<5>: ble_wt(PN) disable
	{0x405, 0x04,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x420, 0x1e,	TCMD_UNDER_BOTH | TCMD_WRITE},//access code match threshold
	{0x421, 0xa1,	TCMD_UNDER_BOTH | TCMD_WRITE},
    //AGC table
    {0x460, 0x36, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x46, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x51, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x61, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6d, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x78, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
};

const TBLCMDSET  tbl_rf_1m_no_pn[] = {

	//these setting below is for AURA RF module BLE 1M mode
	{0x1220, 0x16,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1221, 0x0a,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1222, 0x20,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1223, 0x23,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x124a, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_0
	{0x124b, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_1
	{0x124e, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_0
	{0x124f, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_1
	//8258 script set 0x1f, but 8255 ble  SDK set 0x0f
	//BIT<4>:tx output data mode, 0: xor 32'hcccccccc; 1: xor 32'h66666666
	{0x400, 0x1f,	TCMD_UNDER_BOTH | TCMD_WRITE}, //poweron_dft: 0x1f    	//<4>default,8255 use 0 with error
	{0x401, 0x00,	TCMD_UNDER_BOTH | TCMD_WRITE},	// PN disable
	//core_402, [7:5] trailer_len,  [4:0] new 2m mode preamble length
	//8255 is 0x26, 8258 script is 0x43
	{0x402, 0x46,	TCMD_UNDER_BOTH | TCMD_WRITE},//<4:0>preamble length   	//<7:4>default

	{0x404, 0xd5,	TCMD_UNDER_BOTH | TCMD_WRITE},//BLE header and 1Mbps  BIT<5>: ble_wt(PN) disable

	{0x405, 0x04,	TCMD_UNDER_BOTH | TCMD_WRITE},

	{0x420, 0x1e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //sync thre bit match
	{0x421, 0xa1,	TCMD_UNDER_BOTH | TCMD_WRITE}, //len_0_en

	{0x430, 0x3e,	TCMD_UNDER_BOTH | TCMD_WRITE},
    //AGC table
    {0x460, 0x34, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x4f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x5f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6b, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x76, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
    {0x1276, 0x45,  TCMD_UNDER_BOTH | TCMD_WRITE}, //FREQ_CORR_CFG2_0
    {0x1277, 0x7b,  TCMD_UNDER_BOTH | TCMD_WRITE}, //FREQ_CORR_CFG2_1
    {0x1279, 0x08,  TCMD_UNDER_BOTH | TCMD_WRITE}, //set 1279 bit<3>:1 to deal with the ble_floor unstable (per not around 0 when power big enough sometime)
              												//changed by zhiwei,confirmed by junwen 2019/10/30
};

const TBLCMDSET  tbl_rf_2m[] =
{
	{0x1220, 0x04,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1221, 0x2a,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1222, 0x43,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1223, 0x06,	TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1254, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_2M_0
    {0x1255, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_2M_1
    {0x1256, 0x0c,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD2_2M_0
    {0x1257, 0x08,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD2_2M_1
    {0x1258, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_2M_0
    {0x1259, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_2M_1

	{0x400, 0x1f,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x401, 0x01,	TCMD_UNDER_BOTH | TCMD_WRITE},	// PN enable
	{0x402, 0x46,	TCMD_UNDER_BOTH | TCMD_WRITE},//preamble length<4:0>
	{0x404, 0xe5,	TCMD_UNDER_BOTH | TCMD_WRITE},//BLE header and 2Mbps  BIT<5>: ble_wt(PN) enable

	{0x405, 0x04,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x420, 0x1e,	TCMD_UNDER_BOTH | TCMD_WRITE},//access code match threshold
	{0x421, 0xa1,	TCMD_UNDER_BOTH | TCMD_WRITE},

	{0x430, 0x3e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<1> hd_timestamp

    //AGC table
    {0x460, 0x36, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x46, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x51, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x61, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6d, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x78, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
};

const TBLCMDSET  tbl_rf_s2_500k[] =
{
	{0x1220, 0x17,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1221, 0x0a,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1222, 0x20,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1223, 0x23,	TCMD_UNDER_BOTH | TCMD_WRITE},

	{0x1273, 0x21,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1237, 0x8c,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1236, 0xee,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1238, 0xc8,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1239, 0x7d,  TCMD_UNDER_BOTH | TCMD_WRITE},//  poweron_dft: 0x71  // //add to resolve frequency offset(add by zhaowei 2019-7-25)

	{0x124a, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_0
	{0x124b, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_1
	{0x124e, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_0
	{0x124f, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_1
	{0x400, 0x1f,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x401, 0x01,	TCMD_UNDER_BOTH | TCMD_WRITE},	// PN enable
	{0x402, 0x4a,	TCMD_UNDER_BOTH | TCMD_WRITE},//preamble length<4:0>
	{0x404, 0xf5,	TCMD_UNDER_BOTH | TCMD_WRITE},//BLE header and 1Mbps  BIT<5>: ble_wt(PN) enable

	{0x420, 0xf0,	TCMD_UNDER_BOTH | TCMD_WRITE},//access code match threshold
	{0x421, 0xa1,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x405, 0xa4,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<7>read only

	{0x430, 0x3e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<1> hd_timestamp

    //AGC table 1M
    {0x460, 0x34, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x4f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x5f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6b, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x76, TCMD_UNDER_BOTH | TCMD_WRITE} //grx_5
};

const TBLCMDSET  tbl_rf_s8_125k[] =
{
	{0x1220, 0x17,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1221, 0x0a,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1222, 0x20,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1223, 0x23,	TCMD_UNDER_BOTH | TCMD_WRITE},

	{0x1273, 0x21,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1237, 0x8c,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1236, 0xf6,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1238, 0xc8,	TCMD_UNDER_BOTH | TCMD_WRITE},

	{0x1239, 0x7d,  TCMD_UNDER_BOTH | TCMD_WRITE},  //added by junwen kangpingpian

	{0x124a, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_0
	{0x124b, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_1
	{0x124e, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_0
	{0x124f, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_1

	{0x400, 0x1f,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x401, 0x01,	TCMD_UNDER_BOTH | TCMD_WRITE},	// PN enable
	{0x402, 0x4a,	TCMD_UNDER_BOTH | TCMD_WRITE},//preamble length<4:0>
	{0x404, 0xf5,	TCMD_UNDER_BOTH | TCMD_WRITE},//BLE header and 1Mbps  BIT<5>: ble_wt(PN) enable

	{0x420, 0xf0,	TCMD_UNDER_BOTH | TCMD_WRITE},//access code match threshold
	{0x421, 0xa1,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x405, 0xb4,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<7>read only

	{0x430, 0x3e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<1> hd_timestamp

    //AGC table 1M
    {0x460, 0x34, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x4f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x5f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6b, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x76, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
};

const TBLCMDSET tbl_rf_private_250k[] = {
    {0x1220, 0x16,  TCMD_UNDER_BOTH | TCMD_WRITE}, //BW_CODE & SC_CODE
    {0x1221, 0x0a,  TCMD_UNDER_BOTH | TCMD_WRITE}, //IF_FREQ
    {0x1222, 0x20,  TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1223, 0x23,  TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x123f, 0x02,  TCMD_UNDER_BOTH | TCMD_WRITE}, //MODE_CFG_RX2_1
    {0x122b, 0x0a,  TCMD_UNDER_BOTH | TCMD_WRITE}, //DEBUG_TXRX1_1
	{0x124a, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_0
	{0x124b, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_1
	{0x124e, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_0
	{0x124f, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_1

    {0x403, 0x05,  TCMD_UNDER_BOTH | TCMD_WRITE}, //BBDBG
    {0x400, 0x1f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //TXMODE1
    {0x402, 0x44,  TCMD_UNDER_BOTH | TCMD_WRITE}, //preamble and trailer length
    {0x404, 0xda,  TCMD_UNDER_BOTH | TCMD_WRITE}, //FORMAT
    {0x421, 0xa1,  TCMD_UNDER_BOTH | TCMD_WRITE}, //RXGTH1

	{0x430, 0x3e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<1> hd_timestamp

    //AGC table 1M
    {0x460, 0x34, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x4f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x5f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6b, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x76, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
};
const TBLCMDSET  tbl_rf_private_500k[] = {
    {0x1220, 0x16, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1221, 0x0a, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1222, 0x20, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x1223, 0x23, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x123f, 0x06, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x122b, 0x06, TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x124a, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_0
	{0x124b, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_1
	{0x124e, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_0
	{0x124f, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_1

    {0x403, 0x07, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x400, 0x1f, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x402, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x404, 0xda, TCMD_UNDER_BOTH | TCMD_WRITE},
    {0x421, 0xa1, TCMD_UNDER_BOTH | TCMD_WRITE},

	{0x430, 0x3e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<1> hd_timestamp

    //AGC table 1M
    {0x460, 0x34, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x4f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x5f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6b, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x76, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
};
const TBLCMDSET  tbl_rf_private_1m[] = {
    {0x1220, 0x16, TCMD_UNDER_BOTH | TCMD_WRITE},//SC_CODE
    {0x1221, 0x0a, TCMD_UNDER_BOTH | TCMD_WRITE},//IF_FREQ	s
    {0x1222, 0x20, TCMD_UNDER_BOTH | TCMD_WRITE},//HPMC_EXP_DIFF_COUNT_L
    {0x1223, 0x23, TCMD_UNDER_BOTH | TCMD_WRITE},//HPMC_EXP_DIFF_COUNT_H
	{0x124a, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_0
	{0x124b, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_1
	{0x124e, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_0
	{0x124f, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_1
    {0x400, 0x1f, TCMD_UNDER_BOTH | TCMD_WRITE},//tx mode
    {0x402, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//preamble length
    {0x404, 0xda, TCMD_UNDER_BOTH | TCMD_WRITE},//nordic head
    {0x421, 0xa1, TCMD_UNDER_BOTH | TCMD_WRITE},//len_0_en

	{0x430, 0x3e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<1> hd_timestamp

    //AGC table
    {0x460, 0x34, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x4f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x5f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6b, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x76, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
};
const TBLCMDSET  tbl_rf_private_2m[] = {
    {0x1220, 0x04, TCMD_UNDER_BOTH | TCMD_WRITE},//SC_CODE
    {0x1221, 0x2a, TCMD_UNDER_BOTH | TCMD_WRITE},//IF_FREQ
    {0x1222, 0x43, TCMD_UNDER_BOTH | TCMD_WRITE},//HPMC_EXP_DIFF_COUNT_L
    {0x1223, 0x06, TCMD_UNDER_BOTH | TCMD_WRITE},//HPMC_EXP_DIFF_COUNT_H
    {0x1254, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_2M_0
    {0x1255, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_2M_1
    {0x1256, 0x0c,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD2_2M_0
    {0x1257, 0x08,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD2_2M_1
    {0x1258, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_2M_0
    {0x1259, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_2M_1

    {0x400, 0x1f, TCMD_UNDER_BOTH | TCMD_WRITE},//tx mode
    {0x402, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//preamble length
    {0x404, 0xca, TCMD_UNDER_BOTH | TCMD_WRITE},//nordic head
    {0x421, 0xa1, TCMD_UNDER_BOTH | TCMD_WRITE},//len_0_en

	{0x430, 0x3e,	TCMD_UNDER_BOTH | TCMD_WRITE}, //<1> hd_timestamp
    //AGC table
    {0x460, 0x36, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
    {0x461, 0x46, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
    {0x462, 0x51, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
    {0x463, 0x61, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
    {0x464, 0x6d, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
    {0x465, 0x78, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5
};


const TBLCMDSET  TblRFANT[] = {

	{0x1220, 0x16,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1221, 0x0a,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1222, 0x20,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x1223, 0x23,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x124a, 0x0e,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_0
	{0x124b, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD1_1M_1
	{0x124e, 0x09,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_0
	{0x124f, 0x0f,  TCMD_UNDER_BOTH | TCMD_WRITE}, //AGC_THRSHLD3_1M_1


	{0x400, 0x1f,	TCMD_UNDER_BOTH | TCMD_WRITE},
	{0x402, 0x43,	TCMD_UNDER_BOTH | TCMD_WRITE},//<4:0>preamble length
	{0x404, 0xdb,	TCMD_UNDER_BOTH | TCMD_WRITE},//BLE header and 1Mbps
	{0x405, 0x04,	TCMD_UNDER_BOTH | TCMD_WRITE},//access length
	{0x420, 0x20,	TCMD_UNDER_BOTH | TCMD_WRITE},//sync thre
	{0x421, 0xa1,	TCMD_UNDER_BOTH | TCMD_WRITE},//len_0_en

	//AGC table 1M
	{0x460, 0x34, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_0
	{0x461, 0x44, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_1
	{0x462, 0x4f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_2
	{0x463, 0x5f, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_3
	{0x464, 0x6b, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_4
	{0x465, 0x76, TCMD_UNDER_BOTH | TCMD_WRITE},//grx_5

};

/**
 * @brief   	This function serves to set RF  ble 1m Register initialization .
 * @param[in]   none.
 * @return  	none.
 */

_attribute_ram_code_ void rf_ble_1m_param_init(void)
{
	//core_12xx  core_13xx register must be written by byte, can not write word and half word
	write_reg8(0x12d2, 0x9b); // DCOC_DBG0
	write_reg8(0x12d3, 0x19); // DCOC_DBG1
	write_reg8(0x127b, 0x0e); //BYPASS_FILT_1
	write_reg8(0x1276, 0x45); //FREQ_CORR_CFG2_1	poweron_dft: 0x57
	write_reg8(0x1277, 0x7b); //FREQ_CORR_CFG2_0    poweron_dft: 0x73
	write_reg8(0x1279, 0x08); //set 1279 bit<3>:1 to deal with the ble_floor unstable (per not around 0 when power big enough sometime)   poweron_dft: 0x00

//	write_reg8(0x1220, 0x16); 	//poweron_dft: 0x16, no need set to save timing
//	write_reg8(0x1221, 0x0a); 	//poweron_dft: 0x0a, no need set to save timing
//	write_reg8(0x1222, 0x20); 	//poweron_dft: 0x20, no need set to save timing
//	write_reg8(0x1223, 0x23); 	//poweron_dft: 0x23, no need set to save timing
	write_reg8(0x124a, 0x0e);   // AGC_THRSHLD1_1M_0
	write_reg8(0x124b, 0x09);   // AGC_THRSHLD1_1M_1
//	write_reg8(0x124e, 0x09);   // AGC_THRSHLD3_1M_0  //poweron_dft: 0x09, no need set to save timing
//	write_reg8(0x124f, 0x0f);	// AGC_THRSHLD3_1M_1  //poweron_dft: 0x0f, no need set to save timing

//	write_reg8(0x400, 0x1f);  //poweron_dft: 0x1f, no need set to save timing
	write_reg8(0x401, 0x01);
	write_reg8(0x402, 0x46);
	write_reg8(0x404, 0xf5);
//	write_reg8(0x405, 0x04);  //poweron_dft: 0x04, no need set to save timing
//	write_reg8(0x420, 0x1e);  //poweron_dft: 0x1e, no need set to save timing
//	write_reg8(0x421, 0xa1);  //poweron_dft: 0xa1, no need set to save timing
	write_reg8(0x430, 0x36);  //0x3e->0x36 disable tx timestamp, only use rx timestamp
#if(BQB_5P0_TEST_ENABLE)
	write_reg8(0x420, 0x20); // for scan test, access code must match 32bit
#endif


	write_reg32(0x460, 0x5f4f4434);  //grx_3~0
	write_reg16(0x464, 0x766b);      //grx_5~4


	write_reg8(0xf06, 0x00);
	write_reg8(0xf0c, 0x50);
	write_reg8(0xf0e, 0x00);
	write_reg8(0xf10, 0x00);
}

/**
*	@brief     This function serves to initiate the mode of RF
*	@param[in] rf_mode  -  mode of RF
*	@return	   none.
*/

_attribute_ram_code_ void rf_drv_init (RF_ModeTypeDef rf_mode)
{
#if 1
	rf_ble_1m_param_init();
#else
	LoadTblCmdSet(tbl_rf_init, sizeof(tbl_rf_init)/sizeof(TBLCMDSET));

    if (rf_mode == RF_MODE_BLE_1M) {
    	LoadTblCmdSet (tbl_rf_1m, sizeof (tbl_rf_1m)/sizeof (TBLCMDSET));
	}
    else if(rf_mode == RF_MODE_BLE_1M_NO_PN) {
    	LoadTblCmdSet (tbl_rf_1m_no_pn, sizeof (tbl_rf_1m_no_pn)/sizeof (TBLCMDSET));
	}
    else if(rf_mode == RF_MODE_BLE_2M) {
    	LoadTblCmdSet (tbl_rf_2m, sizeof (tbl_rf_2m)/sizeof (TBLCMDSET));
	}
    else if (rf_mode == RF_MODE_LR_S2_500K) {
    	LoadTblCmdSet (tbl_rf_s2_500k, sizeof (tbl_rf_s2_500k)/sizeof (TBLCMDSET));
	}
    else if (rf_mode == RF_MODE_LR_S8_125K) {
    	LoadTblCmdSet (tbl_rf_s8_125k, sizeof (tbl_rf_s8_125k)/sizeof (TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_ZIGBEE_250K) {
    	LoadTblCmdSet(tbl_rf_zigbee_250k, sizeof(tbl_rf_zigbee_250k)/sizeof(TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_PRIVATE_250K) {
    	LoadTblCmdSet(tbl_rf_private_250k, sizeof(tbl_rf_private_250k)/sizeof(TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_PRIVATE_500K) {
    	LoadTblCmdSet(tbl_rf_private_500k, sizeof(tbl_rf_private_500k)/sizeof(TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_PRIVATE_1M) {
    	LoadTblCmdSet(tbl_rf_private_1m, sizeof(tbl_rf_private_1m)/sizeof(TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_PRIVATE_2M) {
    	LoadTblCmdSet(tbl_rf_private_2m, sizeof(tbl_rf_private_2m)/sizeof(TBLCMDSET));
	}
    else if (rf_mode == RF_MODE_ANT) {
    	LoadTblCmdSet(TblRFANT, sizeof(TblRFANT)/sizeof(TBLCMDSET));
	}
    else if (rf_mode == RF_MODE_BLE_2M_NO_PN) {
          	LoadTblCmdSet(tbl_rf_2m_no_pn, sizeof(tbl_rf_2m_no_pn)/sizeof(TBLCMDSET));
     }
#endif

	reg_dma_chn_en |= FLD_DMA_CHN_RF_RX  | FLD_DMA_CHN_RF_TX;

	g_RFMode = rf_mode;
}

/**
*	@brief     This function serves to RF mode initialization
*			   SDK  may remove some features of the rf_drv_init,and thus not supporting multiple modes
*			   rf_multi_mode_drv_init function does not allow modification and is used to support multiple modes			 
*	@param[in] rf_mode  -  mode of RF
*	@return	   none.
*/

void rf_multi_mode_drv_init(RF_ModeTypeDef rf_mode)
{

	LoadTblCmdSet(tbl_rf_init, sizeof(tbl_rf_init)/sizeof(TBLCMDSET));

    if (rf_mode == RF_MODE_BLE_1M) {
    	LoadTblCmdSet (tbl_rf_1m, sizeof (tbl_rf_1m)/sizeof (TBLCMDSET));
	}
    else if(rf_mode == RF_MODE_BLE_1M_NO_PN) {
    	LoadTblCmdSet (tbl_rf_1m_no_pn, sizeof (tbl_rf_1m_no_pn)/sizeof (TBLCMDSET));
	}
    else if(rf_mode == RF_MODE_BLE_2M) {
    	LoadTblCmdSet (tbl_rf_2m, sizeof (tbl_rf_2m)/sizeof (TBLCMDSET));
	}
    else if (rf_mode == RF_MODE_LR_S2_500K) {
    	LoadTblCmdSet (tbl_rf_s2_500k, sizeof (tbl_rf_s2_500k)/sizeof (TBLCMDSET));
	}
    else if (rf_mode == RF_MODE_LR_S8_125K) {
    	LoadTblCmdSet (tbl_rf_s8_125k, sizeof (tbl_rf_s8_125k)/sizeof (TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_ZIGBEE_250K) {
    	LoadTblCmdSet(tbl_rf_zigbee_250k, sizeof(tbl_rf_zigbee_250k)/sizeof(TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_PRIVATE_250K) {
    	LoadTblCmdSet(tbl_rf_private_250k, sizeof(tbl_rf_private_250k)/sizeof(TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_PRIVATE_500K) {
    	LoadTblCmdSet(tbl_rf_private_500k, sizeof(tbl_rf_private_500k)/sizeof(TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_PRIVATE_1M) {
    	LoadTblCmdSet(tbl_rf_private_1m, sizeof(tbl_rf_private_1m)/sizeof(TBLCMDSET));
    }
    else if (rf_mode == RF_MODE_PRIVATE_2M) {
    	LoadTblCmdSet(tbl_rf_private_2m, sizeof(tbl_rf_private_2m)/sizeof(TBLCMDSET));
	}
    else if (rf_mode == RF_MODE_ANT) {
    	LoadTblCmdSet(TblRFANT, sizeof(TblRFANT)/sizeof(TBLCMDSET));
	}
    else if (rf_mode == RF_MODE_BLE_2M_NO_PN) {
          	LoadTblCmdSet(tbl_rf_2m_no_pn, sizeof(tbl_rf_2m_no_pn)/sizeof(TBLCMDSET));
     }

	reg_dma_chn_en |= FLD_DMA_CHN_RF_RX  | FLD_DMA_CHN_RF_TX;

	g_RFMode = rf_mode;
}


/**
 * @brief   	This function serves to set RF channel.
 * @param[in]  	RF_PowerTypeDef - the RF power types.
 * @return  	none.
 */

void rf_set_channel (signed char chn, unsigned short set)//general
{
	unsigned short rf_chn =0;
	unsigned char ctrim;
	unsigned short chnl_freq;

	if (set & RF_CHN_TABLE) {
		chn = rf_chn_table[chn];
	}

	rf_chn = chn+2400;

	if (rf_chn >= 2550)
	    ctrim = 0;
	else if (rf_chn >= 2520)
	    ctrim = 1;
	else if (rf_chn >= 2495)
	    ctrim = 2;
	else if (rf_chn >= 2465)
	    ctrim = 3;
	else if (rf_chn >= 2435)
		ctrim = 4;
	else if (rf_chn >= 2405)
	    ctrim = 5;
	else if (rf_chn >= 2380)
	    ctrim = 6;
	else
	    ctrim = 7;

	chnl_freq = rf_chn * 2;

	write_reg8(0x1244, ((chnl_freq & 0x7f)<<1) | 1  );   //CHNL_FREQ_DIRECT   CHNL_FREQ_L
	write_reg8(0x1245,  ((read_reg8(0x1245) & 0xc0)) | ((chnl_freq>>7)&0x3f) );  //CHNL_FREQ_H
	write_reg8(0x1229,  (read_reg8(0x1229) & 0xC3) | (ctrim<<2) );  //FE_CTRIM

}

/**
 * @brief   	This function serves to set The channel .
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return  	none.
 */
void rf_set_channel_500k(signed short chn, unsigned short set)//general
{
    unsigned short rf_chn =0;
    unsigned char ctrim;
    unsigned short chnl_freq;

    if (set & RF_CHN_TABLE) {
        chn = rf_chn_table[chn];
    }

    rf_chn = chn/2+2400;

    if (rf_chn >= 2550)
        ctrim = 0;
    else if (rf_chn >= 2520)
        ctrim = 1;
    else if (rf_chn >= 2495)
        ctrim = 2;
    else if (rf_chn >= 2465)
        ctrim = 3;
    else if (rf_chn >= 2435)
        ctrim = 4;
    else if (rf_chn >= 2405)
        ctrim = 5;
    else if (rf_chn >= 2380)
        ctrim = 6;
    else
        ctrim = 7;

    chnl_freq = rf_chn * 2 + (chn % 2);

    write_reg8(0x1244, ((chnl_freq & 0x7f)<<1) | 1  );   //CHNL_FREQ_DIRECT   CHNL_FREQ_L
    write_reg8(0x1245, ((read_reg8(0x1245) & 0xc0)) | ((chnl_freq>>7)&0x3f) );  //CHNL_FREQ_H
    write_reg8(0x1229, (read_reg8(0x1229) & 0xC3) | (ctrim<<2) );  //FE_CTRIM
}

/**
 * @brief   	This function serves to set RF power level index.
 * @param[in]  	RF_PowerTypeDef - the RF power types.
 * @return  	none.
 */
_attribute_ram_code_ void rf_set_power_level_index (RF_PowerTypeDef level)
{
	unsigned char value;

	if(level & BIT(7)){    //VANT
		REG_ADDR8(0x1225) |= BIT(6);
	}
	else{  //VBAT
		REG_ADDR8(0x1225) &= ~BIT(6);
	}

    value = (unsigned char)(level & 0x3F);

	write_reg8(0x1226, (read_reg8(0x1226) & 0x7f) | ((value&0x01)<<7) );
	write_reg8(0x1227, (read_reg8(0x1227) & 0xe0) | (value>>1) );

	txPower_index = (unsigned char)level;
}

/**
 * @brief   	This function serves to set RF power level index of zigbee.
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return  	none.
 */

void rf_set_power_level_index_zgb (int level_idx)
{
	unsigned char value;
	unsigned char level = 0;
	if(level_idx < sizeof(rf_power_Level_list)/sizeof(unsigned char)){
		level = rf_power_Level_list[level_idx];
	}

	if(level & BIT(7)){    //VANT
		REG_ADDR8(0x1225) |= BIT(6);
	}
	else{  //VBAT
		REG_ADDR8(0x1225) &= ~BIT(6);
	}

    value = (unsigned char)(level & 0x3F);

	write_reg8(0x1226, (read_reg8(0x1226) & 0x7f) | ((value&0x01)<<7) );
	write_reg8(0x1227, (read_reg8(0x1227) & 0xe0) | (value>>1) );

	txPower_index = (unsigned char)level;
}


/**
 * @brief   This function serves to get RF TX power level .
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return  none.
 */
signed char rf_get_tx_power_level(void)
{
	signed char txPower;
	if(txPower_index & BIT(7)){
		unsigned char power = txPower_index & 0x7F;

		if(power > 58){
			txPower = 3;
		}
		else if(power > 51){
			txPower = 2;
		}
		else if(power > 43){
			txPower = 1;
		}
		else{
			txPower = 0;
		}
	}
	else{
		txPower = (txPower_index+1)/6;
	}

	return txPower;
}



/**
*	@brief	  	this function is to Set byte length for access_code.
*	@param[in]	len  	Optional range: 2~5 , The effect for 3-byte access_code is not good.
*	@return	 	none
*/
void rf_acc_len_set(unsigned char len)
{
	write_reg8(0x405, (read_reg8(0x405)&0xf8) | len); //access_byte_num[2:0]

	REG_RF_ACCESS_CODE_GTH = (len*8); //full bit match
}


/**
*	@brief	  	This function serves to swap bit .
*	@param[in]	original  	The objective needs be swaped.
*	@return	 	The result of swaping
*/
static unsigned char bit_swap8(unsigned char original)
{
    unsigned char ret = 0;
    int i = 0;
    for (i = 0; i < 8; i++) {
        if (original & 0x01) {
            ret |= 0x01;
        }
        else {
            ret &= 0xfe;
        }
        if (i == 7) {
            break;
        }
        ret <<= 1;
        original >>= 1;
    }
    return ret;
}

/**
*	@brief	  	This function servers to set multi byte write.
*   @param[in]      reg_start - register start.
*   @param[in]      buf - store bytes, which need to be write.
*   @param[in]      len - the length of byte.
*	@return	 	rf_ed:0x00~0xff
*/
static void multi_byte_reg_write(unsigned int reg_start, unsigned char *buf, int len)
{
    int i = 0;
    for (i = 0; i < len; i++,reg_start++) {
        write_reg8(reg_start, buf[i]);
    }
}
/**
*	@brief	  	This function servers to set multi byte read.
*   @param      reg_start - register start.
*   @param      buf - store bytes, which need to be read.
*   @param      len - the length of byte.
*	@return	 	rf_ed:0x00~0xff
*/
static void multi_byte_reg_read(unsigned int reg_start, unsigned char *buf, int len)
{
    int i = 0;
    for (i = 0; i < len; i++,reg_start++) {
        buf[i] = read_reg8(reg_start);
    }
}
/**
*	@brief	  	this function is to set access code.
*	@param[in]	pipe  	index number for access_code channel.
*	@param[in]	addr    the access code address.
*	@return	 	none
*/

void rf_acc_code_set(unsigned char pipe_id, const unsigned char *addr)
{
    unsigned char temp[5];
    unsigned char acc_len = read_reg8(0x405) & 0x07;
    unsigned char i = 0;
    for (i = 0; i < acc_len; i++) {
        temp[i] = bit_swap8(addr[i]);
    }
    switch (pipe_id) {
        case 0:
        case 1:
            multi_byte_reg_write(ACCESS_CODE_BASE_PIPE0 + (pipe_id*8), temp, acc_len);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            write_reg8((ACCESS_CODE_BASE_PIPE2 + (pipe_id-2)), temp[0]);
            break;
        default:
            break;
    }
}


/**
*	@brief	  	this function is to set access code.
*	@param[in]	pipe_id  	index number for access_code channel.
*	@param[in]	addr    the access code address.
*	@return	 	none
*/

void rf_acc_code_get(unsigned char pipe_id, unsigned char *addr)
{
    unsigned char acc_len = read_reg8(0x405) & 0x07;
    unsigned char i = 0;
    switch (pipe_id) {
        case 0:
        case 1:
            multi_byte_reg_read(ACCESS_CODE_BASE_PIPE0 + (pipe_id*8), addr, acc_len);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            *addr = read_reg8((ACCESS_CODE_BASE_PIPE2 + (pipe_id-2)));
            break;
        default:
            break;
    }
    for (i = 0; i < acc_len; i++) {
        addr[i] = bit_swap8(addr[i]);
    }
}

/**
*	@brief	  	This function serves to judge RF Tx/Rx state.
*	@param[in]	rf_status - Tx/Rx status.
*	@param[in]	rf_channel - RF channel.
*	@return	 	failed -1,else success.
*/

int rf_trx_state_set(RF_StatusTypeDef rf_status, signed char rf_channel)
{
    int err = 0;

    write_reg8(0xf02, 0x45);  //close tx and rx before set channel

    rf_set_channel(rf_channel, 0);

    if (rf_status == RF_MODE_TX) {

        write_reg8(0xf02, 0x55);  //tx_en
        write_reg8(0x428, read_reg8(0x428) & 0xfe);  //rx disable
        RF_TRxState = RF_MODE_TX;
    }
    else if (rf_status == RF_MODE_RX) {
        write_reg8(0xf02, 0x65);
        write_reg8(0x428, read_reg8(0x428) | 0x01);  //rx disable
        RF_TRxState = RF_MODE_RX;
    }
    else if (rf_status == RF_MODE_AUTO) {
        write_reg8(0xf00,0x80); //stop cmd
        write_reg8(0xf16, 0x29); // reg0xf16 pll_en_man and tx_en_dly_en  enable

        write_reg8(0x428, read_reg8(0x428) & 0xfe);  //rx disable
        write_reg8(0xf02, read_reg8(0xf02) & 0xce);  //reg0xf02 disable rx_en_man and tx_en_man
        RF_TRxState = RF_MODE_AUTO;
    }
    else {
        err = -1;
    }
    return  err;
}

/**
*	@brief	  	This function serves to get RF status.
*	@param[in]	none.
*	@return	 	RF Rx/Tx status.
*/
RF_StatusTypeDef rf_trx_state_get(void)
{
    return RF_TRxState;
}



/**
*	@brief	  	This function serves to set RF Tx packet.
*	@param[in]	addr - the address RF to send packet.
*	@return	 	none.
*/

void rf_tx_pkt(unsigned char *rf_txaddr)
{
    reg_dma3_addrHi = 0x04;
    reg_dma_rf_tx_addr = (unsigned short)(unsigned int)(rf_txaddr);
    reg_dma_tx_rdy0 |= FLD_DMA_CHN_RF_TX;
}

/**
 * @brief   This function serves to send packet for RF.
 * @param[in]  addr - the address need to be send.
 * @return  none.
 */
void rf_tx_pkt_auto (void* addr)
{
    write_reg8(0x800f16, read_reg8(0x800f16) & (~BIT(2)) );	// disable cmd_schedule mode
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
	write_reg8 (0x800f00, 0x85);						// single TX

}

/**
*	@brief	  	This function is to set rx buffer
*
*	@param[out]	RF_RxAddr  	Pointer for Rx buffer in RAM(Generally it¡¯s starting
*							address of an array.Should be 4-byte aligned)
*	@param[in]	size   		Rx buffer size (It¡¯s an integral multiple of 16)
*	@param[in]	PingpongEn 	Enable/Disable Ping-Pong buffer 1£ºEnable 0£ºDisable
*							Note:
*							When ¡°PingpongEn¡± is set as 0, received RF data will
*							be stored in RAM pointed by ¡° RF_RxAddr¡±.
*							When ¡°PingpongEn¡± is set as 1, received RF data will
*							be stored in buffer0 and buffer1 successively.
*							The RAM size reserved for received RF data should be
*							double of ¡°Size¡±.
*
*	@return	 	none
*/

void  rf_rx_buffer_set(unsigned char *  RF_RxAddr, int size, unsigned char  PingpongEn)
{
    unsigned char mode;

	mode = PingpongEn ? 0x03 : 0x01;
	reg_dma2_addr = (unsigned short)((unsigned int)RF_RxAddr);
	reg_dma2_size = size>>4;
	reg_dma2_mode = mode;
	g_RFRxPingpongEn = PingpongEn;
}
/**
*	@brief	  	This function serves to start Tx of ble_mode.
*	@param[in]	addr   Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick  Tick value of system timer. It determines when to
*						  	  start ble mode and send packet.
*	@return	 	none
*/
void rf_start_btx (void* addr, unsigned int tick)
{
//	write_reg32 (0x800f28, 0x0fffffff); 	//first rx timeout is not valid in BTX mode (confirmed by sihui & junwen 20190805)
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8 (0x800f00, 0x81);						// ble tx

	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
}

/**
*	@brief	  	This function serves to start Tx of ble_mode.
*	@param[in]	addr   Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick  Tick value of system timer. It determines when to
*						  	  start ble mode and send packet.
*	@return	 	none
*/
void rf_start_brx  (void* addr, unsigned int tick)
{
	write_reg32 (0x800f28, 0x0fffffff);
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8 (0x800f00, 0x82);						// ble rx
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
}

/**
*	@brief	  	This function serves to start Tx.
*	@param[in]	addr   Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick  Tick value of system timer.
*	@return	 	none
*/

_attribute_ram_code_ void rf_start_stx  (void* addr, unsigned int tick)
{
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8 (0x800f00, 0x85);						// single TX
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
}

/**
*	@brief	  	This function serves to start Rx.
*	@param[in]	tick  Tick value of system timer.
*	@return	 	none
*/

void rf_start_srx  (unsigned int tick)
{
	write_reg32 (0x800f28, 0x0fffffff);					// first timeout
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg16 (0x800f00, 0x3f86);						// srx
}

/**
*	@brief	  	This function serves to start stx2rx mode of auto_mode.
*				In this mode, a packet is sent first,RF module waits for 10us,
*				stays in Rx status until data is received or timeout expires,
*				then exits this mode.Timeout duration is set by the parameter
*				¡°timeout_us¡±.The address to store received data is set by the
*				function ¡°RF_RxBufferSet¡±.
*
*	@param[in]	addr  Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick   	Tick value of system timer. It determines when
*								to start StxToRx mode and send packet.
*
*	@return	 	none
*/

void rf_start_stx2rx  (void* addr, unsigned int tick)
{
	write_reg32(0x800f18, tick);
	write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8  (0x800f00, 0x87);	// single tx2rx
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
}

/**
*	@brief	  	This function serves to start srx2tx mode of auto_mode.
*				In this mode,RF module stays in Rx status until a packet is
*				received or it fails to receive packetwhen timeout expires.
*				If a packet is received within the timeout duration, RF module
*				will wait for 10us,send a packet, and then exit this mode.
*				If it fails to receive packet when timeout expires, RF module
*				will directly exit this mode.Timeout duration is set by the
*				parameter "timeout_us".	The address to store received data is set
*				by the function ¡°RF_RxBufferSet¡±.
*
*	@param[in]	addr 	Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick   Tick value of system timer. It determines when to
*								start SrxToTx mode.
*	@param[in]	timeout_us  Unit is us. It indicates timeout duration in Rx status.
*								Max value: 0xffffff (16777215)
*	@return	 	none
*/

void rf_start_srx2tx  (void* addr, unsigned int tick)
{
	write_reg32 (0x800f28, 0x0fffffff);					// first timeout
	write_reg32(0x800f18, tick);
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
	write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg16 (0x800f00, 0x3f88);								// single rx2tx
}

/*******************************            Private APIs            **************************/

/**
 * @brief   	This function serves to set the ble channel.
 * @param[in]   chn_num - channel numbers.
 * @return  	none.
 */
_attribute_ram_code_ void rf_set_ble_channel (signed char chn_num)
{
	int freq;
	unsigned short chnl_freq;
    unsigned char ctrim;

	write_reg8 (0x80040d, chn_num);
	if (chn_num < 11)
		chn_num += 2;
    else if (chn_num < 37)
    	chn_num += 3;
    else if (chn_num == 37)
    	chn_num = 1;
    else if (chn_num == 38)
    	chn_num = 13;
    else
    	chn_num = 40;

	chn_num = chn_num << 1;

    freq = 2400 + chn_num;
    chnl_freq = freq<<1;


    //calculate FE_CTRIM
    if(freq >= 2550){
        ctrim = 0;
    }
    else if (freq >= 2520){
        ctrim = 1;
    }
    else if (freq >= 2495){
        ctrim = 2;
    }
    else if (freq >= 2465){
        ctrim = 3;
    }
    else if (freq >= 2435){
        ctrim = 4;
    }
    else if (freq >= 2405){
        ctrim = 5;
    }
    else if (freq >= 2380){
        ctrim = 6;
    }
    else{
        ctrim = 7;
    }

    write_reg8(0x1228,  (read_reg8(0x1228) & 0xC0) | ((chn_num - 2)>>1) );
    write_reg8(0x1229,  (read_reg8(0x1229) & 0xC3) | (ctrim<<2) );  //FE_CTRIM
}
/**
*	@brief	  	This function serves to simulate 100k Tx by 500k Tx
*   @param[in]  *input  		- the content of payload
*   @param[in]	len 			- the length of payload
*   @param[in]  init_val 		- the initial value of CRC
*	@return	 	init_val 		- CRC
*/

unsigned short crc16_ccitt_cal(unsigned char *input, unsigned int len, unsigned short init_val)
{
    unsigned short poly = 0x1021;
    unsigned char tmp = 0;
    int i = 0;
    while (len--) {
        tmp = *(input++);
        init_val ^= (tmp << 8);
        for (i = 0;i < 8;i++) {
            if(init_val & 0x8000)
                init_val = (init_val << 1) ^ poly;
            else
                init_val = init_val << 1;
        }
    } 
    return init_val;
}

/**
*	@brief	  	This function serves to simulate 100k Tx by 500k Tx
*   @param[in]  *input  		- the content of payload
*   @param[in]	len 			- the length of payload
*   @param[in]  init_val 		- the initial value of CRC
*	@return	 	init_val 		- CRC
*/

static void rf_byte_extend(unsigned char value, unsigned char *buf)
{
    unsigned long long extended_val = 0x0000000000000000;
    unsigned int temp = 0x00000000;
    unsigned char bit_1 = 0x1f;
    unsigned char bit_offset = 0;
    unsigned char i = 0;
    for (i = 4; i < 8; i++, bit_offset += 5) {
        if (value & (0x01<<i)) {
            temp |= bit_1<<bit_offset;
        }
        else {
            temp &= (~(bit_1<<bit_offset));
        }
    }
    extended_val = temp;
    extended_val <<= 20;
    bit_offset = 0;
    temp = 0;
    for (i = 0; i < 4; i++, bit_offset += 5) {
        if (value & (0x01<<i)) {
            temp |= bit_1<<bit_offset;
        }
        else {
            temp &= (~(bit_1<<bit_offset));
        }
    }
    extended_val |= temp;
    buf[4] = extended_val;
    buf[3] = extended_val >> 8;
    buf[2] = extended_val >> 16;
    buf[1] = extended_val >> 24;
    buf[0] = extended_val >> 32;
}

/**
*	@brief	  	This function serves to simulate 100k Tx by 500k Tx
*   @param[in]  *preamble  		- the content of preamble
*   @param[in]	preamble_len 	- the length of preamble
*   @param[in]  *acc_code 		- the content of access code
*   @param[in]  acc_len			- the length of access code
*   @param[in]  *payload		- the content of payload
*   @param[in]	pld_len			- the length of payload
*   @param[in]	*tx_buf			- the data need to be sent
*   @param[in]	crc_init		- the initial value of CRC
*	@return	 	none
*/

void rf_tx_500k_simulate_100k(unsigned char *preamble, unsigned char preamble_len,
                              unsigned char *acc_code, unsigned char acc_len,
                              unsigned char *payload, unsigned char pld_len,
                              unsigned char *tx_buf, unsigned short crc_init)
{
    unsigned char tx_len = (preamble_len + acc_len + pld_len + 2) * 5;
    tx_buf[0] = tx_len;
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x00;

    rf_nordic_shockburst(tx_len);

    unsigned char *p = &tx_buf[4];
    unsigned char i = 0;
    for (i = 0; i < preamble_len; i++, p += 5) {
        rf_byte_extend(preamble[i], p);
    }
    for (i = 0; i < acc_len; i++, p += 5) {
        rf_byte_extend(acc_code[acc_len-1-i], p);
    }
    for (i = 0; i < pld_len; i++, p += 5) {
        rf_byte_extend(payload[i], p);
    }

    unsigned short crc = crc16_ccitt_cal(payload, pld_len, crc_init);

    rf_byte_extend(crc >> 8, p);
    p += 5;
    rf_byte_extend(crc & 0xff, p);
} 

/**
*	@brief	  	This function serves to get rssi
*   @param[in]      none
*	@return	 	none
*/

static inline signed char rf_rssi_get_154(void)
{
	return (((signed char)(read_reg8(0X441))) - 110);
}


signed char rssi_max = -110;
/**
*	@brief	  	This function is to start energy detect of the current channel for zigbee mode
*				Before using it.
*   @param[in]  none
*	@return	 	none
*/
void rf_ed_detecct_154(void)
{
    signed char rssi;
    rssi = rf_rssi_get_154();

    if (rssi > rssi_max) {
        rssi_max = rssi;
    }
}

/**
*	@brief	  	This function is to stop energy detect and get energy detect value of
*				the current channel for zigbee mode.
*   @param[in]  none
*	@return	 	rf_ed:0x00~0xff
*
*/


unsigned char rf_stop_ed_154(void)
{
    unsigned char rf_ed;
    unsigned int Temp;
    if (rssi_max <= -72) {
        rf_ed = 0;
    }
    else if (rssi_max >= -35) {
        rf_ed = 0xff;
    }
    else{
        Temp = (rssi_max + 76) * 255;
        rf_ed = Temp/40;
        if(rssi_max == -50)
        {
            rf_ed = rf_ed+2;
        }
    }
    rssi_max = -110;
    return rf_ed;
}

/**
*	@brief	  	This function serves to set pin for RFFE of RF
*   @param[in]  tx_pin - select pin to send
*   @param[in]  rx_pin - select pin to receive
*	@return	 	none
*
*/
void rf_rffe_set_pin(RF_PATxPinDef tx_pin, RF_LNARxPinDef rx_pin)
{
	unsigned char val = 0;
	unsigned char mask = 0xff;
	unsigned short reg;
	BM_CLR(reg_gpio_func(tx_pin), tx_pin&0xff);
	BM_CLR(reg_gpio_func(rx_pin), rx_pin&0xff);

	switch(tx_pin)
	{
		case RFFE_TX_PB3:
			val = BIT(7);
			mask = (unsigned char)~(BIT(7)|BIT(6));;
			break;

		case RFFE_TX_PC7:
			val = 0;
			mask = (unsigned char)~(BIT(7)|BIT(6));
			break;

		case RFFE_TX_PD1:
			val = 0;
			mask = (unsigned char)~(BIT(3)|BIT(2));
			break;

		default:
			val = 0;
			mask = 0xff;
			break;
	}
	reg = 0x5a8 + ((tx_pin>>8)<<1) + ((tx_pin&0x0f0) ? 1 : 0 );
	write_reg8(reg, ( read_reg8(reg) & mask) | val);


	switch(rx_pin)
	{
		case RFFE_RX_PB2:
			val = BIT(6);
			mask = (unsigned char)~(BIT(7)|BIT(6));
			break;

		case RFFE_RX_PC6:
			val = 0;
			mask = (unsigned char)~(BIT(5)|BIT(4));
			break;

		case RFFE_RX_PD0:
			val = 0;
			mask = (unsigned char)~(BIT(1)|BIT(0));
			break;

		default:
			val = 0;
			mask = 0xff;
			break;
	}
	reg = 0x5a8 + ((rx_pin>>8)<<1) + ((rx_pin&0x0f0) ? 1 : 0 );
	write_reg8(reg, ( read_reg8(reg) & mask) | val);
}


#define    CRC_INIT_VAL                   0x1D0F
#define    CRC_LEN_1MBPS                  3
static unsigned char pn_sequence_1mbps_shockburst[32] = {0xb1, 0x4b, 0xea, 0x85, 0xbc, 0xe5, 0x66, 0x0d,
                                                         0xae, 0x8c, 0x88, 0x12, 0x69, 0xee, 0x1f, 0xc7,
                                                         0x62, 0x97, 0xd5, 0x0b, 0x79, 0xca, 0xcc, 0x1b,
                                                         0x5d, 0x19, 0x10, 0x24, 0xd3, 0xdc, 0x3f, 0x8e};
/**
 * @brief      This function process the received packet in 1mbps shockburst mode only for hanshow for the
 *             compatiblity with third-party chips. The process includes data-whitening
               transformation and crc check.
 * @param[in]  rx_buf - the rf rx buffer containing the received packet(dma length+payload+3 byte crc)
 * @param[in]  len - the expected rx length of shockburst mode, containing payload and 3byte crc
 *
 * @return     the status of the processing procesure. 1: the received packet is correct, 0: the received packet is incorrect
 */
unsigned char rx_packet_process_1mbps(unsigned char *rx_buf, unsigned int len)
{
    unsigned char i = 0;
    unsigned char ret = 0;
    unsigned int crc_in_packet = 0;
    unsigned int crc_tmp = 0;

    if ((0 == rx_buf) || (len > 32)) {
        return ret;
    }

    //do data-whitening transformation for both payload and crc fields
    for (i = 0; i < len; i++) {
        rx_buf[4+i] ^= pn_sequence_1mbps_shockburst[i];
    }
    //parse the 3byte crc from the received packet
    crc_in_packet = (rx_buf[4+len-3]<<16) + (rx_buf[4+len-2]<<8) + rx_buf[4+len-1];
    //caculate the crc value of the received payload field
    crc_tmp = crc16_ccitt_cal(&rx_buf[4], len-CRC_LEN_1MBPS, CRC_INIT_VAL);
    crc_tmp <<= 8; //L-shift 8 bit to became a 3byte crc for the compatibility with 2640's 1mbps shockburst mode

    if (crc_tmp != crc_in_packet) {
        return ret;
    }

    //reverse the bit order of the payload field
    for (i = 0; i < (len-CRC_LEN_1MBPS); i++) {
        rx_buf[4+i] = bit_swap8(rx_buf[4+i]);
    }

    ret = 1;
    return ret;
}

/**
 * @brief      This function process the tx packet in 1mbps shockburst mode only for hanshow for the
 *             compatiblity with third-party chips. The process includes data-whitening
               transformation and crc padding.
 * @param[in]  tx_buf - the rf tx buffer containing the tx packet(dma length+payload+3 byte crc)
 * @param[in]  len - the expected tx length of shockburst mode, containing payload and 3byte crc
 *
 * @return     none
 */
void tx_packet_process_1mbps(unsigned char *tx_buf, unsigned char *payload, unsigned int len)
{
    unsigned char i = 0;
    unsigned short crc = 0;

    if ((0 == tx_buf) || (0 == payload) || (len > 32)) {
        return;
    }

    //reverse bit order first
    for (i = 0; i < (len-CRC_LEN_1MBPS); i++) {
        payload[i] = bit_swap8(payload[i]);
    }

    //fill Tx Buffer
    tx_buf[0] = len;
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x00;
    //reverse bit order first
    for (i = 0; i < (len-CRC_LEN_1MBPS); i++) {
        tx_buf[4+i] = payload[i];
    }

    //fill the 3byte crc, note that crc is trasmitted MSByte first
    crc = crc16_ccitt_cal(payload, len-CRC_LEN_1MBPS, CRC_INIT_VAL);
    tx_buf[4+len-CRC_LEN_1MBPS] = (unsigned char)(crc >> 8);
    tx_buf[4+len-CRC_LEN_1MBPS+1] = (unsigned char)(crc & 0x00ff);
    tx_buf[4+len-CRC_LEN_1MBPS+2] = 0;

    //do data-whitening transformation both for the payload and crc fields
    for (i = 0; i < len; i++) {
        tx_buf[4+i] ^= pn_sequence_1mbps_shockburst[i];
    }

    return;
}



/**
 * @brief      This function
 * @param[in]  size - the expected size of dma2
 * @param[in]  PingpongEn - the flag of g_RFRxPingpongEn
 * @return     none
 */
void  rf_rx_cfg(int size, unsigned char  PingpongEn)
{
    unsigned char mode;
	mode = PingpongEn ? 0x03 : 0x01;
	reg_dma2_size = size>>4;
	reg_dma2_mode = mode;
	g_RFRxPingpongEn = PingpongEn;
}
/**
 * @brief      This function is to disable PN
 * @return     none
 */
void  rf_pn_disable()
{
	write_reg8 (0x401, 0);//// PN disable
	write_reg8 (0x404, read_reg8(0x404)&0xdf);//BIT<5>: ble_wt(PN) disable
}

