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

#include "pfcp/pfcpIEs/QERCorrelationId.h"
#include "pfcp/include/pfcpUtil.h"

int QerCorrelationIDEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    QerCorrelationID_t *qerCorrelationId = (QerCorrelationID_t *)ptr;

    if(buffer == NULL || bufLen < 0 || qerCorrelationId == NULL || encodedLen == NULL)
    {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_QER_CORRELATION_ID_LEN + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len += addIEI(buffer + len, qerCorrelationId->IEI);
    len += addLEN(buffer + len, PFCP_QER_CORRELATION_ID_LEN);
    pfcpLog("IEI %d", qerCorrelationId->IEI);
    pfcpLog("IEI and LEN encoded, len %d", len);

    len += addU32(buffer + len, qerCorrelationId->value);
    pfcpLog("QER Correlation ID Value %llu encoded, len %d", qerCorrelationId->value, len);

    pfcpLog("QER Correlation ID encoded, len %d", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;

}

int QerCorrelationIDDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    QerCorrelationID_t *qerCorrelationId = (QerCorrelationID_t *)ptr;

    if(buffer == NULL || bufLen < 0 || qerCorrelationId == NULL || decodedLen == NULL)
    {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_QER_CORRELATION_ID_LEN + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len+= getIEI(buffer + len, &qerCorrelationId->IEI);
    pfcpLog("IEI %d decoded, len %d", qerCorrelationId->IEI, len);

    uint16_t ieLen = 0;
    len+= getLEN(buffer + len, &ieLen);
    pfcpLog("IE Len %d decoded, len %d", qerCorrelationId->IEI, len);

    if( ieLen != PFCP_QER_CORRELATION_ID_LEN) {
        higLog("Length Value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    len += getU32(buffer + len, &qerCorrelationId->value);
    pfcpLog("QER Correlation ID Value %llu decoded, len %d", qerCorrelationId->value, len);

    pfcpLog("QER Correlation ID decoded, len %d", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
