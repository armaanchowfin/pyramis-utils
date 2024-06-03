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

#include "pfcp/pfcpIEs/sdInterface.h"
#include "pfcp/include/pfcpUtil.h"

int sdIfaceEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                         uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    sdIface_t *sdiface = (sdIface_t *) ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_SOURCE_IFACE_LEN + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, no more len checks necessary */

    len += addIEI(buffer + len, sdiface->IEI);
    len += addLEN(buffer + len, PFCP_SOURCE_IFACE_LEN);
    pfcpLog("Added len and IEI, len %d", len);

    buffer[len] = sdiface->interfaceValue;
    len++;
    pfcpLog("Encoded interfaceValue %d", sdiface->interfaceValue);

    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}

int sdIfaceDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                         uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    sdIface_t *sdiface = (sdIface_t *) ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_SOURCE_IFACE_LEN + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, no more len checks necessary */

    len += getIEI(buffer + len, &sdiface->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);

    if(ieLen != PFCP_SOURCE_IFACE_LEN ) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Incorrect ieLen");
        pfcpLogEXIT;
        return FAILURE;
    }

    sdiface->interfaceValue = buffer[len] & 0x0F;
    len++;

    if(sdiface->interfaceValue > PFCP_SOURCE_IFACE_VALUE_CP_FUNC) {
        errno =E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Invalid  interfaceValue %d", sdiface->interfaceValue);
        pfcpLogEXIT;
        return FAILURE;
    }

    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
