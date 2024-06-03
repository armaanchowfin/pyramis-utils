#ifndef __SERVICE_REQUEST_H
#define __SERVICE_REQUEST_H
#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/mobility/_5gmmMsgHeader.h"
#include "nas/NasDatatypes/5GSServiceType.h"
#include "nas/NasDatatypes/5G-S-TMSI.h"
#include "nas/NasDatatypes/UplinkDataStatus.h"
#include "nas/NasDatatypes/PDUSessionStatus.h"
#include "nas/NasDatatypes/NasKeySetId.h"



#define SERVICE_REQUEST_UPLINK_DATA_STATUS_PRESENT (1<<0)
#define SERVICE_REQUEST_PDU_SESSION_STATUS_PRESENT (1<<1)


typedef enum serviceReqIEI {
    E_SERV_REQUEST_IEI_UPLINK_DATA_STATUS        = 0x40,
    E_SERV_REQUEST_IEI_PDU_SESSION_STATUS        = 0x50,
}serviceReqIEI_t; 



typedef struct ServiceRequest {
    

    _5gmmMsgHeader_t                        mmHeader;
    NaskeysetId_t                           ngKsi;
    _5gServiceType_t                        _5gservType;
    _5gsTmsi_t                              _5gsTmsi; 
    


    

    uint32_t                                 presenceMask;
    UplinkDataStatus_t                       uplinkDataStatus;
    pduSessionStatus_t                       pduSessionStatus;
}ServiceRequest_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeServReq(uint8_t *buffer, uint32_t bufLen,
		ServiceRequest_t *msgStruct,uint32_t* encodedLen);
int decodeServReq(uint8_t *buffer,ServiceRequest_t *msgStruct_p,
		 uint32_t decodLen);

#endif
