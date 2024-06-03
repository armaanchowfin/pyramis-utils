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

#ifndef __AUTHENTICATION_RESPONSE
#define __AUTHENTICATION_RESPONSE

#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "common/include/aka.h"
#include "nas/NasDatatypes/AuthRespParam.h"



#define  NAS_AUTH_RESP_OPT_AUTH_RESP_PARAM_PRESENT     (1<<0)
#define  NAS_AUTH_RESP_AUTH_RESP_PARAM_IEI             0x2D



#define NAS_AUTH_RESP_OPT_EAP_MSG_PRESENT              (1<<1)
#define NAS_AUTH_RESP_EAP_MSG_PRESENT_IEI              0x7D



typedef struct AuthenticationResponseMsg {
    

    _5gmmMsgHeader_t                   mmHeader;

    

    uint32_t                           presenceMask;
    AuthRespParameter_t                authRespParam;
} AuthenticationResponseMsg_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int decodeAuthResp(uint8_t *buffer, AuthenticationResponseMsg_t *msgStruct_p,
		   uint32_t bufLen);
int encodeAuthResp(uint8_t *buffer, uint32_t bufLen,
		  AuthenticationResponseMsg_t *msgStruct, uint32_t *encodedLen);

#endif
