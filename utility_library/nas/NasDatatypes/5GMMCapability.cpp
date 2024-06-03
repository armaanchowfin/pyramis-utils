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

#include "nas/NasDatatypes/5GMMCapability.h"

int decode5GMMCap(uint8_t *buffer, uint32_t bufLen,
			           _5gmmCapability_t* _5gmmCap, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

       	if(buffer == NULL || bufLen == 0 || _5gmmCap == NULL || decodedLen == NULL) {
            higLog("%s", "Invalid Input, Failure decoding");
            errno = E_NAS_ERROR_INVALID_INPUT;
            nasLogEXIT;
	    return FAILURE;
	}

	_5gmmCap->len = *(buffer + len);
	len++;

	if(len + _5gmmCap->len > bufLen) {
	    higLog("%s", "FAILURE, Incomplete buffer");
            errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}

	else if(_5gmmCap->len < _5GMM_CAPABILITY_LEN_MIN ||
	        _5gmmCap->len > _5GMM_CAPABILITY_LEN_MAX) {
	    higLog("%s", "5GMMCapability incorrect length");
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
	    nasLogEXIT;
	    return FAILURE;
	}

	_5gmmCap->hoS1 = *(buffer + len);
	len++;

	nasLog("5GMMCapability decoded | currentLen = %d | decoded %d bytes",
	                                        len, _5gmmCap->len);
	nasLog("LPP 0x%01x | HO 0x%01x | S1Mode 0x%01x",(_5gmmCap->hoS1 & 0x4)>>2,
	        (_5gmmCap->hoS1 & 0x2)>>1, (_5gmmCap->hoS1 & 0x1));

	*decodedLen = len;
    	nasLogEXIT;
	return SUCCESS;

}

int encode5GMMCap(uint8_t *buffer, uint32_t bufLen,
                       _5gmmCapability_t* _5gmmCap, uint32_t *encodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;

    if(buffer == NULL || bufLen == 0 || _5gmmCap == NULL || encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
    }

    if( _5gmmCap->len < _5GMM_CAPABILITY_LEN_MIN  ||
        _5gmmCap->len > _5GMM_CAPABILITY_LEN_MAX ) {
        errno = E_NAS_ERROR_INVALID_INPUT;
        higLog("Invalid length %d", _5gmmCap->len);
        nasLogEXIT;
        return FAILURE;
    }


    buffer[len++] = _5gmmCap->len;

    if (len + _5gmmCap->len > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[len++] = _5gmmCap->hoS1;

    nasLog("5GMMCapability encoded | currentLen = %d | encoded %d bytes", len,
	_5gmmCap->len);
    nasLog("LPP 0x%01x | HO 0x%01x |  S1 0x%01x",(_5gmmCap->hoS1 & 0x4)>>2,
	(_5gmmCap->hoS1 & 0x2)>>1, (_5gmmCap->hoS1 & 0x1));

    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
