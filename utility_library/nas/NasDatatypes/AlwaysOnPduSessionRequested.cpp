#include "nas/NasDatatypes/AlwaysOnPduSessionRequested.h"

int decodeAlwaysOnPduSessionRequested(uint8_t *buffer, uint32_t bufLen, AlwaysOnPduSessionRequested_t *alwaysOnPduSessionRequested, uint32_t* decodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || alwaysOnPduSessionRequested == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + ALWAYS_ON_PDU_SESSION_REQUESTED_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
		alwaysOnPduSessionRequested->spare= (buffer[len] & 0xe0) >> 5;
		alwaysOnPduSessionRequested->apsr= (buffer[len] & 0x10) >> 4;

	}else{
		alwaysOnPduSessionRequested->spare= (buffer[len] & 0x0e) >> 1;
		alwaysOnPduSessionRequested->apsr= (buffer[len] & 0x01);

	}
	len = ALWAYS_ON_PDU_SESSION_REQUESTED_LEN;
	*decodedLen = len;
	nasLog("Always-on_PDU_session_requested decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeAlwaysOnPduSessionRequested(uint8_t *buffer, uint32_t bufLen, AlwaysOnPduSessionRequested_t *alwaysOnPduSessionRequested, uint32_t* encodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || alwaysOnPduSessionRequested == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + ALWAYS_ON_PDU_SESSION_REQUESTED_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
	buffer[len] = (alwaysOnPduSessionRequested->spare << 5) | (alwaysOnPduSessionRequested->apsr << 4);


	}else{
	buffer[len] = (alwaysOnPduSessionRequested->spare << 1) | (alwaysOnPduSessionRequested->apsr);


	}
	len = ALWAYS_ON_PDU_SESSION_REQUESTED_LEN;
	*encodedLen = len;
	nasLog("Always-on_PDU_session_requested encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
