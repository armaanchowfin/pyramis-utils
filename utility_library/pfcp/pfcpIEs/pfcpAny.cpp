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

#include "pfcp/pfcpIEs/pfcpAny.h"
#include "pfcp/include/pfcpUtil.h"

int pfcpAnyEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    int len = 0;
    pfcpAny *any = (pfcpAny *)ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(PFCP_IE_HDR_LEN + any->bufLen > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += addIEI(buffer + len, any->IEI);
    len += addLEN(buffer + len, any->bufLen);
    memcpy(buffer + len, any->buffer, any->bufLen);
    len+= any->bufLen;

    *encodedLen = len;
    pfcpLog("Encoded %d bytes", any->bufLen);
    pfcpLogEXIT;
}

int pfcpAnyDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    int len = 0;
    pfcpAny *any = (pfcpAny *)ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(PFCP_IE_HDR_LEN + any->bufLen > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += getIEI(buffer + len, &any->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);
    if(ieLen + PFCP_IE_HDR_LEN > PFCP_ANY_BUF_SIZE ||
            ieLen < PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    any->bufLen = ieLen;
    if(len + any->bufLen > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    memcpy(any->buffer, buffer + len, any->bufLen);
    len += any->bufLen;

    *encodedLen = len;
    pfcpLog("Encoded %d bytes", any->bufLen);
    pfcpLogEXIT;
}
