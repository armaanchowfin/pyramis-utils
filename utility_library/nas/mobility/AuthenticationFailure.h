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

#ifndef __AUTHENTICATION_FAILURE
#define __AUTHENTICATION_FAILURE

#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "common/include/aka.h"
#include "nas/NasDatatypes/AuthFailParam.h"



#define  NAS_AUTH_FAIL_OPT_AUTH_FAIL_PARAM_PRESENT     (1<<0)
#define  NAS_AUTH_FAIL_AUTH_FAIL_PARAM_IEI             0x30



typedef struct AuthenticationFailureMsg {
    

    _5gmmMsgHeader_t                   mmHeader;
	uint8_t							   _5gmmCause;

    

    uint32_t                           presenceMask;
    AuthFailParameter_t                authFailParam;
} AuthenticationFailureMsg_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int decodeAuthFail(uint8_t *buffer, AuthenticationFailureMsg_t *msgStruct_p,
		   uint32_t bufLen);
int encodeAuthFail(uint8_t *buffer, uint32_t bufLen,
		  AuthenticationFailureMsg_t *msgStruct, uint32_t *encodedLen);

#endif
