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

#include "pfcp/pfcpIEs/gateStatus.h"
#include "pfcp/include/pfcpUtil.h"

int pfcpGateStatusEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
            uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    gateStatus_t *gateStatus = (gateStatus_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || gateStatus == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_GATE_STATUS_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len += addIEI(buffer+len, gateStatus->IEI);
    pfcpLog("IEI %d encoded, len %d", gateStatus->IEI, len);
    len += addLEN(buffer + len, PFCP_GATE_STATUS_LEN);
    pfcpLog("IE Len %d encoded, len %d", PFCP_GATE_STATUS_LEN, len);

    uint8_t mask = 0;
    mask |= gateStatus->DL_GATE;
    mask |= gateStatus->UL_GATE << 2;   //TODO: Check this bit shifting given that mask is 16 bits not 8bits

    buffer[len] = mask;
    len++;
    pfcpLog("Gate Status encoded DL_GATE %d, UL_GATE %d,"
        "len %d",  gateStatus->DL_GATE, gateStatus->UL_GATE, len);
    pfcpLog("Gate Status Encoded, len %d", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}


int pfcpGateStatusDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
            uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    gateStatus_t *gateStatus = (gateStatus_t *)ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || decodedLen== NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

     if(bufLen < PFCP_GATE_STATUS_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len+= getIEI(buffer + len, &gateStatus->IEI);
    uint16_t ieLen = 0;
    len+= getLEN(buffer + len, &ieLen);
    if( ieLen != PFCP_GATE_STATUS_LEN) {
        higLog("Length Value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    uint8_t mask = 0;
    mask = buffer[len];
    len++;
    gateStatus->DL_GATE = mask;
    gateStatus->UL_GATE = mask >> 2;    //TODO: Check this bit shifting given that mask is 16 bits not 8bits

    pfcpLog("Gate Status decoded DL_GATE %d, UL_GATE %d,"
        "len %d",  gateStatus->DL_GATE, gateStatus->UL_GATE, len);

    pfcpLog("Gate Status Decoded, len %d", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
