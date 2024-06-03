/*********************************************************************
*
* IITB CONFIDENTIAL
* __________________
*
* All Rights Reserved © 2018-19 IITB, India
* All Rights Reserved © 2019-20 IITB, India
*
* NOTICE: All information contained in this document/file is, and remains the 
* property of Indian Institute of Technology, Bombay (IITB).The intellectual
* and technical concepts contained herein may be proprietary to IITB and are
* protected by trade secret or copyright laws. Unauthorized use, dissemination, 
* reproduction, duplication, reverse engineering, any form of redistribution, 
* or use in part or in whole, by any means is strictly forbidden unless prior, 
* express, written and signed consent is obtained from IITB.
*
* Institute   : Indian Institute of Technology Bombay (IITB)
*/

#include "nas/NasDatatypes/PDUSessionType.h"

int decodePDUSessionType(uint8_t *buffer, uint32_t bufLen,
			           pduSessionType_t *pduSType, uint32_t* decodedLen,bool isOptional)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || bufLen < SIZE_OF_PDU_SESSION_TYPE ||
		pduSType == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}
	if(len + SIZE_OF_PDU_SESSION_TYPE > bufLen) {
	    higLog("Buffer insufficient");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}
	uint8_t pdu_session_type_value = 0;

	if(isOptional){
		pdu_session_type_value = (buffer[0] & 0x07);
	}
	else{
		pdu_session_type_value = ((buffer[0] & 0x70)>>4);
	}

    pduSType->pdu_session_type_value = pdu_session_type_value;
    len  += SIZE_OF_PDU_SESSION_TYPE;
    *decodedLen = len;
    nasLog("PDU Session Type Value = %d | currentLen = %d | decoded %d bytes", 
    	pduSType->pdu_session_type_value,len,SIZE_OF_PDU_SESSION_TYPE);
	nasLogEXIT;	
	return SUCCESS;
}

int encodePDUSessionType(uint8_t *buffer, uint32_t bufLen,
                       pduSessionType_t *pduSType, uint32_t *encodedLen,bool isOptional)
{
	nasLogENTRY;
	uint32_t len = 0;
	if(bufLen < SIZE_OF_PDU_SESSION_TYPE || buffer == NULL || pduSType == NULL ||
       encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + SIZE_OF_PDU_SESSION_TYPE > bufLen) {
	    higLog("Buffer insufficient");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}

    if(isOptional){
    	/*Field is optional in case of PDU Session Establishment Request*/
    	buffer[0] &=0xF0;/*Masking*/
    	buffer[0] |= (pduSType->pdu_session_type_value);
    }
    else{
    	buffer[0] &=0x0F;/*Masking*/
    	buffer[0] |= (pduSType->pdu_session_type_value)<<4;
    }

    len += SIZE_OF_PDU_SESSION_TYPE;
    *encodedLen = len;
    
    nasLog("PDU Session Type Value = %d | currentLen = %d | encoded %d bytes", 
    	pduSType->pdu_session_type_value,len,SIZE_OF_PDU_SESSION_TYPE);

	nasLogEXIT;	
	return SUCCESS;
}
