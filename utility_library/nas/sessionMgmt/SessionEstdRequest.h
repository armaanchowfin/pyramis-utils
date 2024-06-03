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

#ifndef __SESSION_ESTABLISHMENT_REQUEST_H
#define __SESSION_ESTABLISHMENT_REQUEST_H
#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/NasDatatypes/IntegProtMaxDataRate.h"
#include "nas/NasDatatypes/PDUSessionType.h"
#include "nas/NasDatatypes/SSCMode.h"
#include "nas/NasDatatypes/_5gsmCapability.h"
#include "nas/NasDatatypes/MaxNoSupportedPackFilters.h"

#define MIN_SIZE_OF_PDU_SESSION_ESTB_REQ SIZE_5G_SESSION_MESSAGE_HEADER



#define SESSION_ESTB_REQUEST_PDU_SESSION_TYPE 	(1<<0)

#define SESSION_ESTB_REQUEST_SSC_MODE 			(1<<1)

#define SESSION_ESTB_REQUEST_5GSM_CAPABILITY 	(1<<2)

#define SESSION_ESTB_REQUEST_MAX_NO_SUP_PAC_FLTRS (1<<3)





#define E_SESSION_ESTB_REQ_IEI_PDU_SESSION_TYPE 	0x90
#define E_SESSION_ESTB_REQ_IEI_SSC_MODE 			0xA0
#define E_SESSION_ESTB_REQ_IEI_5GSM_CAPABILITY 		0x28
#define E_SESSION_ESTB_REQ_IEI_MAX_NO_SUP_PAC_FLTRS 0x55

typedef struct SessionEstdRequest {
    _5gsmMsgHeader_t       smHeader;
    integProtMaxDataRate_t ipmdr;
    


    

    uint32_t			presenceMask;
    pduSessionType_t	pduSessionType;	
    sscMode_t			sscMode; 
    _5gsmCapability_t	_5gsmCapability;
    maxNoSupPackFilts_t maxNoSupportedPacketFilters; 
}SessionEstdRequest_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeSessionEstdReq(uint8_t *buffer, uint32_t bufLen,
		SessionEstdRequest_t *msgStruct,uint32_t* encodedLen);
int decodeSessionEstdReq(uint8_t *buffer,SessionEstdRequest_t *msgStruct_p,
		 uint32_t decodLen);

#endif
