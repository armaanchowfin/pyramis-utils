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

#include "nas/NasDatatypes/5GSRegistrationType.h"

int decode5GSRegType(uint8_t *buffer, uint32_t bufLen,
			           _5gRegistrationType_t *regType, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || regType == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        errno = E_NAS_ERROR_INVALID_INPUT;
		return FAILURE;
	}

    /* NOTE: 5GS registration type is used only in Registraion Request *
     * and it is a mandatory IE. An octet is shared by Reg Type and ngKSI */

    /* Reg Type is an IE of type V and has length of 1/2 octet.
     * Hence bits on wire will not carry length value.
     * Harcoding length to 1: the min granularity */

	int _5gregTypeLen = 1;
    if( len + _5gregTypeLen > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    /* For coding style consistency using memcpy instead of assignment */
    /* TODO: Check in wireshark if LSB side bits are Reg Type */
    memcpy(regType, buffer + len, sizeof(*regType));
    len += sizeof(*regType);
    nasLog("5regType : FOR %d | value %d", regType->FOR, regType->value);

    *decodedLen = len;
    nasLogEXIT;
	return SUCCESS;

}

int encode5GSRegType(uint8_t *buffer, uint32_t bufLen,
                       _5gRegistrationType_t *regType, uint32_t *encodedLen)
{
	nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || regType == NULL || encodedLen == NULL)
    {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }
    unsigned int len = 0;

	if (len + sizeof(*regType) > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

    /* NOTE: 5GS registration type is used only in Registraion Request *
     * and it is a mandatory IE. An octet is shared by Reg Type and ngKSI */

    /* For coding style consistency using memcpy instead of assignment */
    /* TODO: Check in wireshark if LSB side bits are Reg Type */
    memcpy(buffer + len, regType, sizeof(*regType));
    len += sizeof(*regType);
	
	nasLog("5regType : FOR %d | value %d", regType->FOR, regType->value);
	
	*encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
