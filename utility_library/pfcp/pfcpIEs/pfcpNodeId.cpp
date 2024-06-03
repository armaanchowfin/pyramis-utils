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

#include "pfcp/pfcpIEs/pfcpNodeId.h"
#include "pfcp/include/pfcpUtil.h"

int nodeIdEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    nodeId_t *nId = (nodeId_t *)ptr;

    if(buffer == NULL || bufLen < 0 || nId == NULL || encodedLen == NULL) {
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

    len += addIEI(buffer + len, nId->IEI);
    int lenOffset = len;
    len += 2;
    pfcpLog("IEI and LEN encoded, len %d", len);

    if(len + PFCP_NODE_ID_TYPE_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    buffer[len] = nId->nodeIdType;
    len++;
    pfcpLog("nodeId type %d", nId->nodeIdType);

    switch(nId->nodeIdType) {
        case PFCP_NODE_ID_TYPE_IPV4: {
            if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(buffer + len, nId->u.ipv4Address, PFCP_IPV4_ADDR_LEN);
            len += PFCP_IPV4_ADDR_LEN;
            pfcpLog("ipv4 Address added %d bytes | len %d",
                    PFCP_IPV4_ADDR_LEN, len);
        }
        break;

        case PFCP_NODE_ID_TYPE_IPV6: {
            if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(buffer + len, nId->u.ipv6Address, PFCP_IPV6_ADDR_LEN);
            len += PFCP_IPV6_ADDR_LEN;
            pfcpLog("ipv6 Address added %d bytes | len %d",
                    PFCP_IPV6_ADDR_LEN, len);
        }
        break;

        default:
            pfcpLog("Unknown or unsupported nodeIdType %d", nId->nodeIdType);
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
int nodeIdDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{
    pfcpLogENTRY;
    nodeId_t *nId = (nodeId_t *)ptr;
    uint16_t len = 0;

    if(buffer == NULL || bufLen < 0 || nId == NULL) {
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

    len += getIEI(buffer + len, &nId->IEI);
    pfcpLog("IEI %d decoded, len %d", nId->IEI, len);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);
    pfcpLog("IE Len %d decoded, len %d", ieLen, len);

    if(ieLen < PFCP_NODE_ID_MIN_LEN || ieLen > PFCP_NODE_ID_MAX_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Length incorrect");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_NODE_ID_TYPE_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }

    nId->nodeIdType = buffer[len] & 0x0F;
    len++;

    switch(nId->nodeIdType) {
        case PFCP_NODE_ID_TYPE_IPV4: {
            if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(nId->u.ipv4Address, buffer +len, PFCP_IPV4_ADDR_LEN);
            len += PFCP_IPV4_ADDR_LEN;
            pfcpLog("ipv4 Address added %d bytes | len %d",
                    PFCP_IPV4_ADDR_LEN, len);
        }
        break;

        case PFCP_NODE_ID_TYPE_IPV6: {
            if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }

            memcpy(nId->u.ipv6Address, buffer +len, PFCP_IPV6_ADDR_LEN);
            len += PFCP_IPV6_ADDR_LEN;
            pfcpLog("ipv6 Address added %d bytes | len %d",
                    PFCP_IPV6_ADDR_LEN, len);
        }
        break;

        default:
            higLog("Unkown or Unsupported nodeIdType, %d", nId->nodeIdType);
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
