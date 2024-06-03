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

#include "pfcp/pfcpIEs/usrPlaneIpResInfo.h"
#include "pfcp/include/pfcpUtil.h"

int usrPlaneIpResInfoEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                            uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    usrPlaneIpResInfo_t *upInfo = (usrPlaneIpResInfo_t *)ptr;
    if(buffer == NULL || bufLen < 0 || ptr == NULL || encodedLen == NULL) {
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

    len += addIEI(buffer + len, upInfo->IEI);
    int lenOffset = len;
    len += 2;
    pfcpLog("IEI and LEN encoded, len %d", len);

    if(len + UP_IP_RES_INFO_MASK_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    buffer[len] = (upInfo->ASSO_SI << 6) | (upInfo->ASSO_NI << 5) |
                  (upInfo->TEIDRI  << 2) | (upInfo->V6 << 1) | (upInfo->V4);
    len += UP_IP_RES_INFO_MASK_LEN;
    pfcpLog("Encoded ASSO_SI %d, ASSO_NI %d, TEIDRI %d, V6 %d, V4 %d",
            upInfo->ASSO_SI, upInfo->ASSO_NI, upInfo->TEIDRI, upInfo->V6,
            upInfo->V4);

    if(upInfo->TEIDRI > 0) {
        if(len + UP_IP_RES_INFO_TEID_RANGE_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        buffer[len] = upInfo->teidRange;
        len += UP_IP_RES_INFO_TEID_RANGE_LEN;
        pfcpLog("Encoded TEID Range %d, len %d", upInfo->teidRange, len);
    }

    if(upInfo->V4 == 1) {
        if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, upInfo->ipv4Address, PFCP_IPV4_ADDR_LEN);
        len += PFCP_IPV4_ADDR_LEN;
        pfcpLog("IPv4 address added, len %d", len);
    }
    if(upInfo->V6 == 1) {
        if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, upInfo->ipv6Address, PFCP_IPV6_ADDR_LEN);
        len += PFCP_IPV6_ADDR_LEN;
        pfcpLog("IPv6 address added, len %d", len);
    }
    if(upInfo->ASSO_NI) {
        if(len + upInfo->netwInstLen > bufLen ||
                   upInfo->netwInstLen > UP_IP_RES_INFO_NTW_INST_LEN) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, upInfo->netwInst, upInfo->netwInstLen);
        len += upInfo->netwInstLen;
        pfcpLog("Encoded netwInst %d bytes, len %d", upInfo->netwInstLen, len);
    }
    if(upInfo->ASSO_SI) {
        if(len + UP_IP_RES_INFO_SRC_IFACE_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        buffer[len] = upInfo->srcIface;
        len += UP_IP_RES_INFO_SRC_IFACE_LEN;
        pfcpLog("Encoded srcIface %d, len %d", upInfo->srcIface, len);
    }

    pfcpLog("Encoded User Plane IP Resource Info, %d bytes", len);
    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);

    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}

int usrPlaneIpResInfoDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                            uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    usrPlaneIpResInfo_t *upInfo = (usrPlaneIpResInfo_t *)ptr;
    if(buffer == NULL || bufLen < 0 || ptr == NULL || decodedLen == NULL) {
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

    len += getIEI(buffer + len, &upInfo->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);
    pfcpLog("IEI and LEN decoded, len %d", len);

    if(ieLen > UP_IP_RES_INFO_MAX_LEN || ieLen < UP_IP_RES_INFO_MIN_LEN ||
       ieLen > bufLen) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("ieLen %d invalid", ieLen);
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + UP_IP_RES_INFO_MASK_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    upInfo->ASSO_SI = (buffer[len] >> 6) & 0x1;
    upInfo->ASSO_NI = (buffer[len] >> 5) & 0x1;
    upInfo->TEIDRI  = (buffer[len] >> 2) & 0x7;
    upInfo->V6 = (buffer[len] >> 1) & 0x1;
    upInfo->V4 = buffer[len] & 0x1;

    len += UP_IP_RES_INFO_MASK_LEN;
    pfcpLog("Decoded ASSO_SI %d, ASSO_NI %d, TEIDRI %d, V6 %d, V4 %d",
            upInfo->ASSO_SI, upInfo->ASSO_NI, upInfo->TEIDRI, upInfo->V6,
            upInfo->V4);

    if(upInfo->TEIDRI > 0) {
        if(len + UP_IP_RES_INFO_TEID_RANGE_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        upInfo->teidRange = buffer[len];
        if(upInfo->teidRange > (2<<upInfo->TEIDRI)) {
            errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
            higLog("TEIDRI: %d bits, can't contain teidRange %d",
                    upInfo->TEIDRI, upInfo->teidRange);
            LOG_EXIT;
            return FAILURE;
        }
        len += UP_IP_RES_INFO_TEID_RANGE_LEN;
        pfcpLog("Decoded TEID Range %d, len %d", upInfo->teidRange, len);
    }

    if(upInfo->V4 == 1) {
        if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(upInfo->ipv4Address, buffer + len, PFCP_IPV4_ADDR_LEN);
        len  += PFCP_IPV4_ADDR_LEN;
        pfcpLog("IPv4 address decoded, len %d", len);
    }
    if(upInfo->V6 == 1) {
        if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(upInfo->ipv6Address, buffer + len, PFCP_IPV6_ADDR_LEN);
        len  += PFCP_IPV6_ADDR_LEN;
        pfcpLog("IPv6 address decoded, len %d", len);
    }
    /* the bufLen left is equal to ntwInst (var length)
     * plus srcIface (last octet) */
    if(upInfo->ASSO_NI) {
        uint16_t exceptNIlen = len - PFCP_IE_HDR_LEN +
                        (upInfo->ASSO_SI ? UP_IP_RES_INFO_SRC_IFACE_LEN : 0);
                /* all the length except NI (& HDR) */
        if(exceptNIlen > ieLen){
            errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
            higLog("ieLen and bitmask dont match");
            pfcpLogEXIT;
            return FAILURE;
        }
        upInfo->netwInstLen = ieLen - exceptNIlen;

        if(len + upInfo->netwInstLen > bufLen ||
                   upInfo->netwInstLen > UP_IP_RES_INFO_NTW_INST_LEN) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(upInfo->netwInst, buffer + len, upInfo->netwInstLen);
        len += upInfo->netwInstLen;
        pfcpLog("Decoded netwInst %d bytes, len %d", upInfo->netwInstLen, len);
    }
    if(upInfo->ASSO_SI) {
        if(len + UP_IP_RES_INFO_SRC_IFACE_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        upInfo->srcIface = buffer[len] & 0xF;
        len += UP_IP_RES_INFO_SRC_IFACE_LEN;
        pfcpLog("Encoded srcIface %d, len %d", upInfo->srcIface, len);
    }

    if(len != ieLen + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("ieLen and len dont match");
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLog("Decoded User Plane IP Resource Info, %d bytes", len);

    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
