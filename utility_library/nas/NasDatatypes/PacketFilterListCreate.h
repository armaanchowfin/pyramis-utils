#ifndef __PACKET_FILTER_LIST_CREATE_H
#define __PACKET_FILTER_LIST_CREATE_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/PacketFilterContents.h"
#include "nas/nasUtil/nasLogging.h"

typedef struct packetFilterListCreate {
	uint8_t	spare:2;
	uint8_t	packetFilterDirection:2;
	uint8_t	packetFilterIdentifier:4;
	uint8_t	lengthOfPacketFilterContents;
	PacketFilterContents_t	packetFilterContents; 
}PacketFilterListCreate_t;

int decodePacketFilterListCreate(uint8_t *buffer, uint32_t bufLen, PacketFilterListCreate_t *packetFilterListCreate, uint32_t* decodedLen);

int encodePacketFilterListCreate(uint8_t *buffer, uint32_t bufLen, PacketFilterListCreate_t *packetFilterListCreate, uint32_t* encodedLen);

#endif 

