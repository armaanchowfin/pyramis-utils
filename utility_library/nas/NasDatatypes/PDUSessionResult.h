#ifndef __PDUSESSIONRESULT_H
#define __PDUSESSIONRESULT_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define PDU_SESSION_RESULT_LEN 2



typedef struct pduSessionResult {
    uint8_t         len;
    uint16_t        psi;
}pduSessionResult_t;


int decodePDUSessionResult(uint8_t *buffer, uint32_t bufLen,
			           pduSessionResult_t *pduSessionResult, uint32_t* decodedLen);
int encodePDUSessionResult(uint8_t *buffer, uint32_t bufLen,
                       pduSessionResult_t *pduSessionResult, uint32_t *encodedLen);

#endif 

