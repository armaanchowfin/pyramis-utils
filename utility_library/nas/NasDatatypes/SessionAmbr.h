#ifndef __SESSION_AMBR_H
#define __SESSION_AMBR_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define SESSION_AMBR_LEN	8


#define SESSION_AMBR_VALUE_IS_NOT_USED	0b00000000
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_KBPS	0b00000001
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_KBPS	0b00000010
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_KBPS	0b00000011
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_KBPS	0b00000100
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_KBPS	0b00000101
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_MBPS	0b00000110
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_MBPS	0b00000111
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_MBPS	0b00001000
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_MBPS	0b00001001
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_MBPS	0b00001010
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_GBPS	0b00001011
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_GBPS	0b00001100
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_GBPS	0b00001101
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_GBPS	0b00001110
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_GBPS	0b00001111
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_TBPS	0b00010000
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_TBPS	0b00010001
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_TBPS	0b00010010
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_TBPS	0b00010011
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_TBPS	0b00010100
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_PBPS	0b00010101
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_PBPS	0b00010110
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_PBPS	0b00010111
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_PBPS	0b00011000
#define SESSION_AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_PBPS	0b00011001

typedef struct sessionAmbr {
	uint8_t	lengthOfSessionAmbrContents;
	uint8_t	unitForSessionAmbrForDownlink;
	uint16_t	sessionAmbrForDownlink;
	uint8_t	unitForSessionAmbrForUplink;
	uint16_t	sessionAmbrForUplink;
}SessionAmbr_t;

int decodeSessionAmbr(uint8_t *buffer, uint32_t bufLen, SessionAmbr_t *sessionAmbr, uint32_t* decodedLen);

int encodeSessionAmbr(uint8_t *buffer, uint32_t bufLen, SessionAmbr_t *sessionAmbr, uint32_t* encodedLen);

#endif 

