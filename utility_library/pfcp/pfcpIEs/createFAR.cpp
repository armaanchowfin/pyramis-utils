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

#include "pfcp/pfcpIEs/createFAR.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> createFarIEData {
    {PFCP_IE_FAR_ID, {PFCP_IED_FLAG_MANDATORY}},
    {PFCP_IE_APPLY_ACTION, {PFCP_IED_FLAG_MANDATORY}},
    {PFCP_IE_FORWARDING_PARAMETERS, {PFCP_IED_FLAG_CONDITIONAL}},
    {PFCP_IE_DUPLICATING_PARAMETERS, {PFCP_IED_FLAG_CONDITIONAL}},

        /* Optional IEs */
    {PFCP_IE_BAR_ID, {PFCP_IED_FLAG_OPTIONAL}},
};

int createFARencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    createFAR_t *cFar = (createFAR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("invalid input parameters");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeEncode(buffer, bufLen, cFar->IElist,
                    createFarIEData, encodedLen) == FAILURE) {
        higLog("createFAR decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("CreateFAR encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int createFARdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    createFAR_t *cFar = (createFAR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("invalid input parameters");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeDecode(buffer, bufLen, cFar->IElist,
                    createFarIEData, decodedLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("CreateFAR decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
