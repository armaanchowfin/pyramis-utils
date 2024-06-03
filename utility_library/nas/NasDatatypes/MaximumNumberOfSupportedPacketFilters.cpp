#include "nas/NasDatatypes/MaximumNumberOfSupportedPacketFilters.h"

int decodeMaximumNumberOfSupportedPacketFilters(uint8_t *buffer, uint32_t bufLen, MaximumNumberOfSupportedPacketFilters_t *maximumNumberOfSupportedPacketFilters, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || maximumNumberOfSupportedPacketFilters == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	maximumNumberOfSupportedPacketFilters->maximumNumberOfSupportedPacketFilters |= ((uint16_t)buffer[len++]) << 8;
	maximumNumberOfSupportedPacketFilters->maximumNumberOfSupportedPacketFilters|= (buffer[len] & 0xe0) >> 5;

	maximumNumberOfSupportedPacketFilters->spare= (buffer[len] & 0x1f);
	len++;


	*decodedLen = len;
	nasLog("Maximum_number_of_supported_packet_filters decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeMaximumNumberOfSupportedPacketFilters(uint8_t *buffer, uint32_t bufLen, MaximumNumberOfSupportedPacketFilters_t *maximumNumberOfSupportedPacketFilters, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || maximumNumberOfSupportedPacketFilters == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (maximumNumberOfSupportedPacketFilters->maximumNumberOfSupportedPacketFilters) >> 3;
	buffer[len] = ((maximumNumberOfSupportedPacketFilters->maximumNumberOfSupportedPacketFilters&0x07) << 5) | 
(maximumNumberOfSupportedPacketFilters->spare);
	len++;


	*encodedLen = len;
	nasLog("Maximum_number_of_supported_packet_filters encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
