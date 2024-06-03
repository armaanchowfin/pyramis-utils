#ifndef __UplinkDataStatus_H
#define __UplinkDataStatus_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define UPLINK_DATA_STATUS_LEN 2



typedef struct UplinkDataStatus {
    uint8_t         len;
    uint16_t        psi;
}UplinkDataStatus_t;


int decodeUplinkDataStatus(uint8_t *buffer, uint32_t bufLen,
			           UplinkDataStatus_t *uplinkDataStatus, uint32_t* decodedLen);
int encodeUplinkDataStatus(uint8_t *buffer, uint32_t bufLen,
                       UplinkDataStatus_t *uplinkDataStatus, uint32_t *encodedLen);

#endif 

