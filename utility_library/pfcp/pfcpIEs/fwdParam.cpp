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

#include "pfcp/pfcpIEs/fwdParam.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> fwdParamIEData {
    {PFCP_IE_DESTINATION_INTERFACE, {PFCP_IED_FLAG_MANDATORY}},
    {PFCP_IE_REDIRECT_INFORMATION, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_OUTER_HEADER_CREATION, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_TRANSPORT_LEVEL_MARKING, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_FORWARDING_POLICY, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_TRAFFIC_ENDPOINT_ID, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_PROXYING, {PFCP_IED_FLAG_CONDITIONAL}},

        /* Optional IEs */
    {PFCP_IE_NETWORK_INSTANCE, {PFCP_IED_FLAG_OPTIONAL}},
    {PFCP_IE_HEADER_ENRICHMENT, {PFCP_IED_FLAG_OPTIONAL}},
    
};

int fwdParamEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    fwdParam_t *fP = (fwdParam_t *)ptr;

    if(pfcpIeEncode(buffer, bufLen, fP->IElist,
                    fwdParamIEData, encodedLen) == FAILURE) {
        higLog("Forwarding Parameter decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("Forwarding Parameter encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int fwdParamDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    fwdParam_t *fP = (fwdParam_t *)ptr;

    if(pfcpIeDecode(buffer, bufLen, fP->IElist,
                    fwdParamIEData, decodedLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("Forwarding Parameter decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
