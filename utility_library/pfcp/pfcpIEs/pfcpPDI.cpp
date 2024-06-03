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

#include "pfcp/pfcpIEs/pfcpPDI.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> pfcpPdiIEData {
    {PFCP_IE_SOURCE_INTERFACE, {PFCP_IED_FLAG_MANDATORY}},
    {PFCP_IE_F_TEID, {PFCP_IED_FLAG_OPTIONAL}},
    {PFCP_IE_UE_IP_ADDRESS, {PFCP_IED_FLAG_OPTIONAL}},
    {PFCP_IE_QFI, {PFCP_IED_FLAG_OPTIONAL}},
    {PFCP_IE_SDF_FILTER, {PFCP_IED_FLAG_OPTIONAL}},
    /* Incomplete */
};

int pfcpPDIencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    pfcpPDI_t *pdi = (pfcpPDI_t*) ptr;

    if(pfcpIeEncode(buffer, bufLen, pdi->IElist,
                    pfcpPdiIEData, encodedLen) == FAILURE) {
        higLog("IE encode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("PDI encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
}

int pfcpPDIdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    pfcpPDI_t *pdi = (pfcpPDI_t*) ptr;

    if(pfcpIeDecode(buffer, bufLen, pdi->IElist,
                    pfcpPdiIEData, decodedLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("PDI decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
}
