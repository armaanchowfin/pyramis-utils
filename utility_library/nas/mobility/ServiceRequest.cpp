#include <bits/stdc++.h>
#include "common/include/setup.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/ServiceRequest.h"

int decodeServReq(uint8_t *buffer,ServiceRequest_t *msgStruct,
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

    /*1 copy _5gServiceType_t */
    if(decode5GSServType(buffer + len, bufLen - len,
                    &msgStruct->_5gservType, &decodLen) == FAILURE) {
            higLog("Decoding Service Type Failed");
            nasLogEXIT;
            return FAILURE;
    }

    /* Since a single octet is shared between Ser Type and ngKSI
     * below line 'len += decodLen' has been commented deliberately */
    //len += decodLen;
    decodLen = 0;
    if(decodeNasKeySetId(buffer + len, &(msgStruct->ngKsi), 
                bufLen - len, &decodLen, false) == FAILURE) {
            higLog("Decoding NAS Key Set ID Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += decodLen;
    nasLog("ngKsi decoded : tsc %d ngksi %d | currentLen = %d | decoded %d bytes",
    msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi, len, sizeof(msgStruct->ngKsi));

    decodLen = 0;
    if(decode5gsTmsi(buffer + len, bufLen - len,
        &(msgStruct->_5gsTmsi), &decodLen) == FAILURE) {
            higLog("Decoding 5G_S_TMSI Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += decodLen;
    nasLog("5G_S_TMSI decoded | currentLen = %d | decoded %d bytes", len,
                            decodLen);

    /* identityType = NO_IDENTITY - Unsupported */
    /*end of Mandatory field decoding*/

    msgStruct->presenceMask = 0;
    while(len < bufLen) {
        uint8_t iei = *(buffer + len);
        len++;
        switch(iei) {

            case E_SERV_REQUEST_IEI_UPLINK_DATA_STATUS: {

                /* Decode Uplink Data Status */
                decodLen = 0;
                if(decodeUplinkDataStatus(buffer + len, bufLen - len,
                    &(msgStruct->uplinkDataStatus), &decodLen) == FAILURE) {
                        higLog("Decoding Uplink Data Status Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= SERVICE_REQUEST_UPLINK_DATA_STATUS_PRESENT;
                nasLog("Uplink Data Status decoded | currentLen = %d | decoded %d bytes",
                        len, sizeof(msgStruct->uplinkDataStatus));
            }
            break;

            case E_SERV_REQUEST_IEI_PDU_SESSION_STATUS: {

                /* Decode PDU Session Status */
                decodLen = 0;
                if(decodePDUSessionStatus(buffer + len, bufLen - len,
                    &(msgStruct->pduSessionStatus), &decodLen) == FAILURE) {
                        higLog("Decoding PDU Session Status Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= SERVICE_REQUEST_PDU_SESSION_STATUS_PRESENT;
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

int encodeServReq(uint8_t *buffer, uint32_t bufLen,
                 ServiceRequest_t *msgStruct,uint32_t* encodedLen) {
    nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL ) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    unsigned int len = 0;
    uint32_t encLen =0;

    /*4. copy _5gServiceType_t, a _5gServiceType_t typedef */
    if(encode5GSServType(buffer + len, bufLen - len,
                    &msgStruct->_5gservType, &encLen) == FAILURE) {
            higLog("Encoding Service Type Failed");
            nasLogEXIT;
            return FAILURE;
    }

    /* Since a single octet is shared between Reg Type and ngKSI
     * below line 'len += encLen' has been commented deliberately */
    //len += encLen;
    encLen = 0;
    if(encodeNasKeySetId(buffer + len, bufLen - len,
                    &msgStruct->ngKsi, &encLen, false) == FAILURE) {
            higLog("Encoding NAS Key Set ID Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += encLen;


    nasLog("ngKsi encoded: tsc %d ngksi %d | currentLen = %d | encoded %d bytes"
    ,msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi,len, sizeof(msgStruct->ngKsi));

    encLen = 0;
    if(encode5gsTmsi(buffer + len, bufLen - len,
        &(msgStruct->_5gsTmsi), &encLen) == FAILURE) {
            higLog("Encoding 5G_S_TMSI Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += encLen;
    nasLog("5G_S_TMSI encoded | currentLen = %d | encoded %d bytes", len,encLen);

    /* identityType = NO_IDENTITY - Unsupported */
    /*end of Mandatory field encoding*/

    /*7. copy uplinkDataStatus, a struct */
    if((msgStruct->presenceMask & SERVICE_REQUEST_UPLINK_DATA_STATUS_PRESENT)
            == SERVICE_REQUEST_UPLINK_DATA_STATUS_PRESENT) {

        buffer[len] = E_SERV_REQUEST_IEI_UPLINK_DATA_STATUS;
        len++;

        encLen = 0;
        if(encodeUplinkDataStatus(buffer + len, bufLen - len,
            &(msgStruct->uplinkDataStatus), &encLen) == FAILURE) {
                higLog("Encoding Uplink Data Status Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Uplink Data Status encoded | currentLen = %d | encoded %d bytes",
            len, encLen);

    } else
        nasLog("Uplink Data Status is not set");

    /*8. copy pduSessionStatus, a struct */
    if((msgStruct->presenceMask & SERVICE_REQUEST_PDU_SESSION_STATUS_PRESENT) 
            == SERVICE_REQUEST_PDU_SESSION_STATUS_PRESENT) {

        buffer[len] = E_SERV_REQUEST_IEI_PDU_SESSION_STATUS;
        len++;

        encLen = 0;
        if(encodePDUSessionStatus(buffer + len, bufLen - len,
            &(msgStruct->pduSessionStatus), &encLen) == FAILURE) 
        {
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


