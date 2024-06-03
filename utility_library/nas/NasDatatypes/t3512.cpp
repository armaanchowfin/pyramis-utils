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

#include "nas/NasDatatypes/t3512.h"

int decodet3512(uint8_t *buffer, uint32_t bufLen,
			           t3512_t* t3512, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint8_t mask = 0;

    if(buffer == NULL || bufLen == 0 || t3512 == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

	uint32_t t3512Len = *(buffer + len);
	len++;
	if (t3512Len - REG_ACCEPT_T3512_IEI_OFFSET > bufLen - len) {
	    higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}
	nasLog("Timer 3512 : current len %d | decoded 1 byte", len);

	t3512->len = t3512Len;
	nasLog("Timer 3512 : len %d | current len %d | len %d", 
	    t3512->len, len, sizeof(t3512->len));

	mask = *(buffer + len);
	t3512->value = mask & 0x1F;
	t3512->unit  = (mask & 0xE0) >> 5;
	len++;
	
	nasLog("unit %d | value %d | current len %d | decoded 1 byte", 
	        t3512->unit, t3512->value, len);

	*decodedLen = len;
    nasLogEXIT;
	return SUCCESS;
}

int encodet3512(uint8_t *buffer, uint32_t bufLen,
                       t3512_t* t3512, uint32_t *encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint8_t mask = 0;

    if(buffer == NULL || bufLen == 0 || t3512 == NULL || encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

	if(len + sizeof(*t3512) > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}
	
	memcpy(buffer + len, &(t3512->len), sizeof(t3512->len));
	len += sizeof(t3512->len);
	nasLog("Timer 3512 : len %d | current len %d | encoded %d bytes", 
		t3512->len, len, sizeof(t3512->len));

	mask = 0;
	mask = mask | ((t3512->value));
	mask = mask | (t3512->unit << 5);
	*(buffer + len) = mask;
	len++;

	nasLog("unit %d | value %d | current len %d | encoded 1 byte", 
		t3512->unit, t3512->value, len);

	*encodedLen = len;
    nasLogEXIT;
	return SUCCESS;
}
