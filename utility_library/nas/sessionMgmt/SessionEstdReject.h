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

#ifndef __SESSION_ESTABLISHMENT_REJECT_H
#define __SESSION_ESTABLISHMENT_REJECT_H

#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/nasUtil/nasCommon.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/NasDatatypes/_5gsmCause.h"

typedef struct SessionEstdReject {
    

    _5gsmMsgHeader_t    smHeader;
    _5gsmCause_t        _5gsmCause;

    

    uint32_t            presenceMask;
}SessionEstdReject_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeSessionEstdReject(uint8_t *buffer, uint32_t bufLen,
		SessionEstdReject_t *msgStruct,uint32_t* encodedLen);
int decodeSessionEstdReject(uint8_t *buffer,SessionEstdReject_t *msgStruct,
		 uint32_t bufLen);

#endif
