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

#include "pfcp/pfcpIEs/packetDetectionRuleId.h"
#include "pfcp/include/pfcpUtil.h"

int pfcpPdrIDencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    pfcpPdrId_t *pdrId = (pfcpPdrId_t *) ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_PDR_ID_LEN + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed Len IE, no more len checks necessary */

    len += addIEI(buffer + len, pdrId->IEI);
    int lenOffset = len;
    len+= PFCP_IE_LEN_SIZE;
    pfcpLog("IEI and LEN encoded, len %d", len);

    buffer[len] = pdrId->ruleId >> 8;
    len++;
    buffer[len] = pdrId->ruleId & 0xFF;
    len++;
    pfcpLog("Rule Id encoded %d, len %d", pdrId->ruleId, len);

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);
    *encodedLen = len;

    pfcpLogEXIT;
    return SUCCESS;
}

int pfcpPdrIDdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    pfcpPdrId_t *pdrId = (pfcpPdrId_t *) ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_PDR_ID_LEN + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed Len IE, no more len checks necessary */

    len += getIEI(buffer + len, &pdrId->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);
    pfcpLog("IEI and LEN decoded, len %d", len);

    if(ieLen != PFCP_PDR_ID_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Length incorrect");
        pfcpLogEXIT;
        return FAILURE;
    }

    pdrId->ruleId = (((uint16_t)buffer[len]) << 8) | (buffer[len+1]);
    len += 2;
    pfcpLog("rule ID decoded %d", pdrId->ruleId);

    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
