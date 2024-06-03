#ifndef __ALWAYS_ON_PDU_SESSION_REQUESTED_H
#define __ALWAYS_ON_PDU_SESSION_REQUESTED_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define ALWAYS_ON_PDU_SESSION_REQUESTED_LEN	1


#define ALWAYS_ON_PDU_SESSION_REQUESTED_ALWAYS_ON_PDU_SESSION_NOT_ALLOWED	0b0
#define ALWAYS_ON_PDU_SESSION_REQUESTED_ALWAYS_ON_PDU_SESSION_REQUIRED	0b1

typedef struct alwaysOnPduSessionRequested {
	uint8_t	spare:3;
	uint8_t	apsr:1;
}AlwaysOnPduSessionRequested_t;

int decodeAlwaysOnPduSessionRequested(uint8_t *buffer, uint32_t bufLen, AlwaysOnPduSessionRequested_t *alwaysOnPduSessionRequested, uint32_t* decodedLen, bool msbAligned = false);

int encodeAlwaysOnPduSessionRequested(uint8_t *buffer, uint32_t bufLen, AlwaysOnPduSessionRequested_t *alwaysOnPduSessionRequested, uint32_t* encodedLen, bool msbAligned = false);

#endif 

