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

#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "common/include/setup.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/UplinkNasTransport.h"


int decodeUlNAS(uint8_t *buffer, UplinkNasTransport_t *msgStruct,
		   uint32_t bufLen)
{

	nasLogENTRY;

	uint32_t consumedLen = 0;
	uint32_t len = 0;

	if(buffer == NULL || msgStruct == NULL || bufLen < 0) {
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

        case E_UPLINK_NAS_IEI_PDU_SESSION_ID: {
            if(decodePDUSessionId2(buffer + len, bufLen - len,
                        &msgStruct->pduSessionId, &decodLen) == FAILURE) {
                higLog("Decoding PDU Session ID failed");
                nasLogEXIT;
                return FAILURE;
            }

            len+=decodLen;
            msgStruct->presenceMask |= UPLINK_NAS_IE_PDU_SESSION_ID_PRESENT;
            nasLog("PDU Session ID decoded | currentLen = %d | decoded %d bytes",
                    len, decodLen);
        }
        break;

        case E_UPLINK_NAS_IEI_DNN: {
            if(decodeDNN(buffer + len, bufLen - len, &msgStruct->dnn,
                                                &decodLen) == FAILURE) {
                higLog("Decoding DNN failed");
                nasLogEXIT;
                return FAILURE;
            }

            len+=decodLen;
            msgStruct->presenceMask |= UPLINK_NAS_IE_DNN_PRESENT;
            nasLog("DNN decoded | currentLen = %d | decoded %d bytes",
                    len, decodLen);
        }
        break;
        case E_UPLINK_NAS_IEI_NSSAI: {
            if(decodeSnssai(buffer + len,bufLen - len,&msgStruct->snssai,
                                &decodLen) == FAILURE) {
                higLog("Decoding S-NSSAI failed");
                nasLogEXIT;
                return FAILURE;
            }

            len += decodLen;
            msgStruct->presenceMask |= UPLINK_NAS_IE_NSSAI_PRESENT;
            nasLog("S-NSSAI decoded | currentLen = %d | decoded %d bytes",
                    len,decodLen);   
        }
        break;
        default : {

            /*Might be a half octet IEI */
            uint8_t iei2 = iei >> 4;
            len--;  /* IE will be within this octet */

            switch(iei2) {
            case E_UPLINK_NAS_IEI_REQUEST_TYPE: {
                if(decodeRequestType(buffer + len, bufLen -len,
                            &msgStruct->requestType, &decodLen) == FAILURE){
                    higLog("Decoding Request Type Failed");
                    nasLogEXIT;
                    return FAILURE;
                }

                len+=decodLen;
                msgStruct->presenceMask |= UPLINK_NAS_IE_REQUEST_TYPE;
                nasLog("Request Type decoded | currentLen = %d | decoded %d bytes",
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



int encodeUlNAS(uint8_t *buffer, uint32_t bufLen,
		  UplinkNasTransport_t *msgStruct, uint32_t *encodedLen)
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

	len = encLen+len;
    nasLog("PayLoad container encoded| currentLen %d ", len);

    if(msgStruct->presenceMask & UPLINK_NAS_IE_PDU_SESSION_ID_PRESENT) {

        buffer[len] = E_UPLINK_NAS_IEI_PDU_SESSION_ID;
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

    if(msgStruct->presenceMask & UPLINK_NAS_IE_DNN_PRESENT) {
        buffer[len] = E_UPLINK_NAS_IEI_DNN;
        len++;

        encLen = 0;
        if(encodeDNN(buffer + len, bufLen -len,
                     &msgStruct->dnn, &encLen) == FAILURE) {
            higLog("Encoding DNN Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("DNN encoded | currentLen %d | encoded %d bytes",
                len, encLen);
    } else
        nasLog("DNN is not set");

    if(msgStruct->presenceMask & UPLINK_NAS_IE_REQUEST_TYPE) {
        buffer[len] = (E_UPLINK_NAS_IEI_REQUEST_TYPE << 4);
        /*length not incremented since half octect IEI*/

        encLen = 0;
        if(encodeRequestType(buffer + len, bufLen - len,
                    &msgStruct->requestType, &encLen) == FAILURE) {
            higLog("Encoding Request Type Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("Request Type Encoded | currentLen %d | encoded %d bytes",
                len, encLen);
    }
    
    /* check if single nssai IE is present of not is UL NAS */
    if(msgStruct->presenceMask & UPLINK_NAS_IE_NSSAI_PRESENT) {
        buffer[len] = E_UPLINK_NAS_IEI_NSSAI;
        len++;

        encLen = 0;
        if(encodeSnssai(buffer + len, bufLen - len,
                    &msgStruct->snssai,&encLen) == FAILURE) {
            higLog("Encoding Request Type Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("S-NSSAI Encoded | currentLen %d | encoded %d bytes",
                len,encLen);
    }

    *encodedLen = len;
    nasLog("Successfully encoded UL Nas Message %d bytes", len);
    nasLogEXIT;
    return SUCCESS;

}
