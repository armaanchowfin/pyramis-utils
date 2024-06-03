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

#include "nas/sessionMgmt/_5gsmMsgApi.h"

int _5gsmMsgEncode(uint8_t *buffer,uint32_t bufLen,_5GSMMsg_t *msgStruct,uint32_t *encodedLen)
{
    nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL){
        higLog("%s","Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    _5gsmMsgHeader_t *header = &(msgStruct->smheader);

    unsigned int len = 0;
    if(len + sizeof(header->epd) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
		return FAILURE;
    }

    memcpy(buffer + len,&(header->epd),sizeof(header->epd));
    len += sizeof(header->epd);

    nasLog("epd value : 0x%x | currentLen = %d | sizeof : %d",header->epd,len,
        sizeof(header->epd));

    if(len + sizeof(header->PduSessionid) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
		return FAILURE;
    }
    memcpy(buffer + len,&(header->PduSessionid),sizeof(header->PduSessionid));
    len += sizeof(header->PduSessionid);
    nasLog("PduSessionid value : %d | currentLen = %d | sizeof : %d",
        header->PduSessionid,len,sizeof(header->PduSessionid));


    if(len + sizeof(header->Pti) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
		return FAILURE;
    }
    memcpy(buffer + len,&(header->Pti),sizeof(header->Pti));
    len += sizeof(header->Pti);
    nasLog("Pti value : %d | currentLen = %d | sizeof : %d",
        header->Pti,len,sizeof(header->Pti));

    if(len + sizeof(header->msgType) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
		return FAILURE;
    }
    memcpy(buffer + len,&(header->msgType),sizeof(header->msgType));
    len += sizeof(header->msgType);
    nasLog("msgType value : %d | currentLen = %d | sizeof : %d",
        header->msgType,len,sizeof(header->msgType));

    int rc;
    uint32_t __encodedLen = 0;
    switch(header->msgType) {
        case SESSION_ESTABLISHMENT_REQUEST: {
            rc = encodeSessionEstdReq(buffer+len, bufLen - len,
                    &(msgStruct->SessionEstdRequest), &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionEstdReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_ESTABLISHMENT_REJECT: {
            rc = encodeSessionEstdReject(buffer+len, bufLen - len,
                    &(msgStruct->SessionEstdReject), &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionEstdReject() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_RELEASE_REQUEST: {
            rc = encodeSessionRelReq(buffer+len,bufLen - len,
                    &(msgStruct->SessionRelReq), &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionRelReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_RELEASE_COMMAND: {
            rc = encodeSessionRelCommand(buffer+len,bufLen - len,
                    &(msgStruct->sessionRelCommand), &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionRelCommand() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_ESTABLISHMENT_ACCEPT: {
            rc = encodeSessionEstdAccept(buffer+len,bufLen - len,&(msgStruct->SessionEstdAccept),
                &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionEstdAccept() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_RELEASE_COMPLETE: {
            rc = encodeSessionRelComplete(buffer+len,bufLen - len,
                    &(msgStruct->sessionRelComplete), &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionRelComplete() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_MODIFICATION_REQUEST: {
            rc = encodePduSessionModificationRequest(buffer+len,bufLen - len,&(msgStruct->SessionModRequest),
                &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionModReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_MODIFICATION_COMMAND: {
            rc = encodePduSessionModificationCommand(buffer+len, bufLen - len,
                            &(msgStruct->sessionModCommand), &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionModCommand() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_MODIFICATION_COMPLETE: {
            rc = encodePduSessionModificationComplete(buffer+len, bufLen - len,
                            &(msgStruct->sessionModComplete), &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSessionModComplete() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_MODIFICATION_REJECT: {
            rc = encodePduSessionModificationReject(buffer+len, bufLen - len,
                            &(msgStruct->SessionModReject), &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodePduSessionModificationReject() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        default: {
            higLog("%s"," Unknown msgType");
            errno = E_NAS_ERROR_INVALID_PDU;
            nasLogEXIT;
            return FAILURE;
        }
    }

    *encodedLen = len + __encodedLen;
    nasLog("Exit from _5gsmMsgEncode : encoded %d, len %d",*encodedLen, len);
    nasLogEXIT;
    return SUCCESS;
}

int _5gsmMsgDecode(uint8_t *buffer,_5GSMMsg_t *msgStruct,uint32_t decodLen)
{

	nasLogENTRY;
    uint32_t len = 0;

	if(buffer == NULL || msgStruct == NULL ||
       decodLen < SIZE_5G_SESSION_MESSAGE_HEADER) {
        errno = E_NAS_ERROR_INVALID_INPUT;
		higLog("%s", "Invalid Input, Failure encoding");
        nasLogEXIT;
		return FAILURE;
	}
    _5gsmMsgHeader_t  *header = &(msgStruct->smheader);

    memcpy(&(header->epd),buffer + len,sizeof(header->epd));
    len += sizeof(header->epd);
    nasLog("epd value : 0x%x | currentLen = %d | sizeof : %d",header->epd,len,
        sizeof(header->epd));

    memcpy(&(header->PduSessionid),buffer + len,sizeof(header->PduSessionid));
    len += sizeof(header->PduSessionid);
    nasLog("PduSessionid value : %d | currentLen = %d | sizeof : %d",header->PduSessionid,len,
        sizeof(header->PduSessionid));

    memcpy(&(header->Pti),buffer + len,sizeof(header->Pti));
    len += sizeof(header->Pti);
    nasLog("Pti value : %d | currentLen = %d | sizeof : %d",header->Pti,len,
        sizeof(header->Pti));

    memcpy(&(header->msgType),buffer + len,sizeof(header->msgType));
    len += sizeof(header->msgType);
    nasLog("msgType value : %d | currentLen = %d | sizeof : %d",header->msgType,len,
        sizeof(header->msgType));

    int rc;
    switch(header->msgType) {
        case SESSION_ESTABLISHMENT_REQUEST:{
            rc = decodeSessionEstdReq(buffer + len,&(msgStruct->SessionEstdRequest),
                decodLen - len);
            if(rc == FAILURE) {
                higLog("%s","decode sessEstbRequest() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_ESTABLISHMENT_REJECT:{
            rc = decodeSessionEstdReject(buffer + len,&(msgStruct->SessionEstdReject),
                decodLen - len);
            if(rc == FAILURE) {
                higLog("%s","decode sessEstbReject() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_ESTABLISHMENT_ACCEPT:{
            rc = decodeSessionEstdAccept(buffer + len,&(msgStruct->SessionEstdAccept),
                decodLen - len);
            if(rc == FAILURE) {
                higLog("%s","decode sessEstbAccept() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_MODIFICATION_REQUEST:{
            rc = decodePduSessionModificationRequest(buffer + len,&(msgStruct->SessionModRequest),
                decodLen - len);
            if(rc == FAILURE) {
                higLog("%s"," decodePduSessionModificationRequest() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_RELEASE_REQUEST:{
            rc = decodeSessionRelReq(buffer + len,
                    &(msgStruct->SessionRelReq), decodLen - len);
            if(rc == FAILURE) {
                higLog("decodeSessionRelReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_RELEASE_COMMAND:{
            rc = decodeSessionRelCommand(buffer + len,
                    &(msgStruct->sessionRelCommand), decodLen - len);
            if(rc == FAILURE) {
                higLog("decodeSessionRelCommand() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_RELEASE_COMPLETE:{
            rc = decodeSessionRelComplete(buffer + len,
                    &(msgStruct->sessionRelComplete), decodLen - len);
            if(rc == FAILURE) {
                higLog("decodeSessionRelComplete() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_MODIFICATION_COMMAND:{
            rc = decodePduSessionModificationCommand(buffer + len,
                    &(msgStruct->sessionModCommand), decodLen - len);
            if(rc == FAILURE) {
                higLog("decodeSessionModCommand() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_MODIFICATION_COMPLETE:{
            rc = decodePduSessionModificationComplete(buffer + len,
                    &(msgStruct->sessionModComplete), decodLen - len);
            if(rc == FAILURE) {
                higLog("decodeSessionModComplete() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SESSION_MODIFICATION_REJECT:{
            rc = decodePduSessionModificationReject(buffer + len,
                    &(msgStruct->SessionModReject), decodLen - len);
            if(rc == FAILURE) {
                higLog("decodePduSessionModificationReject() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        default: {
            higLog("Unknown/Unsupported message type %d",header->msgType);
            errno = E_NAS_ERROR_INVALID_PDU;
            nasLogEXIT;
            return FAILURE;
        }
    }

    nasLog("Successfully decoded %d bytes", decodLen);
    nasLogEXIT;
    return SUCCESS;

}
