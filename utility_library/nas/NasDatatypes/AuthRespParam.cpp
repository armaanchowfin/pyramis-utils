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

#include "nas/NasDatatypes/AuthRespParam.h"

int decodeAuthRespParam(uint8_t *buffer, AuthRespParameter_t *arp,
                        uint32_t bufLen, uint32_t* decodedLen)
{
    nasLogENTRY;
    if(bufLen < AUTH_RESP_PARAM_MIN_LEN || buffer == NULL || arp == NULL ||
       decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /* check if len is valid */
    if(buffer[0] < AUTH_RESP_PARAM_MIN_LEN ||
       buffer[0] > AUTH_RESP_PARAM_MAX_LEN) {
        higLog("Invalid Length %d", buffer[0]);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    arp->arpLen = buffer[0];
    *decodedLen = sizeof(arp->arpLen);

    if(arp->arpLen > bufLen - (*decodedLen)){
        higLog("Unsufficient BuffLen %d, arplen %d", bufLen, buffer[0]);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(arp->RESstar, buffer + (*decodedLen), arp->arpLen);
    *decodedLen += arp->arpLen;
    nasLog("AuthRespParameter decoded | arpLen = %d | decodedLen %d",
            arp->arpLen, *decodedLen);
    /*TODO print RES*/

    nasLogEXIT;
    return SUCCESS;
}

int encodeAuthRespParam(uint8_t *buffer, uint32_t bufLen,
                        AuthRespParameter *arp, uint32_t *encodedLen)
{
    nasLogENTRY;
    if(bufLen < AUTH_RESP_PARAM_MIN_LEN || buffer == NULL || arp == NULL ||
       encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /* check if len is valid */
    if(arp->arpLen < AUTH_RESP_PARAM_MIN_LEN ||
       arp->arpLen > AUTH_RESP_PARAM_MAX_LEN) {
        higLog("Invalid Length %d", arp->arpLen);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[0] = arp->arpLen;
    *encodedLen = sizeof(arp->arpLen);
    if(arp->arpLen > bufLen - (*encodedLen)){
        higLog("Unsufficient BuffLen %d, arplen %d", bufLen, buffer[0]);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }


    memcpy(buffer + (*encodedLen), arp->RESstar, arp->arpLen);
    *encodedLen += arp->arpLen;
    nasLog("AuthRespParameter encoded | arpLen = %d | encodedLen %d",
            arp->arpLen, encodedLen);
    /*TODO print RES */

    nasLogEXIT;
    return SUCCESS;
}
