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
#include "nas/mobility/RegistrationReject.h"
#include "common/include/setup.h"
#include "platform/include/platform.h"
#include "nas/nasUtil/nasLogging.h"


int decodeRegRej(uint8_t *buffer,RegistrationReject_t *msgStruct,
                 uint32_t decodLen)
{
    unsigned int len = 0;
    nasLogENTRY;
    if(buffer == NULL || msgStruct == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /*1 5GMMCause*/
    if (len + sizeof(msgStruct->_5gmmCause) > decodLen) {
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

    nasLog("Successfully decoded, len = %d",len);
    nasLogEXIT;
    return SUCCESS;
}



int encodeRegRej(uint8_t *buffer, unsigned int bufLen,
		RegistrationReject_t *msgStruct, uint32_t* encodedLen)
{
    nasLogENTRY;

    if(buffer == NULL || bufLen == 0 || msgStruct == NULL || encodedLen == NULL ) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    unsigned int len = 0;

    /*1 5GMMCause*/
    if (len + sizeof(msgStruct->_5gmmCause) > bufLen) {
		higLog("%s", "FAILURE, buffer Insufficient");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		return FAILURE;
	}
    memcpy(buffer + len,&(msgStruct->_5gmmCause),sizeof(msgStruct->_5gmmCause));
    len += sizeof(msgStruct->_5gmmCause);
    nasLog("encoded 5GMM cause %d | current len %d | sizeof %d",
            msgStruct->_5gmmCause, len,
            sizeof(msgStruct->_5gmmCause));

    nasLog("Successfully encoded len = %d",len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
