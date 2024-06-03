#ifndef __NAS_KEY_SET_IDENTIFIER_H
#define __NAS_KEY_SET_IDENTIFIER_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define NAS_KEY_SET_IDENTIFIER_LEN	1



typedef struct nasKeySetIdentifier {
	uint8_t	tsc:1;
	uint8_t	nasKeySetIdentifier:3;
}NasKeySetIdentifier_t;

int decodeNasKeySetIdentifier(uint8_t *buffer, uint32_t bufLen, NasKeySetIdentifier_t *nasKeySetIdentifier, uint32_t* decodedLen, bool msbAligned = false);

int encodeNasKeySetIdentifier(uint8_t *buffer, uint32_t bufLen, NasKeySetIdentifier_t *nasKeySetIdentifier, uint32_t* encodedLen, bool msbAligned = false);

#endif 

