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
#include "nas/mobility/DeregistrationRequest.h"

/* elements are filled into user allocated nasMessage in the function.
 * returns SUCCESS/FAILURE
 */
int decodeDeregReq(uint8_t *buffer,DeregistrationRequest_t *msgStruct,
                 uint32_t bufLen)
{ 
    nasLogENTRY;
    unsigned int len = 0;
    uint32_t decodLen = 0;
    
    if(buffer == NULL || msgStruct == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        return FAILURE;
    }

    /*1 copy _5gDeregistrationType_t, 2 octets*/
    if(decode5GSDeregType(buffer + len, bufLen - len,
                    &msgStruct->_5gderegType, &decodLen) == FAILURE) 
    {
            higLog("Decoding Deregistration Type Failed");
            nasLogEXIT;
            return FAILURE;
    }
    /* Since a single octet is shared between Reg Type and ngKSI
     * below line 'len += decodLen' has been commented deliberately */
    //len += decodLen;
    nasLog("5gderegType decoded | currentLen = %d | decoded %d bytes", len,
                                        sizeof(msgStruct->_5gderegType));

    decodLen = 0;
    if(decodeNasKeySetId(buffer + len, &(msgStruct->ngKsi), 
                bufLen - len, &decodLen, true) == FAILURE)
    {
            higLog("Decoding NAS Key Set ID Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += decodLen;
    nasLog("ngKsi decoded : tsc %d ngksi %d | currentLen = %d | decoded %d bytes"
    ,msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi, len, sizeof(msgStruct->ngKsi));

    /*3 copy _5gmobileId, 18 octets. TBD actual length of the 5GMobile ID*/
    uint16_t _5gMobileIdLenN = *(uint16_t*)(buffer + len);
    uint16_t _5gMobileIdLen = ntohs(_5gMobileIdLenN);
    
    if ((len + _5gMobileIdLen) > bufLen) {
		higLog("%s", "FAILURE, Incomplete buffer");
        nasLogEXIT;
		return FAILURE;
	}
    else if( !(_5gMobileIdLen >= REG_REQ_MIN_5G_MOBILE_ID_LEN)) {
        higLog("%s", "Mobile ID length incorrect");
        nasLogEXIT;
        return FAILURE;
    }
    memcpy(&(msgStruct->_5gmobileId.len), buffer + len, 
                                    sizeof(msgStruct->_5gmobileId.len));
    len += sizeof(msgStruct->_5gmobileId.len);
    nasLog("5G Mobile ID : len %d | current len %d | decoded %d bytes", 
           _5gMobileIdLen, len, sizeof(msgStruct->_5gmobileId.len));

    uint8_t identityType = buffer[len] & (0x07);
    if(identityType == E_5G_GUTI){
        
        decodLen = 0;
        /* For now GUTI mandatory and optional length is fixed as it does not *
         * have any variable length fields. So hard check on it's length is fine */
        if(_5gMobileIdLen != GUTI_LENGTH){
            higLog("%s", "Mobile ID length incorrect");
            nasLogEXIT;
		    return FAILURE;
        }
       
        if(decode5GGuti(buffer +len, bufLen - len,
            &(msgStruct->_5gmobileId.guti5gMobileId), &decodLen) == FAILURE) 
        {
            higLog("Decoding Guti Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += decodLen;
        nasLog("Guti decoded | currentLen = %d | decoded %d bytes", len,
                            sizeof(msgStruct->_5gmobileId.guti5gMobileId));
    }
    else if(identityType == E_SUCI){
        
        decodLen = 0;
        /* schemeOutput field in SUCI is of variable length.
         * So hard check on SUCI length is not possible.
         * Since we are considering only SUPI format IMSI
         * and Null Scheme we can say it should have minimum 8 octects */
        if((_5gMobileIdLen > SUCI_LENGTH) || (_5gMobileIdLen < SUCI_LENGTH_MIN)){
            higLog("%s", "Mobile ID length incorrect");
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
                                sizeof(msgStruct->_5gmobileId.suci5gMobileId));
    }

    /*end of Mandatory field decoding*/

    nasLog("Successfully decoded, len = %d , expected : %d",len,decodLen);
    nasLogEXIT;
    return SUCCESS;
}

int encodeDeregReq(uint8_t *buffer, uint32_t bufLen,
                 DeregistrationRequest_t *msgStruct,uint32_t* encodedLen)
{
    nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL ) {
        higLog("%s", "Invalid Input, Failure encoding");
        nasLogEXIT;
        return FAILURE;
    }

    unsigned int len = 0;
    uint32_t encLen =0;
     
    /*4. copy _5gRegistrationType_t, a _5gRegistrationType_t typedef */
    if(encode5GSDeregType(buffer +len, bufLen - len,
                    &msgStruct->_5gderegType, &encLen) == FAILURE) 
    {
            higLog("Encoding Registration Type Failed");
            nasLogEXIT;
            return FAILURE;
    }
    /* Since a single octet is shared between Reg Type and ngKSI
     * below line 'len += encLen' has been commented deliberately */
    //len += encLen;
    nasLog("5GderegType encoded | currentLen = %d | encoded %d bytes", len,
        sizeof(msgStruct->_5gderegType));

    encLen = 0;
    if(encodeNasKeySetId(buffer +len, bufLen - len,
                    &msgStruct->ngKsi, &encLen, true) == FAILURE)
    {
            higLog("Encoding NAS Key Set ID Failed");
            nasLogEXIT;
            return FAILURE;
    }
    len += encLen;
    nasLog("ngKsi encoded : tsc %d ngksi %d | currentLen = %d | encoded %d bytes"
    ,msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi, len, sizeof(msgStruct->ngKsi));

    uint16_t _5gMobileIdLen = msgStruct->_5gmobileId.len;
    uint16_t _5gMobileIdLenN = htons(_5gMobileIdLen);
    if (len + _5gMobileIdLen > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        nasLogEXIT;
        return FAILURE;
    }
    memcpy(buffer + len, &_5gMobileIdLenN, sizeof(msgStruct->_5gmobileId.len));
    len += sizeof(msgStruct->_5gmobileId.len);
    nasLog("5g Mobile ID : len %d | current len %d | sizeof %d", 
        _5gMobileIdLen, len, sizeof(msgStruct->_5gmobileId.len));

    /* suci5gMobileId and guti5gMobileId structures are union
     * in 5gmobileId structure. So can get identityType from any one of them */
    if((msgStruct->_5gmobileId.suci5gMobileId.identityType) == E_SUCI){
        encLen = 0;
        if(encodeSuci(buffer +len, bufLen - len,
            &(msgStruct->_5gmobileId.suci5gMobileId), &encLen) == FAILURE) {
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
            &(msgStruct->_5gmobileId.guti5gMobileId), &encLen) == FAILURE){
                higLog("Encoding Guti Failed");
                nasLogEXIT;
                return FAILURE;
        }
        len += encLen;
        nasLog("Guti encoded | currentLen = %d | encoded %d bytes", len,encLen);
    }
    /*end of Mandatory field encoding*/

    nasLog("Successfully encoded len = %d",len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
