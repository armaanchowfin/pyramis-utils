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

#ifndef __REGISTRATION_ACCEPT
#define __REGISTRATION_ACCEPT
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/5GSRegistrationResult.h"
#include "nas/NasDatatypes/5GMobileID.h"
#include "nas/NasDatatypes/t3512.h"
#include "nas/NasDatatypes/rejectedNssai.h"
#include "nas/NasDatatypes/PDUSessionStatus.h"
#include "nas/NasDatatypes/PDUSessionResult.h"
#include "nas/NasDatatypes/PDUSessionResultCause.h"
#include "nas/NasDatatypes/TAIList.h"

/* Bit mask for 5G GUTI optional field presence in 
Registration Accept Msg */
#define REGISTRATION_ACCEPT_5G_GUTI_PRESENT                     (1<<0)
#define REGISTRATION_ACCEPT_T3512_PRESENT                       (1<<1)
#define REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT               (1<<2)
#define REGISTRATION_ACCEPT_REJ_NSSAI_PRESENT                   (1<<3)
#define REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT          (1<<4)
#define REGISTRATION_ACCEPT_PDU_SESSION_RESULT_PRESENT          (1<<5)
#define REGISTRATION_ACCEPT_PDU_SESSION_RESULT_CAUSE_PRESENT    (1<<6)
#define REGISTRATION_ACCEPT_TAI_LIST_PRESENT                    (1<<7)


typedef enum registrationAcceptIEI {
    E_REG_ACCEPT_IEI_5G_GUTI                          = 0x77,
    E_REG_ACCEPT_IEI_T3512                            = 0x5E,
    E_REG_ACCEPT_IEI_ALLOWED_NSSAI                    = 0x15,
    E_REG_ACCEPT_IEI_REJ_NSSAI                        = 0x11,
    E_REG_ACCEPT_IEI_PDU_SESSION_STATUS               = 0x50,
    E_REG_ACCEPT_IEI_PDU_SESSION_RESULT               = 0x26,
    E_REG_ACCEPT_IEI_PDU_SESSION_RESULT_CAUSE         = 0x72,
    E_REG_ACCEPT_IEI_TAI_LIST                         = 0x54
}registrationAcceptIEI_t; 

typedef struct RegistrationAcceptMsg {
    

    _5gmmMsgHeader_t                        mmHeader;
    _5gRegistrationResult_t					_5gregResult;

    

    uint32_t         						presenceMask;
    _5gMobileId_t							_5gmobileId;

    t3512_t							        timer3512;
    nssai_t                                 allowedNssai;
    rejectedNssai_t                         rejectedNssai;
    pduSessionStatus_t                      pduSessionStatus;
    pduSessionResult_t                      pduSessionResult;
    pduSessionResultCause_t                 pduSessionResultCause;
    tAIList_t                               taiList;

}RegistrationAcceptMsg_t;


int decodeRegAccept(uint8_t *buffer, RegistrationAcceptMsg_t *msgStruct,
		  uint32_t bufLen);
int encodeRegAccept(uint8_t *buffer, uint32_t bufLen,
		  RegistrationAcceptMsg_t *msgStruct, uint32_t *encodedLen);

#endif
