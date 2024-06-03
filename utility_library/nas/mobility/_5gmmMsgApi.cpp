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
    TODO unify the log statements
*/
#include "nas/mobility/_5gmmMsgApi.h"

int _5gmmMsgEncode(uint8_t *buffer,uint32_t bufLen,_5gMMMsg_t *msgStruct,
                   uint32_t *encodedLen)
{
    nasLogENTRY;

    /*
        Check the invalid cases at the beggining
    */
    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL){
        higLog("%s","Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /*
        peek the _5gmmMsgHeader_t of the _5gMMMsg_t only : _5gMMMsg_t is an union
    */
    _5gmmMsgHeader_t *header = &(msgStruct->mmheader);
    /*
        Create a new variable len that will keep track of the
        total length encoded till a point
    */
    unsigned int len = 0;
    /*
        Encode the ExtendedProtocolDiscriminator
    */
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
    /*
        Encode the SecurityHeaderType
    */
    if(len + sizeof(header->secuHeader) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
		return FAILURE;
    }
    /*Omkar: Plain message should have null sec header always*/
    *(buffer + len) = SECURITY_HEADER_TYPE_NOT_PROTECTED;
    len += sizeof(header->secuHeader);
    nasLog("secuHeader value : %d | currentLen = %d | sizeof : %d",
        header->secuHeader,len,sizeof(header->secuHeader));
    /*
        Encode the MessageType
    */
    if(len + sizeof(header->msgType) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
		return FAILURE;
    }
    memcpy(buffer + len,&(header->msgType),sizeof(header->msgType));
    len += sizeof(header->msgType);
    nasLog("msgType value : %d | currentLen = %d | sizeof : %d",header->msgType,
        len,sizeof(header->msgType));


    // done encoding initial three header fields
    /*
        Peek the msgType inside the header of the emmMsg and
        decide the correct encoder to call
    */
    int rc;
    uint32_t __encodedLen = 0;
    switch(header->msgType) {
        case REGISTRATION_REQUEST: {
            rc = encodeRegReq(buffer+len,bufLen - len,&(msgStruct->regReqMsg),
                &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeRegReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case REGISTRATION_ACCEPT: {
            rc = encodeRegAccept(buffer + len,bufLen - len,&(msgStruct->regAcceptMsg),
                &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeRegAccept() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case AUTHENTICATION_REQUEST: {
            rc = encodeAuthReq(buffer + len,bufLen - len,&(msgStruct->authReqMsg),
                &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeAuthReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case AUTHENTICATION_RESPONSE: {
            rc = encodeAuthResp(buffer + len,bufLen - len,&(msgStruct->authRespMsg),
                &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeAuthResp() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case AUTHENTICATION_FAILURE: {
            rc = encodeAuthFail(buffer + len,bufLen - len,&(msgStruct->authFailMsg),
                &__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeAuthFail() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SECURITY_MODE_COMMAND: {
            rc = encodeSecuModeCommand(buffer + len,bufLen - len,
                &(msgStruct->secModeCmdMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSecuModeCommand() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SECURITY_MODE_COMPLETE : {
            rc = encodeSecuModeComplete(buffer + len,bufLen - len,
                &(msgStruct->secModeCompMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSecuModeComplete() failed");
                return FAILURE;
            }
        }
        break;
        case SECURITY_MODE_REJECT : {
            rc = encodeSecModeReject(buffer + len,bufLen - len,
                &(msgStruct->secModeRejMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeSecuModeComplete() failed");
                return FAILURE;
            }
        }
        break;
        case REGISTRATION_COMPLETE: {
            rc = encodeRegComplete(buffer + len,bufLen - len,
                &(msgStruct->regComplMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeRegComplete() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case REGISTRATION_REJECT: {
            rc = encodeRegRej(buffer + len,bufLen - len,
                &(msgStruct->regReject),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeRegReject() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case DEREGISTRATION_REQUEST_UE_ORIGINATING: {
            rc = encodeDeregReq(buffer + len,bufLen - len,
                &(msgStruct->deregReqMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeDeregReq failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case DEREGISTRATION_ACCEPT_UE_ORIGINATING: {
            rc = encodeDeregAccept(buffer + len,bufLen - len,
                &(msgStruct->deregAcceptMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeDeregAccept failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case AUTHENTICATION_REJECT: {
            rc = encodeAuthRej(buffer + len, bufLen - len,
                    &(msgStruct->authRej), &__encodedLen);
            if (rc == FAILURE) {
                higLog("%s", "encodeAuthRej failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case UL_NAS_TRANSPORT:{
            rc = encodeUlNAS(buffer+len, bufLen-len,
                             &(msgStruct->uplinkNasTransport),&__encodedLen);
            if(rc==FAILURE){
               higLog("%s", "encodeUlNAS failed");
               nasLogEXIT;
               return FAILURE;
             }
        }
        break;
        case DL_NAS_TRANSPORT:{
            rc = encodeDlNAS(buffer+len, bufLen-len,
                            &(msgStruct->downlinkNasTransport),&__encodedLen);
            if(rc==FAILURE){
               higLog("%s", "encodeDlNAS failed");
               nasLogEXIT;
               return FAILURE;
             }
        }
        break;
        case _5GMM_STATUS: {
            rc = encode5gmmStatus(buffer + len,bufLen - len,
                &(msgStruct->_5gmmStatus),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encode5gmmStatus() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SERVICE_REQUEST: {
            rc = encodeServReq(buffer + len,bufLen - len,
                &(msgStruct->servReqMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeServReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SERVICE_ACCEPT: {
            rc = encodeServAcc(buffer + len,bufLen - len,
                &(msgStruct->servAccMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeAccReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SERVICE_REJECT: {
            rc = encodeServRej(buffer + len,bufLen - len,
                &(msgStruct->servRejMsg),&__encodedLen);
            if(rc == FAILURE) {
                higLog("%s","encodeRejReq() failed");
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

    *encodedLen  = len + __encodedLen;
    nasLog("Exit from _5gmmMsgEncode : encoded %d, len %d",*encodedLen, len);
    nasLogEXIT;
    return SUCCESS;

}

int _5gmmMsgDecode(uint8_t *buffer,_5gMMMsg_t *msgStruct,uint32_t decodLen)
{
	nasLogENTRY;
    uint32_t len = 0;

	if(buffer == NULL || msgStruct == NULL ||
       decodLen < SIZE_5G_MOBILITY_MESSAGE_HEADER) {
        errno = E_NAS_ERROR_INVALID_INPUT;
		higLog("%s", "Invalid Input, Failure encoding");
        nasLogEXIT;
		return FAILURE;
	}
    _5gmmMsgHeader_t  *header = &(msgStruct->mmheader);

    /* 1.  decode the epd */
    memcpy(&(header->epd),buffer + len,sizeof(header->epd));
    len += sizeof(header->epd);
    nasLog("epd value : 0x%x | currentLen = %d | sizeof : %d",header->epd,len,
        sizeof(header->epd));

    /* 2.  decode the security header */
    memcpy(&(header->secuHeader),buffer + len,sizeof(header->secuHeader));
    len += sizeof(header->secuHeader);
    nasLog("secuHeader value : %d | currentLen = %d | sizeof : %d",header->secuHeader,len,
        sizeof(header->secuHeader));

    if(header->secuHeader != SECURITY_HEADER_TYPE_NOT_PROTECTED) { 
        errno = E_NAS_ERROR_INVALID_INPUT; /* TODO: will think of any other error no if possible and handling it as well*/
        higLog("Inner security header must be NOT_PROTECTED");
        nasLogEXIT;
        return FAILURE;
    }

    /*2. decode the nas message type */
    memcpy(&(header->msgType),buffer + len,sizeof(header->msgType));
    len += sizeof(header->msgType);
    nasLog("msgType value : %d | currentLen = %d | sizeof : %d",header->msgType,len,
        sizeof(header->msgType));

    int rc;
    switch(header->msgType) {
        case REGISTRATION_REQUEST:{
            rc = decodeRegReq(buffer + len,&(msgStruct->regReqMsg),
                decodLen - len);
            if(rc == FAILURE) {
                higLog("%s"," decodeRegReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case AUTHENTICATION_REQUEST: {
            rc = decodeAuthReq(buffer + len,&(msgStruct->authReqMsg),
                decodLen - len);
            if(rc == FAILURE) {
                higLog("%s"," decodeAuthReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SECURITY_MODE_COMMAND: {
            rc = decodeSecuModeCommand(buffer + len,&(msgStruct->secModeCmdMsg),
                decodLen -len );
            if(rc == FAILURE) {
                higLog("%s","decodeSecuModeCommand failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SECURITY_MODE_COMPLETE : {
            rc = decodeSecuModeComplete(buffer + len,&(msgStruct->secModeCompMsg),
                decodLen -len );
            if(rc == FAILURE) {
                higLog("%s","encodeSecuModeComplete failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SECURITY_MODE_REJECT : {
            higLog("%s","Received SECURITY_MODE_REJECT from UE!!");
            //Empty Call, actually doing nothing
            rc = decodeSecModeReject(buffer + len,&(msgStruct->secModeRejMsg),
                decodLen -len );
            if(rc == FAILURE) {
                higLog("%s","encodeSecuModeComplete failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case AUTHENTICATION_RESPONSE: {
            rc = decodeAuthResp(buffer + len, &(msgStruct->authRespMsg),
                    decodLen - len);
            if (rc == FAILURE) {
                higLog("%s", "decodeAuthResp failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case AUTHENTICATION_FAILURE: {
            rc = decodeAuthFail(buffer + len, &(msgStruct->authFailMsg),
                    decodLen - len);
            if (rc == FAILURE) {
                higLog("%s", "decodeFailResp failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case REGISTRATION_REJECT: {
            rc = decodeRegRej(buffer + len, &(msgStruct->regReject),
                    decodLen-len);
            if (rc == FAILURE) {
                higLog("%s", "decodeRegReq failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case AUTHENTICATION_REJECT: {
            rc = decodeAuthRej(buffer + len, &(msgStruct->authRej),
                    decodLen - len);
            if (rc == FAILURE) {
                higLog("%s", "decodeAuthRej failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case REGISTRATION_ACCEPT: {
            rc = decodeRegAccept(buffer + len, &(msgStruct->regAcceptMsg),
                    decodLen - len);
            if (rc == FAILURE) {
                higLog("%s", "decodeRegAccept failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case REGISTRATION_COMPLETE: {
            rc = decodeRegComplete(buffer + len, &(msgStruct->regComplMsg),
                    decodLen - len);
            if (rc == FAILURE) {
                higLog("%s", "decodeRegComplete failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case DEREGISTRATION_REQUEST_UE_ORIGINATING: {
            rc = decodeDeregReq(buffer + len, &(msgStruct->deregReqMsg),
                    decodLen - len);
            if (rc == FAILURE) {
                higLog("%s", "decodeDeregReq failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case DEREGISTRATION_ACCEPT_UE_ORIGINATING: {
            rc = decodeDeregAccept(buffer + len, &(msgStruct->deregAcceptMsg),
                    decodLen - len);
            if (rc == FAILURE) {
                higLog("%s", "decodeDeregAccept failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case UL_NAS_TRANSPORT:{
            rc = decodeUlNAS(buffer+len, &(msgStruct->uplinkNasTransport),
                             decodLen-len);
            if(rc==FAILURE){
               higLog("%s", "decodeUlNAS failed");
               nasLogEXIT;
               return FAILURE;
             }
        }
        break;
        case DL_NAS_TRANSPORT:{
            rc = decodeDlNAS(buffer+len, &(msgStruct->downlinkNasTransport),
                               decodLen-len);
            if(rc==FAILURE){
               higLog("%s", "decodeDlNAS failed");
               nasLogEXIT;
               return FAILURE;
             }
        }
        break;
        case _5GMM_STATUS:{
            rc = decode5gmmStatus(buffer+len, &(msgStruct->_5gmmStatus),
                                 decodLen-len);
            if(rc==FAILURE){
               higLog("%s", "decode5gmmStatus failed");
               nasLogEXIT;
               return FAILURE;
             }
        }
        break;
        case SERVICE_REQUEST:{
            rc = decodeServReq(buffer + len,&(msgStruct->servReqMsg),
                	      decodLen - len);
            if(rc == FAILURE) {
                higLog("%s"," decodeServReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SERVICE_ACCEPT:{
            rc = decodeServAcc(buffer + len,&(msgStruct->servAccMsg),
                	      decodLen - len);
            if(rc == FAILURE) {
                higLog("%s"," decodeAccReq() failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
        break;
        case SERVICE_REJECT:{
            rc = decodeServRej(buffer + len,&(msgStruct->servRejMsg),
                	      decodLen - len);
            if(rc == FAILURE) {
                higLog("%s"," decodeRejReq() failed");
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
