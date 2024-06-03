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

#include "pfcp/pfcpIEs/removeFAR.h"
#include "pfcp/include/pfcpUtil.h"

/* Possible IEs */
const static std::map<uint16_t, pfcpIEdata_t> removeFARData {
    {PFCP_IE_FAR_ID, {PFCP_IED_FLAG_MANDATORY}},
};

int removeFARencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                    uint16_t *encodedLen)
{
    pfcpLogENTRY;
    removeFAR_t *rFar = (removeFAR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeEncode(buffer, bufLen, rFar->IElist,
                    removeFARData, encodedLen) == FAILURE) {
        higLog("removeFAR decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("Remove FAR encoded in %d bytes", *encodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
int removeFARdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    removeFAR_t *rFar = (removeFAR_t *)ptr;

    if(buffer == NULL || bufLen == 0 || ptr == NULL || decodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(pfcpIeDecode(buffer, bufLen, rFar->IElist,
                    removeFARData, decodedLen) == FAILURE) {
        higLog("removeFAR Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("remove FAR decoded in %d bytes", *decodedLen);

    pfcpLogEXIT;
    return SUCCESS;
}
