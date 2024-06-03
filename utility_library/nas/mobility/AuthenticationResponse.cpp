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
#include "nas/mobility/AuthenticationResponse.h"

/* elements are filled into user allocated nasMessage in the function.
 * returns SUCCESS/FAILURE
 */
int decodeAuthResp(uint8_t *buffer, AuthenticationResponseMsg_t *msgStruct,
		   unsigned int bufLen)
{
	nasLogENTRY;
	unsigned int len = 0 /* copyLen = 0 */;
    uint32_t consumedLen = 0;

	if(buffer == NULL || msgStruct == NULL) {
		higLog("Invalid Input parameters. Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    /* Optional fields Start */
    msgStruct->presenceMask = 0;

    while (len < bufLen) {
        consumedLen = 0;
        uint8_t iei = buffer[len];
        len++;
        nasLog("iei 0x%x  | currentLen %d | sizeof 1", iei, len);
        switch (iei) {

        case NAS_AUTH_RESP_AUTH_RESP_PARAM_IEI:
            if(decodeAuthRespParam(buffer +len, &msgStruct->authRespParam,
                                   bufLen - len, &consumedLen) == FAILURE){
                higLog("decoding Auth Resp Param failed");
                nasLogEXIT;
                return FAILURE;
            }
            len += consumedLen;
            msgStruct->presenceMask |=NAS_AUTH_RESP_OPT_AUTH_RESP_PARAM_PRESENT;
            nasLog("Auth Resp Param decoded | currentLen %d | sizeof %d", len,
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
int encodeAuthResp(uint8_t *buffer, uint32_t bufLen,
		   AuthenticationResponseMsg_t *msgStruct, uint32_t *encodedLen)
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

    /* Optional Params */

    /*1 Auth Resp Parameter, 6 to 18 octets*/
    if(msgStruct->presenceMask & NAS_AUTH_RESP_OPT_AUTH_RESP_PARAM_PRESENT) {
        buffer[len] = NAS_AUTH_RESP_AUTH_RESP_PARAM_IEI;
        len++;
        nasLog("Auth Resp Param IEI 0x%x encoded | currentLen %d | sizeof 1",
                NAS_AUTH_RESP_AUTH_RESP_PARAM_IEI, len);
        if(encodeAuthRespParam(buffer +len, bufLen - len,
                    &msgStruct->authRespParam, &encLen) == FAILURE) {
            higLog("encoding Auth Resp Param Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("Auth Resp Param encoded | currentLen %d | sizeof %d",
                len, encLen);
    } else
        nasLog("Auth Resp Param Not Set");


	*encodedLen = len;
	nasLog("Encoded %d bytes", len);
	nasLogEXIT;

	return SUCCESS;
}
