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
#include "common/include/datatypes.h"
#include "common/include/setup.h"
#include "platform/include/logging.h"
#include "nas/sessionMgmt/_5gsmMsgApi.h"
#include "nas/sessionMgmt/SessionEstdReject.h"


int encodeSessionEstdReject(uint8_t *buffer, uint32_t bufLen,
        SessionEstdReject_t *msgStruct,uint32_t* encodedLen)
{
    nasLogENTRY;
    uint32_t len =0;
    uint32_t encLen = 0;

    if(bufLen == 0 || buffer == NULL ||
        msgStruct == NULL || encodedLen == NULL) {
        higLog("Invalid Input parameters. Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /*Encode Code Here*/
    /*5GSM Cause*/
    encLen = 0;
    if(encode5gsmCause(buffer + len, bufLen - len,
        &(msgStruct->_5gsmCause), &encLen) == FAILURE)
    {
        higLog("Encoding 5GSM Cause Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += encLen;
    nasLog("5GSM Cause encoded | currentLen = %d | encoded %d bytes", len,
                            encLen);

    nasLog("Session Estb Reject Encoded, len %d", len);
    *encodedLen  = len;
    nasLogEXIT;
    return SUCCESS;

}

int decodeSessionEstdReject(uint8_t *buffer,SessionEstdReject_t *msgStruct,
        uint32_t bufLen)
{
    nasLogENTRY;
    unsigned int len = 0;
    uint32_t decodLen = 0;

    if(buffer == NULL || msgStruct == NULL|| bufLen ==0) {
        higLog("Invalid Input parameters. Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /*Decode Code Here*/
    /*5GSM Cause*/
   decodLen = 0;
   if(decode5gsmCause(buffer + len, bufLen - len,
                    &(msgStruct->_5gsmCause), &decodLen)
                                 == FAILURE)
    {
            higLog("Decoding 5GSM Cause Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += decodLen;

    nasLog("5GSM Cause decoded | currentLen %d, len %d",
                decodLen, len);
    nasLog("Session Estb Reject Decoded, len %d", len);

    nasLogEXIT;
    return SUCCESS;
}
