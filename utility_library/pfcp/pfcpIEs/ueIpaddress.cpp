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

#include "pfcp/pfcpIEs/ueIpaddress.h"
#include "pfcp/include/pfcpUtil.h"

int ueIpaddressEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    ueIpaddress_t *ueIp = (ueIpaddress_t *) ptr;

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
    len += addIEI(buffer + len, ueIp->IEI);
    uint16_t lenOffset = len;
    len += 2;

    if(len + PFCP_UE_IPADDR_MIN_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    buffer[len] = (ueIp->IPv6D ? 0x08 : 0x0 ) | (ueIp->SrcOrDest ? 0x04 : 0x0)|
                  (ueIp->V4 ? 0x02 : 0x0 ) | (ueIp->V6 ? 0x01 : 0x0);
    len += PFCP_UE_IPADDR_MASK_LEN;

    if(ueIp->V4 ==1) {
        if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, ueIp->ipv4Address, PFCP_IPV4_ADDR_LEN);
        len+= PFCP_IPV4_ADDR_LEN;
        pfcpLog("Added IPv4 Address, len %d", len);
    }

    if(ueIp->V6 ==1) {
        if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(buffer + len, ueIp->ipv6Address, PFCP_IPV6_ADDR_LEN);
        len+= PFCP_IPV6_ADDR_LEN;
        pfcpLog("Added IPv6 Address, len %d", len);
    }

    if(ueIp->IPv6D == 1) {
        if(len + PFCP_UE_IPADDR_IPV6_PD_BITS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        buffer[len] = ueIp->ipv6PrefixDelegBits;
        len += PFCP_UE_IPADDR_IPV6_PD_BITS_LEN;
        pfcpLog("Added ipv6PrefixDelegBits %d, len %d",
                    ueIp->ipv6PrefixDelegBits, len);
    }

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);
    pfcpLog("UE IP Address encoded, len %d", len);

    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
int ueIpaddressDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    ueIpaddress_t *ueIp = (ueIpaddress_t *) ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || decodedLen == NULL) {
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
    len += getIEI(buffer + len, &ueIp->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);

    if(len + PFCP_UE_IPADDR_MIN_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    ueIp->IPv6D = (buffer[len] & 0x08) >> 3;
    ueIp->SrcOrDest = (buffer[len] & 0x04) >> 2;
    ueIp->V4 = (buffer[len] & 0x02) >> 1;
    ueIp->V6 = buffer[len] & 0x01;
    len += PFCP_UE_IPADDR_MASK_LEN;

    if(ueIp->IPv6D == 1 && ueIp->V6 == 0) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("IPv6D can only be set if V6 bit is set");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(ueIp->V4 ==1) {
        if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(ueIp->ipv4Address, buffer + len, PFCP_IPV4_ADDR_LEN);
        len+= PFCP_IPV4_ADDR_LEN;
        pfcpLog("Decoded IPv4 Address, len %d", len);
    }

    if(ueIp->V6 ==1) {
        if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        memcpy(ueIp->ipv6Address, buffer+len, PFCP_IPV6_ADDR_LEN);
        len+= PFCP_IPV6_ADDR_LEN;
        pfcpLog("Decoded IPv6 Address, len %d", len);
    }

    if(ueIp->IPv6D == 1) {
        if(len + PFCP_UE_IPADDR_IPV6_PD_BITS_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        ueIp->ipv6PrefixDelegBits = buffer[len];
        len += PFCP_UE_IPADDR_IPV6_PD_BITS_LEN;
        pfcpLog("Added ipv6PrefixDelegBits %d, len %d",
                    ueIp->ipv6PrefixDelegBits, len);
    }

    if(len - PFCP_IE_HDR_LEN != ieLen) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("ieLen %d value incorrect, expected %d", ieLen,
                len - PFCP_IE_HDR_LEN);
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLog("UE IP Address decoded, len %d", len);

    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
