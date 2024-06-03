#ifndef __PDUSESSIONRESULTCAUSE_H
#define __PDUSESSIONRESULTCAUSE_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"
#include "nas/NasDatatypes/5GMMCause.h"
#include "nas/NasDatatypes/PDUSessionId2.h"

#define MAX_NO_OF_PSI_CAUSE 256

typedef struct psiCause {
    uint8_t psi;
    _5gmmCause_t _5gmmCause;
}psiCause_t;

typedef struct pduSessionResultCause {
    uint16_t length;
    psiCause_t psiCause[MAX_NO_OF_PSI_CAUSE];
}pduSessionResultCause_t;



int decodePDUSessionResultCause(uint8_t *buffer,uint32_t bufLen
     ,pduSessionResultCause_t *msgStruct, uint32_t *decodedLen); 

int encodePDUSessionResultCause(uint8_t *buffer, uint32_t bufLen
     ,pduSessionResultCause_t *msgStruct,uint32_t *encodedLen);


#endif


