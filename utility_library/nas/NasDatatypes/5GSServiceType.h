#ifndef __5GS_SERVICE_TYPE_H
#define __5GS_SERVICE_TYPE_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define _5G_SERV_TYPE_LEN 1




typedef struct _5gServiceType {
    

    uint8_t          spare:4;   

    uint8_t          value:4;
}_5gServiceType_t;

typedef enum _5gServiceTypeValue {
   E_SIGNALLING = 0b0000,
   E_DATA  = 0b0001,
   E_MOBILE_TERMINATED_SERVICES = 0b0010,
   E_EMERGENCY_SERVICES = 0b0011,
   E_EMERGENCY_SERVICES_FALLBACK = 0b0100,
   E_HIGH_PRIORITY_ACCESS = 0b0101,
   E_UNUSED_1 = 0b0110,
   E_UNUSED_2 = 0b0111,
   E_UNUSED_3 = 0b1000,
   E_UNUSED_4 = 0b1001,
   E_UNUSED_5 = 0b1010,
   E_UNUSED_6 = 0b10101
}_5gServiceTypeValue_t;

int decode5GSServType(uint8_t *buffer, uint32_t bufLen,
			           _5gServiceType_t *servType, uint32_t* decodedLen);
int encode5GSServType(uint8_t *buffer, uint32_t bufLen,
                       _5gServiceType_t *servType, uint32_t *encodedLen);

#endif 

