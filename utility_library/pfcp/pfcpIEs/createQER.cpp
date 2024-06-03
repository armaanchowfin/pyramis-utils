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

#include "pfcp/pfcpIEs/createQER.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> createQerIEData {
    {PFCP_IE_QER_ID, {PFCP_IED_FLAG_MANDATORY}},
    {PFCP_IE_GATE_STATUS, {PFCP_IED_FLAG_MANDATORY}},

        /* Optional IEs */
    {PFCP_IE_QER_CORRELATION_ID, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_MBR, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_GBR, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_PACKET_RATE, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_DL_FLOW_LEVEL_MARKING, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_QFI, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_RQI, {PFCP_IED_FLAG_CONDITIONAL}},
};

int createQERencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    createQER_t *cQer = (createQER_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeEncode(buffer, bufLen, cQer->IElist,
                    createQerIEData, encodedLen) == FAILURE) {
        higLog("createQER encode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("CreateQER encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int createQERdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    createQER_t *cQer = (createQER_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeDecode(buffer, bufLen, cQer->IElist,
                    createQerIEData, decodedLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("CreateQER decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
