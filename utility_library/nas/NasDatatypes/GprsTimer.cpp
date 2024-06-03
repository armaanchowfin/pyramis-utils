#include "nas/NasDatatypes/GprsTimer.h"

int decodeGprsTimer(uint8_t *buffer, uint32_t bufLen, GprsTimer_t *gprsTimer, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || gprsTimer == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + GPRS_TIMER_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	gprsTimer->unit= (buffer[len] & 0xe0) >> 5;
	gprsTimer->timerValue= (buffer[len] & 0x1f);
	len++;


	*decodedLen = len;
	nasLog("GPRS_timer decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeGprsTimer(uint8_t *buffer, uint32_t bufLen, GprsTimer_t *gprsTimer, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || gprsTimer == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + GPRS_TIMER_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len] = (gprsTimer->unit << 5) | (gprsTimer->timerValue);
	len++;


	*encodedLen = len;
	nasLog("GPRS_timer encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
