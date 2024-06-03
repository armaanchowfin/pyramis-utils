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

#include "nas/NasDatatypes/UESecCapability.h"

int decodeUESecCap(uint8_t *buffer, uint32_t bufLen,
			           UeSecurityCapability_t* ueSecCap, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || ueSecCap == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

	int ueSecCapLen = *(buffer + len); 
	if(len + ueSecCapLen + sizeof(ueSecCap->len) > bufLen) {
	    higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}
	else if(ueSecCapLen != UE_SECURITY_CAPABILITY_NAS_LEN &&
	        ueSecCapLen != UE_SECURITY_CAPABILITY_NAS_AS_LEN) {
	    higLog("%s", "ueSecuCapability incorrect length");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
	    nasLogEXIT;
	    return FAILURE;
	}
	
	memcpy(ueSecCap,buffer + len, ueSecCapLen + sizeof(ueSecCap->len));
	len += ueSecCapLen + sizeof(ueSecCap->len);
	nasLog("ueSecuCapability decoded | currentLen = %d | decoded %d bytes", 
	                                        len, ueSecCapLen);
	nasLog("_5gNASEncAlgo 0x%x |  _5gNASIntAlgo 0x%x",
	        ueSecCap->_5gNASEncAlgo, ueSecCap->_5gNASIntAlgo);
	if(ueSecCapLen == UE_SECURITY_CAPABILITY_NAS_AS_LEN) {
	    nasLog("_5gASEncAlgo 0x%x |  _5gASIntAlgo 0x%x",
	            ueSecCap->_5gASEncAlgo, ueSecCap->_5gASIntAlgo);
	} else
        nasLog("_5gASEncAlgo ABSENT |  _5gASIntAlgo ABSENT");

	*decodedLen = len;
    nasLogEXIT;
	return SUCCESS;


}

int encodeUESecCap(uint8_t *buffer, uint32_t bufLen,
                       UeSecurityCapability_t* ueSecCap, uint32_t *encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || ueSecCap == NULL || encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    if(ueSecCap->len != UE_SECURITY_CAPABILITY_NAS_LEN  &&
            ueSecCap->len != UE_SECURITY_CAPABILITY_NAS_AS_LEN) {
        higLog("Invalid ueSecCap->len %d", ueSecCap->len);
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

	if (len + ueSecCap->len + sizeof(ueSecCap->len) > bufLen) {
	    higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
	    nasLogEXIT;
	    return FAILURE;
	}
	
	/* TODO: use offset, rm len*/
	memcpy(buffer + len, ueSecCap, ueSecCap->len + sizeof(ueSecCap->len));
	len += ueSecCap->len + sizeof(ueSecCap->len);
	nasLog("ueSecuCapability encoded | currentLen = %d | encoded %d bytes", len,
		ueSecCap->len);
	nasLog("_5gNASEncAlgo 0x%x |  _5gNASIntAlgo 0x%x",
		ueSecCap->_5gNASEncAlgo, ueSecCap->_5gNASIntAlgo);
	if(ueSecCap->len == UE_SECURITY_CAPABILITY_NAS_AS_LEN) {
		nasLog("_5gASEncAlgo 0x%x |  _5gASIntAlgo 0x%x",
			ueSecCap->_5gASEncAlgo, ueSecCap->_5gASIntAlgo);
	} else
        nasLog("_5gASEncAlgo ABSENT | _5gASIntAlgo ABSENT");

	*encodedLen = len;
    nasLogEXIT;
	return SUCCESS;
}
