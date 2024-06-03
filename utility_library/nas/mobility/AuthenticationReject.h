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

#ifndef __AUTHENTICATION_REJECT
#define __AUTHENTICATION_REJECT

#include <bits/stdc++.h>
#include "nas/mobility/_5gmmMsgHeader.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

typedef struct AuthenticationRejectMsg {
    

    _5gmmMsgHeader_t    mmHeader;

    

}AuthenticationRejectMsg_t;

int decodeAuthRej(uint8_t *buffer, AuthenticationRejectMsg_t *msgStruct,
                  uint32_t decodLen);
int encodeAuthRej(uint8_t *buffer, uint32_t bufLen,
                  AuthenticationRejectMsg_t *msgStruct, uint32_t *encodedLen);

#endif
