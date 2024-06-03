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

#include "nas/sessionMgmt/SessionRelCommand.h"


int encodeSessionRelCommand(uint8_t *buffer, uint32_t bufLen,
                        SessionRelCommand_t *msgStruct,uint32_t* encodedLen)
{
    nasLogENTRY;
    uint32_t len =0;

    if(buffer == NULL||bufLen == 0||msgStruct == NULL||encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + sizeof(msgStruct->_5gsmCause) > bufLen){
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[len] = msgStruct->_5gsmCause;
    len++;
    nasLog("encoded 5GSM cause %d | current len %d | sizeof %d",
            msgStruct->_5gsmCause, len, sizeof(msgStruct->_5gsmCause));

    *encodedLen  = len;
    nasLogEXIT;
    return SUCCESS;
}
int decodeSessionRelCommand(uint8_t *buffer, SessionRelCommand_t *msgStruct,
                        uint32_t decodLen)
{
    nasLogENTRY;
    uint32_t len =0;

    if(buffer == NULL || msgStruct == NULL || decodLen == 0) {
        higLog("Invalid Input parameters. Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + sizeof(msgStruct->_5gsmCause) > decodLen){
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    msgStruct->_5gsmCause = buffer[len];
    len++;
    nasLog("decoded 5GSM cause %d | current len %d | sizeof %d",
            msgStruct->_5gsmCause, len, sizeof(msgStruct->_5gsmCause));


    nasLogEXIT;
    return SUCCESS;
}
