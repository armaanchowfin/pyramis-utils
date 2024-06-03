#include "nas/NasDatatypes/_5gsRegistrationType.h"

int decode5gsRegistrationType(uint8_t *buffer, uint32_t bufLen, _5gsRegistrationType_t *_5gsRegistrationType, uint32_t* decodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gsRegistrationType == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GS_REGISTRATION_TYPE_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
		_5gsRegistrationType->for1= (buffer[len] & 0x80) >> 7;
		_5gsRegistrationType->_5gsRegistrationTypeValue= (buffer[len] & 0x70) >> 4;

	}else{
		_5gsRegistrationType->for1= (buffer[len] & 0x08) >> 3;
		_5gsRegistrationType->_5gsRegistrationTypeValue= (buffer[len] & 0x07);

	}
	len = _5GS_REGISTRATION_TYPE_LEN;
	*decodedLen = len;
	nasLog("5GS_registration_type decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encode5gsRegistrationType(uint8_t *buffer, uint32_t bufLen, _5gsRegistrationType_t *_5gsRegistrationType, uint32_t* encodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gsRegistrationType == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GS_REGISTRATION_TYPE_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
	buffer[len] = (_5gsRegistrationType->for1 << 7) | (_5gsRegistrationType->_5gsRegistrationTypeValue << 4);


	}else{
	buffer[len] = (_5gsRegistrationType->for1 << 3) | (_5gsRegistrationType->_5gsRegistrationTypeValue);


	}
	len = _5GS_REGISTRATION_TYPE_LEN;
	*encodedLen = len;
	nasLog("5GS_registration_type encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
