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

#include "pfcp/pfcpIEs/recTimeStamp.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcp/include/pfcpUtil.h"


int recTimeStampEncode(uint8_t *buffer, uint16_t bufLen, void *ptr,
                        uint16_t* encodedLen)
{
    pfcpLogENTRY;
    uint len = 0;
    recTimeStamp_t *rts = (recTimeStamp_t*) ptr;

    if(buffer == NULL || bufLen < 0 ||
            rts == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_REC_TIME_STAMP_LENGTH + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* since the IE is of fixed length, length check is complete above.
     * Edit the buffer without any fear :) */

    len += addIEI(buffer + len, rts->IEI);
    len += addLEN(buffer + len, PFCP_REC_TIME_STAMP_LENGTH);

    memcpy(buffer + len, rts->timestamp, PFCP_REC_TIME_STAMP_LENGTH);
    len += PFCP_REC_TIME_STAMP_LENGTH;

    pfcpLog("Recovery Time stamp encoded, %d bytes", len);

    *encodedLen = len;

    pfcpLogEXIT;
    return SUCCESS;
}

int recTimeStampDecode(uint8_t *buffer, uint16_t bufLen, void *ptr,
        uint16_t *decodedLen)
{
    pfcpLogENTRY;
    recTimeStamp_t *rts = (recTimeStamp_t*) ptr;
    uint len = 0;

    if(buffer == NULL || bufLen < 0 || rts == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_REC_TIME_STAMP_LENGTH + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }
    /*fixed length IE, length checks comeplete above */

    len+= getIEI(buffer + len, &rts->IEI);

    uint16_t msgLen = 0;
    len+= getLEN(buffer + len, &msgLen);
    if(msgLen != PFCP_REC_TIME_STAMP_LENGTH) {
        higLog("Length value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    memcpy(rts->timestamp, buffer + len, PFCP_REC_TIME_STAMP_LENGTH);
    len+= PFCP_REC_TIME_STAMP_LENGTH;

    pfcpLog("Recovery Time Stamp decoded, %d bytes", len);

    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
