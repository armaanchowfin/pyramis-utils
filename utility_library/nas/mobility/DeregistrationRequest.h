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

#ifndef __DEREGISTRATION_REQUEST_H
#define __DEREGISTRATION_REQUEST_H
#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/mobility/_5gmmMsgHeader.h"
#include "nas/NasDatatypes/5GSDeregistrationType.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/5GMobileID.h"



typedef struct DeregistrationRequest {
    

    _5gmmMsgHeader_t                        mmHeader;
    _5gDeregistrationType_t                 _5gderegType;
    NaskeysetId_t                           ngKsi;
    _5gMobileId_t                           _5gmobileId; 
    


}DeregistrationRequest_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeDeregReq(uint8_t *buffer, uint32_t bufLen,
		DeregistrationRequest_t *msgStruct,uint32_t* encodedLen);
int decodeDeregReq(uint8_t *buffer,DeregistrationRequest_t *msgStruct_p,
		 uint32_t decodLen);

#endif
