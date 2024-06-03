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

#include "nas/NasDatatypes/5GSDeregistrationType.h"

int decode5GSDeregType(uint8_t *buffer, uint32_t bufLen,
			           _5gDeregistrationType_t *deregType, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || deregType == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
		return FAILURE;
	}

	int _5gDeregTypeLen = sizeof(*deregType);
	
	if( len + _5gDeregTypeLen > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        nasLogEXIT;
        return FAILURE;
    }

	memcpy(deregType, buffer + len, sizeof(*deregType));
    len += sizeof(*deregType);
    nasLog("5gDeregType : accessType %d | regRequired %d | switchoff %d", 
           deregType->accessType, deregType->regRequired, deregType->switchoff);

    *decodedLen = len;

	#if 0

	uint8_t mask = *buffer;
	deregType->switchoff = (mask & 0x08) >> 3;
	deregType->regRequired = (mask & 0x04) >> 2;
	deregType->accessType = (mask & 0x03);
	len++;
	
	#endif
    
    nasLogEXIT;
	return SUCCESS;

}

int encode5GSDeregType(uint8_t *buffer, uint32_t bufLen,
                       _5gDeregistrationType_t *deregType, uint32_t *encodedLen)
{
	nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || deregType == NULL || encodedLen == NULL)
    {
        higLog("%s", "Invalid Input, Failure encoding");
        nasLogEXIT;
        return FAILURE;
    }
    unsigned int len = 0;

	if (len + sizeof(*deregType) > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
		nasLogEXIT;
		return FAILURE;
	}

	int _5gDeregTypeLen = sizeof(*deregType);
    
	memcpy(buffer + len, deregType, _5gDeregTypeLen);
    len += _5gDeregTypeLen;

    nasLog("5gDeregType : accessType %d | regRequired %d | switchoff %d", 
           deregType->accessType, deregType->regRequired, deregType->switchoff);
	*encodedLen = len;

	#if 0

	uint8_t mask = 0;
	mask |= 0x08 & ((deregType->switchoff) << 3);
	mask |= 0x04 & ((deregType->regRequired) << 2);
	mask |= 0x03 & (deregType->accessType);
	*(buffer) = mask;

	#endif

    nasLogEXIT;
    return SUCCESS;
}
