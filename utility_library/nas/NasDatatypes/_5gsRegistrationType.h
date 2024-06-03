#ifndef ___5GS_REGISTRATION_TYPE_H
#define ___5GS_REGISTRATION_TYPE_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define _5GS_REGISTRATION_TYPE_LEN	1


#define _5GS_REGISTRATION_TYPE_INITIAL_REGISTRATION	0b001
#define _5GS_REGISTRATION_TYPE_MOBILITY_REGISTRATION_UPDATING	0b010
#define _5GS_REGISTRATION_TYPE_PERIODIC_REGISTRATION_UPDATING	0b011
#define _5GS_REGISTRATION_TYPE_EMERGENCY_REGISTRATION	0b100
#define _5GS_REGISTRATION_TYPE_NO_FOLLOW_ON_REQUEST_PENDING	0b0
#define _5GS_REGISTRATION_TYPE_FOLLOW_ON_REQUEST_PENDING	0b1

typedef struct _5gsRegistrationType {
	uint8_t	for1:1;
	uint8_t	_5gsRegistrationTypeValue:3;
}_5gsRegistrationType_t;

int decode5gsRegistrationType(uint8_t *buffer, uint32_t bufLen, _5gsRegistrationType_t *_5gsRegistrationType, uint32_t* decodedLen, bool msbAligned = false);

int encode5gsRegistrationType(uint8_t *buffer, uint32_t bufLen, _5gsRegistrationType_t *_5gsRegistrationType, uint32_t* encodedLen, bool msbAligned = false);

#endif 

