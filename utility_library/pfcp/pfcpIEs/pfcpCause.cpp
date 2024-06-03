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

#include "pfcp/pfcpIEs/pfcpCause.h"
#include "pfcp/include/pfcpUtil.h"

int pfcpCauseEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    pfcpCause_t *cause = (pfcpCause_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || cause == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_CAUSE_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len += addIEI(buffer + len, cause->IEI);
    len += addLEN(buffer + len, PFCP_CAUSE_LEN);

    buffer[len] = cause->cause;
    len += PFCP_CAUSE_LEN;

    pfcpLog("Cause %d Encoded, encoded %d bytes ", cause->cause, len);

    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}

int pfcpCauseDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{
    pfcpLogENTRY;
    pfcpCause_t *cause = (pfcpCause_t *)ptr;
    uint len = 0;

    if(buffer == NULL || bufLen <= 0 || cause == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_CAUSE_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len+= getIEI(buffer + len, &cause->IEI);
    uint16_t ieLen = 0;
    len+= getLEN(buffer + len, &ieLen);
    if( ieLen != PFCP_CAUSE_LEN) {
        higLog("Length Value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    cause->cause = buffer[len];
    len+= PFCP_CAUSE_LEN;

    pfcpLog("Cause %d Decoded, decoded %d bytes ", cause->cause, len);

    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
