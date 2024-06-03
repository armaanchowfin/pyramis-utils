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

#include "pfcp/pfcpIEs/outHeadCreatn.h"
#include "pfcp/include/pfcpUtil.h"

int outHdrCreatnEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    outHdrCreatn_t *ohc = (outHdrCreatn_t *)ptr;

    if(buffer == NULL || bufLen < 0 || ohc == NULL || encodedLen == NULL) 
    {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += addIEI(buffer + len, ohc->IEI);
    int lenOffset = len;
    len += 2;
    pfcpLog("IEI %d", ohc->IEI);
    pfcpLog("IEI and LEN encoded, len %d", len);

    if(len + PFCP_OH_CREATN_DESCRIPTN_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += addU16(buffer + len, ohc->description);
    pfcpLog("Outer Header Creation Description %d encoded, len %d", ohc->description, len);

    if(len + PFCP_OH_CREATN_TEID_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += addU32(buffer + len, ohc->teid);
    pfcpLog("TEID %llu encoded, len %d", ohc->teid, len);

    if((ohc->description & PFCP_OH_CREATN_GTPU_UDP_IPV4) || 
                            (ohc->description & PFCP_OH_CREATN_UDP_IPV4)){

       if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
           errno = E_PFCP_ERROR_BUFF_INSUFF;
           higLog("Buffer Insufficient to encode");
           pfcpLogEXIT;
           return FAILURE;
       }

       memcpy(buffer + len, ohc->ipv4Addr, PFCP_IPV4_ADDR_LEN);
       len += PFCP_IPV4_ADDR_LEN;
       pfcpLog("ipv4 Address added %d bytes | len %d",
               PFCP_IPV4_ADDR_LEN, len); 
    }

    if((ohc->description & PFCP_OH_CREATN_GTPU_UDP_IPV6) || 
                            (ohc->description & PFCP_OH_CREATN_UDP_IPV6)) {

        if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }

        memcpy(buffer + len, ohc->ipv6Addr, PFCP_IPV6_ADDR_LEN);
        len += PFCP_IPV6_ADDR_LEN;
        pfcpLog("ipv6 Address added %d bytes | len %d",
                PFCP_IPV6_ADDR_LEN, len);

    }

    if(ohc->description == PFCP_OH_CREATN_UDP_IPV4 ||
        ohc->description == PFCP_OH_CREATN_UDP_IPV6) {
        if(len + PFCP_OH_PORT_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        len += addU16(buffer + len, ohc->port);
        pfcpLog("Port %d encoded, len %d", ohc->port, len);
    }

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);
    *encodedLen = len;

    pfcpLog("Encoded %d bytes", len);

    pfcpLogEXIT;
    return SUCCESS;
    
}


int outHdrCreatnDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    outHdrCreatn_t *ohc = (outHdrCreatn_t *)ptr;

    if(buffer == NULL || bufLen < 0 || ohc == NULL || decodedLen == NULL) 
    {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += getIEI(buffer + len, &ohc->IEI);
    pfcpLog("IEI %d decoded, len %d", ohc->IEI, len);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);
    pfcpLog("IE Len %d decoded, len %d", ieLen, len);

    if(ieLen < PFCP_OH_CREATN_MIN_LEN || ieLen > PFCP_OH_CREATN_MAX_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Length incorrect");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_OH_CREATN_DESCRIPTN_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += getU16(buffer + len, &ohc->description);
    pfcpLog("Outer Header Creation Description %d decoded, len %d", ohc->description, len);

    if(len + PFCP_OH_CREATN_TEID_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += getU32(buffer + len, &ohc->teid);
    pfcpLog("TEID %llu decoded, len %d", ohc->teid, len);

    if((ohc->description & PFCP_OH_CREATN_GTPU_UDP_IPV4) || 
                            (ohc->description & PFCP_OH_CREATN_UDP_IPV4)){

       if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
           errno = E_PFCP_ERROR_BUFF_INSUFF;
           higLog("Buffer Insufficient to encode");
           pfcpLogEXIT;
           return FAILURE;
       }

       memcpy(ohc->ipv4Addr, buffer + len, PFCP_IPV4_ADDR_LEN);
       len += PFCP_IPV4_ADDR_LEN;
       pfcpLog("ipv4 Address added %d bytes | len %d",
               PFCP_IPV4_ADDR_LEN, len); 
    }

    if((ohc->description & PFCP_OH_CREATN_GTPU_UDP_IPV6) || 
                            (ohc->description & PFCP_OH_CREATN_UDP_IPV6)) {

        if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }

        memcpy(ohc->ipv6Addr, buffer + len, PFCP_IPV6_ADDR_LEN);
        len += PFCP_IPV6_ADDR_LEN;
        pfcpLog("ipv6 Address added %d bytes | len %d",
                PFCP_IPV6_ADDR_LEN, len);

    }



    if(ohc->description == PFCP_OH_CREATN_UDP_IPV4 ||
        ohc->description == PFCP_OH_CREATN_UDP_IPV6) {
        if(len + PFCP_OH_PORT_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        len += getU16(buffer + len, &ohc->port);
        pfcpLog("Port %d decoded, len %d", ohc->port, len);
    }

    if(PFCP_IE_HDR_LEN + ieLen != len ) {
        higLog("Incorrect IE Len");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLog("Decode Complete %d bytes", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;


}
