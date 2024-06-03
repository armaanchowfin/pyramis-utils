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

#include "nas/NasDatatypes/requestType.h"

int decodeRequestType(uint8_t *buffer,  uint32_t bufLen,
                      requestType_t *reqType, uint32_t *decodedLen)
{
    nasLogENTRY;
    uint32_t len  = 0;

    if(buffer == NULL || bufLen == 0|| reqType == NULL || decodedLen == NULL){
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    uint8_t value = buffer[len] & 0x07;
    len++;
    if(value == REQUEST_TYPE_INVALID_VALUE) {
        higLog("Invalid Value, %d", value);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }
    reqType->value = value;
    nasLog("Request Type value %d", reqType->value);

    *decodedLen = len;
    nasLogEXIT;
    nasLog("Request Type decoded, len %d", len);
    return SUCCESS;
}

int encodeRequestType(uint8_t *buffer, uint32_t bufLen,
                      requestType_t *reqType, uint32_t *encodedLen)
{
    nasLogENTRY;
    uint32_t len  = 0;

    if(buffer == NULL || bufLen == 0|| reqType == NULL || encodedLen == NULL){
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[len] |= reqType->value;
    len++;
    nasLog("Request Type value %d", reqType->value);

    *encodedLen = len;
    nasLog("Request Type encoded, len %d", len);

    nasLogEXIT;
    return SUCCESS;
}
