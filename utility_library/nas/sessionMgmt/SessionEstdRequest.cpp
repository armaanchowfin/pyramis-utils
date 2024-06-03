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
#include "nas/sessionMgmt/SessionEstdRequest.h"


int encodeSessionEstdReq(uint8_t *buffer, uint32_t bufLen,
		SessionEstdRequest_t *msgStruct,uint32_t* encodedLen)
{
    nasLogENTRY;
    uint32_t len =0;

    if(bufLen == 0 || buffer == NULL ||
        msgStruct == NULL || encodedLen == NULL) {
        higLog("Invalid Input parameters. Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    uint32_t encLen = 0;
    if(encodeIntegProtMaxDataRate(buffer + len, bufLen - len,
                                  &msgStruct->ipmdr, &encLen) == FAILURE) {
        higLog("encodeIntegProtMaxDataRate failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += encLen;

    /*PDU Session Type*/
    if((msgStruct->presenceMask & SESSION_ESTB_REQUEST_PDU_SESSION_TYPE)
            == SESSION_ESTB_REQUEST_PDU_SESSION_TYPE) {

        if(len + SIZE_OF_IEI > bufLen) {
            higLog("Buffer insufficient");
            errno = E_NAS_ERROR_BUFF_INCOMPLETE;
            nasLogEXIT;
            return FAILURE;
        }

        buffer[len] = E_SESSION_ESTB_REQ_IEI_PDU_SESSION_TYPE;
        //Here We are not incrementing length as PDUSession type IE is of 4bits only
        //We will OR our value with this buffer
        uint32_t encLen = 0;
        if(encodePDUSessionType(buffer +len, bufLen - len,
            &(msgStruct->pduSessionType), &encLen, true) == FAILURE)
        {
                higLog("Encoding PDU Session Type Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("PDU Session Type encoded | currentLen = %d | encoded %d bytes", len,
                                SIZE_OF_PDU_SESSION_TYPE);

    } else
        nasLog("PDU Session Type is not set");
    /*SSC Mode*/
    if((msgStruct->presenceMask & SESSION_ESTB_REQUEST_SSC_MODE)
            == SESSION_ESTB_REQUEST_SSC_MODE) {

        if(len + SIZE_OF_IEI > bufLen) {
            higLog("Buffer insufficient");
            errno = E_NAS_ERROR_BUFF_INCOMPLETE;
            nasLogEXIT;
            return FAILURE;
        }

        buffer[len] = E_SESSION_ESTB_REQ_IEI_SSC_MODE;
        //Here We are not incrementing length as PDUSession type IE is of 4bits only
        //We will OR our value with this buffer
        uint32_t encLen = 0;
        if(encodeSSCMode(buffer +len, bufLen - len,
            &(msgStruct->sscMode), &encLen) == FAILURE)
        {
                higLog("Encoding SSC Mode Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("SSC Mode encoded | currentLen = %d | encoded %d bytes", len,
                                SIZE_OF_SSC_MODE);

    } else
        nasLog("SSC Mode is not set");

    /*5GSM Capability*/
    if((msgStruct->presenceMask & SESSION_ESTB_REQUEST_5GSM_CAPABILITY)
            == SESSION_ESTB_REQUEST_5GSM_CAPABILITY) {

        if(len + SIZE_OF_IEI > bufLen) {
            higLog("Buffer insufficient");
            errno = E_NAS_ERROR_BUFF_INCOMPLETE;
            nasLogEXIT;
            return FAILURE;
        }

        buffer[len] = E_SESSION_ESTB_REQ_IEI_5GSM_CAPABILITY;
        len++;
        uint32_t encLen = 0;
        if(encode5gsmCapability(buffer +len, bufLen - len,
            &(msgStruct->_5gsmCapability), &encLen) == FAILURE)
        {
                higLog("Encoding 5GSMCapability Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("5GSM Capability encoded | currentLen = %d | encoded %d bytes", len,
                                encLen);

    } else
        nasLog("5GSM Capability is not set");

    /*Max No. of Supported Packet Filters*/
    if((msgStruct->presenceMask & SESSION_ESTB_REQUEST_MAX_NO_SUP_PAC_FLTRS)
            == SESSION_ESTB_REQUEST_MAX_NO_SUP_PAC_FLTRS) {

        if(len + SIZE_OF_IEI > bufLen) {
            higLog("Buffer insufficient");
            errno = E_NAS_ERROR_BUFF_INCOMPLETE;
            nasLogEXIT;
            return FAILURE;
        }

        buffer[len] = E_SESSION_ESTB_REQ_IEI_MAX_NO_SUP_PAC_FLTRS;
        len++;
        uint32_t encLen = 0;
        if(encodeMaxNoSupPackFilts(buffer +len, bufLen - len,
            &(msgStruct->maxNoSupportedPacketFilters), &encLen) == FAILURE)
        {
                higLog("Encoding Max No Supported Packet Filters Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Max No Supported Packet Filters encoded | currentLen = %d"
            "| encoded %d bytes", len,SIZE_OF_MAX_NO_SUPPORTED_PACKET_FILTERS);

    } else
        nasLog("Max No Supported Packet Filters is not set");


    *encodedLen  = len;
    nasLogEXIT;
    return SUCCESS;

}

int decodeSessionEstdReq(uint8_t *buffer,SessionEstdRequest_t *msgStruct,
		 uint32_t bufLen)
{
    nasLogENTRY;
    unsigned int len = 0;
    uint32_t decodLen = 0;

    if(buffer == NULL || msgStruct == NULL || bufLen  == 0) {
        higLog("Invalid Input parameters. Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(decodeIntegProtMaxDataRate(buffer + len, bufLen - len,
                                  &msgStruct->ipmdr, &decodLen) == FAILURE) {
        higLog("decodeIntegProtMaxDataRate failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += decodLen;

    msgStruct->presenceMask = 0;
    while(len < bufLen) {
        uint8_t iei = *(buffer + len);
        len++;
        switch(iei) {
            /*Few ies are 1 byte long while few are 4 bits long*/
            case E_SESSION_ESTB_REQ_IEI_5GSM_CAPABILITY : {
            /*  5GSM Capability */
                decodLen = 0;
                if(decode5gsmCapability(buffer +len, bufLen - len,
                    &(msgStruct->_5gsmCapability), &decodLen)  == FAILURE)
                {
                        higLog("Decoding 5GSM Capability Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= SESSION_ESTB_REQUEST_5GSM_CAPABILITY;
                nasLog("5GSM Capability decoded | currentLen = %d | decoded %d bytes",
                        len, decodLen);
            }
            break;
            case E_SESSION_ESTB_REQ_IEI_MAX_NO_SUP_PAC_FLTRS : {
            /*  Max No of Supported Packet Filters*/
                decodLen = 0;
                if(decodeMaxNoSupPackFilts(buffer +len, bufLen - len,
                    &(msgStruct->maxNoSupportedPacketFilters), &decodLen)  == FAILURE)
                {
                        higLog("Decoding Max No of Supported Packet Filters Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= SESSION_ESTB_REQUEST_MAX_NO_SUP_PAC_FLTRS;
                nasLog("Max No of Supported Packet Filters decoded | currentLen = %d | decoded %d bytes",
                        len, decodLen);
            }
            break;
            default : {
                //Few of the IEI are 4 bits(Upper Half) only
                uint8_t iei2 = iei & 0xF0;
                len--;//Data is within same byte
                switch(iei2)
                {
                    case E_SESSION_ESTB_REQ_IEI_PDU_SESSION_TYPE : {
                        /* 4. Decode PDU Session Type */
                        decodLen = 0;
                        if(decodePDUSessionType(buffer +len, bufLen - len,
                            &(msgStruct->pduSessionType), &decodLen,true)  == FAILURE)
                        {
                                higLog("Decoding PDU Session Type Failed");
                                nasLogEXIT;
                                return FAILURE;
                        }
                        len += decodLen;
                        msgStruct->presenceMask |= SESSION_ESTB_REQUEST_PDU_SESSION_TYPE;
                        nasLog("PDU Session Type decoded | currentLen = %d | decoded %d bytes",
                                len, SIZE_OF_PDU_SESSION_TYPE);
                    }
                    break;

                    case E_SESSION_ESTB_REQ_IEI_SSC_MODE : {
                        /* 4. Decode SSC Mode */
                        decodLen = 0;
                        if(decodeSSCMode(buffer +len, bufLen - len,
                            &(msgStruct->sscMode), &decodLen)  == FAILURE)
                        {
                                higLog("Decoding SSC Mode Failed");
                                nasLogEXIT;
                                return FAILURE;
                        }
                        len += decodLen;
                        msgStruct->presenceMask |= SESSION_ESTB_REQUEST_SSC_MODE;
                        nasLog("SSC Mode decoded | currentLen = %d | decoded %d bytes",
                                len, SIZE_OF_SSC_MODE);
                    }
                    break;

                    default:{
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

    nasLogEXIT;
    return SUCCESS;
}
