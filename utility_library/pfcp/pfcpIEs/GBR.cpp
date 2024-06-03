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

#include "pfcp/pfcpIEs/GBR.h"
#include "pfcp/include/pfcpUtil.h"

int GBREncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    GBR_t *GBR = (GBR_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || GBR == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_GBR_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len += addIEI(buffer+len, GBR->IEI);
    pfcpLog("IEI %d encoded, len %d", GBR->IEI, len);
    len += addLEN(buffer+len, PFCP_GBR_LEN);
    pfcpLog("IE Len %d encoded, len %d", PFCP_GBR_LEN, len);

    if(len+PFCP_GBR_UL_GBR_LEN > bufLen){
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    buffer[len++] = GBR->UL_GBR >> 32;
    buffer[len++] = (GBR->UL_GBR >> 24) & 0xFF;
    buffer[len++] = (GBR->UL_GBR >> 16) & 0xFF;
    buffer[len++] = (GBR->UL_GBR >> 8) & 0xFF;
    buffer[len++] = GBR->UL_GBR & 0xFF;
    pfcpLog("GBR encoded UL GBR %d"
            "len %d", GBR->UL_GBR, len);

    if(len+PFCP_GBR_DL_GBR_LEN > bufLen){
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    buffer[len++] = GBR->DL_GBR >> 32;
    buffer[len++] = (GBR->DL_GBR >> 24) & 0xFF;
    buffer[len++] = (GBR->DL_GBR >> 16) & 0xFF;
    buffer[len++] = (GBR->DL_GBR >> 8) & 0xFF;
    buffer[len++] = GBR->DL_GBR & 0xFF;

    pfcpLog("GBR encoded DL GBR %d"
        "len %d", GBR->DL_GBR, len);

    pfcpLog("GBR encoded, len %d", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}

int GBRDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    GBR_t *GBR = (GBR_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || GBR == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_GBR_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len+= getIEI(buffer + len, &GBR->IEI);
    pfcpLog("IEI %d decoded, len %d", GBR->IEI, len);
    uint16_t ieLen = 0;
    len+= getLEN(buffer + len, &ieLen);
    pfcpLog("IE Len %d decoded, len %d", GBR->IEI, len);
    if( ieLen != PFCP_GBR_LEN) {
        higLog("Length Value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    GBR->UL_GBR = 0 | ((uint64_t)buffer[len] << 32) |
               ((uint64_t)buffer[len+1] << 24) |  ((uint64_t)buffer[len+2] << 16) |
               ((uint64_t)buffer[len+3] << 8) | ((uint64_t)buffer[len+4]);
    len+=5;
    pfcpLog("GBR decoded UL_GBR %d"
        "len %d", GBR->UL_GBR, len);

    GBR->DL_GBR = 0 | ((uint64_t)buffer[len] << 32) |
               ((uint64_t)buffer[len+1] << 24) |  ((uint64_t)buffer[len+2] << 16) |
               ((uint64_t)buffer[len+3] << 8) | ((uint64_t)buffer[len+4]);
    len+=5;
    pfcpLog("GBR decoded DL_GBR %d"
        "len %d", GBR->DL_GBR, len);

    pfcpLog("GBR decoded,len %d", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
