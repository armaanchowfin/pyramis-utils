#ifndef __PACKET_FILTER_LIST_DELETE_H
#define __PACKET_FILTER_LIST_DELETE_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"




typedef struct packetFilterListDelete {
	uint8_t	spare:4;
	uint8_t	packetFilterIdentifier:4;
}PacketFilterListDelete_t;

int decodePacketFilterListDelete(uint8_t *buffer, uint32_t bufLen, PacketFilterListDelete_t *packetFilterListDelete, uint32_t* decodedLen);

int encodePacketFilterListDelete(uint8_t *buffer, uint32_t bufLen, PacketFilterListDelete_t *packetFilterListDelete, uint32_t* encodedLen);

#endif 

