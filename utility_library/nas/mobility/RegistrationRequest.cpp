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
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/RegistrationRequest.h"

/* elements are filled into user allocated nasMessage in the function.
 * returns SUCCESS/FAILURE
 */
int decodeRegReq(uint8_t *buffer,RegistrationRequest_t *msgStruct,
                 uint32_t bufLen)
{ 
    nasLogENTRY;
    unsigned int len = 0;
    uint32_t decodLen = 0;
    
    if(buffer == NULL || msgStruct == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /*1 copy _5gRegistrationType_t */
    if(decode5GSRegType(buffer + len, bufLen - len,
                    &msgStruct->_5gregType, &decodLen) == FAILURE) 
    {
            higLog("Decoding Registration Type Failed");
            nasLogEXIT;
            return FAILURE;
    }

    /* Since a single octet is shared between Reg Type and ngKSI
     * below line 'len += decodLen' has been commented deliberately */
    //len += decodLen;
    decodLen = 0;
    if(decodeNasKeySetId(buffer + len, &(msgStruct->ngKsi), 
                bufLen - len, &decodLen, true) == FAILURE) 
    {
            higLog("Decoding NAS Key Set ID Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += decodLen;
    nasLog("5gregType decoded | regType %d | FOR %d",
        msgStruct->_5gregType.value, msgStruct->_5gregType.FOR);
    nasLog("ngKsi decoded : tsc %d ngksi %d | currentLen = %d | decoded %d bytes",
    msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi, len, sizeof(msgStruct->ngKsi));

    /*3 copy _5gmobileId, 18 octets. TBD actual length of the 5GMobile ID*/
    uint16_t _5gMobileIdLenN = *(uint16_t*)(buffer + len);
    uint16_t _5gMobileIdLen = ntohs(_5gMobileIdLenN);
    if (len + _5gMobileIdLen > bufLen) {
		higLog("FAILURE, Incomplete buffer. len %u. bufLen %u", (len + _5gMobileIdLen), bufLen);
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
		return FAILURE;
	}

    if( !(_5gMobileIdLen >= REG_REQ_MIN_5G_MOBILE_ID_LEN)) {
        higLog("Mobile ID length incorrect. _5gMobileIdLen %u", _5gMobileIdLen);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(&(msgStruct->_5gmobileId.len), buffer + len, 
                                    sizeof(msgStruct->_5gmobileId.len));
    len += sizeof(msgStruct->_5gmobileId.len);
    nasLog("SUCI : len %d | current len %d | decoded %d bytes", 
         _5gMobileIdLen, len, sizeof(msgStruct->_5gmobileId.len));

    uint8_t identityType = buffer[len] & (0x07);
    if(identityType == E_5G_GUTI){
        
        decodLen = 0;
        /* For now GUTI mandatory and optional length is fixed as it does not
         * have any variable length fields. So hard check on it's length is
         * fine */
        if(_5gMobileIdLen != GUTI_LENGTH){
            higLog("%s", "Mobile ID length incorrect");
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
            nasLogEXIT;
		    return FAILURE;
        }

        if(decode5GGuti(buffer +len, bufLen - len,
            &(msgStruct->_5gmobileId.guti5gMobileId), &decodLen) == FAILURE){
            higLog("Decoding Guti Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += decodLen;
        nasLog("Guti decoded | currentLen = %d | decoded %d bytes", len,
                            decodLen);
    }
    else if(identityType == E_SUCI){
        
        decodLen = 0;
        /* schemeOutput field in SUCI is of variable length.
         * So hard check on SUCI length is not possible.
         * Since we are considering only SUPI format IMSI
         * and Null Scheme we can say it should have minimum 8 octects */
        if((_5gMobileIdLen > SUCI_LENGTH) || (_5gMobileIdLen < SUCI_LENGTH_MIN)){
            higLog("%s", "Mobile ID length incorrect");
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
            nasLogEXIT;
		    return FAILURE;
        }

        if(decodeSuci(buffer +len, bufLen - len,
            &(msgStruct->_5gmobileId.suci5gMobileId), &decodLen) == FAILURE) 
        {
                higLog("Decoding Suci Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += decodLen;
        nasLog("Suci decoded | currentLen = %d | decoded %d bytes", len,
                                decodLen);
    }
    /* identityType = NO_IDENTITY - Unsupported */
    /*end of Mandatory field decoding*/

    msgStruct->presenceMask = 0;
    while(len < bufLen) {
        uint8_t iei = *(buffer + len);
        len++;
        switch(iei) {

        case E_REG_REQUEST_IEI_5GMM_CAPABILITY : {

            /* Decode 5GMMCapabilityIE */
            decodLen = 0;
            if(decode5GMMCap(buffer +len, bufLen - len,
                &(msgStruct->_5gmmCapability), &decodLen) == FAILURE) 
            {
                    higLog("Decoding 5GMM Capability Failed");
                    nasLogEXIT;
                    return FAILURE;
            }
            len += decodLen;
            msgStruct->presenceMask |= REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT;
            nasLog("5GMM Capability decoded | currentLen = %d | decoded %d bytes",
                    len, sizeof(msgStruct->_5gmmCapability));
        }
        break;

        case E_REG_REQUEST_IEI_UE_SEC_CAPABILITY : {

            /* Decode ueSecuCapabilityIE */
            decodLen = 0;
            if(decodeUESecCap(buffer +len, bufLen - len,
                &(msgStruct->ueSecuCapability), &decodLen) == FAILURE) 
            {
                    higLog("Decoding UE Security Capability Failed");
                    nasLogEXIT;
                    return FAILURE;
            }
            len += decodLen;
            msgStruct->presenceMask |= REGISTRATION_REQUEST_UE_SEC_CAPABILITY_PRESENT;
            nasLog("UE Security Capability decoded | currentLen = %d | decoded %d bytes",
                    len, sizeof(msgStruct->ueSecuCapability));
        }
        break;

        case E_REG_REQUEST_IEI_REQ_NSSAI : {

            /* Decode NSSAI IE */
            decodLen = 0;
            if(decodeNssai(buffer +len, bufLen - len,
                &(msgStruct->requestedNssai), &decodLen) == FAILURE)
            {
                    higLog("Decoding NSSAI Failed");
                    nasLogEXIT;
                    return FAILURE;
            }
            len += decodLen;
            msgStruct->presenceMask |= REGISTRATION_REQUEST_REQNSSAI_PRESENT;
            nasLog("NSSAI decoded | currentLen = %d | decoded %d bytes",
                    len, decodLen);
        }
        break;

        case E_REG_REQUEST_IEI_UPLINK_DATA_STATUS: {

            /* Decode Uplink Data Status */
            decodLen = 0;
            if(decodeUplinkDataStatus(buffer + len, bufLen - len,
                &(msgStruct->uplinkDataStatus), &decodLen) == FAILURE) {
                    higLog("Decoding Uplink Data Status Failed");
                    nasLogEXIT;
                    return FAILURE;
            }
            len += decodLen;
            msgStruct->presenceMask |= REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT;
            nasLog("Uplink Data Status decoded | currentLen = %d | decoded %d bytes",
                    len, sizeof(msgStruct->uplinkDataStatus));
        }
        break;

        case E_REG_REQUEST_IEI_PDU_SESSION_STATUS: {

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
                REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT;
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
    if( len > bufLen) {
        higLog("ERROR: crossed buffer boundaries");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    nasLog("Successfully decoded, len = %d , expected : %d",len,decodLen);
    nasLogEXIT;
    return SUCCESS;
}

int encodeRegReq(uint8_t *buffer, uint32_t bufLen,
                 RegistrationRequest_t *msgStruct,uint32_t* encodedLen)
{
    nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL ) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    unsigned int len = 0;
    uint32_t encLen =0;
     
    /*4. copy _5gRegistrationType_t, a _5gRegistrationType_t typedef */
    if(encode5GSRegType(buffer +len, bufLen - len,
                    &msgStruct->_5gregType, &encLen) == FAILURE) 
    {
            higLog("Encoding Registration Type Failed");
            nasLogEXIT;
            return FAILURE;
    }

    /* Since a single octet is shared between Reg Type and ngKSI
     * below line 'len += encLen' has been commented deliberately */
    //len += encLen;
    encLen = 0;
    if(encodeNasKeySetId(buffer +len, bufLen - len,
                    &msgStruct->ngKsi, &encLen, true) == FAILURE) 
    {
            higLog("Encoding NAS Key Set ID Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += encLen;

    nasLog("5regType encoded | regType %d | FOR %d",
        msgStruct->_5gregType.value, msgStruct->_5gregType.FOR);

    nasLog("ngKsi encoded: tsc %d ngksi %d | currentLen = %d | encoded %d bytes"
    ,msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi,len, sizeof(msgStruct->ngKsi));

    uint16_t _5gMobileIdLen = msgStruct->_5gmobileId.len;
    if (len + _5gMobileIdLen > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }
   
    uint16_t _5gMobileIdLenN = htons(_5gMobileIdLen);
    memcpy(buffer + len,&_5gMobileIdLenN,
                                sizeof(msgStruct->_5gmobileId.len));
    len += sizeof(msgStruct->_5gmobileId.len);
    nasLog("5g Mobile ID : len %d | current len %d | sizeof %d", 
        _5gMobileIdLen, len, sizeof(msgStruct->_5gmobileId.len));

    /* suci5gMobileId and guti5gMobileId structures are union
     * in 5gmobileId structure. So can get identityType from any one of them */
    if((msgStruct->_5gmobileId.suci5gMobileId.identityType) == E_SUCI){
        encLen = 0;
        if(encodeSuci(buffer +len, bufLen - len,
            &(msgStruct->_5gmobileId.suci5gMobileId), &encLen) == FAILURE) 
        {
                higLog("Encoding Suci Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Suci encoded | currentLen = %d | encoded %d bytes", len,encLen);
    }
    else if((msgStruct->_5gmobileId.suci5gMobileId.identityType) == E_5G_GUTI){
        encLen = 0;
        if(encode5GGuti(buffer +len, bufLen - len,
            &(msgStruct->_5gmobileId.guti5gMobileId), &encLen) == FAILURE) 
        {
                higLog("Encoding Guti Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Guti encoded | currentLen = %d | encoded %d bytes", len,encLen);
    }
    /* identityType = NO_IDENTITY - Unsupported */
    /*end of Mandatory field encoding*/

    /*7. copy uplinkDataStatus, a struct */
    if((msgStruct->presenceMask &
        REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT)
        == REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT) {

        buffer[len] = E_REG_REQUEST_IEI_UPLINK_DATA_STATUS;
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

    /*7. copy _5gmmCapability, a struct */
    if((msgStruct->presenceMask & REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT) 
            == REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT) {

        buffer[len] = E_REG_REQUEST_IEI_5GMM_CAPABILITY;
        len++;

        encLen = 0;
        if(encode5GMMCap(buffer +len, bufLen - len,
            &(msgStruct->_5gmmCapability), &encLen) == FAILURE) 
        {
                higLog("Encoding 5GMM Capability Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("5GMM Capability encoded | currentLen = %d | encoded %d bytes", 
            len, encLen);

    } else
        nasLog("5GMM Capability is not set");

    /*8. copy UeSecurityCapability_t, a struct */
    if((msgStruct->presenceMask & REGISTRATION_REQUEST_UE_SEC_CAPABILITY_PRESENT) 
            == REGISTRATION_REQUEST_UE_SEC_CAPABILITY_PRESENT) {

        buffer[len] = E_REG_REQUEST_IEI_UE_SEC_CAPABILITY;
        len++;

        encLen = 0;
        if(encodeUESecCap(buffer +len, bufLen - len,
            &(msgStruct->ueSecuCapability), &encLen) == FAILURE) 
        {
                higLog("Encoding UE Security Capability Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("UE Security Capability encoded | currentLen = %d | encoded %d bytes"
            , len, encLen);

    }else
        nasLog("UE Security Capability is not set");

    /*9. copy NSSAI structure */
    if((msgStruct->presenceMask & REGISTRATION_REQUEST_REQNSSAI_PRESENT)
            == REGISTRATION_REQUEST_REQNSSAI_PRESENT) {

        buffer[len] = E_REG_REQUEST_IEI_REQ_NSSAI;
        len++;

        encLen = 0;
        if(encodeNssai(buffer +len, bufLen - len,
            &(msgStruct->requestedNssai), &encLen) == FAILURE)
        {
                higLog("Encoding NSSAI Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Requested NSSAI encoded | currentLen = %d | encoded %d bytes",
                                    len, encLen);

    }else
        nasLog("Requested NSSAI is not set");

    /*8. copy pduSessionStatus, a struct */
    if((msgStruct->presenceMask &
        REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT) 
        == REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT) {

        buffer[len] = E_REG_REQUEST_IEI_PDU_SESSION_STATUS;
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

    }else {
        nasLog("PDU Session Status is not set");
    }

    nasLog("Successfully encoded len = %d",len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
