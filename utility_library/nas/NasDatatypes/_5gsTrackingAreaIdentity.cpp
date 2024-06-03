#include "nas/NasDatatypes/_5gsTrackingAreaIdentity.h"

int decode5gsTrackingAreaIdentity(uint8_t *buffer, uint32_t bufLen, _5gsTrackingAreaIdentity_t *_5gsTrackingAreaIdentity, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gsTrackingAreaIdentity == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GS_TRACKING_AREA_IDENTITY_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	_5gsTrackingAreaIdentity->mccDigit2= (buffer[len] & 0xf0) >> 4;
	_5gsTrackingAreaIdentity->mccDigit1= (buffer[len] & 0x0f);
	len++;

	_5gsTrackingAreaIdentity->mncDigit3= (buffer[len] & 0xf0) >> 4;
	_5gsTrackingAreaIdentity->mccDigit3= (buffer[len] & 0x0f);
	len++;

	_5gsTrackingAreaIdentity->mncDigit2= (buffer[len] & 0xf0) >> 4;
	_5gsTrackingAreaIdentity->mncDigit1= (buffer[len] & 0x0f);
	len++;

	_5gsTrackingAreaIdentity->tac |= ((uint32_t)buffer[len++]) << 16;
	_5gsTrackingAreaIdentity->tac |= ((uint32_t)buffer[len++]) << 8;
	_5gsTrackingAreaIdentity->tac |= ((uint32_t)buffer[len++]);


	*decodedLen = len;
	nasLog("5GS_tracking_area_identity decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encode5gsTrackingAreaIdentity(uint8_t *buffer, uint32_t bufLen, _5gsTrackingAreaIdentity_t *_5gsTrackingAreaIdentity, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gsTrackingAreaIdentity == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GS_TRACKING_AREA_IDENTITY_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len] = (_5gsTrackingAreaIdentity->mccDigit2 << 4) | (_5gsTrackingAreaIdentity->mccDigit1);
	len++;

	buffer[len] = (_5gsTrackingAreaIdentity->mncDigit3 << 4) | (_5gsTrackingAreaIdentity->mccDigit3);
	len++;

	buffer[len] = (_5gsTrackingAreaIdentity->mncDigit2 << 4) | (_5gsTrackingAreaIdentity->mncDigit1);
	len++;

	buffer[len++] = (_5gsTrackingAreaIdentity->tac) >> 16;
	buffer[len++] = (_5gsTrackingAreaIdentity->tac) >> 8;
	buffer[len++] = (_5gsTrackingAreaIdentity->tac);

	*encodedLen = len;
	nasLog("5GS_tracking_area_identity encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
