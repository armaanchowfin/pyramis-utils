#include "nas/NasDatatypes/PacketFilterListCreate.h"

int decodePacketFilterListCreate(uint8_t *buffer, uint32_t bufLen, PacketFilterListCreate_t *packetFilterListCreate, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t pfcDecodeLength = 0;
	if(buffer == NULL || packetFilterListCreate == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}


	packetFilterListCreate->spare= (buffer[len] & 0xc0) >> 6;
	packetFilterListCreate->packetFilterDirection= (buffer[len] & 0x30) >> 4;
	packetFilterListCreate->packetFilterIdentifier= (buffer[len] & 0x0f);
	len++;

	packetFilterListCreate->lengthOfPacketFilterContents = buffer[len++];

	if(decodePacketFilterContents(buffer+len,(bufLen-len),
		&(packetFilterListCreate->packetFilterContents),&pfcDecodeLength,
		packetFilterListCreate->lengthOfPacketFilterContents)==FAILURE)
	{
		higLog("Failed decoding Packet Filter Content");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;	
	}
	len += pfcDecodeLength;

    nasLog(" spare %u packetFilterDirection %u packetFilterIdentifier %u "
            "lengthOfPacketFilterContents %u ",
            packetFilterListCreate->spare,
            packetFilterListCreate->packetFilterDirection,
            packetFilterListCreate->packetFilterIdentifier,
            packetFilterListCreate->lengthOfPacketFilterContents
        );

	*decodedLen = len;
	nasLog("Packet_filter_list_create decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodePacketFilterListCreate(uint8_t *buffer, uint32_t bufLen, PacketFilterListCreate_t *packetFilterListCreate, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || packetFilterListCreate == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

    nasLog(" spare %u packetFilterDirection %u packetFilterIdentifier %u "
            "lengthOfPacketFilterContents %u ",
            packetFilterListCreate->spare,
            packetFilterListCreate->packetFilterDirection,
            packetFilterListCreate->packetFilterIdentifier,
            packetFilterListCreate->lengthOfPacketFilterContents
        );

	buffer[len] = (packetFilterListCreate->spare << 6) | (packetFilterListCreate->packetFilterDirection << 4) | (packetFilterListCreate->packetFilterIdentifier);
	len++;
	int indexOfLength = len;//Length before encoding, Discuss for better approach
	len++;
	uint32_t pfcEncodeLength = 0;
	if(encodePacketFilterContents(buffer+len,(bufLen-len),
		&(packetFilterListCreate->packetFilterContents),&pfcEncodeLength)==FAILURE)
	{
		higLog("Failed encoding Packet Filter Content");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;	
	} 
	len += pfcEncodeLength;
	buffer[indexOfLength] = pfcEncodeLength;

	*encodedLen = len;
	nasLog("Packet_filter_list_create encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
