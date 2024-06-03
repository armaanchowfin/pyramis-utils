#ifndef ___5GMM_CAPABILITY_H
#define ___5GMM_CAPABILITY_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define _5GMM_CAPABILITY_MIN_LEN	1
#define _5GMM_CAPABILITY_MAX_LEN	13

#define SPARE1_MAX_NO	 64

#define _5GMM_CAPABILITY_S1_MODE_NOT_SUPPORTED	0b0
#define _5GMM_CAPABILITY_S1_MODE_SUPPORTED	0b1
#define _5GMM_CAPABILITY_HANDOVER_NOT_SUPPORTED	0b0
#define _5GMM_CAPABILITY_HANDOVER_SUPPORTED	0b1
#define _5GMM_CAPABILITY_LPP_IN_N1_MODE_NOT_SUPPORTED	0b0
#define _5GMM_CAPABILITY_LPP_IN_N1_MODE_SUPPORTED_	0b1

typedef struct _5gmmCapability {
	uint8_t	lengthOf5gmmCapabilityContents;
	uint8_t	spare:5;
	uint8_t	lpp:1;
	uint8_t	hoAttach:1;
	uint8_t	s1Mode:1;
	uint8_t	spare1[SPARE1_MAX_NO];
}_5gmmCapability_t;

int decode5gmmCapability(uint8_t *buffer, uint32_t bufLen, _5gmmCapability_t *_5gmmCapability, uint32_t* decodedLen);

int encode5gmmCapability(uint8_t *buffer, uint32_t bufLen, _5gmmCapability_t *_5gmmCapability, uint32_t* encodedLen);

#endif 

