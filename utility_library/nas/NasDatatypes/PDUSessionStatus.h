#ifndef __PDUSessionStatus_H
#define __PDUSessionStatus_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define PDU_SESSION_STATUS_LEN 2



typedef struct pduSessionStatus {
    uint8_t         len;
    uint16_t        psi;
}pduSessionStatus_t;


int decodePDUSessionStatus(uint8_t *buffer, uint32_t bufLen,
			           pduSessionStatus_t *pduSessionStatus, uint32_t* decodedLen);
int encodePDUSessionStatus(uint8_t *buffer, uint32_t bufLen,
                       pduSessionStatus_t *pduSessionStatus, uint32_t *encodedLen);

#endif 

