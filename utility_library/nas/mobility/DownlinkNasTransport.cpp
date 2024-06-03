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

#include "nas/mobility/DownlinkNasTransport.h"


int decodeDlNAS(uint8_t *buffer, DownlinkNasTransport_t *msgStruct,
        uint32_t bufLen)
{

    nasLogENTRY;

    uint32_t consumedLen = 0;
    uint32_t len = 0;

    if(buffer == NULL || msgStruct == NULL || bufLen == 0) {
        higLog("Invalid Input parameters. Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + sizeof(uint8_t) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    msgStruct->payloadContainerType = buffer[len] & 0x0F;
    len++;
    nasLog("payloadcontainerType value : %d | currentLen = %d | sizeof : 1",
            msgStruct->payloadContainerType, len);

    if((msgStruct->payloadContainerType < NAS_PAYLOAD_N1_SM_INFORMATION ||
        msgStruct->payloadContainerType > NAS_PAYLOAD_UE_POLICY) &&
        msgStruct->payloadContainerType != NAS_PAYLOAD_MULITPLE_PAYLOADS) {
        higLog("Unknown Payload Container Type %d",
                msgStruct->payloadContainerType);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    consumedLen = 0;
    if(decodePayloadContainer(buffer+len , &msgStruct->container,
                bufLen - len, &consumedLen) == FAILURE){
        higLog("decoding Payload Container failed");
        nasLogEXIT;
        return FAILURE;
    }
    len+=consumedLen;
    nasLog("Payload Decoded | currentLen %d | sizeof %d", len, consumedLen);

    msgStruct->presenceMask = 0;
    while(len < bufLen) {
        uint8_t iei = *(buffer + len);
        len++;

        uint32_t decodLen = 0;
        switch(iei) {
        case E_DOWNLINK_NAS_IEI_PDU_SESSION_ID: {
            if(decodePDUSessionId2(buffer + len, bufLen - len,
                        &msgStruct->pduSessionId, &decodLen) == FAILURE) {
                higLog("Decoding PDU Session ID failed");
                nasLogEXIT;
                return FAILURE;
            }

            len+=decodLen;
            msgStruct->presenceMask |= DOWNLINK_NAS_IE_PDU_SESSION_ID_PRESENT;
            nasLog("PDU Session ID decoded | currentLen = %d | decoded %d bytes",
                      len, decodLen);
        }
        break;

        default: {
           higLog("Unknown Optional Field IEI 0x%02x", iei);
           errno = E_NAS_ERROR_INVALID_PDU;
           nasLogEXIT;
           return FAILURE;
        }
        }
    }

    if( len > bufLen) {
        higLog("ERROR: crossed buffer boundaries");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
    nasLog("Successfully decoded %d bytes", len);

    nasLogEXIT;
    return SUCCESS;
}



int encodeDlNAS(uint8_t *buffer, uint32_t bufLen,
        DownlinkNasTransport_t *msgStruct, uint32_t *encodedLen)
{

    nasLogENTRY;

    uint32_t encLen=0;
    uint32_t len=0;
    if(buffer == NULL || msgStruct == NULL || encodedLen == NULL) {
        higLog("Invalid Input parameters. Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + sizeof(uint8_t) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[len] = msgStruct->payloadContainerType;
    len++;
    nasLog("payloadcontainerType value : %d | currentLen = %d | sizeof : 1",
            msgStruct->payloadContainerType, len);

    if(encodePayloadContainer(buffer+len, bufLen-len,
                &msgStruct->container, &encLen) == FAILURE) {
        higLog("encoding Payload Container Failed");
        nasLogEXIT;
        return FAILURE;
    }

    len += encLen;
    nasLog("PayLoad container encoded| currentLen %d ", len);

    if(msgStruct->presenceMask & DOWNLINK_NAS_IE_PDU_SESSION_ID_PRESENT) {
        buffer[len] = E_DOWNLINK_NAS_IEI_PDU_SESSION_ID;
        len++;

        encLen = 0;
        if(encodePDUSessionId2(buffer +len, bufLen -len,
                    &msgStruct->pduSessionId, &encLen) == FAILURE) {
            higLog("Encoding PDUSession ID Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("PDU Session ID encoded | currentLen %d | encoded %d bytes",
                len, encLen);
    } else
        nasLog("PDU Session ID is not set");

    *encodedLen = len;
    nasLog("Successfully encoded UL Nas Message %d bytes", len);
    nasLogEXIT;
    return SUCCESS;

}
