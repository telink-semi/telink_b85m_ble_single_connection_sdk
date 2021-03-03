/********************************************************************************************************
 * @file     ota.h
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     Sep. 18, 2015
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
/*
 * ble_ll_ota.h
 *
 *  Created on: 2015-7-20
 *      Author: Administrator
 */

#ifndef BLE_LL_OTA_H_
#define BLE_LL_OTA_H_


#ifndef BLE_OTA_FW_CHECK_EN
#define BLE_OTA_FW_CHECK_EN					1
#endif

#define FW_MIN_SIZE							0x04000	 		//16K
//#define FW_MAX_SIZE							0x40000			//256K

#define CMD_OTA_VERSION						0xFF00	//client -> server
#define CMD_OTA_START						0xFF01	//client -> server
#define CMD_OTA_END							0xFF02	//client -> server

#define CMD_OTA_START_EXT					0xFF03	//client -> server
#define CMD_OTA_FW_VERSION_REQ				0xFF04	//client -> server
#define CMD_OTA_FW_VERSION_RSP				0xFF05	//server -> client
#define CMD_OTA_RESULT						0xFF06	//server -> client



#define OTA_FLOW_VERSION					BIT(0)
#define OTA_FLOW_START						BIT(1)
#define OTA_FLOW_DATA_COME					BIT(2)
#define OTA_FLOW_VALID_DATA					BIT(3)
#define OTA_FLOW_GET_SIZE					BIT(5)
#define OTA_FLOW_END						BIT(6)

#define OTA_STEP_IDLE						0
#define OTA_STEP_START						BIT(0)
#define OTA_STEP_DATA						BIT(1)
#define OTA_STEP_FEEDBACK					BIT(2)  //feedback OTA result to peer device
#define OTA_STEP_FINISH						BIT(3)



#define DATA_PENDING_VERSION_RSP			1
#define DATA_PENDING_OTA_RESULT				2
#define DATA_PENDING_TERMINATE_CMD			3



typedef struct{
	u8	ota_step;
	u8	otaResult;
	u8	version_accept;
	u8	resume_mode;   //1: resume_mode enable; 0: resume_mode disable

	u8	ota_busy;
	u8 	fw_check_en;
	u8  fw_check_match;
	u8	flow_mask;

	u8	pdu_len; 	    //OTA valid data length
	u8	last_pdu_crc_offset;
	u8	last_actual_pdu_len;
	u8	last_valid_pdu_len;  //maximum value 240

	u8	data_pending_type;  //mark, and also data length
	u8	otaInit;
	u8	newFwArea_clear;
	u8	u8_rsvd;

	u16 local_version_num;  //default value:0; use use API to set version
	u16 ota_attHandle;
	u16 ota_connHandle;
	u16 last_adr_index;

	u32 fw_crc_default;
	u32 fw_crc_init;
	u32 fw_crc_addr;

	u32 firmware_size;
	int flash_addr_mark;  //must be "s32", have special useage with "< 0"
	int cur_adr_index; //must be "s32"

	u32 feedback_begin_tick;  //add a OTA feedback timeout control, prevent some extreme case which lead to OTA flow blocked
	u32 ota_start_tick;
	u32 data_packet_tick;
	u32 process_timeout_us;
	u32 packet_timeout_us;

}ota_server_t;






/**
 *  @brief data structure of OTA command "CMD_OTA_START"
 */
typedef struct {
	u16  	ota_cmd;
} ota_start_t;

/**
 *  @brief data structure of OTA command "CMD_OTA_START_EXT"
 */
typedef struct {
	u16  	ota_cmd;
	u8		pdu_length;			//must be: 16*n(n is in range of 1 ~ 15); pdu_length: 16,32,48,...240
	u8		version_compare;	//0: no version compare; 1: only higher version can replace lower version
} ota_startExt_t;


/**
 *  @brief data structure of OTA command "CMD_OTA_END"
 */
typedef struct {
	u16  	ota_cmd;
	u16		adr_index_max;
	u16		adr_index_max_xor;
} ota_end_t;


/**
 *  @brief data structure of OTA command "CMD_OTA_RESULT"
 */
typedef struct {
	u16  	ota_cmd;
	u8		result;
} ota_result_t;



/**
 *  @brief data structure of OTA command "CMD_OTA_FW_VERSION_REQ"
 */
typedef struct {
	u16  	ota_cmd;
	u16		version_num;
	u8		version_compare;   //1: only higher version can replace lower version
} ota_versionReq_t;


/**
 *  @brief data structure of OTA command "CMD_OTA_FW_VERSION_RSP"
 */
typedef struct {
	u16  	ota_cmd;
	u16		version_num;
	u8		version_accept;      //1: accept firmware update; 0: reject firmware update(version compare enable, and compare result: fail)
} ota_versionRsp_t;





/**
 * @brief 	Multiple boot address enumarion
 */
typedef enum{
	MULTI_BOOT_ADDR_0x20000 	= 0x20000,	//128 K
	MULTI_BOOT_ADDR_0x40000		= 0x40000,  //256 K
}multi_boot_addr_e;



