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

#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/AuthparamAUTN.h"

int decodeAuthParamAUTN(uint8_t *buffer, AUTN_t *autn, uint32_t bufLen,
                        uint32_t* decodedLen)
{
    nasLogENTRY;
    if(bufLen < AUTH_PARAM_AUTN_LEN_MIN || buffer == NULL || autn == NULL ||
       decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /* check if the length is valid */
    if(buffer[0] < AUTH_PARAM_AUTN_LEN_MIN
       || buffer[0] > AUTH_PARAM_AUTN_LEN_MAX) {
        higLog("Invalid length %d", buffer[0]);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }
    autn->len = buffer[0];
    *decodedLen = sizeof(autn->len);

    if(autn->len > bufLen - (*decodedLen)) {
        higLog("Buffer insufficient to copy AUTN");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(autn->AUTN, buffer + (*decodedLen), buffer[0]);
    *decodedLen += autn->len;
    nasLog("Auth Param decoded autn len %d | decoded %d", autn->len,
            *decodedLen);

    nasLogEXIT;
    return SUCCESS;
}

int encodeAuthParamAUTN(uint8_t *buffer, uint32_t bufLen, AUTN_t *autn,
                        uint32_t *encodedLen)
{
    nasLogENTRY;
    if(bufLen < AUTH_PARAM_AUTN_LEN_MIN || buffer == NULL || autn == NULL ||
       encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /* check if the length is valid */
    if(autn->len < AUTH_PARAM_AUTN_LEN_MIN
       || autn->len > AUTH_PARAM_AUTN_LEN_MAX) {
        higLog("Invalid length %d", autn->len);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }
    buffer[0] = autn->len;
    *encodedLen = sizeof(autn->len);

    if(autn->len > bufLen - (*encodedLen)) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
    memcpy(buffer + (*encodedLen), autn->AUTN, autn->len);
    *encodedLen += autn->len;
    nasLog("Auth Param decoded autn len %d | decoded %d", autn->len,
            *encodedLen);

    nasLogEXIT;
    return SUCCESS;
}
