#include <bits/stdc++.h>
#include "common/include/setup.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/ServiceAccept.h"

int decodeServAcc(uint8_t *buffer,ServiceAccept_t *msgStruct,
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

    /*end of Mandatory field decoding since header is already decoded*/

    msgStruct->presenceMask = 0;
    while(len < bufLen) {
        uint8_t iei = *(buffer + len);
        len++;
        switch(iei) {

            case E_SERV_ACCEPT_IEI_PDU_SESSION_STATUS: {

                /* Decode PDU Session Status */
                decodLen = 0;
                if(decodePDUSessionStatus(buffer + len, bufLen - len,
                    &(msgStruct->pduSessionStatus), &decodLen) == FAILURE) {
                        higLog("Decoding PDU Session Status Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT;
                nasLog("PDU Session Status decoded | currentLen = %d | decoded %d bytes",
                        len, sizeof(msgStruct->pduSessionStatus));
            }
            break;

            case E_SERV_ACCEPT_IEI_PDU_SESSION_RESULT: {

                /* Decode PDU Session Status */
                decodLen = 0;
                if(decodePDUSessionResult(buffer + len, bufLen - len,
                    &(msgStruct->pduSessionResult), &decodLen) == FAILURE) {
                        higLog("Decoding PDU Session Result Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= SERVICE_ACCEPT_PDU_SESSION_RESULT_PRESENT;
                nasLog("PDU Session Result decoded | currentLen = %d | decoded %d bytes",
                        len, sizeof(msgStruct->pduSessionResult));
            }
            break;

            case E_SERV_ACCEPT_IEI_PDU_SESSION_RESULT_CAUSE: {

                /* Decode PDU Session Status */
                decodLen = 0;
                if(decodePDUSessionResultCause(buffer +len, bufLen - len,
                    &(msgStruct->pduSessionResultCause), &decodLen) == FAILURE) {
                        higLog("Decoding PDU Session Result Cause Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= 
                SERVICE_ACCEPT_PDU_SESSION_RESULT_CAUSE_PRESENT; 
                nasLog("PDU Session Result Cause decoded | currentLen = %d | decoded %d bytes",
                        len, sizeof(msgStruct->pduSessionResultCause));
            
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

int encodeServAcc(uint8_t *buffer, uint32_t bufLen,
                 ServiceAccept_t *msgStruct,uint32_t* encodedLen) {
    nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL ) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    unsigned int len = 0;
    uint32_t encLen =0;
     
    /*end of Mandatory field encoding since header is already encoded*/

    if((msgStruct->presenceMask & SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT) 
            == SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT) {

        buffer[len] = E_SERV_ACCEPT_IEI_PDU_SESSION_STATUS;
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

    if((msgStruct->presenceMask & SERVICE_ACCEPT_PDU_SESSION_RESULT_PRESENT) 
            == SERVICE_ACCEPT_PDU_SESSION_RESULT_PRESENT) {

        buffer[len] = E_SERV_ACCEPT_IEI_PDU_SESSION_RESULT;
        len++;

        encLen = 0;
        if(encodePDUSessionResult(buffer + len, bufLen - len,
            &(msgStruct->pduSessionResult), &encLen) == FAILURE) {
                higLog("Encoding PDU Session Result Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("PDU Session Result encoded | currentLen = %d | encoded %d bytes"
            , len, encLen);

    }else
        nasLog("PDU Session Result is not set");

    if((msgStruct->presenceMask & SERVICE_ACCEPT_PDU_SESSION_RESULT_CAUSE_PRESENT) 
            == SERVICE_ACCEPT_PDU_SESSION_RESULT_CAUSE_PRESENT) {

        buffer[len] = E_SERV_ACCEPT_IEI_PDU_SESSION_RESULT_CAUSE;
        len++;

        encLen = 0;
        if(encodePDUSessionResultCause(buffer + len, bufLen - len,
            &(msgStruct->pduSessionResultCause), &encLen) == FAILURE) {
                higLog("encoding PDU Session Result Cause Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("PDU Session Result Cause encoded | currentLen = %d | encoded %d bytes"
            , len, encLen);

    }else
        nasLog("PDU Session Result Cause is not set");


    nasLog("Successfully encoded len = %d",len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}



