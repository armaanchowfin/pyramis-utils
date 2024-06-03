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



#ifndef __5GMOBILITYMGMT_H
#define __5GMOBILITYMGMT_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "common/include/setup.h"
#include "platform/include/platform.h"
#include "platform/include/logging.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/mobility/_5gmmMsgHeader.h"


#include "nas/mobility/RegistrationRequest.h"
#include "nas/mobility/ServiceRequest.h"
#include "nas/mobility/ServiceAccept.h"
#include "nas/mobility/ServiceReject.h"
#include "nas/mobility/AuthenticationRequest.h"
#include "nas/mobility/AuthenticationResponse.h"
#include "nas/mobility/AuthenticationFailure.h"
#include "nas/mobility/SecurityModeCommand.h"
#include "nas/mobility/RegistrationAccept.h"
#include "nas/mobility/RegistrationComplete.h"
#include "nas/mobility/RegistrationReject.h"
#include "nas/mobility/AuthenticationReject.h"
#include "nas/mobility/SecurityModeComplete.h"
#include "nas/mobility/DeregistrationRequest.h"
#include "nas/mobility/DeregistrationAccept.h"
#include "nas/mobility/SecurityModeReject.h"
#include "nas/mobility/UplinkNasTransport.h"
#include "nas/mobility/DownlinkNasTransport.h"
#include "nas/mobility/_5gmmStatus.h"

typedef union {
    _5gmmMsgHeader_t                mmheader;
    RegistrationRequest_t           regReqMsg;
    ServiceRequest_t                servReqMsg;
    ServiceAccept_t                 servAccMsg;
    ServiceReject_t                 servRejMsg;
    AuthenticationRequestMsg_t      authReqMsg;
    AuthenticationResponseMsg_t     authRespMsg;
    AuthenticationFailureMsg_t      authFailMsg;
    RegistrationAcceptMsg_t	    regAcceptMsg;
    SecurityModeCommand_t           secModeCmdMsg;
    SecurityModeComplete_t          secModeCompMsg;
    SecurityModeReject_t            secModeRejMsg;
    RegistrationCompleteMsg_t       regComplMsg;
    RegistrationReject_t            regReject;
    AuthenticationRejectMsg_t       authRej;
    DeregistrationRequest_t         deregReqMsg;
    DeregistrationAccept_t	        deregAcceptMsg;
    UplinkNasTransport_t            uplinkNasTransport;
    DownlinkNasTransport_t          downlinkNasTransport;
    _5gmmStatus_t				    _5gmmStatus;
}_5gMMMsg_t;


int _5gmmMsgEncode(uint8_t *buffer,uint32_t bufLen,_5gMMMsg_t *msgStruct,uint32_t *encodedLen);
int _5gmmMsgDecode(uint8_t *buffer,_5gMMMsg_t *msgStruct,uint32_t decodedLen);

#endif
