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

#include "pfcp/pfcpIEs/createBAR.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> createBarIEData {
    { PFCP_IE_BAR_ID, {PFCP_IED_FLAG_MANDATORY}},
    { PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY, {PFCP_IED_FLAG_CONDITIONAL}},
    { PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT, {PFCP_IED_FLAG_CONDITIONAL}},
        /* Optional IEs */
};

int createBARencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    createBAR_t *cBar = (createBAR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("invalid input parameters");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeEncode(buffer, bufLen, cBar->IElist,
                    createBarIEData, encodedLen) == FAILURE) {
        higLog("createBAR decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("CreateBAR encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int createBARdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    createBAR_t *cBar = (createBAR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("invalid input parameters");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeDecode(buffer, bufLen, cBar->IElist,
                    createBarIEData, decodedLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("CreateBAR decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
