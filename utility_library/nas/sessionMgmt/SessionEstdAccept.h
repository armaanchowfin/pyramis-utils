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

#ifndef __SESSION_ESTABLISHMENT_ACCEPT_H
#define __SESSION_ESTABLISHMENT_ACCEPT_H

#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/nasUtil/nasCommon.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/NasDatatypes/QosRules.h"
#include "nas/NasDatatypes/SessionAMBR.h"
#include "nas/NasDatatypes/PDUaddress.h"
#include "nas/NasDatatypes/QosFlowDescriptions.h"

#include "nas/NasDatatypes/PDUSessionType.h"
#include "nas/NasDatatypes/DNN.h"

#define MIN_SIZE_OF_PDU_SESSION_ESTB_ACCEPT 14

#define SESSION_ACCEPT_NAS_IE_DNN_PRESENT                       (1<<0)
#define SESSION_ESTB_ACCEPT_PDU_ADDR_PRESENT                    (1<<1)
#define SESSION_ACCEPT_NAS_IE_5GSM_CAUSE_PRESENT                (1<<2)
#define SESSION_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT (1<<3)

enum SessionAcceptIEI {
    E_SESSION_ACCEPT_NAS_IEI_DNN                        = 0x25,
    E_SESSION_ESTB_ACCEPT_PDU_ADDR_IEI                  = 0x29,
    E_SESSION_ACCEPT_NAS_IEI_5GSM_CAUSE                 = 0x59,
    E_SESSION_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS   = 0x79
};

typedef struct SessionEstdAccept {
    

    _5gsmMsgHeader_t    smHeader;
    pduSessionType_t    selected_pduSessionType;
    sscMode_t           selected_sscMode;
    QosRules_t          authorizedQosRules;
    SessionAMBR_t       ambr;

    

    uint32_t            presenceMask;
    DNN_t               dnn;
    pduAddress_t        pduAddress;
    _5gsmCause_t        _5gsmCause;
    QosFlowDescriptions_t authorizedQoSFlowDesc;
}SessionEstdAccept_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeSessionEstdAccept(uint8_t *buffer, uint32_t bufLen,
		SessionEstdAccept_t *msgStruct,uint32_t* encodedLen);
int decodeSessionEstdAccept(uint8_t *buffer,SessionEstdAccept_t *msgStruct,
		 uint32_t bufLen);

#endif
