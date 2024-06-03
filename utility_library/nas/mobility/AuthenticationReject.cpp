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
#include "nas/mobility/AuthenticationReject.h"
#include "common/include/setup.h"
#include <stdlib.h>
#include "platform/include/platform.h"
#include "nas/nasUtil/nasLogging.h"

/* TODO LATER: Use a fixed mem pool into which decoded data can be written.
 */

/* elements are filled into user allocated nasMessage in the function.
 * returns SUCCESS/FAILURE
 * TODO: remove commented logs added for debugging
 */
int decodeAuthRej(uint8_t *buffer, AuthenticationRejectMsg_t *msgStruct,
		  uint32_t decodLen)
{
    nasLogENTRY;
	unsigned int len = 0;

	if(buffer == NULL || msgStruct == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    /*TODO: empty for now, add Auth Rej elements*/

	/*TODO: Must Equality (len == decodLen) be enforced ?*/
	nasLog("Successfully decoded %d bytes", len);
    nasLogEXIT;
	return SUCCESS;
}

/* An empty buffer and a filled AuthRej should be passed.
 * returns SUCCESS/FAILURE AND Number of bytes encoded is set in encodedLen.
 */
int encodeAuthRej(uint8_t *buffer, uint32_t bufLen,
		  AuthenticationRejectMsg_t *msgStruct, uint32_t *encodedLen)
{
    nasLogENTRY;
	unsigned int len = 0;

	if( buffer == NULL || bufLen == 0 || msgStruct == NULL ||
			encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    /* TODO: empty for now, add Auth Rej elements */

	*encodedLen = len;
	nasLog("Encoded %d bytes", len);
    nasLogEXIT;
	return SUCCESS;
}
