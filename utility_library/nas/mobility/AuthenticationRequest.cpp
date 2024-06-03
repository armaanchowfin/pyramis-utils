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
#include "nas/mobility/AuthenticationRequest.h"

/* TODO LATER: Use a fixed mem pool into which decoded data can be written.
 */

/* elements are filled into user allocated nasMessage in the function.
 * returns SUCCESS/FAILURE
 * TODO: remove commented logs added for debugging
 */
int decodeAuthReq(uint8_t *buffer, AuthenticationRequestMsg_t *msgStruct,
                  uint32_t decodLen)
{
	nasLogENTRY;
    uint32_t consumedLen = 0;
	uint32_t len = 0;

	if(buffer == NULL || msgStruct == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    /*1. NaskeysetId */
    if(decodeNasKeySetId(buffer + len, &msgStruct->ngKsi, decodLen - len,
                      &consumedLen) == FAILURE) {
        higLog("decoding Nas Key Set Id failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += consumedLen;
    nasLog("ngKsi tsc: %d | ngksi: %d | currentLen %d | sizeof : %d",
            msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi, len, consumedLen);

    /*2. ABBA */
    if(decodeABBA(buffer + len, &msgStruct->abba, decodLen - len,
                      &consumedLen) == FAILURE) {
        higLog("decoding ABBA failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += consumedLen;
    nasLog("ABBA contents: %d | currentLen %d | sizeof : %d",
            msgStruct->abba.contents, len, consumedLen);

    /* Optional fields Start */
    msgStruct->presenceMask = 0;

    while (len < decodLen) {
        consumedLen = 0;
        uint8_t iei = buffer[len];
        len++;
        nasLog("iei 0x%02x  | currentLen %d | sizeof 1", iei, len);
        switch (iei) {
        case NAS_AUTH_REQUEST_RAND_IEI:
            if(decodeAuthParamRAND(buffer + len, &msgStruct->rand,
                                   decodLen - len, &consumedLen) ==FAILURE){
                higLog("decoding RAND failed");
                nasLogEXIT;
                return FAILURE;
            }
            len += consumedLen;
            msgStruct->presenceMask |= NAS_AUTH_REQUEST_OPT_RAND_PRESENT;
            nasLog("RAND decoded | currentLen %d | sizeof %d", len,
                    consumedLen);
        break;

        case NAS_AUTH_REQUEST_AUTN_IEI:
            if(decodeAuthParamAUTN(buffer + len, &msgStruct->autn,
                                   decodLen - len, &consumedLen) ==FAILURE){
                higLog("decoding AUTN failed");
                nasLogEXIT;
                return FAILURE;
            }
            len += consumedLen;
            msgStruct->presenceMask |= NAS_AUTH_REQUEST_OPT_AUTN_PRESENT;
            nasLog("AUTN decoded | currentLen %d | sizeof %d", len,
                    consumedLen);
        break;

        default: {
            higLog("Unknown Optional IEI %d", buffer[len]);
            nasLogEXIT;
            return FAILURE;
        }
        }
    }

    if( len > decodLen) {
        higLog("ERROR: crossed buffer boundaries");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

	nasLog("Successfully decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;
}

/* returns SUCCESS/FAILURE AND Number of bytes encoded is set in encodedLen.
 */
int encodeAuthReq(uint8_t *buffer, uint32_t bufLen,
		  AuthenticationRequestMsg_t *msgStruct, uint32_t *encodedLen)
{
	nasLogENTRY;
	unsigned int len = 0;
    uint32_t encLen = 0;

	if( buffer == NULL || bufLen == 0 || msgStruct == NULL ||
			encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    /*1. NaskeysetId_t */
    if( encodeNasKeySetId(buffer + len, bufLen - len, &msgStruct->ngKsi,
                &encLen) == FAILURE) {
        higLog("encoding NAS Key Set Id failed");
        nasLogEXIT;
        return FAILURE;
    }
	len += encLen;
    nasLog("ngKsi encoded | tsc %d | ngksi %d | currentLen %d | sizeof %d",
            msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi, len, encLen);

    /*2. ABBA */
    if(encodeABBA(buffer + len, bufLen - len, &msgStruct->abba,
                &encLen) == FAILURE) {
        higLog("encoding ABBA failed");
        nasLogEXIT;
        return FAILURE;
    }
	len += encLen;
    nasLog("ABBA encoded | contents %d | currentLen %d | sizeof %d",
            msgStruct->abba.contents, len, encLen);


    nasLog("optional Mask 0x%x", msgStruct->presenceMask);

    /* Optional fields Start */
    /* 2. RAND */
    if(msgStruct->presenceMask & NAS_AUTH_REQUEST_OPT_RAND_PRESENT) {
        buffer[len] = NAS_AUTH_REQUEST_RAND_IEI;
        len++;
        nasLog("RAND IEI 0x%x encoded | currentLen %d | sizeof 1",
                NAS_AUTH_REQUEST_RAND_IEI, len);

        encLen = 0;
        if(encodeAuthParamRAND(buffer+len, bufLen - len, &msgStruct->rand,
                            &encLen) == FAILURE) {
            higLog("encoding RAND failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("RAND encoded | currentLen %d | sizeof %d", len, encLen);
    } else
        nasLog("RAND not set");

    /* 2. AUTN */
    if(msgStruct->presenceMask & NAS_AUTH_REQUEST_OPT_AUTN_PRESENT) {
        buffer[len] = NAS_AUTH_REQUEST_AUTN_IEI;
        len++;
        nasLog("AUTN IEI 0x%x encoded | currentLen %d | sizeof 1",
                NAS_AUTH_REQUEST_AUTN_IEI, len);

        encLen = 0;
        if(encodeAuthParamAUTN(buffer+len, bufLen - len, &msgStruct->autn,
                            &encLen) == FAILURE) {
            higLog("encoding AUTN failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("AUTN encoded | currentLen %d | sizeof %d", len, encLen);
    } else
        nasLog("AUTN not set");

	*encodedLen = len;
	nasLog("Encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;
}
