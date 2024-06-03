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

#ifndef __AUTHENTICATION_RESPONSE_PARAMETER_H
#define __AUTHENTICATION_RESPONSE_PARAMETER_H

#include <cstdint>
#include <cstdlib>
#include "common/include/aka.h"
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"

#define AUTH_RESP_PARAM_MIN_LEN     4
#define AUTH_RESP_PARAM_MAX_LEN     16



typedef struct AuthRespParameter {
	uint8_t    arpLen;     
 

	uint8_t    RESstar[XRES_SIZE];
	

}AuthRespParameter_t;

int decodeAuthRespParam(uint8_t *buffer, AuthRespParameter_t *arp,
                        uint32_t bufLen, uint32_t* decodedLen);
int encodeAuthRespParam(uint8_t *buffer, uint32_t buflen,
                        AuthRespParameter *arp, uint32_t *encodedLen);

#endif 

