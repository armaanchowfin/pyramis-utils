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

#include "pfcp/pfcpIEs/pfcpFailedRuleId.h"
#include "pfcp/include/pfcpUtil.h"

int failedRuleIdencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                       uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    failedRuleId_t *frID = (failedRuleId_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN <  bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    len += addIEI(buffer + len, frID->IEI);
    uint16_t lenOffset = len;
    len += PFCP_IE_LEN_SIZE;
    pfcpLog("Encoded IEI and LEN, len %d", len);

    if(len + PFCP_FAILED_RULE_ID_TYPE_SIZE > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    buffer[len] = frID->ruleIdType;
    len += PFCP_FAILED_RULE_ID_TYPE_SIZE;
    pfcpLog("Encoded ruleIdType %d", frID->ruleIdType);

    switch(frID->ruleIdType) {
        case PFCP_FAILED_RULE_ID_TYPE_PDR:
            if(len + PFCP_FAILED_RULE_ID_PDR_ID_SIZE > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }
            len += addU16(buffer + len, frID->ruleIdValue.pdrId);
            pfcpLog("Added PDR Id %d, len %d", frID->ruleIdValue.pdrId, len);
            break;

        case PFCP_FAILED_RULE_ID_TYPE_FAR:
            if(len + PFCP_FAILED_RULE_ID_FAR_ID_SIZE > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }
            len += addU32(buffer + len, frID->ruleIdValue.farId);
            pfcpLog("Added FAR Id %llu, len %d", frID->ruleIdValue.farId, len);
            break;

        default:
            errno = E_PFCP_ERROR_INVALID_INPUT;
            higLog("Unkown/Unsupported rule id type %d", frID->ruleIdType);
            pfcpLogEXIT;
            return FAILURE;
    }

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);

    *encodedLen = len;
    pfcpLog("Failed Rule ID encoded, len %d");
    pfcpLogEXIT;
    return SUCCESS;
}
int failedRuleIddecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                       uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    failedRuleId_t *frID = (failedRuleId_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN <  bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    len += getIEI(buffer + len, &frID->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);
    if(ieLen < PFCP_FAILED_RULEID_MIN_LENGTH
            || ieLen > PFCP_FAILED_RULEID_MAX_LENGTH) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Incorrect ieLen %d", ieLen);
        return FAILURE;
    }
    pfcpLog("Decoded IEI and LEN, len %d", len);

    if(len + PFCP_FAILED_RULE_ID_TYPE_SIZE > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    frID->ruleIdType = buffer[len] & 0x1F; /* Five Bits */
    len += PFCP_FAILED_RULE_ID_TYPE_SIZE;

    if(frID->ruleIdType > PFCP_FAILED_RULE_ID_TYPE_FAR) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Unkown/Unsupported rule id type %d", frID->ruleIdType);
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLog("Decoded ruleIdType %d", frID->ruleIdType);

    switch(frID->ruleIdType) {
        case PFCP_FAILED_RULE_ID_TYPE_PDR:
            if(len + PFCP_FAILED_RULE_ID_PDR_ID_SIZE > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }
            len += getU16(buffer + len, &frID->ruleIdValue.pdrId);
            pfcpLog("Decoded PDR Id %d, len %d", frID->ruleIdValue.pdrId, len);
            break;

        case PFCP_FAILED_RULE_ID_TYPE_FAR:
            if(len + PFCP_FAILED_RULE_ID_FAR_ID_SIZE > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }
            len += getU32(buffer + len, &frID->ruleIdValue.farId);
            pfcpLog("Decoded FAR Id %llu, len %d", frID->ruleIdValue.farId,len);
            break;

        default:
            errno = E_PFCP_ERROR_INVALID_INPUT;
            higLog("Unkown/Unsupported rule id type %d", frID->ruleIdType);
            pfcpLogEXIT;
            return FAILURE;
    }

    *decodedLen = len;
    pfcpLog("Failed Rule ID decoded, len %d");
    pfcpLogEXIT;
    return SUCCESS;
}
