/********************************************************************************************************
 * @file	ble_stack.h
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
#ifndef BLE_STACK_H_
#define BLE_STACK_H_





//Extended Header BIT
#define 		EXTHD_BIT_ADVA         							BIT(0)
#define 		EXTHD_BIT_TARGETA      							BIT(1)
#define			EXTHD_BIT_CTE_INFO         						BIT(2)
#define 		EXTHD_BIT_ADI		    						BIT(3)
#define 		EXTHD_BIT_AUX_PTR      							BIT(4)
#define 		EXTHD_BIT_SYNC_INFO	    						BIT(5)
#define 		EXTHD_BIT_TX_POWER	     						BIT(6)

#define 		LL_EXTADV_MODE_NON_CONN_NON_SCAN    			(0x00)
#define 		LL_EXTADV_MODE_CONN        						(0x01)		//connectable, none_scannable
#define 		LL_EXTADV_MODE_SCAN        						(0x02)      //scannable,   none_connectable

/******************************************** ATT ***************************************************************/





/**
 *  @brief  Definition for Error Response of ATTRIBUTE PROTOCOL PDUS
 *  See the Core_v5.0(Vol 3/Part F/3.4.1.1, "Error Response") for more information.
 */
typedef enum {

	ATT_SUCCESS = 0,

    ATT_ERR_INVALID_HANDLE,                              //!< The attribute handle given was not valid on this server
    ATT_ERR_READ_NOT_PERMITTED,                          //!< The attribute cannot be read
    ATT_ERR_WRITE_NOT_PERMITTED,                         //!< The attribute cannot be written
    ATT_ERR_INVALID_PDU,                                 //!< The attribute PDU was invalid
    ATT_ERR_INSUFFICIENT_AUTH,                           //!< The attribute requires authentication before it can be read or written
    ATT_ERR_REQ_NOT_SUPPORTED,                           //!< Attribute server does not support the request received from the client
    ATT_ERR_INVALID_OFFSET,                              //!< Offset specified was past the end of the attribute
    ATT_ERR_INSUFFICIENT_AUTHOR,                         //!< The attribute requires authorization before it can be read or written
    ATT_ERR_PREPARE_QUEUE_FULL,                          //!< Too many prepare writes have been queued
    ATT_ERR_ATTR_NOT_FOUND,                              //!< No attribute found within the given attribute handle range
    ATT_ERR_ATTR_NOT_LONG,                               //!< The attribute cannot be read or written using the Read Blob Request
    ATT_ERR_INSUFFICIENT_KEY_SIZE,                       //!< The Encryption Key Size used for encrypting this link is insufficient
    ATT_ERR_INVALID_ATTR_VALUE_LEN,                      //!< The attribute value length is invalid for the operation
    ATT_ERR_UNLIKELY_ERR,                                //!< The attribute request that was requested has encountered an error that was unlikely, and therefore could not be completed as requested
    ATT_ERR_INSUFFICIENT_ENCRYPT,                        //!< The attribute requires encryption before it can be read or written
    ATT_ERR_UNSUPPORTED_GRP_TYPE,                        //!< The attribute type is not a supported grouping attribute as defined by a higher layer specification
    ATT_ERR_INSUFFICIENT_RESOURCES,                      //!< Insufficient Resources to complete the request



}att_err_t;



void smemset(void * dest, int val, int len);
void smemcpy(void *pd, void *ps, int len);
int smemcmp(void * m1, void * m2, int len);


#endif /* BLE_STACK_H_ */
