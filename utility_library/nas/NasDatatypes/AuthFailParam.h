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

#ifndef __AUTHENTICATION_FAILURE_PARAMETER_H
#define __AUTHENTICATION_FAILURE_PARAMETER_H

#include <cstdint>
#include <cstdlib>
#include "common/include/aka.h"
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"

#define AUTH_FAIL_PARAM_LEN         14
#define AUTH_FAIL_PARAM_HEADER_LEN  2 /* afpIEI & afpLen 2 octets
                                       * afpLen - AFP_HEADER_LEN => AUTS len */



typedef struct AuthFailParameter {
	uint8_t    afpLen;     
 

	uint8_t    AUTS[AUTS_SIZE];
	

}AuthFailParameter_t;

int decodeAuthFailParam(uint8_t *buffer, AuthFailParameter_t *arp,
                        uint32_t bufLen, uint32_t* decodedLen);
int encodeAuthFailParam(uint8_t *buffer, uint32_t buflen,
                        AuthFailParameter *arp, uint32_t *encodedLen);

#endif 

