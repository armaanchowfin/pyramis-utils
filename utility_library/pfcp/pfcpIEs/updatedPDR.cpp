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

#include "pfcp/pfcpIEs/updatedPDR.h"
#include "pfcp/include/pfcpUtil.h"
#include "pfcp/include/pfcpMsgContents.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> updatedPdrIEData {
    {PFCP_IE_PACKET_DETECTION_RULE_ID, {PFCP_IED_FLAG_MANDATORY | PFCP_IED_FLAG_MULTIPLE}},
    
        /* Optional IEs */
    {PFCP_IE_F_TEID, {PFCP_IED_FLAG_CONDITIONAL}},
};

int updatedPDRencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    updatedPDR_t *updatedPdr = (updatedPDR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    for (auto &i: updatedPdr->IElist) {
        midLog("IE = %d", i.IEI);
    }

    if(pfcpIeEncode(buffer, bufLen, updatedPdr->IElist,
                    updatedPdrIEData, encodedLen) == FAILURE) {
        higLog("UpdatedPDR encode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("UpdatedPDR encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int updatedPDRdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    updatedPDR_t *updatedPdr = (updatedPDR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeDecode(buffer, bufLen, updatedPdr->IElist,
                    updatedPdrIEData, decodedLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("UpdatedPDR decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
