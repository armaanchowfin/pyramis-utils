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

#include "nas/NasDatatypes/AuthFailParam.h"

int decodeAuthFailParam(uint8_t *buffer, AuthFailParameter_t *afp,
                        uint32_t bufLen, uint32_t* decodedLen)
{
    nasLogENTRY;
    /* All we need is AUTH_FAIL_PARAM_LEN - 1 == 15 bytes of space 
     * to be able to decode successfully */
    if((bufLen < AUTH_FAIL_PARAM_LEN-1) || buffer == NULL || afp == NULL ||
       decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /* check if len is valid */
    if(buffer[0] != AUTH_FAIL_PARAM_LEN) {
        higLog("Invalid Length %d", buffer[0]);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    afp->afpLen = buffer[0];
    *decodedLen = sizeof(afp->afpLen);

    if(afp->afpLen - (uint)AUTH_FAIL_PARAM_HEADER_LEN > bufLen - (*decodedLen)){
        higLog("Unsufficient BuffLen %d, afplen %d", bufLen, buffer[0]);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(afp->AUTS, buffer + (*decodedLen),
            afp->afpLen - AUTH_FAIL_PARAM_HEADER_LEN);
    *decodedLen += afp->afpLen - AUTH_FAIL_PARAM_HEADER_LEN;
    nasLog("AuthFailParameter decoded | afpLen = %d | decodedLen %d",
            afp->afpLen, *decodedLen);
    /*TODO print RES*/

    nasLogEXIT;
    return SUCCESS;
}

int encodeAuthFailParam(uint8_t *buffer, uint32_t bufLen,
                        AuthFailParameter *afp, uint32_t *encodedLen)
{
    nasLogENTRY;
    /* buffer must have already been filled with IEI value 
     * hence 1 accounts for IEI in equation bufLen < AUTH_FAIL_PARAM_LEN - 1 */
    if((bufLen < AUTH_FAIL_PARAM_LEN-1) || buffer == NULL || afp == NULL ||
       encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /* check if len is valid */
    if(afp->afpLen != AUTH_FAIL_PARAM_LEN) {
        higLog("Invalid Length %d", afp->afpLen);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[0] = afp->afpLen;
    *encodedLen = sizeof(afp->afpLen);
    if(afp->afpLen - (uint)AUTH_FAIL_PARAM_HEADER_LEN > bufLen - (*encodedLen)){
        higLog("Unsufficient BuffLen %d, afplen %d", bufLen, buffer[0]);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }


    memcpy(buffer + (*encodedLen), afp->AUTS,
            afp->afpLen - AUTH_FAIL_PARAM_HEADER_LEN);
    *encodedLen += afp->afpLen - AUTH_FAIL_PARAM_HEADER_LEN;
    nasLog("AuthFailParameter encoded | afpLen = %d | encodedLen %d",
            afp->afpLen, encodedLen);
    /*TODO print AUTS */

    nasLogEXIT;
    return SUCCESS;
}
