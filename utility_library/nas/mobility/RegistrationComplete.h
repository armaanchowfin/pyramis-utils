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

#ifndef __REGISTRATION_COMPLETE
#define __REGISTRATION_COMPLETE
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/mobility/_5gmmMsgApi.h"

typedef struct RegistrationCompleteMsg {
    

    _5gmmMsgHeader_t        mmHeader;
    
}RegistrationCompleteMsg_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int decodeRegComplete(uint8_t *buffer, RegistrationCompleteMsg_t *msgStruct,
		  uint32_t decodLen);
int encodeRegComplete(uint8_t *buffer, uint32_t bufLen,
		  RegistrationCompleteMsg_t *msgStruct, uint32_t *encodedLen);

#endif
