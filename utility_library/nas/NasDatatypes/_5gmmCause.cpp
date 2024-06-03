#include "nas/NasDatatypes/_5gmmCause.h"

int decode5gmmCause(uint8_t *buffer, uint32_t bufLen, _5gmmCause_t *_5gmmCause, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gmmCause == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GMM_CAUSE_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	_5gmmCause->causeValue = buffer[len++];


	*decodedLen = len;
	nasLog("5GMM_cause decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encode5gmmCause(uint8_t *buffer, uint32_t bufLen, _5gmmCause_t *_5gmmCause, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gmmCause == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GMM_CAUSE_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (_5gmmCause->causeValue);

	*encodedLen = len;
	nasLog("5GMM_cause encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
