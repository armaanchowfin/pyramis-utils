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

#include "nas/NasDatatypes/SSCMode.h"

int decodeSSCMode(uint8_t *buffer, uint32_t bufLen,
			           sscMode_t *sscMode, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || bufLen == 0 ||
		sscMode == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}
	if(len + SIZE_OF_SSC_MODE > bufLen) {
	    higLog("Buffer insufficient");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}
	uint8_t ssc_mode_value=(buffer[0] & 0x07);
	if((ssc_mode_value==0) || (ssc_mode_value==7)){
		higLog("Constraint Check failed for ssc_mode_value");
		nasLogEXIT;
		return FAILURE;
	}	
    sscMode->ssc_mode_value = ssc_mode_value;
    len  += SIZE_OF_SSC_MODE;
    *decodedLen = len;
    nasLog("SSC Mode Value = %d | currentLen = %d | decoded %d bytes", 
    	sscMode->ssc_mode_value,len,SIZE_OF_SSC_MODE);

	nasLogEXIT;	
	return SUCCESS;
}

int encodeSSCMode(uint8_t *buffer, uint32_t bufLen,
                       sscMode_t *sscMode, uint32_t *encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	if(bufLen == 0 || buffer == NULL || sscMode == NULL ||
       encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + SIZE_OF_SSC_MODE > bufLen) {
	    higLog("Buffer insufficient");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}
	buffer[0] &= 0xF0;
    buffer[0] |= (sscMode->ssc_mode_value);
    len += SIZE_OF_SSC_MODE;
    *encodedLen = len;
    
    nasLog("SSC Mode Value = %d | currentLen = %d | encoded %d bytes", 
    	sscMode->ssc_mode_value,len,SIZE_OF_SSC_MODE);

	nasLogEXIT;	
	return SUCCESS;
}
