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

#include "pfcp/pfcpIEs/updateFAR.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> updateFarIEData {
    {PFCP_IE_FAR_ID, {PFCP_IED_FLAG_MANDATORY}},
    {PFCP_IE_APPLY_ACTION, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_UPDATE_FORWARDING_PARAMETERS, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_UPDATE_DUPLICATING_PARAMETERS, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_BAR_ID, {PFCP_IED_FLAG_CONDITIONAL}},
};

int updateFARencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    updateFAR_t *cFar = (updateFAR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("invalid input parameters");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeEncode(buffer, bufLen, cFar->IElist,
                    updateFarIEData, encodedLen) == FAILURE) {
        higLog("updateFAR encode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("updateFAR encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int updateFARdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    updateFAR_t *cFar = (updateFAR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("invalid input parameters");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeDecode(buffer, bufLen, cFar->IElist,
                    updateFarIEData, decodedLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("updateFAR decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
