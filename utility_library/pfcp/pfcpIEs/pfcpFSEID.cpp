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

#include "pfcp/pfcpIEs/pfcpFSEID.h"
#include "pfcp/include/pfcpUtil.h"

int fseidEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen){

    pfcpLogENTRY;
    uint16_t len = 0;
    pfcpFSEID_t *fseid = (pfcpFSEID_t *)ptr;

    if(buffer == NULL || bufLen < 0 || fseid == NULL || encodedLen == NULL) {
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

    len += addIEI(buffer + len, fseid->IEI);
    int lenOffset = len;
    len += 2;
    pfcpLog("IEI %d", fseid->IEI);
    pfcpLog("IEI and LEN encoded, len %d", len);

    if(len + PFCP_FSEID_MASK_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    buffer[len] = fseid->v4v6;
    len++;
    pfcpLog("V4 V6 encoded %d, len %d", fseid->v4v6, len);

    if(len + PFCP_FSEID_SEID_LENGTH > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    len += addU64(buffer + len, fseid->seid);
    pfcpLog("SEID in F-SEID encoded, len %d", len);

    switch(fseid->v4v6){
        case PFCP_FSEID_IPV4_ONLY:
            if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(buffer +len, fseid->ipv4Address, PFCP_IPV4_ADDR_LEN);
            len += PFCP_IPV4_ADDR_LEN;  
            pfcpLog("ipv4 Address added %d bytes | len %d",
                    PFCP_IPV4_ADDR_LEN, len);
        break;

        case PFCP_FSEID_IPV6_ONLY:
            if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(buffer +len, fseid->ipv6Address, PFCP_IPV6_ADDR_LEN);
            len += PFCP_IPV6_ADDR_LEN;
            pfcpLog("ipv6 Address added %d bytes | len %d",
                    PFCP_IPV6_ADDR_LEN, len);
        break;

        case PFCP_FSEID_IPV4_IPV6:
            if(len + PFCP_IPV6_ADDR_LEN + PFCP_IPV4_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(buffer +len, fseid->ipv4Address, PFCP_IPV4_ADDR_LEN);
            len += PFCP_IPV4_ADDR_LEN;
            memcpy(buffer +len, fseid->ipv6Address, PFCP_IPV6_ADDR_LEN);
            len += PFCP_IPV6_ADDR_LEN;
            pfcpLog("ipv4 and ipv6 Addresses added %d bytes | len %d",
                    PFCP_IPV6_ADDR_LEN + PFCP_IPV4_ADDR_LEN, len);

        break;

        default:
            pfcpLog("Unknown or unsupported IP address type %02x",fseid->v4v6);
            errno = E_PFCP_ERROR_INVALID_INPUT;
            pfcpLogEXIT;
            return FAILURE;
    }

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);
    *encodedLen = len;

    pfcpLog("Encoded %d bytes", len);

    pfcpLogEXIT;
    return SUCCESS;
}



int fseidDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen){

    pfcpLogENTRY;

    uint16_t len = 0;
    pfcpFSEID_t *fseid = (pfcpFSEID_t *)ptr;
    
    if(buffer == NULL || bufLen < 0 || fseid == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }

    len += getIEI(buffer + len, &fseid->IEI);
    pfcpLog("IEI %d", fseid->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);
    pfcpLog("IE Len %d", ieLen);

    if(ieLen < PFCP_FSEID_MIN_LEN || ieLen > PFCP_FSEID_MAX_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Length incorrect");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_FSEID_MASK_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    fseid->v4v6 = (buffer[len] & 0x03);
    len++;
    pfcpLog("V4 V6 decoded %02x, len %d", fseid->v4v6, len);

    if(len + PFCP_FSEID_SEID_LENGTH > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    len += getU64(buffer + len, &fseid->seid);
    pfcpLog("SEID in F-SEID decoded, len %d", len);

    switch(fseid->v4v6){
        case PFCP_FSEID_IPV4_ONLY:
            if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(fseid->ipv4Address, buffer +len, PFCP_IPV4_ADDR_LEN);
            len += PFCP_IPV4_ADDR_LEN;  
            pfcpLog("ipv4 Address added %d bytes | len %d",
                    PFCP_IPV4_ADDR_LEN, len);
        break;

        case PFCP_FSEID_IPV6_ONLY:
            if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(fseid->ipv6Address, buffer +len, PFCP_IPV6_ADDR_LEN);
            len += PFCP_IPV6_ADDR_LEN;
            pfcpLog("ipv6 Address added %d bytes | len %d",
                    PFCP_IPV6_ADDR_LEN, len);
        break;

        case PFCP_FSEID_IPV4_IPV6:
            if(len + PFCP_IPV6_ADDR_LEN + PFCP_IPV4_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(fseid->ipv4Address, buffer +len, PFCP_IPV4_ADDR_LEN);
            len += PFCP_IPV4_ADDR_LEN;
            memcpy(fseid->ipv6Address, buffer +len, PFCP_IPV6_ADDR_LEN);
            len += PFCP_IPV6_ADDR_LEN;
            pfcpLog("ipv4 and ipv6 Addresses added %d bytes | len %d",
                    PFCP_IPV6_ADDR_LEN + PFCP_IPV4_ADDR_LEN, len);

        break;

        default:
            pfcpLog("Unknown or unsupported IP address type %02x",fseid->v4v6);
            errno = E_PFCP_ERROR_INVALID_INPUT;
            pfcpLogEXIT;
            return FAILURE;
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
