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

#include "nas/NasDatatypes/PDUaddress.h"

int decodePduAddress(uint8_t *buffer,  uint32_t bufLen,
                     pduAddress_t *pduAddr, uint32_t *decodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;
    if(buffer == NULL || bufLen == 0 || pduAddr == NULL || decodedLen == NULL){
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    uint8_t contentLen = buffer[len];
    len += sizeof(contentLen);
    nasLog("content len decoded | len %d, currentLen %d",
                sizeof(contentLen), len);

    if(contentLen != PDU_ADDRESS_IE_IPV4_LEN &&
            contentLen != PDU_ADDRESS_IE_IPV6_LEN &&
            contentLen != PDU_ADDRESS_IE_IPV4V6_LEN) {
        higLog("Invalid Length %d", contentLen);
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + contentLen > bufLen) {
        higLog("Buffer incomplete");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    pduAddr->type = buffer[len++] & 0x07;
    nasLog("type decoded | len %d, currentLen %d",
                PDU_ADDRESS_CONTENT_TYPE_SIZE, len);

    switch(pduAddr->type) {
        case PDU_ADDRESS_TYPE_IPV6: {
            if(len + PDU_ADDRESS_LL_IPV6_LEN > bufLen) {
                higLog("Buffer incomplete");
                errno = E_NAS_ERROR_BUFF_INCOMPLETE;
                nasLogEXIT;
                return FAILURE;
            }
            memcpy(pduAddr->ipv6Address, buffer + len, PDU_ADDRESS_LL_IPV6_LEN);
            len += PDU_ADDRESS_LL_IPV6_LEN;
            nasLog("IPv6 addr decoded | len %d, currentLen %d",
                    PDU_ADDRESS_LL_IPV6_LEN, len);
        }
        break;
    case PDU_ADDRESS_TYPE_IPV4: {
            if(len + PDU_ADDRESS_IPV4_LEN > bufLen) {
                higLog("Buffer incomplete");
                errno = E_NAS_ERROR_BUFF_INCOMPLETE;
                nasLogEXIT;
                return FAILURE;
            }
            memcpy(pduAddr->ipv4Address, buffer + len, PDU_ADDRESS_IPV4_LEN);
            len += PDU_ADDRESS_IPV4_LEN;
            nasLog("IPv4 addr decoded | len %d, currentLen %d",
                    PDU_ADDRESS_IPV4_LEN, len);
        }
        break;
        case PDU_ADDRESS_TYPE_IPV4v6: {
            if(len + PDU_ADDRESS_LL_IPV6_LEN > bufLen) {
                higLog("Buffer incomplete");
                errno = E_NAS_ERROR_BUFF_INCOMPLETE;
                nasLogEXIT;
                return FAILURE;
            }
            memcpy(pduAddr->ipv6Address, buffer + len, PDU_ADDRESS_LL_IPV6_LEN);
            len += PDU_ADDRESS_LL_IPV6_LEN;

            nasLog("IPv6 addr decoded | len %d, currentLen %d",
                    PDU_ADDRESS_LL_IPV6_LEN, len);
            if(len + PDU_ADDRESS_IPV4_LEN > bufLen) {
                higLog("Buffer incomplete");
                errno = E_NAS_ERROR_BUFF_INCOMPLETE;
                nasLogEXIT;
                return FAILURE;
            }
            memcpy(pduAddr->ipv4Address, buffer + len, PDU_ADDRESS_IPV4_LEN);
            len += PDU_ADDRESS_IPV4_LEN;
            nasLog("IPv4 addr decoded | len %d, currentLen %d",
                    PDU_ADDRESS_IPV4_LEN, len);
        }
        break;

        default:
            higLog("Invalid PDU Address type %d", pduAddr->type);
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
            nasLogEXIT;
            return FAILURE;
    }

    nasLog("PDU Address decoded | len %d", len);
    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}

int encodePduAddress(uint8_t *buffer, uint32_t bufLen,
                     pduAddress_t *pduAddr, uint32_t *encodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;
    if(buffer == NULL || bufLen == 0 || pduAddr == NULL || encodedLen == NULL){
        higLog("Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    uint8_t contentLen = 0;
    switch(pduAddr->type) {
        case PDU_ADDRESS_TYPE_IPV4: {
            contentLen = PDU_ADDRESS_IE_IPV4_LEN;
        }
        break;
        case PDU_ADDRESS_TYPE_IPV6: {
            contentLen = PDU_ADDRESS_IE_IPV6_LEN;
        }
        break;
        case PDU_ADDRESS_TYPE_IPV4v6: {
            contentLen = PDU_ADDRESS_IE_IPV4V6_LEN;
        }
        break;
        default: {
            higLog("Invalid PDU Addr Type %d", pduAddr->type);
            errno = E_NAS_ERROR_INVALID_PDUADDR_TYPE;
            nasLogEXIT;
            return FAILURE;
        }
    }

    if(len + contentLen > bufLen) {
        higLog("Buffer insufficient");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[len++] = contentLen;
    nasLog("content len encoded | len %d, currentLen %d",
                sizeof(contentLen), len);

    buffer[len++] = pduAddr->type;
    nasLog("type encoded | len %d, currentLen %d",
                PDU_ADDRESS_CONTENT_TYPE_SIZE, len);

    switch(pduAddr->type) {
        case PDU_ADDRESS_TYPE_IPV6: {
            if(len + PDU_ADDRESS_LL_IPV6_LEN > bufLen) {
                higLog("Buffer insufficient");
                errno = E_NAS_ERROR_INVALID_INPUT;
                nasLogEXIT;
                return FAILURE;
            }
            memcpy(buffer + len, pduAddr->ipv6Address, PDU_ADDRESS_LL_IPV6_LEN);
            len += PDU_ADDRESS_LL_IPV6_LEN;
            nasLog("IPv6 addr encoded | len %d, currentLen %d",
                    PDU_ADDRESS_LL_IPV6_LEN, len);
        }
        break;
    case PDU_ADDRESS_TYPE_IPV4: {
            if(len + PDU_ADDRESS_IPV4_LEN > bufLen) {
                higLog("Buffer insufficient");
                errno = E_NAS_ERROR_INVALID_INPUT;
                nasLogEXIT;
                return FAILURE;
            }
            memcpy(buffer + len, pduAddr->ipv4Address, PDU_ADDRESS_IPV4_LEN);
            len += PDU_ADDRESS_IPV4_LEN;
            nasLog("IPv4 addr encoded | len %d, currentLen %d",
                    PDU_ADDRESS_IPV4_LEN, len);
        }
        break;
        case PDU_ADDRESS_TYPE_IPV4v6: {
            if(len + PDU_ADDRESS_LL_IPV6_LEN > bufLen) {
                higLog("Buffer insufficient");
                errno = E_NAS_ERROR_INVALID_INPUT;
                nasLogEXIT;
                return FAILURE;
            }
            memcpy(buffer + len, pduAddr->ipv6Address, PDU_ADDRESS_LL_IPV6_LEN);
            len += PDU_ADDRESS_LL_IPV6_LEN;
            nasLog("IPv6 addr encoded | len %d, currentLen %d",
                    PDU_ADDRESS_LL_IPV6_LEN, len);

            if(len + PDU_ADDRESS_IPV4_LEN > bufLen) {
                higLog("Buffer insufficient");
                errno = E_NAS_ERROR_INVALID_INPUT;
                nasLogEXIT;
                return FAILURE;
            }
            memcpy(buffer + len, pduAddr->ipv4Address, PDU_ADDRESS_IPV4_LEN);
            len += PDU_ADDRESS_IPV4_LEN;
            nasLog("IPv4 addr encoded | len %d, currentLen %d",
                    PDU_ADDRESS_IPV4_LEN, len);
        }
        break;

        default:
            higLog("Invalid PDU Address type %d", pduAddr->type);
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
            nasLogEXIT;
            return FAILURE;
    }

    nasLog("PDU Address encoded | len %d", len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
