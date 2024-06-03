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

#include "pfcp/pfcpIEs/SDFFilter.h"
#include "pfcp/include/pfcpUtil.h"

int sdfFilterEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    sdfFilter_t *sdfFilter = (sdfFilter_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || sdfFilter == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

   if(len + PFCP_IE_SDF_FILTER_MIN_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
   
    len += addIEI(buffer+len, sdfFilter->IEI);
    pfcpLog("IEI %d encoded, len %d", sdfFilter->IEI, len);
    uint16_t lenOffset = len;
    len += PFCP_IE_LEN_SIZE;
    
    buffer[len] = (sdfFilter->BID ? 0x10 : 0x0 ) |
        (sdfFilter->FL ? 0x08 : 0x0 ) | (sdfFilter->SPI ? 0x04 : 0x0)|
        (sdfFilter->TTC ? 0x02 : 0x0 ) | (sdfFilter->FD ? 0x01 : 0x0);
    /*TODO: get spare value 3 bits 
    buffer[len] |= (sdfFilter->spare);*/
    len += PFCP_IE_SDF_FILTER_MASK_LEN;
    
    buffer[len] = sdfFilter->spare;
    len++;

    if (sdfFilter->spare != 0) {
            errno = E_PFCP_ERROR_INVALID_INPUT;
            higLog("Invalid Input");
            pfcpLogEXIT;
            return FAILURE;
    }

    if(sdfFilter->FD) {
        if(len + PFCP_SDF_FILTER_FD_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        len+= addLEN(buffer + len, sdfFilter->lengthOfFlowDescription);
        //memcpy(buffer + len, sdfFilter->flowDescription.buf, sdfFilter->lengthOfFlowDescription);
        memcpy(buffer + len, &sdfFilter->flowDescription, sdfFilter->lengthOfFlowDescription);
        //free(sdfFilter->flowDescription.buf);
        len+= sdfFilter->lengthOfFlowDescription;
        pfcpLog("Added flow description, len %d", len);
    }

    if(sdfFilter->TTC) {
        if(len + PFCP_SDF_FILTER_TTC_CONTENTS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, &sdfFilter->tosTrafficClass, PFCP_SDF_FILTER_TTC_CONTENTS_LEN);
        //memcpy(buffer + len, sdfFilter->tosTrafficClass.buf, sdfFilter->tosTrafficClass.size);
        /*if(PFCP_SDF_FILTER_TTC_CONTENTS_LEN != sdfFilter->tosTrafficClass.size) {
            errno = E_PFCP_ERROR_INVALID_INPUT;
            higLog("Invalid Input");
            pfcpLogEXIT;
            return FAILURE;
        }*/
        //free(sdfFilter->tosTrafficClass.buf);
        len+= PFCP_SDF_FILTER_TTC_CONTENTS_LEN;
        pfcpLog("Added ToS Traffic class, len %d", len);
    }

    if(sdfFilter->SPI) {
        if(len + PFCP_SDF_FILTER_SPI_CONTENTS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, &sdfFilter->securityParameterIndex, PFCP_SDF_FILTER_SPI_CONTENTS_LEN);
        len += PFCP_SDF_FILTER_SPI_CONTENTS_LEN;
        pfcpLog("Added Security parameter Index, len %d", len);
    }

    if(sdfFilter->FL) {
        if(len + PFCP_SDF_FILTER_FL_CONTENTS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, sdfFilter->flowLabel, PFCP_SDF_FILTER_FL_CONTENTS_LEN);
        len+= PFCP_SDF_FILTER_FL_CONTENTS_LEN;
        pfcpLog("Added flow Label, len %d", len);
    }

    if(sdfFilter->BID) {
        if(len + PFCP_SDF_FILTER_BID_CONTENTS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, &sdfFilter->sdfFilterId, PFCP_SDF_FILTER_BID_CONTENTS_LEN);
        len += PFCP_SDF_FILTER_BID_CONTENTS_LEN;
        pfcpLog("Added SDF filter Id, len %d", len);
    }


    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);
    *encodedLen = len;

    pfcpLog("SDF Filter buffer");
    for(uint8_t i = 0; i < len; i++) {
        printf("%d ", buffer[i]);
    }
    pfcpLog("SDF Filter encoded, len %d", len);


    pfcpLogEXIT;
    return SUCCESS;
}

int sdfFilterDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    sdfFilter_t *sdfFilter = (sdfFilter_t *)ptr;

    if(buffer == NULL || bufLen <= 0 || sdfFilter == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_SDF_FILTER_MIN_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    
    len+= getIEI(buffer + len, &sdfFilter->IEI);
    pfcpLog("IEI %d decoded, len %d", sdfFilter->IEI, len);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);
    pfcpLog("IE Len %d decoded, len %d", ieLen, len);

    sdfFilter->BID = (buffer[len] & 0x10) >> 4;
    sdfFilter->FL = (buffer[len] & 0x08) >> 3;
    sdfFilter->SPI = (buffer[len] & 0x04) >> 2;
    sdfFilter->TTC = (buffer[len] & 0x02) >> 1;
    sdfFilter->FD = buffer[len] & 0x01;
    sdfFilter->spare = ((uint16_t)buffer[len] & 0xE0) << 3;
    len += PFCP_IE_SDF_FILTER_MASK_LEN;

    sdfFilter->spare |= buffer[len];
    len++;

    if (sdfFilter->spare != 0) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLog("SDF FD %d TTC %d SPI %d FL %d BID %d decoded, len %d",
       sdfFilter->FD, sdfFilter->TTC, sdfFilter->SPI, sdfFilter->FL, sdfFilter->BID, len);

    if(sdfFilter->FD ==1) {
        if(len + PFCP_SDF_FILTER_FD_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        len+= getLEN(buffer + len, &sdfFilter->lengthOfFlowDescription);
        memcpy(sdfFilter->flowDescription, buffer + len, sdfFilter->lengthOfFlowDescription);
        //sdfFilter->flowDescription.buf = (uint8_t *)(buffer + len);
        //sdfFilter->flowDescription.size = sdfFilter->lengthOfFlowDescription;
        len+= sdfFilter->lengthOfFlowDescription;
        pfcpLog("Decoded flow description, len %d, decodedLen %d", sdfFilter->lengthOfFlowDescription, len);
    }

    if(sdfFilter->TTC ==1) {
        if(len + PFCP_SDF_FILTER_TTC_CONTENTS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(sdfFilter->tosTrafficClass, buffer + len, PFCP_SDF_FILTER_TTC_CONTENTS_LEN);
        //sdfFilter->tosTrafficClass.buf = (uint8_t *)(buffer + len);
        //sdfFilter->tosTrafficClass.size = PFCP_SDF_FILTER_TTC_CONTENTS_LEN;
        len += PFCP_SDF_FILTER_TTC_CONTENTS_LEN;
        pfcpLog("Decoded ToS Traffic class, len %d", len);
    }

    if(sdfFilter->SPI ==1) {
        if(len + PFCP_SDF_FILTER_SPI_CONTENTS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(sdfFilter->securityParameterIndex
                , buffer + len, PFCP_SDF_FILTER_SPI_CONTENTS_LEN);
        len += PFCP_SDF_FILTER_SPI_CONTENTS_LEN;
        pfcpLog("Decoded Security parameter Index, len %d", len);
    }

    if(sdfFilter->FL ==1) {
        if(len + PFCP_SDF_FILTER_FL_CONTENTS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(sdfFilter->flowLabel, buffer + len, PFCP_SDF_FILTER_FL_CONTENTS_LEN);
        len+= PFCP_SDF_FILTER_FL_CONTENTS_LEN;
        pfcpLog("Decoded flow Label, len %d", len);
    }

    if(sdfFilter->BID) {
        if(len + PFCP_SDF_FILTER_BID_CONTENTS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(sdfFilter->sdfFilterId, buffer + len, PFCP_SDF_FILTER_BID_CONTENTS_LEN);
        len+= PFCP_SDF_FILTER_BID_CONTENTS_LEN;
        pfcpLog("Decoded SDF filter Id, len %d", len);
    }

    if(len - PFCP_IE_HDR_LEN != ieLen) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("ieLen %d value incorrect, expected %d", ieLen,
                len - PFCP_IE_HDR_LEN);
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLog("SDF Filter decoded, len %d", len);
    *decodedLen = len;

    pfcpLogEXIT;
    return SUCCESS;
}
