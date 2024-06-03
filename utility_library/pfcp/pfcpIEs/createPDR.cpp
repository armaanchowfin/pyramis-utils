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

#include "pfcp/pfcpIEs/createPDR.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> createPdrIEData {
    {PFCP_IE_PACKET_DETECTION_RULE_ID, {PFCP_IED_FLAG_MANDATORY}},
    {PFCP_IE_PRECEDENCE, {PFCP_IED_FLAG_MANDATORY}},
    {PFCP_IE_PDI, {PFCP_IED_FLAG_MANDATORY}},

        /* Optional IEs */
    {PFCP_IE_OUTER_HEADER_REMOVAL, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_FAR_ID, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_QER_ID, {PFCP_IED_FLAG_CONDITIONAL}},
};

int createPDRencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    createPDR_t *cPdr = (createPDR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeEncode(buffer, bufLen, cPdr->IElist,
                    createPdrIEData, encodedLen) == FAILURE) {
        higLog("createPDR encode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("CreatePDR encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int createPDRdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    createPDR_t *cPdr = (createPDR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeDecode(buffer, bufLen, cPdr->IElist,
                    createPdrIEData, decodedLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("CreatePDR decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
