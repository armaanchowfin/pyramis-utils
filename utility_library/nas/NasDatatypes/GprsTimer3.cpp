#include "nas/NasDatatypes/GprsTimer3.h"

int decodeGprsTimer3(uint8_t *buffer, uint32_t bufLen, GprsTimer3_t *gprsTimer3, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || gprsTimer3 == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + GPRS_TIMER3_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	gprsTimer3->lengthOfGprsTimer3Contents = buffer[len++];

	uint8_t ieLen = gprsTimer3->lengthOfGprsTimer3Contents;

	if((ieLen > GPRS_TIMER3_LEN) || (ieLen < GPRS_TIMER3_LEN)) {
		higLog("Invalid Length %d", ieLen);
		errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
		nasLogEXIT;
		return FAILURE;
	}

	if((len + ieLen) > bufLen) {
		higLog("Buffer insufficient to read %d bytes", ieLen);
		errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}


	gprsTimer3->unit= (buffer[len] & 0xe0) >> 5;
	gprsTimer3->timerValue= (buffer[len] & 0x1f);
	len++;


	*decodedLen = len;
	nasLog("GPRS_timer3 decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeGprsTimer3(uint8_t *buffer, uint32_t bufLen, GprsTimer3_t *gprsTimer3, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || gprsTimer3 == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + GPRS_TIMER3_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (gprsTimer3->lengthOfGprsTimer3Contents);
	uint8_t ieLen = gprsTimer3->lengthOfGprsTimer3Contents;

	if((ieLen > GPRS_TIMER3_LEN) || (ieLen < GPRS_TIMER3_LEN)) {
		higLog("Invalid Length %d", ieLen);
		errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
		nasLogEXIT;
		return FAILURE;
	}

	if((len + ieLen) > bufLen) {
		higLog("Buffer insufficient to read %d bytes", ieLen);
		errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len] = (gprsTimer3->unit << 5) | (gprsTimer3->timerValue);
	len++;


	*encodedLen = len;
	nasLog("GPRS_timer3 encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
