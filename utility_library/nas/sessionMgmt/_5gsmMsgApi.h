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



#ifndef __5GSMMSGAPI_H
#define __5GSMMSGAPI_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "common/include/setup.h"
#include "platform/include/platform.h"
#include "platform/include/logging.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"



#include "nas/sessionMgmt/SessionEstdRequest.h"
#include "nas/sessionMgmt/SessionEstdReject.h"
#include "nas/sessionMgmt/SessionRelReq.h"
#include "nas/sessionMgmt/SessionRelCommand.h"
#include "nas/sessionMgmt/SessionRelComplete.h"
#include "nas/sessionMgmt/SessionEstdAccept.h"
#include "nas/sessionMgmt/PduSessionModificationRequest.h"
#include "nas/sessionMgmt/PduSessionModificationReject.h"
#include "nas/sessionMgmt/PduSessionModificationCommand.h"
#include "nas/sessionMgmt/PduSessionModificationComplete.h"


typedef union {
    _5gsmMsgHeader_t                    smheader;
    SessionEstdRequest_t                SessionEstdRequest;
    SessionEstdReject_t                 SessionEstdReject;
    SessionRelReq_t                     SessionRelReq;
    SessionRelCommand_t                 sessionRelCommand;
    PduSessionModificationCommand_t     sessionModCommand;
    PduSessionModificationComplete_t    sessionModComplete;
    SessionRelComplete_t                sessionRelComplete;
    SessionEstdAccept_t                 SessionEstdAccept;
    PduSessionModificationRequest_t     SessionModRequest;
    PduSessionModificationReject_t      SessionModReject;
}_5GSMMsg_t;

int _5gsmMsgEncode(uint8_t *buffer,uint32_t bufLen,_5GSMMsg_t *msgStruct,uint32_t *encodedLen);
int _5gsmMsgDecode(uint8_t *buffer,_5GSMMsg_t *msgStruct,uint32_t decodedLen);

#endif
