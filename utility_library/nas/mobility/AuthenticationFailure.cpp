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
#include "nas/mobility/AuthenticationFailure.h"

/* elements are filled into user allocated nasMessage in the function.
 * returns SUCCESS/FAILURE
 */
int decodeAuthFail(uint8_t *buffer, AuthenticationFailureMsg_t *msgStruct,
		   unsigned int bufLen)
{
	nasLogENTRY;
	unsigned int len = 0;
    uint32_t consumedLen = 0;

	if(buffer == NULL || msgStruct == NULL) {
		higLog("Invalid Input parameters. Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    /*1 5GMMCause*/
    if (len + sizeof(msgStruct->_5gmmCause) > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }   

    memcpy(&(msgStruct->_5gmmCause),buffer + len,sizeof(msgStruct->_5gmmCause));
    len += sizeof(msgStruct->_5gmmCause);
    nasLog("decoded 5GMM cause %d | current len %d | sizeof %d",
            msgStruct->_5gmmCause, len,
            sizeof(msgStruct->_5gmmCause));

    /* Optional fields Start */
    msgStruct->presenceMask = 0;

    while (len < bufLen) {
        consumedLen = 0;
        uint8_t iei = buffer[len];
        len++;
        nasLog("iei 0x%x  | currentLen %d | sizeof 1", iei, len);
        switch (iei) {

        case NAS_AUTH_FAIL_AUTH_FAIL_PARAM_IEI:
            if(decodeAuthFailParam(buffer +len, &msgStruct->authFailParam,
                                   bufLen - len, &consumedLen) == FAILURE){
                higLog("decoding Auth Fail Param failed");
                nasLogEXIT;
                return FAILURE;
            }
            len += consumedLen;
            msgStruct->presenceMask |= NAS_AUTH_FAIL_OPT_AUTH_FAIL_PARAM_PRESENT;
            nasLog("Auth Fail Param decoded | currentLen %d | sizeof %d", len,
                    consumedLen);
        break;

        default:
            higLog("ERROR: Unknown IEI 0x%x", iei);
            errno = E_NAS_ERROR_INVALID_PDU;
            nasLogEXIT;
            return FAILURE;
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

/* returns SUCCESS/FAILURE AND Number of bytes encoded is set in encodedLen.
 */
int encodeAuthFail(uint8_t *buffer, uint32_t bufLen,
		   AuthenticationFailureMsg_t *msgStruct, uint32_t *encodedLen)
{
	nasLogENTRY;
	unsigned int len = 0 /* copyLen = 0 */;
    uint32_t encLen =0;

	if(buffer == NULL || msgStruct == NULL || encodedLen == NULL) {
		higLog("Invalid Input parameters. Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    /*1 5GMMCause*/
    if (len + sizeof(msgStruct->_5gmmCause) > bufLen) {
        higLog("%s", "FAILURE, Insufficient buffer");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }   
    memcpy(buffer + len,&(msgStruct->_5gmmCause),sizeof(msgStruct->_5gmmCause));
    len += sizeof(msgStruct->_5gmmCause);
    nasLog("encoded 5GMM cause %d | current len %d | sizeof %d",
            msgStruct->_5gmmCause, len,
            sizeof(msgStruct->_5gmmCause));

    /* Optional Params */

    /* 1 Auth Fail Parameter, 16 octets */
	/*  This IE shall be included in a 5G AKA based primary authentication and 
		key agreement procedure if and only if the 5GMM cause was #21
		"synch failure". It shall include the response to the authentication
		challenge from the USIM, which is made up of the AUTS parameter (see
		3GPP TS 33.501 [24]). */
	/* _5GMM_CAUSE_IEI_SYNCH_FAILURE not 21 even in spec. It is 0b00010101 */

    if(msgStruct->presenceMask & NAS_AUTH_FAIL_OPT_AUTH_FAIL_PARAM_PRESENT) {
		if (msgStruct->_5gmmCause != _5GMM_CAUSE_IEI_SYNCH_FAILURE) {
			higLog("Expected_5GMM_CAUSE_IEI_SYNCH_FAILURE has not been input");
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
			nasLogEXIT;
			return FAILURE;
		}
        buffer[len] = NAS_AUTH_FAIL_AUTH_FAIL_PARAM_IEI;
        len++;
        nasLog("Auth Fail Param IEI 0x%x encoded | currentLen %d | sizeof 1",
                NAS_AUTH_FAIL_AUTH_FAIL_PARAM_IEI, len);
        if(encodeAuthFailParam(buffer +len, bufLen - len,
                    &msgStruct->authFailParam, &encLen) == FAILURE) {
            higLog("encoding Auth Fail Param Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("Auth Fail Param encoded | currentLen %d | sizeof %d",
                len, encLen);
    } else
        nasLog("Auth Fail Param Not Set");


	*encodedLen = len;
	nasLog("Encoded %d bytes", len);
	nasLogEXIT;

	return SUCCESS;
}
