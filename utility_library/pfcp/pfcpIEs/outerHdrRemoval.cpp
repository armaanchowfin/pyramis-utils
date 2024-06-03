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

#include "pfcp/pfcpIEs/outerHdrRemoval.h"
#include "pfcp/include/pfcpUtil.h"

int outerHdrRemovalEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                          uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    outerHdrRemoval_t *ohr = (outerHdrRemoval_t *)ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    len += addIEI(buffer + len,  ohr->IEI);
    uint16_t lenOffset = len;
    len += PFCP_IE_LEN_SIZE;
    pfcpLog("IEI and LEN encoded, len %d", len);

    if(ohr->gtpUextensionHdrDel == 0) {
        if(len + PFCP_OH_REM_MIN_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        buffer[len] = ohr->description;
        len++;
        pfcpLog("Outer Header Removal description encoded %d, len %d",
                ohr->description, len);
    } else {
        if(len + PFCP_OH_REM_MAX_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        buffer[len] = ohr->description;
        len++;
        pfcpLog("Outer Header Removal description encoded %d, len %d",
                ohr->description, len);
        buffer[len] = ohr->gtpUextensionHdrDel;
        len++;
        pfcpLog("GTP U Ext Hdr Del encoded %d, len %d",
                ohr->gtpUextensionHdrDel, len);
    }

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);

    *encodedLen = len;
    pfcpLog("Encoded Outer Hdr Removal, len %d", len);
    pfcpLogEXIT;
    return SUCCESS;
}
int outerHdrRemovalDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                          uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    outerHdrRemoval_t *ohr = (outerHdrRemoval_t *)ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    len += getIEI(buffer + len, &ohr->IEI);
    uint16_t ieLen;
    len += getLEN(buffer + len, &ieLen);
    pfcpLog("IEI and LEN decoded, len %d", len);

    if(ieLen != PFCP_OH_REM_MIN_LEN && ieLen != PFCP_OH_REM_MAX_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Invalid ieLen %d", ieLen);
        pfcpLogEXIT;
        return FAILURE;
    }

    if(ieLen == PFCP_OH_REM_MIN_LEN) {
        if(len + PFCP_OH_REM_MIN_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        ohr->description = buffer[len];
        len++;
        pfcpLog("Outer Header Removal description decoded %d, len %d",
                ohr->description, len);

    } else {    /* len == PFCP_OH_REM_MAX_LEN */
        if(len + PFCP_OH_REM_MAX_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        buffer[len] = ohr->description;
        len++;
        pfcpLog("Outer Header Removal description encoded %d, len %d",
                ohr->description, len);
        buffer[len] = ohr->gtpUextensionHdrDel;
        len++;
        pfcpLog("GTP U Ext Hdr Del encoded %d, len %d",
                ohr->gtpUextensionHdrDel, len);
    }

    if(ohr->description > PFCP_OH_REM_STAG_AND_CTAG) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Invalid Outer Header Description");
        pfcpLogEXIT;
        return FAILURE;
    }

    *decodedLen = len;
    pfcpLog("Encoded Outer Hdr Removal, len %d", len);
    pfcpLogEXIT;
    return SUCCESS;
}
