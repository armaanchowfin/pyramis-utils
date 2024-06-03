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

#include "pfcp/include/pfcpUtil.h"
#include "pfcp/include/pfcpMsgContents.h"

typedef struct pfcpTrack {
    uint count;
} pfcpTrack_t;

int pfcpIeEncode(uint8_t *buffer, uint16_t bufLen, std::vector<pfcpIE_t> &ieList,
              const std::map<uint16_t, pfcpIEdata_t> typeData,
              uint16_t *encodedLen)
{
    uint16_t len = 0;
    pfcpLogENTRY;

    for(pfcpIE_t pIE : ieList) {
        std::map<uint16_t, pfcpIEdata_t>::const_iterator itr =
                                                        typeData.find(pIE.IEI);
        if(itr == typeData.end()) {
            higLog("PFCP IE %d cannot be a added", pIE.IEI);
            errno = E_PFCP_ERROR_CANNOT_ADD_IE;
            pfcpLogEXIT;
            return FAILURE;
        }

        std::map<uint16_t, pfcpIEops_t>::const_iterator opsItr =
                                                pfcpIEoperations.find(pIE.IEI);
        if(opsItr == pfcpIEoperations.end()) {
            higLog("Invalid or Unsupported IEI");
            errno = E_PFCP_ERROR_INVALID_INPUT;
            pfcpLogEXIT;
            return FAILURE;
        }

        uint16_t encLen = 0;
        if(opsItr->second.encode(buffer + len, bufLen - len,
                                       (void*)&pIE, &encLen)  == FAILURE) {
            higLog("Encode Failed");
            pfcpLogEXIT;
            return FAILURE;
        }
        len += encLen;

        pfcpLog("IEI %u Encoded, %d bytes, encoded %d bytes so far",
                pIE.IEI, encLen, len);
    }

    pfcpLog("Successfully encoded %d bytes", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}

int pfcpIeDecode(uint8_t *buffer, uint16_t bufLen, std::vector<pfcpIE_t> &ieList,
              const std::map<uint16_t, pfcpIEdata_t> typeData,
              uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    std::map<uint16_t, pfcpTrack_t> trackIEs = {};

    while(len + 2 < bufLen) {
        midLog("Length %d bufLen %d", len, bufLen);
        uint16_t IEI;
        getIEI(buffer + len, &IEI);
        std::map<uint16_t, pfcpIEdata_t>::const_iterator itr =
                                                        typeData.find(IEI);
        if(itr == typeData.end()) {
            higLog("PFCP IE %d cannot be added", IEI);
            errno = E_PFCP_ERROR_CANNOT_ADD_IE;
            pfcpLogEXIT;
            return FAILURE;
        }

        std::map<uint16_t, pfcpIEops_t>::const_iterator opsItr =
                                                pfcpIEoperations.find(IEI);
        if(opsItr == pfcpIEoperations.end()) {
            higLog("Invalid or Unsupported IEI");
            errno = E_PFCP_ERROR_INVALID_INPUT;
            pfcpLogEXIT;
            return FAILURE;
        }

        pfcpIE_t pIE = {};
        pIE.IEI = IEI;

        uint16_t decLen = 0;
        if( opsItr->second.decode(buffer + len, bufLen - len,
                                        (void*)&pIE, &decLen) == FAILURE) {
            higLog("Decode Failed");
            pfcpLogEXIT;
            return FAILURE;
        }
        len += decLen;

        ieList.push_back(pIE);

        std::map<uint16_t, pfcpTrack>::iterator trackItr =
                                                        trackIEs.find(pIE.IEI);
        if(trackItr == trackIEs.end()) {
            pfcpTrack_t pt = { 1 };
            trackIEs[pIE.IEI] = pt;
        } else
            trackItr->second.count++;

        pfcpLog("IEI %u Decoded, %d bytes", pIE.IEI, decLen);
    }

    /* Check for Mandatory fields have been recvd */
    for(std::map<uint16_t, pfcpIEdata_t>::const_iterator itr = typeData.begin();
            itr != typeData.end(); itr++) {
        uint16_t IEI = itr->first;
        pfcpIEdata_t pIeData = itr->second;

        std::map<uint16_t, pfcpTrack>::iterator trackItr = trackIEs.find(IEI);
        if(trackItr == trackIEs.end()) {
            if(pIeData.flags & PFCP_IED_FLAG_MANDATORY) {
                higLog("Mandatory IE %d Missing", IEI);
                errno = E_PFCP_ERROR_MAND_IE_MISSING;
                pfcpLogEXIT;
                return FAILURE;
            }
        } else {
            if(trackItr->second.count > 1 &&
               !(pIeData.flags & PFCP_IED_FLAG_MULTIPLE)) {
                higLog("Found %d IEs of type %d, expecting just one",
                        trackItr->second.count, IEI);
                errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
                pfcpLogEXIT;
                return FAILURE;
            }
        }
    }

    pfcpLog("Successfully decoded %d bytes", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
