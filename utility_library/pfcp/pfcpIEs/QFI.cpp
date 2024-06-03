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

#include "pfcp/pfcpIEs/QFI.h"
#include "pfcp/include/pfcpUtil.h"

int QFIEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
            uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    QFI_t *QFI = (QFI_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || QFI == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_QFI_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len += addIEI(buffer+len, QFI->IEI);
    pfcpLog("IEI %d encoded, len %d", QFI->IEI, len);
    len += addLEN(buffer + len, PFCP_QFI_LEN);
    pfcpLog("IE Len %d encoded, len %d", PFCP_QFI_LEN, len);

    uint8_t mask = 0;
    mask |= QFI->value;

    buffer[len] = mask;
    len++;
    pfcpLog("QFI encoded value %d,"
        "len %d",  QFI->value, len);
    pfcpLog("QFI Encoded, len %d", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}


int QFIDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
            uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    QFI_t *QFI = (QFI_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || QFI == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_QFI_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len+= getIEI(buffer + len, &QFI->IEI);
    uint16_t ieLen = 0;
    len+= getLEN(buffer + len, &ieLen);
    if( ieLen != PFCP_QFI_LEN) {
        higLog("Length Value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    uint8_t mask = 0;
    mask = buffer[len];
    len++;
    QFI->value = mask;

    pfcpLog("QFI decoded value %d,"
        "len %d",  QFI->value, len);

    pfcpLog("QFI Decoded, len %d", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
