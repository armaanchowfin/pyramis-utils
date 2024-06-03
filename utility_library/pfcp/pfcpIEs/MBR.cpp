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

#include "pfcp/pfcpIEs/MBR.h"
#include "pfcp/include/pfcpUtil.h"

int MBREncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    MBR_t *MBR = (MBR_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || MBR == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_MBR_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len += addIEI(buffer+len, MBR->IEI);
    pfcpLog("IEI %d encoded, len %d", MBR->IEI, len);
    len += addLEN(buffer+len, PFCP_MBR_LEN);
    pfcpLog("IE Len %d encoded, len %d", PFCP_MBR_LEN, len);

    if(len+PFCP_MBR_UL_MBR_LEN > bufLen){
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    buffer[len++] = MBR->UL_MBR >> 32;
    buffer[len++] = (MBR->UL_MBR >> 24) & 0xFF;
    buffer[len++] = (MBR->UL_MBR >> 16) & 0xFF;
    buffer[len++] = (MBR->UL_MBR >> 8) & 0xFF;
    buffer[len++] = MBR->UL_MBR & 0xFF;

    pfcpLog("MBR encoded UL MBR %d"
        "len %d", MBR->UL_MBR, len);

    if(len+PFCP_MBR_DL_MBR_LEN > bufLen){
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    buffer[len++] = MBR->DL_MBR >> 32;
    buffer[len++] = (MBR->DL_MBR >> 24) & 0xFF;
    buffer[len++] = (MBR->DL_MBR >> 16) & 0xFF;
    buffer[len++] = (MBR->DL_MBR >> 8) & 0xFF;
    buffer[len++] = MBR->DL_MBR & 0xFF;
    pfcpLog("MBR encoded DL MBR %d"
        "len %d", MBR->DL_MBR, len);

    pfcpLog("MBR encoded, len %d", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}

int MBRDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    MBR_t *MBR = (MBR_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || MBR == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_MBR_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len+= getIEI(buffer + len, &MBR->IEI);
    pfcpLog("IEI %d decoded, len %d", MBR->IEI, len);
    uint16_t ieLen = 0;
    len+= getLEN(buffer + len, &ieLen);
    pfcpLog("IE Len %d decoded, len %d", MBR->IEI, len);
    if( ieLen != PFCP_MBR_LEN) {
        higLog("Length Value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    MBR->UL_MBR = 0 | ((uint64_t)buffer[len] << 32) |
               ((uint64_t)buffer[len+1] << 24) |  ((uint64_t)buffer[len+2] << 16) |
               ((uint64_t)buffer[len+3] << 8) | ((uint64_t)buffer[len+4]);
    len+=5;
    pfcpLog("MBR decoded UL_MBR %d"
        "len %d", MBR->UL_MBR, len);

    MBR->DL_MBR = 0 | ((uint64_t)buffer[len] << 32) |
               ((uint64_t)buffer[len+1] << 24) |  ((uint64_t)buffer[len+2] << 16) |
               ((uint64_t)buffer[len+3] << 8) | ((uint64_t)buffer[len+4]);
    len+=5;
    pfcpLog("MBR decoded DL_MBR %d"
        "len %d", MBR->DL_MBR, len);

    pfcpLog("MBR decoded,len %d", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
