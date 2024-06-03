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

#ifndef __REGISTRATION_REJECT_H
#define __REGISTRATION_REJECT_H
#include  <bits/stdc++.h>
#include "nas/mobility/_5gmmMsgHeader.h"
#include "common/include/datatypes.h"

typedef struct registrationReject {
    

    _5gmmMsgHeader_t                        mmHeader;
    uint8_t                                 _5gmmCause;
}RegistrationReject_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeRegRej(uint8_t *buffer, unsigned int bufLen,
		RegistrationReject_t *msgStruct, uint32_t* encodedLen);
int decodeRegRej(uint8_t *buffer,RegistrationReject_t *msgStruct,
                 uint32_t decodLen);

#endif