#ifndef __NETWORK_SLICING_INDICATION_H
#define __NETWORK_SLICING_INDICATION_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define NETWORK_SLICING_INDICATION_LEN	1



typedef struct networkSlicingIndication {
	uint8_t	spare:2;
	uint8_t	dcni:1;
	uint8_t	nssci:1;
}NetworkSlicingIndication_t;

int decodeNetworkSlicingIndication(uint8_t *buffer, uint32_t bufLen, NetworkSlicingIndication_t *networkSlicingIndication, uint32_t* decodedLen, bool msbAligned = false);

int encodeNetworkSlicingIndication(uint8_t *buffer, uint32_t bufLen, NetworkSlicingIndication_t *networkSlicingIndication, uint32_t* encodedLen, bool msbAligned = false);

#endif 

