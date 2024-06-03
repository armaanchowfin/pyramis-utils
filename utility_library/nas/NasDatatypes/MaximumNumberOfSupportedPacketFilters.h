#ifndef __MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_H
#define __MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_LEN	2



typedef struct maximumNumberOfSupportedPacketFilters {
	uint16_t	maximumNumberOfSupportedPacketFilters:11;
	uint8_t	spare:5;
}MaximumNumberOfSupportedPacketFilters_t;

int decodeMaximumNumberOfSupportedPacketFilters(uint8_t *buffer, uint32_t bufLen, MaximumNumberOfSupportedPacketFilters_t *maximumNumberOfSupportedPacketFilters, uint32_t* decodedLen);

int encodeMaximumNumberOfSupportedPacketFilters(uint8_t *buffer, uint32_t bufLen, MaximumNumberOfSupportedPacketFilters_t *maximumNumberOfSupportedPacketFilters, uint32_t* encodedLen);

#endif 

