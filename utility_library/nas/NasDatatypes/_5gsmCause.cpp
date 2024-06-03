#include "nas/NasDatatypes/_5gsmCause.h"

int decode5gsmCause(uint8_t *buffer, uint32_t bufLen, _5gsmCause_t *_5gsmCause, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gsmCause == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GSM_CAUSE_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	_5gsmCause->causeValue = buffer[len++];


	*decodedLen = len;
	nasLog("5GSM_cause decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encode5gsmCause(uint8_t *buffer, uint32_t bufLen, _5gsmCause_t *_5gsmCause, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gsmCause == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GSM_CAUSE_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (_5gsmCause->causeValue);

	*encodedLen = len;
	nasLog("5GSM_cause encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
