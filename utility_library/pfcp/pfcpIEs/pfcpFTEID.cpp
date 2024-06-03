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

#include "pfcp/pfcpIEs/pfcpFTEID.h"
#include "pfcp/include/pfcpUtil.h"


int pfcpFTEIDencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    pfcpFTEID_t *fteid = (pfcpFTEID_t *)ptr;

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
    len += addIEI(buffer + len, fteid->IEI);
    uint16_t lenOffset = len;
    len += 2;
    pfcpLog("IEI and LEN encoded, len %d", len);

    if(len + PFCP_F_TEID_MASK_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    buffer[len] = 0 | (fteid->CHID ? 0x08 : 0x00) | (fteid->CH ? 0x04 : 0x00)
                    | (fteid->V6 ? 0x02 : 0x00)   | (fteid->V4 ? 0x01 : 0x00);
    len += PFCP_F_TEID_MASK_LEN;

    if(fteid->CH == 0) {
        if(len + PFCP_F_TEID_TEID_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        len += addU32(buffer +len, fteid->TEID);
        pfcpLog("TEID %llu encoded, len %d", fteid->TEID, len);

        if(fteid->V6 == 1) {
            if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }
            memcpy(buffer + len, fteid->ipv6Address, PFCP_IPV6_ADDR_LEN);
            len += PFCP_IPV6_ADDR_LEN;
            pfcpLog("IPv6 Address encoded, len %d", len);
        }

        if(fteid->V4 == 1) {
            if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }
            memcpy(buffer + len, fteid->ipv4Address, PFCP_IPV4_ADDR_LEN);
            len += PFCP_IPV4_ADDR_LEN;
            pfcpLog("IPv4 Address encoded, len %d", len);
        }

    } else { /* CH == 1 */
        if(fteid->CHID == 1) {
            if(len + PFCP_F_TEID_CHOOSEID_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }
            buffer[len] = fteid->chooseId;
            len += PFCP_F_TEID_CHOOSEID_LEN;
            pfcpLog("ChooseId %d encoded, len %d", fteid->chooseId, len);
        }
    }

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);

    pfcpLog("F-TEID Encoded, len %d", len);

    *encodedLen =  len;
    pfcpLogEXIT;
    return SUCCESS;
}
int pfcpFTEIDdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    pfcpFTEID_t *fteid = (pfcpFTEID_t *)ptr;

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

    len += getIEI(buffer + len, &fteid->IEI);
    uint16_t ieLen = 0;
    len += getLEN(buffer + len, &ieLen);

    if(ieLen < PFCP_F_TEID_MIN_LEN || ieLen > PFCP_F_TEID_MAX_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Incorrect ieLen %d", ieLen);
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_F_TEID_MASK_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    fteid->CHID = buffer[len] & 0x08;
    fteid->CH   = buffer[len] & 0x04;
    fteid->V6   = buffer[len] & 0x02;
    fteid->V4   = buffer[len] & 0x01;
    len += PFCP_F_TEID_MASK_LEN;

    if(fteid->V6 == 0 && fteid->V4 == 0) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Atleast one of V4 & V6 bits must be set");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(fteid->CH == 0) {
        if(len + PFCP_F_TEID_TEID_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        len += getU32(buffer +len, &fteid->TEID);
        pfcpLog("TEID %llu decoded, len %d", fteid->TEID, len);

        if(fteid->V6 == 1) {
            if(len + PFCP_IPV6_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }
            memcpy(fteid->ipv6Address, buffer + len, PFCP_IPV6_ADDR_LEN);
            len += PFCP_IPV6_ADDR_LEN;
            pfcpLog("IPv6 Address decoded, len %d", len);
        }

        if(fteid->V4 == 1) {
            if(len + PFCP_IPV4_ADDR_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to decode");
                pfcpLogEXIT;
                return FAILURE;
            }
            memcpy(fteid->ipv4Address, buffer + len, PFCP_IPV4_ADDR_LEN);
            len += PFCP_IPV4_ADDR_LEN;
            pfcpLog("IPv4 Address decoded, len %d", len);
        }

    } else { /* CH == 1 */
        if(fteid->CHID == 1) {
            if(len + PFCP_F_TEID_CHOOSEID_LEN > bufLen) {
                errno = E_PFCP_ERROR_BUFF_INSUFF;
                higLog("Buffer Insufficient to encode");
                pfcpLogEXIT;
                return FAILURE;
            }
            fteid->chooseId = buffer[len];
            len += PFCP_F_TEID_CHOOSEID_LEN;
            pfcpLog("ChooseId %d decoded, len %d", fteid->chooseId, len);
        }
    }

    if(ieLen + PFCP_IE_HDR_LEN != len) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Incorrect ieLen %d filled, actually read %d bytes",
                ieLen, len - PFCP_IE_HDR_LEN);
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLog("F-TEID Decoded, len %d", len);

    *decodedLen =  len;
    pfcpLogEXIT;
    return SUCCESS;
}
