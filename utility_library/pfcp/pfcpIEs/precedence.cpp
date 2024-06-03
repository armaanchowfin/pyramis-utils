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

#include "pfcp/pfcpIEs/precedence.h"
#include "pfcp/include/pfcpUtil.h"

int precedenceEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                         uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    precedence_t *pre = (precedence_t*)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_PRECEDENCE_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed Len IE, no more checks necessary */

    len += addIEI(buffer + len, pre->IEI);
    int lenOffset = len;
    len += 2;
    pfcpLog("IEI and LEN encoded, len %d", len);

    len += addU32(buffer + len, pre->precedence);
    pfcpLog("Added precedence %llu, len %d", pre->precedence, len);

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);

    *encodedLen = len;
    pfcpLogEXIT;
}

int precedenceDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                         uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    precedence_t *pre = (precedence_t*)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_PRECEDENCE_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed Len IE, no more checks necessary */

    len += getIEI(buffer + len, &pre->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);

    if(ieLen != PFCP_PRECEDENCE_LEN - PFCP_IE_HDR_LEN) {
        higLog("ieLen %d incorrect, expecting %d", ieLen, PFCP_PRECEDENCE_LEN);
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLog("IEI and LEN %d decoded, len %d", ieLen , len);

    len += getU32(buffer + len, &pre->precedence);
    pfcpLog("Decoded precedence %llu, len %d", pre->precedence, len);

    *decodedLen = len;
    pfcpLogEXIT;
}
