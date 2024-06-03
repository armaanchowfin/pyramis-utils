#include "nas/NasDatatypes/PacketFilterListDelete.h"

int decodePacketFilterListDelete(uint8_t *buffer, uint32_t bufLen, PacketFilterListDelete_t *packetFilterListDelete, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || packetFilterListDelete == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}


	packetFilterListDelete->spare= (buffer[len] & 0xf0) >> 4;
	packetFilterListDelete->packetFilterIdentifier= (buffer[len] & 0x0f);
	len++;

    nasLog("spare %u packetFilterDirection %u packetFilterIdentifier %u ",
            packetFilterListDelete->spare,
            packetFilterListDelete->packetFilterIdentifier
        );

	*decodedLen = len;
	nasLog("Packet_filter_list_delete decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodePacketFilterListDelete(uint8_t *buffer, uint32_t bufLen, PacketFilterListDelete_t *packetFilterListDelete, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || packetFilterListDelete == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

    nasLog("spare %u packetFilterDirection %u packetFilterIdentifier %u ",
            packetFilterListDelete->spare,
            packetFilterListDelete->packetFilterIdentifier
        );

	buffer[len] = (packetFilterListDelete->spare << 4) | (packetFilterListDelete->packetFilterIdentifier);
	len++;


	*encodedLen = len;
	nasLog("Packet_filter_list_delete encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
