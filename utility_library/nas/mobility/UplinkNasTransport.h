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

#ifndef __NAS_HEADER_UPLINKNAS_TRANSPORT_H
#define __NAS_HEADER_UPLINKNAS_TRANSPORT_H
#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/NasDatatypes/ULNasPayloadContainer.h"
#include "nas/sessionMgmt/_5gsmMsgApi.h"
#include "nas/NasDatatypes/payloadContainerType.h"
#include "nas/NasDatatypes/DNN.h"
#include "nas/NasDatatypes/nssai.h"
#include "nas/NasDatatypes/PDUSessionId2.h"
#include "nas/NasDatatypes/requestType.h"

#define UPLINK_NAS_IE_PDU_SESSION_ID_PRESENT      (1<<0)
#define UPLINK_NAS_IE_DNN_PRESENT                 (1<<1)
#define UPLINK_NAS_IE_REQUEST_TYPE                (1<<2)
#define UPLINK_NAS_IE_NSSAI_PRESENT               (1<<3)

enum UplinkNasTransportIEI {
    E_UPLINK_NAS_IEI_PDU_SESSION_ID         = 0x12,
    E_UPLINK_NAS_IEI_DNN                    = 0x25,
    E_UPLINK_NAS_IEI_REQUEST_TYPE           = 0x8,      

    E_UPLINK_NAS_IEI_NSSAI                  = 0x22
};

typedef struct UplinkNasTransport {
    

    _5gmmMsgHeader_t               mmHeader;
    uint8_t                        payloadContainerType:4;
    PayloadContainer_t             container;

    

    uint32_t                        presenceMask;
    PDUSessionId2_t                 pduSessionId;
    DNN_t                           dnn;
    requestType_t                   requestType;
    s_nssai_t                       snssai;
}UplinkNasTransport_t;


int decodeUlNAS(uint8_t *buffer, UplinkNasTransport_t *msgStruct_p,
		   uint32_t bufLen);
int encodeUlNAS(uint8_t *buffer, uint32_t bufLen,
		  UplinkNasTransport_t *msgStruct, uint32_t *encodedLen);


#endif
