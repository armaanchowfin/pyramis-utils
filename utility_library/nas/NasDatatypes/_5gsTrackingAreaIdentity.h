#ifndef ___5GS_TRACKING_AREA_IDENTITY_H
#define ___5GS_TRACKING_AREA_IDENTITY_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define _5GS_TRACKING_AREA_IDENTITY_LEN	6



typedef struct _5gsTrackingAreaIdentity {
	uint8_t	mccDigit2:4;
	uint8_t	mccDigit1:4;
	uint8_t	mncDigit3:4;
	uint8_t	mccDigit3:4;
	uint8_t	mncDigit2:4;
	uint8_t	mncDigit1:4;
	uint32_t	tac:24;
}_5gsTrackingAreaIdentity_t;

int decode5gsTrackingAreaIdentity(uint8_t *buffer, uint32_t bufLen, _5gsTrackingAreaIdentity_t *_5gsTrackingAreaIdentity, uint32_t* decodedLen);

int encode5gsTrackingAreaIdentity(uint8_t *buffer, uint32_t bufLen, _5gsTrackingAreaIdentity_t *_5gsTrackingAreaIdentity, uint32_t* encodedLen);

#endif 

