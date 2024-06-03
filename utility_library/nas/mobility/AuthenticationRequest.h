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

#ifndef __AUTHENTICATION_REQUEST
#define __AUTHENTICATION_REQUEST
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "common/include/aka.h"
#include "ran/intf/include/ue.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/NasDatatypes/ABBA.h"
#include "nas/NasDatatypes/AuthparamRAND.h"
#include "nas/NasDatatypes/AuthparamAUTN.h"



#define  NAS_AUTH_REQUEST_OPT_RAND_PRESENT        (1 << 0)
#define  NAS_AUTH_REQUEST_RAND_IEI                0x21

#define  NAS_AUTH_REQUEST_OPT_AUTN_PRESENT        (1 << 1)
#define  NAS_AUTH_REQUEST_AUTN_IEI                0x20



#define  NAS_AUTH_REQUEST_OPT_ABBA_PRESENT        (1 << 2)
#define  NAS_AUTH_REQUEST_ABBA_IEI                0x38

#define  NAS_AUTH_REQUEST_OPT_EAP_MSG_PRESENT     (1 << 3)
#define  NAS_AUTH_REQUEST_EAP_MSG_IEI             0x78


typedef struct AuthenticationRequestMsg {
    

    _5gmmMsgHeader_t                        mmHeader;
    NaskeysetId_t                           ngKsi;
    ABBA_t                                  abba;

    

    uint32_t                                presenceMask;
    RAND_t                                  rand;
    AUTN_t                                  autn;
}AuthenticationRequestMsg_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int decodeAuthReq(uint8_t *buffer, AuthenticationRequestMsg_t *msgStruct_p,
		  uint32_t decodLen);
int encodeAuthReq(uint8_t *buffer, uint32_t bufLen,
		  AuthenticationRequestMsg_t *msgStruct, uint32_t *encodedLen);

#endif
