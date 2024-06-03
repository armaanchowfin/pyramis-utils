#include "nas/NasDatatypes/NasKeySetIdentifier.h"

int decodeNasKeySetIdentifier(uint8_t *buffer, uint32_t bufLen, NasKeySetIdentifier_t *nasKeySetIdentifier, uint32_t* decodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || nasKeySetIdentifier == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + NAS_KEY_SET_IDENTIFIER_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
		nasKeySetIdentifier->tsc= (buffer[len] & 0x80) >> 7;
		nasKeySetIdentifier->nasKeySetIdentifier= (buffer[len] & 0x70) >> 4;

	}else{
		nasKeySetIdentifier->tsc= (buffer[len] & 0x08) >> 3;
		nasKeySetIdentifier->nasKeySetIdentifier= (buffer[len] & 0x07);

	}
	len = NAS_KEY_SET_IDENTIFIER_LEN;
	*decodedLen = len;
	nasLog("NAS_key_set_identifier decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeNasKeySetIdentifier(uint8_t *buffer, uint32_t bufLen, NasKeySetIdentifier_t *nasKeySetIdentifier, uint32_t* encodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || nasKeySetIdentifier == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + NAS_KEY_SET_IDENTIFIER_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
	buffer[len] = (nasKeySetIdentifier->tsc << 7) | (nasKeySetIdentifier->nasKeySetIdentifier << 4);


	}else{
	buffer[len] = (nasKeySetIdentifier->tsc << 3) | (nasKeySetIdentifier->nasKeySetIdentifier);


	}
	len = NAS_KEY_SET_IDENTIFIER_LEN;
	*encodedLen = len;
	nasLog("NAS_key_set_identifier encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