/**
 * @brief	OTA result
 */
enum{
	//0x00
	OTA_SUCCESS 			= 0,			//success
	OTA_DATA_PACKET_SEQ_ERR,				//OTA data packet sequence number error: repeated OTA PDU or lost some OTA PDU
	OTA_PACKET_INVALID,						//invalid OTA packet: 1. invalid OTA command; 2. addr_index out of range; 3.not standard OTA PDU length
	OTA_DATA_CRC_ERR,						//packet PDU CRC err

	//0x04
	OTA_WRITE_FLASH_ERR,					//write OTA data to flash ERR
 	OTA_DATA_UNCOMPLETE,					//lost last one or more OTA PDU
 	OTA_FLOW_ERR,		    				//peer device send OTA command or OTA data not in correct flow
 	OTA_FW_CHECK_ERR,						//firmware CRC check error

	//0x08
	OTA_VERSION_COMPARE_ERR,				//the version number to be update is lower than the current version
	OTA_PDU_LEN_ERR,						//PDU length error: not 16*n, or not equal to the value it declare in "CMD_OTA_START_EXT" packet
	OTA_FIRMWARE_MARK_ERR,		    		//firmware mark error: not generated by telink's BLE SDK
	OTA_FW_SIZE_ERR,						//firmware size error: no firmware_size; firmware size too small or too big

	//0x0C
	OTA_DATA_PACKET_TIMEOUT,	   			//time interval between two consequent packet exceed a value(user can adjust this value)
 	OTA_TIMEOUT,							//OTA flow total timeout
 	OTA_FAIL_DUE_TO_CONNECTION_TERMIANTE,	//OTA fail due to current connection terminate(maybe connection timeout or local/peer device terminate connection)
};





extern u32	ota_program_offset;
extern int 	ota_firmware_size_k;




/**
 * @brief	OTA start command callback declaration
 */
typedef void (*ota_startCb_t)(void);

/**
 * @brief	OTA version callback declaration
 */
typedef void (*ota_versionCb_t)(void);

/**
 * @brief		OTA result indicate callback declaration
 * @param[in]   result - OTA result
 */
typedef void (*ota_resIndicateCb_t)(int result);





/**
 * @brief      this function is used for user to initialize OTA server module.
 * 			   //attention: this API must called before any other OTA relative settings.
 * @param	   none
 * @return     none
 */
void blc_ota_initOtaServer_module(void);

/**
 * @brief      This function is used to register OTA start command callback.
 * 			   when local device receive OTA command  "CMD_OTA_START" and  "CMD_OTA_START_EXT"  form peer device,
 * 			   after checking all parameters are correct, local device will enter OTA update and trigger OTA start command callback.
 * @param[in]  cb - callback function
 * @return     none
 */
void blc_ota_registerOtaStartCmdCb(ota_startCb_t cb);


/**
 * @brief      This function is used to register OTA version command callback
 * 			   when local device receive OTA command  "CMD_OTA_VERSION", trigger this callback.
 * @param[in]  cb - callback function
 * @return     none
 */
void blc_ota_registerOtaFirmwareVersionReqCb(ota_versionCb_t cb);


/**
 * @brief      This function is used to register OTA result indication callback
 * @param[in]  cb - callback function
 * @return     none
 */
void blc_ota_registerOtaResultIndicationCb(ota_resIndicateCb_t cb);


/**
 * @brief      This function is used to set OTA whole process timeout value
 * 			   if not set, default value is 30 S
 * @param[in]  timeout_second - timeout value, unit: S, should in range of 4 ~ 250
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ota_setOtaProcessTimeout(int timeout_second);



/**
 * @brief      This function is used to set OTA packet interval timeout value
 * 			   if not set, default value is 5 S
 * @param[in]  timeout_ms - timeout value, unit: mS, should in range of 1 ~ 20
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ota_setOtaDataPacketTimeout(int timeout_second);



extern int otaWrite(void * p);
extern int otaRead(void * p);


/**
 * @brief      This function is used to set OTA new firmware storage address on Flash.
 * @param[in]  firmware_size_k - firmware maximum size unit: K Byte; must be 4K aligned
 * @param[in]  boot_addr - new firmware storage address, can only choose from multiple boot address
 * 							 supported by MCU
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t bls_ota_set_fwSize_and_fwBootAddr(int firmware_size_k, multi_boot_addr_e boot_addr);


/**
 * @brief      This function is used to erase flash area which will store new firmware.
 * @param      none
 * @return     none
 */
void bls_ota_clearNewFwDataArea(void);



unsigned short crc16 (unsigned char *pD, int len);


#define	bls_ota_registerStartCmdCb			blc_ota_registerOtaStartCmdCb
#define	bls_ota_registerVersionReqCb		blc_ota_registerOtaFirmwareVersionReqCb
#define	bls_ota_registerResultIndicateCb	blc_ota_registerOtaResultIndicationCb
#define bls_ota_setTimeout(tm_us)			blc_ota_setOtaProcessTimeout( (tm_us)/1000000 )


#endif /* BLE_LL_OTA_H_ */
