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

#include "pfcp/pfcpIEs/removePDR.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> removePDRData {
    {PFCP_IE_PACKET_DETECTION_RULE_ID, {PFCP_IED_FLAG_MANDATORY}},
};

int removePDRencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                    uint16_t *encodedLen)
{
    pfcpLogENTRY;
    removePDR_t *rPdr = (removePDR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeEncode(buffer, bufLen, rPdr->IElist,
                    removePDRData, encodedLen) == FAILURE) {
        higLog("removePDR decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("Remove PDR encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int removePDRdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    removePDR_t *rPdr = (removePDR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeDecode(buffer, bufLen, rPdr->IElist,
                    removePDRData, decodedLen) == FAILURE) {
        higLog("removePDR Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("remove PDR decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
