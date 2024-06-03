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

#include "pfcp/pfcpIEs/updateFwdParam.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> updateFwdParamIEData {
    {PFCP_IE_DESTINATION_INTERFACE, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_NETWORK_INSTANCE, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_REDIRECT_INFORMATION, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_OUTER_HEADER_CREATION, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_TRANSPORT_LEVEL_MARKING, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_FORWARDING_POLICY, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_HEADER_ENRICHMENT, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_TRAFFIC_ENDPOINT_ID, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_DESTINATION_INTERFACE_TYPE, {PFCP_IED_FLAG_CONDITIONAL}},
    
};

int updateFwdParamEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    updateFwdParam_t *fP = (updateFwdParam_t *)ptr;

    if(pfcpIeEncode(buffer, bufLen, fP->IElist,
                    updateFwdParamIEData, encodedLen) == FAILURE) {
        higLog("Update Forwarding Parameter encode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("Update Forwarding Parameter encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int updateFwdParamDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    updateFwdParam_t *fP = (updateFwdParam_t *)ptr;

    if(pfcpIeDecode(buffer, bufLen, fP->IElist,
                    updateFwdParamIEData, decodedLen) == FAILURE) {
        higLog("Update Forwarding Parameter IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("Update Forwarding Parameter decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}

