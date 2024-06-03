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
#include "common/include/setup.h"
#include <stdlib.h>
#include "platform/include/platform.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/RegistrationAccept.h"


int decodeRegAccept(uint8_t *buffer, RegistrationAcceptMsg_t *msgStruct,
		  uint32_t bufLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint32_t decodLen = 0;

	if(buffer == NULL || msgStruct == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    /* 5G Registration result */
    if(decode5GSRegResult(buffer + len, bufLen - len,
                    &msgStruct->_5gregResult, &decodLen) == FAILURE)
    {
            higLog("Decoding Registration Result Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += decodLen;
    nasLog("5gregResult decoded | currentLen = %d | decoded %d bytes", len,
                                        sizeof(msgStruct->_5gregResult));

    /* Decoding optional fields */
    msgStruct->presenceMask = 0;

    while(len < bufLen) {
        uint8_t iei = *(buffer + len);
        len++;
        nasLog(" iei 0x%x | current len %d | decoded 1 byte", iei, len);
        switch(iei) {

            /* 5G mobile Id */
            case E_REG_ACCEPT_IEI_5G_GUTI : {

                uint16_t _5gMobileIdLenN = *(uint16_t*)(buffer + len);
                uint16_t _5gMobileIdLen = ntohs(_5gMobileIdLenN);
                nasLog("Reg Accept Mobile ID len %u", _5gMobileIdLen);
                len += sizeof(msgStruct->_5gmobileId.len);
                if (_5gMobileIdLen != GUTI_LENGTH) {
                    higLog("%s", "Mobile ID length incorrect");
                     nasLogEXIT;
                    return FAILURE;
                }
                if (len + _5gMobileIdLen > bufLen) {
                    higLog("%s", "FAILURE, Incomplete buffer");
                    errno = E_NAS_ERROR_BUFF_INCOMPLETE;
                    nasLogEXIT;
                    return FAILURE;
                }
                decodLen = 0;
                if(decode5GGuti(buffer +len, bufLen - len,
                    &(msgStruct->_5gmobileId.guti5gMobileId), &decodLen) == FAILURE) {
                        higLog("Decoding 5G Guti Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= REGISTRATION_ACCEPT_5G_GUTI_PRESENT;
                nasLog("5g Guti decoded | currentLen = %d | decoded %d bytes", len, decodLen);
            }
            break;

            case E_REG_ACCEPT_IEI_T3512 : {

                /* Timer 3512 */
                decodLen = 0;
                if(decodet3512(buffer +len, bufLen - len,
                    &(msgStruct->timer3512), &decodLen) == FAILURE)
                {
                        higLog("Decoding timer 3512 Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |= REGISTRATION_ACCEPT_T3512_PRESENT;
                nasLog("Timer 3512 decoded | currentLen = %d | decoded %d bytes",
                        len,sizeof(msgStruct->timer3512));
            }
            break;

            case E_REG_ACCEPT_IEI_ALLOWED_NSSAI: {

                /* Decode Allowed NSSAI IE */
                decodLen = 0;
                if(decodeNssai(buffer +len, bufLen - len,
                    &(msgStruct->allowedNssai), &decodLen) == FAILURE)
                {
                        higLog("Decoding NSSAI Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |=
                                REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT;
                nasLog("allowed NSSAI decoded | currentLen = %d | decoded %d bytes",
                        len, decodLen);
            }
            break;

            case E_REG_ACCEPT_IEI_REJ_NSSAI: {

                /* Decode Allowed NSSAI IE */
                decodLen = 0;
                if(decodeRejectedNssai(buffer +len, bufLen - len,
                    &(msgStruct->rejectedNssai), &decodLen) == FAILURE)
                {
                        higLog("Decoding NSSAI Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |=
                                REGISTRATION_ACCEPT_REJ_NSSAI_PRESENT;
                nasLog("rej NSSAI decoded | currentLen = %d | decoded %d bytes",
                        len, decodLen);
            }
            break;

            case E_REG_ACCEPT_IEI_PDU_SESSION_STATUS: {

                /* Decode PDU Session Status */
                decodLen = 0;
                if(decodePDUSessionStatus(buffer + len, bufLen - len,
                    &(msgStruct->pduSessionStatus), &decodLen) == FAILURE) {
                        higLog("Decoding PDU Session Status Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |=
                    REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT;
                nasLog("PDU Session Status decoded | currentLen = %d | decoded %d bytes",
                        len, sizeof(msgStruct->pduSessionStatus));
            }
            break;

            case E_REG_ACCEPT_IEI_PDU_SESSION_RESULT: {

                /* Decode PDU Session Result */
                decodLen = 0;
                if(decodePDUSessionResult(buffer + len, bufLen - len,
                    &(msgStruct->pduSessionResult), &decodLen) == FAILURE) {
                        higLog("Decoding PDU Session Result Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |=
                    REGISTRATION_ACCEPT_PDU_SESSION_RESULT_PRESENT;
                nasLog("PDU Session Result decoded | currentLen = %d | decoded %d bytes",
                        len, sizeof(msgStruct->pduSessionResult));
            }
            break;

            case E_REG_ACCEPT_IEI_PDU_SESSION_RESULT_CAUSE: {

                /* Decode PDU Session Result Cause */
                decodLen = 0;
                if(decodePDUSessionResultCause(buffer +len, bufLen - len,
                    &(msgStruct->pduSessionResultCause), &decodLen) == FAILURE) {
                        higLog("Decoding PDU Session Result Cause Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |=
                    REGISTRATION_ACCEPT_PDU_SESSION_RESULT_CAUSE_PRESENT;
                nasLog("PDU Session Result Cause decoded | currentLen = %d | decoded %d bytes",
                        len, sizeof(msgStruct->pduSessionResultCause));

            }
            break;

            case E_REG_ACCEPT_IEI_TAI_LIST: {

                /* Decode TAI List */
                decodLen = 0;
                if(decodeTAIList(buffer +len, bufLen - len,
                    &(msgStruct->taiList), &decodLen) == FAILURE) {
                        higLog("Decoding TAI List Failed");
                        nasLogEXIT;
                        return FAILURE;
                }
                len += decodLen;
                msgStruct->presenceMask |=
                    REGISTRATION_ACCEPT_TAI_LIST_PRESENT;
                nasLog("TAI List decoded | currentLen = %d | decoded %d bytes",
                        len, decodLen);
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
    nasLog("Successfully decoded, len = %d , expected : %d",len,bufLen);
    nasLogEXIT;
    return SUCCESS;
}


int encodeRegAccept(uint8_t *buffer, uint32_t bufLen,
		  RegistrationAcceptMsg_t *msgStruct, uint32_t *encodedLen)
{

	nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL)
    {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    unsigned int len = 0;
    uint32_t encLen =0;

    /* 5gRegistrationResult */
    if(encode5GSRegResult(buffer +len, bufLen - len,
                    &msgStruct->_5gregResult, &encLen) == FAILURE)
    {
            higLog("Encoding Registration Result Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += encLen;
    nasLog("5regResult encoded | currentLen = %d | encoded %d bytes", len,
                                            msgStruct->_5gregResult.len);

    /* Encode optional fields. Note - Maintain IE order given in Spec */
    /* 5G Mobile ID */
    if((msgStruct->presenceMask & REGISTRATION_ACCEPT_5G_GUTI_PRESENT) ==
                                REGISTRATION_ACCEPT_5G_GUTI_PRESENT) {

        if (len + SIZE_OF_IEI > bufLen) {
            higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
            errno = E_NAS_ERROR_BUFF_INSUFF;
            nasLogEXIT;
            return FAILURE;
        }
        buffer[len] = E_REG_ACCEPT_IEI_5G_GUTI;
        len++;
        nasLog("5G Guti : iei 0x%x | current len %d | encoded 1 byte",
                                                E_REG_ACCEPT_IEI_5G_GUTI, len);

        if (len + sizeof(msgStruct->_5gmobileId.len) > bufLen) {
            higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
            errno = E_NAS_ERROR_BUFF_INSUFF;
            nasLogEXIT;
            return FAILURE;
        }
        uint16_t _5gMobileIdLen = msgStruct->_5gmobileId.len;
        uint16_t _5gMobileIdLenN = htons(_5gMobileIdLen);
        memcpy(buffer + len, &_5gMobileIdLenN, sizeof(msgStruct->_5gmobileId.len));
        len += sizeof(msgStruct->_5gmobileId.len);
        nasLog("5G Guti : len %d | current len %d | encoded %d bytes",
           _5gMobileIdLen, len, sizeof(msgStruct->_5gmobileId.len));

        encLen = 0;
        if(encode5GGuti(buffer +len, bufLen - len,
            &(msgStruct->_5gmobileId.guti5gMobileId), &encLen) == FAILURE){
                higLog("Encoding 5G Guti Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("5g Guti encoded | currentLen = %d | encoded %d bytes", len,
                                encLen);
    }else {
        nasLog("_5gMobileID not set");
    }

    /* TAI List */
    if((msgStruct->presenceMask & REGISTRATION_ACCEPT_TAI_LIST_PRESENT) ==
                                REGISTRATION_ACCEPT_TAI_LIST_PRESENT) {

        buffer[len] = E_REG_ACCEPT_IEI_TAI_LIST;
        uint8_t lenOffset = len;
        len++;

        encLen = 0;
        if(encodeTAIList(buffer + len, bufLen - len,
            &(msgStruct->taiList), &encLen) == FAILURE) {
                higLog("encoding TAI List Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        printArray("TAI List encoded buffer", buffer + lenOffset, encLen +1);
        nasLog("TAI List encoded | currentLen = %d | encoded %d bytes"
            , len, encLen);
    } else {
        nasLog("TAI List is not set");
    }

    /* Allowed NSSAI */
    if((msgStruct->presenceMask & REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT)
            == REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT) {

        buffer[len] = E_REG_ACCEPT_IEI_ALLOWED_NSSAI;
        len++;

        encLen = 0;
        if(encodeNssai(buffer +len, bufLen - len,
            &(msgStruct->allowedNssai), &encLen) == FAILURE)
        {
                higLog("Encoding NSSAI Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Allowed NSSAI encoded | currentLen = %d | encoded %d bytes",
                                    len, encLen);
    } else {
        nasLog("Allowed NSSAI is not set");
    }

    /* Rejected NSSAI */
    if((msgStruct->presenceMask & REGISTRATION_ACCEPT_REJ_NSSAI_PRESENT)
            == REGISTRATION_ACCEPT_REJ_NSSAI_PRESENT) {

        buffer[len] = E_REG_ACCEPT_IEI_REJ_NSSAI;
        len++;

        encLen = 0;
        if(encodeRejectedNssai(buffer +len, bufLen - len,
            &(msgStruct->rejectedNssai), &encLen) == FAILURE)
        {
                higLog("Encoding NSSAI Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Rejected NSSAI encoded | currentLen = %d | encoded %d bytes",
                                    len, encLen);
    } else {
        nasLog("Rejected NSSAI is not set");
    }

    /* PDU Session Status */
    if((msgStruct->presenceMask &
        REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT)
        == REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT) {

        buffer[len] = E_REG_ACCEPT_IEI_PDU_SESSION_STATUS;
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

    }else {
        nasLog("PDU Session Status is not set");
    }

    /* PDU session reactivation result */
    if((msgStruct->presenceMask &
        REGISTRATION_ACCEPT_PDU_SESSION_RESULT_PRESENT)
        == REGISTRATION_ACCEPT_PDU_SESSION_RESULT_PRESENT) {

        buffer[len] = E_REG_ACCEPT_IEI_PDU_SESSION_RESULT;
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

    }else {
        nasLog("PDU Session Result is not set");
    }

    /* PDU session reactivation result error cause */
    if((msgStruct->presenceMask &
        REGISTRATION_ACCEPT_PDU_SESSION_RESULT_CAUSE_PRESENT)
        == REGISTRATION_ACCEPT_PDU_SESSION_RESULT_CAUSE_PRESENT) {

        buffer[len] = E_REG_ACCEPT_IEI_PDU_SESSION_RESULT_CAUSE;
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

    }else {
        nasLog("PDU Session Result Cause is not set");
    }

    /* T3512 */
    if((msgStruct->presenceMask & REGISTRATION_ACCEPT_T3512_PRESENT) ==
                                REGISTRATION_ACCEPT_T3512_PRESENT) {

        buffer[len] = E_REG_ACCEPT_IEI_T3512;
        len++;
        nasLog("Timer 3512 : iei 0x%x | current len %d | encoded 1 byte",
            E_REG_ACCEPT_IEI_T3512, len);

        encLen = 0;
        if(encodet3512(buffer +len, bufLen - len,
            &(msgStruct->timer3512), &encLen) == FAILURE)
        {
                higLog("Encoding timer 3512 Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Timer 3512 encoded | currentLen = %d | encoded %d bytes", len,
                                sizeof(msgStruct->timer3512));
    }else {
        nasLog("T3512 is not set");
    }

 	nasLog("Successfully encoded len = %d",len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;

}
