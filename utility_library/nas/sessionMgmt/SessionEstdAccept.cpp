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
#include "nas/sessionMgmt/_5gsmMsgApi.h"
#include "nas/sessionMgmt/SessionEstdAccept.h"


int encodeSessionEstdAccept(uint8_t *buffer, uint32_t bufLen,
        SessionEstdAccept_t *msgStruct,uint32_t* encodedLen)
{
    nasLogENTRY;
    uint32_t len =0;
    uint32_t encLen = 0;

    if(bufLen == 0 || buffer == NULL ||
        msgStruct == NULL || encodedLen == NULL) {
        higLog("Invalid Input parameters. Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /*Encode Code Here*/
    /*PDU Session Type*/
    encLen = 0;
    if(encodePDUSessionType(buffer +len, bufLen - len,
        &(msgStruct->selected_pduSessionType), &encLen,false) == FAILURE)
    {
            higLog("Encoding PDU Session Type Failed");
            nasLogEXIT;
            return FAILURE;
    }
    /*Length is not incremented ,as upcoming field will be loaded in second
    half of same byte*/
    nasLog("PDU Session Type encoded | currentLen = %d | encoded %d bytes", len,
                            SIZE_OF_PDU_SESSION_TYPE);

    /*SSC Mode*/
    encLen = 0;
    if(encodeSSCMode(buffer +len, bufLen - len,
        &(msgStruct->selected_sscMode), &encLen) == FAILURE)
    {
        higLog("Encoding SSC Mode Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += encLen;
    nasLog("SSC Mode encoded | currentLen = %d | encoded %d bytes", len,
        SIZE_OF_SSC_MODE);

    /*QoS Rules*/
    encLen = 0;
    if(encodeQosRules(buffer +len, bufLen - len,
        &(msgStruct->authorizedQosRules), &encLen) == FAILURE)
    {
        higLog("Encoding authorized QoS rules Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += encLen;
    nasLog("authorized QoS rules encoded | currentLen = %d | encoded %d bytes",
         len, encLen);

    /*Session AMBR*/
    encLen = 0;
    if(encodeSessionAMBR(buffer +len, bufLen - len,
        &(msgStruct->ambr), &encLen) == FAILURE)
    {
        higLog("Encoding Session AMBR Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += encLen;
    nasLog("Session AMBR encoded | currentLen = %d | encoded %d bytes",
         len, encLen);

    /*5GSM Cause*/
    if((msgStruct->presenceMask & SESSION_ACCEPT_NAS_IE_5GSM_CAUSE_PRESENT)
            == SESSION_ACCEPT_NAS_IE_5GSM_CAUSE_PRESENT) {

        if(len + SIZE_OF_IEI > bufLen) {
            higLog("Buffer insufficient");
            errno = E_NAS_ERROR_BUFF_INCOMPLETE;
            nasLogEXIT;
            return FAILURE;
        }

        buffer[len] = E_SESSION_ACCEPT_NAS_IEI_5GSM_CAUSE;
        len++;
        uint32_t encLen = 0;
        if(encode5gsmCause(buffer +len, bufLen - len,
            &(msgStruct->_5gsmCause), &encLen) == FAILURE)
        {
                higLog("Encoding 5GSMCause Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("5GSM Cause encoded | currentLen = %d | encoded %d bytes", len,
                                _5GSM_CAUSE_LEN);

    } else
        nasLog("5GSM Cause is not set");

    if(msgStruct->presenceMask & SESSION_ESTB_ACCEPT_PDU_ADDR_PRESENT) {
        buffer[len] = E_SESSION_ESTB_ACCEPT_PDU_ADDR_IEI;
        len++;
        nasLog("PDU Address: iei 0x%x | current len %d | encoded 1 byte",
                E_SESSION_ESTB_ACCEPT_PDU_ADDR_IEI, len);

        uint32_t encLen  = 0;
        if(encodePduAddress(buffer + len, bufLen - len,
                            &(msgStruct->pduAddress), &encLen) == FAILURE) {
            higLog("Encoding PDU Address failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("PDU Address encoded | len %d , currentLen %d", encLen, len);
    } else
        nasLog("PDU Address is not set");

    if(msgStruct->presenceMask &
        SESSION_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
        buffer[len] = E_SESSION_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS;
        len++;
        nasLog("QoS Flow Descriptions: iei 0x%x | current len %d | encoded 1 byte",
                E_SESSION_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS, len);

        uint32_t encLen  = 0;
        if(encodeQosFlowDescriptions(buffer + len, bufLen - len,
                &(msgStruct->authorizedQoSFlowDesc), &encLen) == FAILURE) {
            higLog("Encoding QoS Flow Descriptions failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("QoS Flow Descriptions encoded | len %d , currentLen %d",
                encLen, len);
    } else
        nasLog("Authorized QoS Flow Descriptions is not set");

    if(msgStruct->presenceMask & SESSION_ACCEPT_NAS_IE_DNN_PRESENT) {
        buffer[len] = E_SESSION_ACCEPT_NAS_IEI_DNN;
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

    *encodedLen  = len;
    nasLogEXIT;
    return SUCCESS;

}

int decodeSessionEstdAccept(uint8_t *buffer,SessionEstdAccept_t *msgStruct,
        uint32_t bufLen)
{
    nasLogENTRY;
    unsigned int len = 0;
    uint32_t decodLen = 0;

    if(buffer == NULL || msgStruct == NULL|| bufLen ==0) {
        higLog("Invalid Input parameters. Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /*Decode Code Here*/
    /*PDU Session Type*/
   decodLen = 0;
   if(decodePDUSessionType(buffer +len, bufLen - len,
        &(msgStruct->selected_pduSessionType), &decodLen,false)  == FAILURE)
    {
            higLog("Decoding PDU Session Type Failed");
            nasLogEXIT;
            return FAILURE;
    }
    /*Length is not incremented ,as upcoming field is loaded in second
    half of same byte*/
    nasLog("PDU Session Type decoded | currentLen = %d | decoded %d bytes",
            len, SIZE_OF_PDU_SESSION_TYPE);

    /*SSC Mode*/
    decodLen = 0;
    if(decodeSSCMode(buffer +len, bufLen - len,
        &(msgStruct->selected_sscMode), &decodLen)  == FAILURE)
    {
        higLog("Decoding SSC Mode Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += decodLen;
    nasLog("SSC Mode decoded | currentLen = %d | decoded %d bytes",
        len, SIZE_OF_SSC_MODE);

    /*QoS Rules*/
    decodLen = 0;
    if(decodeQosRules(buffer +len, bufLen - len,
        &(msgStruct->authorizedQosRules), &decodLen)  == FAILURE)
    {
        higLog("Decoding authorized QoS rules Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += decodLen;
    nasLog("Authorized QoS rules decoded | currentLen = %d | decoded %d bytes",
        len, decodLen);


    if( len > bufLen) {
        higLog("ERROR: crossed buffer boundaries");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    /*Session AMBR*/
    decodLen = 0;
    if(decodeSessionAMBR(buffer +len, bufLen - len,
        &(msgStruct->ambr), &decodLen)  == FAILURE)
    {
        higLog("Decoding Session AMBR Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += decodLen;
    nasLog("Session AMBR decoded | currentLen = %d | decoded %d bytes",
        len, decodLen);

    msgStruct->presenceMask = 0;
    while(len < bufLen) {
        uint8_t iei = *(buffer + len);
        len++;
        nasLog(" iei 0x%x | current len %d | decoded 1 byte", iei, len);

        uint32_t decodLen = 0;
        switch(iei) {

        case E_SESSION_ACCEPT_NAS_IEI_5GSM_CAUSE: {
            /*  5GSM Cause*/
            decodLen = 0;
            if(decode5gsmCause(buffer +len, bufLen - len,
                &(msgStruct->_5gsmCause), &decodLen)
                  == FAILURE)
            {
                    higLog("Decoding 5GSM Cause Failed");
                    nasLogEXIT;
                    return FAILURE;
            }
            len += decodLen;
            msgStruct->presenceMask |= SESSION_ACCEPT_NAS_IE_5GSM_CAUSE_PRESENT;
            nasLog("5GSM Cause decoded | currentLen = %d | decoded %d bytes",
                    len,_5GSM_CAUSE_LEN);
        }
        break;

        case E_SESSION_ACCEPT_NAS_IEI_DNN: {
            if(decodeDNN(buffer + len, bufLen - len,
                        &msgStruct->dnn, &decodLen) == FAILURE) {
                higLog("Decoding DNN failed");
                nasLogEXIT;
                return FAILURE;
            }

            len+=decodLen;
            msgStruct->presenceMask |= SESSION_ACCEPT_NAS_IE_DNN_PRESENT;
            nasLog("DNN decoded | currentLen = %d | decoded %d bytes",
                    len, decodLen);
        }
        break;

        case E_SESSION_ESTB_ACCEPT_PDU_ADDR_IEI: {
            uint32_t decodLen =0;
            if(decodePduAddress(buffer + len, bufLen -len,
                        &(msgStruct->pduAddress), &decodLen) == FAILURE) {
                higLog("Decoding PDU Address Failed");
                nasLogEXIT;
                return FAILURE;
            }
            len += decodLen;
            msgStruct->presenceMask |=
                                SESSION_ESTB_ACCEPT_PDU_ADDR_PRESENT;
            nasLog("PDU Address decoded | currentLen %d, len %d",
                        decodLen, len);
        }
        break;

        case E_SESSION_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS: {
            uint32_t decodLen =0;
            if(decodeQosFlowDescriptions(buffer + len, bufLen -len,
                &(msgStruct->authorizedQoSFlowDesc), &decodLen) == FAILURE) {
                higLog("Decoding QoS Flow Descriptions Failed");
                nasLogEXIT;
                return FAILURE;
            }
            len += decodLen;
            msgStruct->presenceMask |=
                    SESSION_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT;
            nasLog("QoS Flow Descriptions decoded | currentLen %d, len %d",
                        decodLen, len);
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

    if( len > bufLen) {
        higLog("ERROR: crossed buffer boundaries");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
    nasLog("Session Estb Accept Decoded, len %d", len);

    nasLogEXIT;
    return SUCCESS;
}
