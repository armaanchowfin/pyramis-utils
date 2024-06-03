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

#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/ULNasPayloadContainer.h"


int decodePayloadContainer(uint8_t *buffer, PayloadContainer_t *pc,
                        uint32_t bufLen, uint32_t* decodedLen)
{

    nasLogENTRY;
    uint32_t len= 0;

    if(bufLen < MIN_PAYLOAD_LEN || buffer == NULL || pc == NULL ||
       decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }


    pc->Length = (buffer[len]<<8|buffer[len+1]) - PAYLOAD_PARAM_OFFSET;
    // Check for correctness
    len += sizeof(pc->Length);

    if(len + pc->Length > bufLen) {
        higLog("Buffer insufficient to copy PAYLOAD");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(pc->content, buffer + len, pc->Length);
    len += pc->Length;
    nasLog("Container decoded Payload len %d | decoded %d", pc->Length, len);

    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}



int encodePayloadContainer(uint8_t *buffer, uint32_t bufLen,
                        PayloadContainer_t *pc, uint32_t *encodedLen)
{

    nasLogENTRY;
    uint32_t len = 0;

    if(bufLen < MAX_PAYLOAD_LEN || buffer == NULL || pc == NULL ||
       encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /* check if the length is valid */
    if(pc->Length > MAX_PAYLOAD_LEN
       || pc->Length < MIN_PAYLOAD_LEN) {
        higLog("Invalid length %d", pc->Length);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    uint16_t payloadIELen = pc->Length + PAYLOAD_PARAM_OFFSET;
    buffer[0] = (payloadIELen & 0xFF00) >> 8;
    buffer[1] = payloadIELen & 0x00FF;
    len += sizeof(pc->Length);

    if(len + pc->Length > bufLen) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(buffer + len, pc->content, pc->Length);
    len += pc->Length;
    nasLog("Payload Container encoded with len %d | decoded %d", pc->Length,
            len);
    *encodedLen = len;

    nasLogEXIT;
    return SUCCESS;


}

int decodeContainerEntry(uint8_t *buffer, uint32_t bufLen,
                        payloadEntry_t *entry, uint32_t *decLen)
{
    nasLogENTRY;

    if(bufLen == 0 || buffer == NULL || entry == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        return FAILURE;
    }
    uint32_t len = 0;

    if(len + PAYLOAD_ENTRY_OFFSET > bufLen) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
    uint8_t entryLen = buffer[len];
    len++;
    entry->numOfIEs = buffer[len] >> 4;
    entry->containerType = buffer[len] & 0x0F;
    len++;
    nasLog("Entry len %d, numOfIEs %d, containerType %d | currentLen %d",
            entryLen, entry->numOfIEs, entry->containerType, len);

    for(int i=0; i<entry->numOfIEs; i++) {
        if(len + PAYLOAD_OPT_IEI_LEN_SIZE > bufLen) {
            higLog("Buffer of insufficient len");
            errno = E_NAS_ERROR_BUFF_INSUFF;
            nasLogEXIT;
            return FAILURE;
        }

        uint8_t iei = buffer[len];
        len++;
        uint32_t decodLen = 0;
        switch(iei) {
        case PAYLOAD_PDU_SESSION_ID: {
            uint8_t ieLen = buffer[len];
            len++;
            /* encodePDUSessionId2 does not encode len */
            if(decodePDUSessionId2(buffer + len, bufLen - len,
                        &entry->optIEs[i].u.pduSessionId, &decodLen) ==FAILURE){
                higLog("Decoding PDU Session ID failed");
                nasLogEXIT;
                return FAILURE;
            }

            len+=decodLen;
            if(decodLen + PAYLOAD_OPT_IEI_LEN_SIZE != ieLen) {
                higLog("Incorrect IE len");
                errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
                nasLogEXIT;
                return FAILURE;
            }
            entry->optIEs[i].IEI = iei;
            nasLog("PDU Session ID decoded | currentLen = %d | decoded %d bytes",
                      len, decodLen);
        }
        break;

        case PAYLOAD_DNN: {
            /* encodeDNN encodes len. ieLen not needed */
            if(decodeDNN(buffer + len, bufLen - len, &entry->optIEs[i].u.dnn,
                                                &decodLen) == FAILURE) {
                higLog("Decoding DNN failed");
                nasLogEXIT;
                return FAILURE;
            }

            len+=decodLen;
            entry->optIEs[i].IEI = iei;
            nasLog("DNN decoded | currentLen = %d | decoded %d bytes",
                    len, decodLen);
        }
        break;

        default: {
            switch(iei>>4) {

            case PAYLOAD_REQUEST_TYPE: {
                uint8_t ieLen = buffer[len];
                len++;
                if(decodeRequestType(buffer + len, bufLen -len,
                                &entry->optIEs[i].u.requestType,
                                                        &decodLen) == FAILURE){
                    higLog("Decoding Request Type Failed");
                    nasLogEXIT;
                    return FAILURE;
                }

                if(decodLen + PAYLOAD_OPT_IEI_LEN_SIZE != ieLen) {
                    higLog("Incorrect IE len");
                    errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
                    nasLogEXIT;
                    return FAILURE;
                }
                len+=decodLen;
                entry->optIEs[i].IEI = iei;
                nasLog("Request Type decoded | currentLen = %d | decoded %d bytes",
                        len, decodLen);
                }
            break;

            default:
                higLog("Unknown Optional Field IEI 0x%02x", iei);
                errno = E_NAS_ERROR_INVALID_PDU;
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;

        }
    }

    if(len > entryLen) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    entry->contentLen = entryLen - len;
    if(entry->contentLen > MAX_PAYLOAD_LEN) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
    memcpy(entry->content, buffer + len, entry->contentLen);
    len+= entry->contentLen;
    nasLog("content %d bytes encoded| currentLen %d", entry->contentLen, len);

    if(len != entryLen) {
        higLog("encodedLen mismatch");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }
    *decLen = len;

    nasLogEXIT;
    return SUCCESS;
}
int decodeMultiPayloadContainer(PayloadContainer_t *pc, multiplePayloads_t *mp)
{
    nasLogENTRY;
    errno = E_NAS_ERROR_UNKNOWN;
    if(pc == NULL || mp == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        return FAILURE;
    }

    uint32_t len = 0;
    if(len + NUM_PAYLOADS_SIZE > pc->Length) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
    mp->numberOfEntries = pc->content[len];
    len += NUM_PAYLOADS_SIZE;
    nasLog("%d entries, len %d | currentLen %d", mp->numberOfEntries,
                                            NUM_PAYLOADS_SIZE, len);

    if(mp->numberOfEntries == 0 || mp->numberOfEntries > MAX_PAYLOAD_ENTRIES) {
        higLog("Invalid/Unsupported numberOfEntries %d", mp->numberOfEntries);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    for(int i=0; i<mp->numberOfEntries; i++) {
        uint32_t decodeLen = 0;
        if(decodeContainerEntry(pc->content + len, pc->Length - len,
                             &mp->entry[i], &decodeLen) == FAILURE) {
            higLog("decodeContainerEntry failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += decodeLen;
        nasLog("Decoded entry[%d], len %d | currentLen %d", i,
                        decodeLen, len);
    }

    nasLog("Decoded PayloadContainer of Length %d", pc->Length);
    nasLogEXIT;
    return SUCCESS;
}

int encodeContainerEntry(uint8_t *buffer, uint32_t bufLen,
                        payloadEntry_t *entry, uint32_t *encodLen)
{
    nasLogENTRY;

    if(bufLen == 0 || buffer == NULL || entry == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        nasLogEXIT;
        return FAILURE;
    }
    uint32_t len = 0;

    if(len + PAYLOAD_ENTRY_OFFSET > bufLen) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
    uint32_t entryLenOffset = len;
    len++;
    buffer[len] = entry->numOfIEs << 4 |  entry->containerType;
    len++;
    nasLog("Entry len & numOfIEs %d encoded, containerType %d | currentLen %d",
            entry->numOfIEs, entry->containerType, len);

    for(int i=0; i<entry->numOfIEs; i++) {
        if(len + PAYLOAD_OPT_IEI_LEN_SIZE > bufLen) {
            higLog("Buffer of insufficient len");
            errno = E_NAS_ERROR_BUFF_INSUFF;
            nasLogEXIT;
            return FAILURE;
        }

        uint8_t iei = entry->optIEs[i].IEI;
        nasLog("Added iei %d, (%d)th IE", iei, i);
        buffer[len] = iei;
        len++;
        uint32_t encLen = 0;
        switch(iei) {
        case PAYLOAD_PDU_SESSION_ID: {
            uint32_t ieLenOffset = len;
            len++;
            /* encodePDUSessionId2 does not encode len */
            if(encodePDUSessionId2(buffer + len, bufLen - len,
                        &entry->optIEs[i].u.pduSessionId, &encLen) ==FAILURE){
                higLog("Decoding PDU Session ID failed");
                nasLogEXIT;
                return FAILURE;
            }
            buffer[ieLenOffset] = encLen + PAYLOAD_OPT_IEI_LEN_SIZE;
            len+=encLen;
            nasLog("PDU Session ID encoded | currentLen = %d | encoded %d bytes",
                      len, encLen);
        }
        break;

        case PAYLOAD_DNN: {
            /* encodeDNN encodes len. ieLenOffset not needed */
            if(encodeDNN(buffer + len, bufLen - len, &entry->optIEs[i].u.dnn,
                                                &encLen) == FAILURE) {
                higLog("Encoding DNN failed");
                nasLogEXIT;
                return FAILURE;
            }

            len+=encLen;
            entry->optIEs[i].IEI = iei;
            nasLog("DNN encoded | currentLen = %d | encoded %d bytes",
                    len, encLen);
        }
        break;

        default: {
            switch(iei>>4) {

            case PAYLOAD_REQUEST_TYPE: {
                uint32_t ieLenOffset = len;
                len++;
                if(encodeRequestType(buffer + len, bufLen -len,
                                &entry->optIEs[i].u.requestType,
                                                        &encLen) == FAILURE){
                    higLog("Decoding Request Type Failed");
                    nasLogEXIT;
                    return FAILURE;
                }

                buffer[ieLenOffset] = encLen + PAYLOAD_OPT_IEI_LEN_SIZE;
                len+=encLen;
                entry->optIEs[i].IEI = iei;
                nasLog("Request Type encoded | currentLen = %d | decoded %d bytes",
                        len, encLen);
                }
            break;

            default:
                higLog("Unknown Optional Field IEI 0x%02x", iei);
                errno = E_NAS_ERROR_INVALID_PDU;
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;

        }
    }

    if(len > bufLen) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + entry->contentLen > bufLen) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
    memcpy(buffer + len, entry->content, entry->contentLen);
    nasLog("content %d bytes decoded| currentLen %d", entry->contentLen, len);
    len += entry->contentLen;

    buffer[entryLenOffset] = len;
    *encodLen = len;

    nasLogEXIT;
    return SUCCESS;
}
int encodeMultiPayloadContainer(multiplePayloads_t *mp, PayloadContainer_t *pc)
{
    nasLogENTRY;
    errno = E_NAS_ERROR_UNKNOWN;
    if(pc == NULL || mp == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        nasLogEXIT;
        return FAILURE;
    }

    uint32_t len = 0;
    if(len + NUM_PAYLOADS_SIZE > MAX_PAYLOAD_LEN) {
        higLog("Buffer of insufficient len");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    if(mp->numberOfEntries == 0 || mp->numberOfEntries > MAX_PAYLOAD_ENTRIES) {
        higLog("Invalid/Unsupported numberOfEntries %d", mp->numberOfEntries);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }
    pc->content[len] = mp->numberOfEntries;
    len += NUM_PAYLOADS_SIZE;
    nasLog("%d entries, len %d | currentLen %d", mp->numberOfEntries,
                                            NUM_PAYLOADS_SIZE, len);

    for(int i=0; i<mp->numberOfEntries; i++) {
        uint32_t encLen = 0;
        if(encodeContainerEntry(pc->content + len, MAX_PAYLOAD_LEN - len,
                                 &mp->entry[i], &encLen) == FAILURE) {
            higLog("encodeContainerEntry failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("Encoded entry[%d], len %d | currentLen %d", i,
                                    encLen, len);
    }

    pc->Length = len;
    nasLog("Encoded PayloadContainer of Length %d", pc->Length);

    nasLogEXIT;
    return SUCCESS;
}
