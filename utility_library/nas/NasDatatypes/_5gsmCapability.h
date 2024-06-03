#ifndef ___5GSM_CAPABILITY_H
#define ___5GSM_CAPABILITY_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define _5GSM_CAPABILITY_MIN_LEN	1
#define _5GSM_CAPABILITY_MAX_LEN	13

#define SPARE1_MAX_NO	 64

#define _5GSM_CAPABILITY_REFLECTIVE_QOS_NOT_SUPPORTED	0b0
#define _5GSM_CAPABILITY_REFLECTIVE_QOS_SUPPORTED	0b1
#define _5GSM_CAPABILITY_MULTI_HOMED_IPV6_PDU_SESSION_NOT_SUPPORTED	0b0
#define _5GSM_CAPABILITY_MULTI_HOMED_IPV6_PDU_SESSION_SUPPORTED	0b1

typedef struct _5gsmCapability {
	uint8_t	lengthOf5gsmCapabilityContents;
	uint8_t	spare:6;
	uint8_t	mh6Pdu:1;
	uint8_t	rqos:1;
	uint8_t	spare1[SPARE1_MAX_NO];
}_5gsmCapability_t;

int decode5gsmCapability(uint8_t *buffer, uint32_t bufLen, _5gsmCapability_t *_5gsmCapability, uint32_t* decodedLen);

int encode5gsmCapability(uint8_t *buffer, uint32_t bufLen, _5gsmCapability_t *_5gsmCapability, uint32_t* encodedLen);

#endif 

