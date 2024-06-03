#ifndef __SERVICE_REJECT_H
#define __SERVICE_REJECT_H
#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/mobility/_5gmmMsgHeader.h"
#include "nas/NasDatatypes/PDUSessionStatus.h"
#include "nas/NasDatatypes/5GMMCause.h"



#define SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT (1<<0)


typedef enum serviceRejIEI {
    E_SERV_REJECT_IEI_PDU_SESSION_STATUS       = 0x50,
}serviceRejIEI_t; 



typedef struct ServiceReject {
    

    _5gmmMsgHeader_t                        mmHeader;
    _5gmmCause_t                          _5gmmCause;
    


    

    uint32_t                                 presenceMask;
    pduSessionStatus_t                       pduSessionStatus;
}ServiceReject_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeServRej(uint8_t *buffer, uint32_t bufLen,
		ServiceReject_t *msgStruct,uint32_t* encodedLen);
int decodeServRej(uint8_t *buffer,ServiceReject_t *msgStruct_p,
		 uint32_t decodLen);

#endif

