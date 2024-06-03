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

#include <bits/stdc++.h>
#include "nas/sessionMgmt/SessionRelComplete.h"


int encodeSessionRelComplete(uint8_t *buffer, uint32_t bufLen,
                        SessionRelComplete_t *msgStruct,uint32_t* encodedLen)
{
    nasLogENTRY;
    uint32_t len =0;

    if(buffer == NULL||msgStruct == NULL||encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    *encodedLen  = len;
    nasLogEXIT;
    return SUCCESS;
}
int decodeSessionRelComplete(uint8_t *buffer, SessionRelComplete_t *msgStruct,
                        uint32_t decodLen)
{
    nasLogENTRY;
    uint32_t len =0;

    if(buffer == NULL || msgStruct == NULL) {
        higLog("Invalid Input parameters. Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }


    nasLogEXIT;
    return SUCCESS;
}
