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

#ifndef __REGISTRATION_REQUEST_H
#define __REGISTRATION_REQUEST_H
#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/mobility/_5gmmMsgHeader.h"
#include "nas/NasDatatypes/5GMobileID.h"
#include "nas/NasDatatypes/UESecCapability.h"
#include "nas/NasDatatypes/5GMMCapability.h"
#include "nas/NasDatatypes/5GSRegistrationType.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/NasDatatypes/nssai.h"
#include "nas/NasDatatypes/PDUSessionStatus.h"
#include "nas/NasDatatypes/UplinkDataStatus.h"

#define REG_REQ_MAX_NSSAI_LENGTH    74



#define REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT (1<<0)
#define REGISTRATION_REQUEST_UE_SEC_CAPABILITY_PRESENT (1<<1)
#define REGISTRATION_REQUEST_REQNSSAI_PRESENT (1<<2)
#define REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT (1<<3)
#define REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT (1<<4)


typedef enum registrationReqIEI {
    E_REG_REQUEST_IEI_5GMM_CAPABILITY         = 0x10,
    E_REG_REQUEST_IEI_UE_SEC_CAPABILITY       = 0x2E,
    E_REG_REQUEST_IEI_REQ_NSSAI               = 0x2F,
    E_REG_REQUEST_IEI_PDU_SESSION_STATUS      = 0x50,
    E_REG_REQUEST_IEI_UPLINK_DATA_STATUS      = 0x40,
}registrationReqIEI_t; 



typedef struct RegistrationRequest {
    

    _5gmmMsgHeader_t                        mmHeader;
    _5gRegistrationType_t                   _5gregType;
    NaskeysetId_t                           ngKsi;
    _5gMobileId_t                           _5gmobileId; 
    


    

    uint32_t                                 presenceMask;
    _5gmmCapability_t                       _5gmmCapability;
    UeSecurityCapability_t                  ueSecuCapability;
    nssai_t                                 requestedNssai;
    pduSessionStatus_t                      pduSessionStatus;
    UplinkDataStatus_t                      uplinkDataStatus;
}RegistrationRequest_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeRegReq(uint8_t *buffer, uint32_t bufLen,
		RegistrationRequest_t *msgStruct,uint32_t* encodedLen);
int decodeRegReq(uint8_t *buffer,RegistrationRequest_t *msgStruct_p,
		 uint32_t decodLen);

#endif
