#ifndef __SERVICE_ACCEPT_H
#define __SERVICE_ACCEPT_H
#include  <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/mobility/_5gmmMsgHeader.h"
#include "nas/NasDatatypes/5GSServiceType.h"
#include "nas/NasDatatypes/PDUSessionStatus.h"
#include "nas/NasDatatypes/PDUSessionResult.h"
#include "nas/NasDatatypes/PDUSessionResultCause.h"



#define SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT (1<<0)
#define SERVICE_ACCEPT_PDU_SESSION_RESULT_PRESENT (1<<1)
#define SERVICE_ACCEPT_PDU_SESSION_RESULT_CAUSE_PRESENT (1<<2)


typedef enum serviceAccIEI {
    E_SERV_ACCEPT_IEI_PDU_SESSION_STATUS       = 0x50,
    E_SERV_ACCEPT_IEI_PDU_SESSION_RESULT       = 0x26,
    E_SERV_ACCEPT_IEI_PDU_SESSION_RESULT_CAUSE = 0x72,
}serviceAccIEI_t; 



typedef struct ServiceAccept {
    

    _5gmmMsgHeader_t                        mmHeader;
    


    

    uint32_t                                 presenceMask;
    pduSessionStatus_t                       pduSessionStatus;
    pduSessionResult_t                       pduSessionResult;
    pduSessionResultCause_t                  pduSessionResultCause;
}ServiceAccept_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int encodeServAcc(uint8_t *buffer, uint32_t bufLen,
		ServiceAccept_t *msgStruct,uint32_t* encodedLen);
int decodeServAcc(uint8_t *buffer,ServiceAccept_t *msgStruct_p,
		 uint32_t decodLen);

#endif

