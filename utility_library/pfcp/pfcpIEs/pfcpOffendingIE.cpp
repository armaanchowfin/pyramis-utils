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

#include "pfcp/pfcpIEs/pfcpOffendingIE.h"
#include "pfcp/include/pfcpUtil.h"

int offendingIEencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                      uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    offendingIE_t *offIE = (offendingIE_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_OFFENDING_IE_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len += addIEI(buffer + len, offIE->IEI);
    len += addLEN(buffer + len, PFCP_OFFENDING_IE_LEN);
    len += addIEI(buffer + len, offIE->offendingIEI);

    *encodedLen = len;
    pfcpLog("Offending IE %d encoded, len %d", offIE->offendingIEI, len);

    pfcpLogEXIT;
    return SUCCESS;
}
int offendingIEdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                      uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    offendingIE_t *offIE = (offendingIE_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_OFFENDING_IE_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len += getIEI(buffer + len, &offIE->IEI);
    uint16_t ieLen;
    len += getLEN(buffer + len, &ieLen);
    if(ieLen != PFCP_OFFENDING_IE_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Incorrect ieLen %d", ieLen);
        pfcpLogEXIT;
        return FAILURE;
    }

    len += getIEI(buffer + len, &offIE->offendingIEI);

    *decodedLen = len;
    pfcpLog("Offending IE %d decoded, len %d", offIE->offendingIEI, len);

    pfcpLogEXIT;
    return SUCCESS;
}
