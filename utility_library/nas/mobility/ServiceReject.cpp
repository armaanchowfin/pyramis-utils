#include <bits/stdc++.h>
#include "common/include/setup.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/ServiceReject.h"

int decodeServRej(uint8_t *buffer,ServiceReject_t *msgStruct,
                 uint32_t bufLen) { 
    nasLogENTRY;
    unsigned int len = 0;
    uint32_t decodLen = 0;
    
    if(buffer == NULL || msgStruct == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(decode5GMMCause(buffer + len, bufLen - len,
        &(msgStruct->_5gmmCause), &decodLen) == FAILURE) {
            higLog("Decoding 5GMMCause Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += decodLen;
    nasLog("5GMMCause decoded | currentLen = %d | decoded %d bytes", len,
                            decodLen);
    
    /*end of Mandatory field decoding*/

    msgStruct->presenceMask = 0;
    while(len < bufLen) {
        uint8_t iei = *(buffer + len);
        len++;
        switch(iei) {


            case E_SERV_REJECT_IEI_PDU_SESSION_STATUS: {

                /* Decode PDU Session Status */
                decodLen = 0;
                if(decodePDUSessionStatus(buffer + len, bufLen - len,
                    &(msgStruct->pduSessionStatus), &decodLen) == FAILURE) {
                        higLog("Decoding PDU Session Status Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT;
                nasLog("PDU Session Status decoded | currentLen = %d | decoded %d bytes",
                        len, sizeof(msgStruct->pduSessionStatus));
            }
            break;


            default : {
                higLog("Unknown Optional Field IEI 0x%02x", iei);
                errno = E_NAS_ERROR_INVALID_PDU;
                nasLogEXIT;
                return FAILURE;
            }
        }
    }
    
    if(len > bufLen) {
        higLog("ERROR: crossed buffer boundaries");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    nasLog("Successfully decoded, len = %d , expected : %d",len,decodLen);
    nasLogEXIT;
    return SUCCESS;
}

int encodeServRej(uint8_t *buffer, uint32_t bufLen,
                 ServiceReject_t *msgStruct,uint32_t* encodedLen) {
    nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL ) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    unsigned int len = 0;
    uint32_t encLen = 0;
    
    if(encode5GMMCause(buffer + len, bufLen - len,
        &(msgStruct->_5gmmCause), &encLen) == FAILURE) {
            higLog("Encoding 5GMMCause Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += encLen;
    nasLog("5GMMCause encoded | currentLen = %d | encoded %d bytes", len,encLen);
    
    /*end of Mandatory field encoding*/

    if((msgStruct->presenceMask & SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT) 
            == SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT) {

        buffer[len] = E_SERV_REJECT_IEI_PDU_SESSION_STATUS;
        len++;

        encLen = 0;
        if(encodePDUSessionStatus(buffer + len, bufLen - len,
            &(msgStruct->pduSessionStatus), &encLen) == FAILURE) {
                higLog("Encoding PDU Session Status Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("PDU Session Status encoded | currentLen = %d | encoded %d bytes"
            , len, encLen);

    }else
        nasLog("PDU Session Status is not set");


    nasLog("Successfully encoded len = %d",len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}



