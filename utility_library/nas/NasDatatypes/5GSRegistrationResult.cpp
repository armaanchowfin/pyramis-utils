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

#include "nas/NasDatatypes/5GSRegistrationResult.h"

int decode5GSRegResult(uint8_t *buffer, uint32_t bufLen,
    _5gRegistrationResult_t *regResult, uint32_t* decodedLen)
{

	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || regResult == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

	int _5gregResultLen = *(buffer);
    /* TODO handling for regResult being an optional field
    if( _5gregResultLen != sizeof(*regResult) + size of IEI) */
    if( _5gregResultLen != REG_RESULT_SIZE) {
        higLog("Incorrect 5G Reg Result Length: %d, expected %d", _5gregResultLen,
                REG_RESULT_SIZE);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }
    if( len + sizeof(*regResult) > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }
     
    memcpy(regResult, buffer + len, sizeof(*regResult));
    len += sizeof(*regResult);
    nasLog("5gregResult : len %d | smsAllowed: %d | value %d", 
            regResult->len,regResult->smsAllowed, regResult->value);

    *decodedLen = len;
    nasLogEXIT;
	return SUCCESS;

}


int encode5GSRegResult(uint8_t *buffer, uint32_t bufLen,
    _5gRegistrationResult_t *regResult, uint32_t *encodedLen)
{

	nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || regResult == NULL || encodedLen == NULL)
    {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    unsigned int len = 0;
	if (len + sizeof(*regResult) > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
		return FAILURE;
	}

    int _5gregResultLen = regResult->len;
    /* TODO handling for regResult being an optional field
    if( _5gregResultLen != sizeof(*regResult) + size of IEI) */
    if( _5gregResultLen != REG_RESULT_SIZE) {
        higLog("Incorrect 5G Result Length: %d, expected %d", _5gregResultLen,
                REG_RESULT_SIZE);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }
    memcpy(buffer + len, regResult, sizeof(*regResult));
    len += sizeof(*regResult);

    nasLog("5regResult : len %d | smsAllowed %d | value %d",
            regResult->len,regResult->smsAllowed, regResult->value);
    
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
