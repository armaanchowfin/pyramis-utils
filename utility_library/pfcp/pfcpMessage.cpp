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

/*
 * Contains the functions to encode/decode messages.
 *
 *
 * */
#include "include/pfcpMsgContents.h"
#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcpMessage.h"
#include "pfcp/include/pfcpUtil.h"

#define  PFCP_HDR_MAND_PART_LEN   4
/* "Octets 3 to 4 represent the Message Length field. This field shall
 * indicate the length of the message in octets excluding the mandatory part
 * of the PFCP header (the first 4 octets). The SEID (if present) and the
 * Sequence Number shall be included in the length count.
 */


int pfcpEncode(uint8_t *buffer, uint16_t bufLen, pfcpMessage_t *pfcpMsg,
                uint16_t* encodedLen)
{
    pfcpLogENTRY;
    errno = E_PFCP_ERROR_UNKNOWN;
    uint16_t len = 0;

    if(buffer == NULL || bufLen < PFCP_HDR_MSG_LEN ||
            pfcpMsg == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("INVALID INPUT");
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpHeader_t *hdr = &pfcpMsg->header;

    buffer[len] = 0 | (hdr->version << 5) | (hdr->MP << 1) | (hdr->S) ;
    len++;
    pfcpLog("Version %d, MP %d, S %d, 1 octet", hdr->version, hdr->MP, hdr->S);

    buffer[len] = hdr->MessageType;
    len++;
    pfcpLog("MessageType %d", hdr->MessageType);

    uint msgLenOffset = len;
    len += PFCP_HDR_MSG_LEN;
    /* end of mandatory fields within the header */

    if(hdr->S == 1 && isSessionRelatedMsg(hdr->MessageType)) {
        if(len + PFCP_HDR_SEID_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer insufficient");
            pfcpLogEXIT;
            return FAILURE;
        }

        len += addU64(buffer + len, hdr->SEID);
        pfcpLog("SEID encoded %llu", hdr->SEID);
    } else if(hdr->S == 1 && !isSessionRelatedMsg(hdr->MessageType) ||
              hdr->S == 0 && isSessionRelatedMsg(hdr->MessageType)) {
        higLog("bit S %d, Message Type %d, Mismatch",hdr->S, hdr->MessageType);
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    buffer[len++] = (hdr->SequenceNumber >> 16) & 0xFF;
    buffer[len++] = (hdr->SequenceNumber >> 8) & 0xFF;
    buffer[len++] = (hdr->SequenceNumber) & 0xFF;

    if((hdr->MP == 1) && (hdr->S == 1)){
        if(len + sizeof(uint8_t) > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer insufficient");
            pfcpLogEXIT;
            return FAILURE;
        }

        buffer[len] = 0 | (hdr->MessagePriority << 4);
        len++;
        pfcpLog("Message Priority %d, 1 octet", hdr->MessagePriority);

    } else if((hdr->MP == 1) && (hdr->S == 0)){
        higLog("bit S %d, bit MP %d, Mismatch",hdr->S, hdr->MP);
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }
    else{
        buffer[len] = 0;
        len++;
    }

    pfcpLog("PFCP Header encoding complete, header len %d", len);


    /* Start filling IEs based on Message type */
    std::map<uint8_t, std::map<uint16_t, pfcpIEdata_t> >::const_iterator
                                        itr = ieData.find(hdr->MessageType);
    if(itr == ieData.end()) {
        higLog("Unknown or Unsupported Message Type, %d", hdr->MessageType);
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }
    const std::map<uint16_t, pfcpIEdata_t> typeData = itr->second;

    uint16_t encLen = 0;
    if(pfcpIeEncode(buffer + len, bufLen - len, pfcpMsg->body,
                    typeData, &encLen) == FAILURE) {
        higLog("IE Encode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    len += encLen;

    /* fill in the message length */
    addLEN(buffer + msgLenOffset, (len - PFCP_HDR_MAND_PART_LEN));

    /* TODO: fill in the sequence number */


    pfcpLog("Encode Complete, %u bytes encoded", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}

int pfcpDecode(uint8_t *buffer, uint16_t bufLen, pfcpMessage_t *pfcpMsg)
{
    pfcpLogENTRY;
    errno = E_PFCP_ERROR_UNKNOWN;
    uint16_t len = 0;

    if(buffer == NULL || bufLen < PFCP_HDR_MSG_LEN || pfcpMsg == NULL ) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("INVALID INPUT");
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpHeader_t *hdr = &pfcpMsg->header;

    hdr->version  = (buffer[len] >> 5) % 0x07;
    hdr->MP = buffer[len] & 0x02;
    hdr->S  = buffer[len] & 0x01;
    len++;
    pfcpLog("Version %d, MP %d, S %d, 1 octet", hdr->version, hdr->MP, hdr->S);

    hdr->MessageType = buffer[len];
    len++;
    pfcpLog("MessageType %d", hdr->MessageType);

    uint16_t msgLen = 0;
    len += getLEN(buffer + len, &msgLen);
    pfcpLog("msgLen %d", msgLen);

    /* what is read so far + what is left to read should be less than bufLen */
    if(msgLen + PFCP_HDR_MAND_PART_LEN > bufLen) {
        higLog("Buffer insufficient");
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        pfcpLogEXIT;
        return FAILURE;
    }

    /* end of mandatory fields within the header */

    if(hdr->S == 1 && isSessionRelatedMsg(hdr->MessageType)) {
        if(len + PFCP_HDR_SEID_LEN > bufLen) {
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer insufficient");
            pfcpLogEXIT;
            return FAILURE;
        }

        len += getU64(buffer + len, &hdr->SEID);
        pfcpLog("SEID encoded %llu", hdr->SEID);

    } else if(hdr->S == 1 && !isSessionRelatedMsg(hdr->MessageType) ||
              hdr->S == 0 && isSessionRelatedMsg(hdr->MessageType)) {
        higLog("bit S %d, Message Type %d, Mismatch",hdr->S, hdr->MessageType);
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    hdr->SequenceNumber = ((uint32_t)buffer[len++]) << 16;
    hdr->SequenceNumber |= ((uint32_t)buffer[len++]) << 8;
    hdr->SequenceNumber |= buffer[len++];
    pfcpLog("SequenceNumber %d", hdr->SequenceNumber);

    if(len + sizeof(uint8_t) > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer insufficient");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(hdr->MP == 1) {
        hdr->MessagePriority = (buffer[len] >> 4) %0x0F ;
        pfcpLog("Message Priority %d, 1 octet", hdr->MessagePriority);
    }
    len++;

    pfcpLog("PFCP Header decoding complete, header len %d", len);

    /* Start filling IEs based on Message type */
    std::map<uint8_t, std::map<uint16_t, pfcpIEdata_t> >::const_iterator
                                        itr = ieData.find(hdr->MessageType);
    if(itr == ieData.end()) {
        higLog("Unknown or Unsupported Message Type, %d", hdr->MessageType);
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    lowLog ("Message type recieved :: %d", hdr->MessageType);


    const std::map<uint16_t, pfcpIEdata_t> typeData = itr->second;
    uint16_t decLen = 0;
    if(pfcpIeDecode(buffer +len, bufLen - len, pfcpMsg->body,
                typeData, &decLen) == FAILURE) {
        higLog("IE Decode failed");
        pfcpLogEXIT;
        return FAILURE;
    }
    len += decLen;

    pfcpLog("Decode Complete, %u bytes decoded", len);

    pfcpLogEXIT;
    return SUCCESS;
}
