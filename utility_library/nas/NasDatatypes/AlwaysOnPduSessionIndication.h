#ifndef __ALWAYS_ON_PDU_SESSION_INDICATION_H
#define __ALWAYS_ON_PDU_SESSION_INDICATION_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define ALWAYS_ON_PDU_SESSION_INDICATION_LEN	1


#define ALWAYS_ON_PDU_SESSION_INDICATION_ALWAYS_ON_PDU_SESSION_NOT_ALLOWED	0b0
#define ALWAYS_ON_PDU_SESSION_INDICATION_ALWAYS_ON_PDU_SESSION_REQUIRED	0b1

typedef struct alwaysOnPduSessionIndication {
	uint8_t	spare:3;
	uint8_t	apsi:1;
}AlwaysOnPduSessionIndication_t;

int decodeAlwaysOnPduSessionIndication(uint8_t *buffer, uint32_t bufLen, AlwaysOnPduSessionIndication_t *alwaysOnPduSessionIndication, uint32_t* decodedLen, bool msbAligned = false);

int encodeAlwaysOnPduSessionIndication(uint8_t *buffer, uint32_t bufLen, AlwaysOnPduSessionIndication_t *alwaysOnPduSessionIndication, uint32_t* encodedLen, bool msbAligned = false);

#endif 

