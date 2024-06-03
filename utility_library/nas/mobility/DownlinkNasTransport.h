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

#ifndef __NAS_HEADER_DOWNLINKNAS_TRANSPORT_H
#define __NAS_HEADER_DOWNLINKNAS_TRANSPORT_H
#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/mobility/_5gmmMsgHeader.h"
#include "nas/NasDatatypes/ULNasPayloadContainer.h"
#include "nas/NasDatatypes/payloadContainerType.h"
#include "nas/NasDatatypes/PDUSessionId2.h"

#define DOWNLINK_NAS_IE_PDU_SESSION_ID_PRESENT      (1<<0)

enum DownlinkNasTransportIEI {
    E_DOWNLINK_NAS_IEI_PDU_SESSION_ID         = 0x12,
};

typedef struct DownlinkNasTransport {
    _5gmmMsgHeader_t               mmHeader;
    

    uint8_t                        payloadContainerType:4;
            

    PayloadContainer_t             container;

    

    uint32_t                        presenceMask;
    PDUSessionId2_t                 pduSessionId;
}DownlinkNasTransport_t;


int decodeDlNAS(uint8_t *buffer, DownlinkNasTransport_t *msgStruct_p,
		   uint32_t bufLen);
int encodeDlNAS(uint8_t *buffer, uint32_t bufLen,
		  DownlinkNasTransport_t *msgStruct, uint32_t *encodedLen);


#endif
