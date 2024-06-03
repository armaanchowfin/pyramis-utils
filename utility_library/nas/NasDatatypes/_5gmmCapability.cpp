#include "nas/NasDatatypes/_5gmmCapability.h"

int decode5gmmCapability(uint8_t *buffer, uint32_t bufLen, _5gmmCapability_t *_5gmmCapability, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gmmCapability == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GMM_CAPABILITY_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	_5gmmCapability->lengthOf5gmmCapabilityContents = buffer[len++];

	uint8_t ieLen = _5gmmCapability->lengthOf5gmmCapabilityContents;

	if((ieLen > _5GMM_CAPABILITY_MAX_LEN) || (ieLen < _5GMM_CAPABILITY_MIN_LEN)) {
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


	_5gmmCapability->spare= (buffer[len] & 0xf8) >> 3;
	_5gmmCapability->lpp= (buffer[len] & 0x04) >> 2;
	_5gmmCapability->hoAttach= (buffer[len] & 0x02) >> 1;
	_5gmmCapability->s1Mode= (buffer[len] & 0x01);
	len++;

	uint32_t spare1Len = _5gmmCapability->lengthOf5gmmCapabilityContents - (len - 1);
	/* need to find better way for finding the remaining length */
	memcpy(_5gmmCapability->spare1, buffer + len, spare1Len);
	len += spare1Len;

	*decodedLen = len;
	nasLog("5GMM_capability decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encode5gmmCapability(uint8_t *buffer, uint32_t bufLen, _5gmmCapability_t *_5gmmCapability, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gmmCapability == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GMM_CAPABILITY_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (_5gmmCapability->lengthOf5gmmCapabilityContents);
	uint8_t ieLen = _5gmmCapability->lengthOf5gmmCapabilityContents;

	if((ieLen > _5GMM_CAPABILITY_MAX_LEN) || (ieLen < _5GMM_CAPABILITY_MIN_LEN)) {
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

	buffer[len] = (_5gmmCapability->spare << 3) | (_5gmmCapability->lpp << 2) | (_5gmmCapability->hoAttach << 1) | (_5gmmCapability->s1Mode);
	len++;

	uint32_t spare1Len = _5gmmCapability->lengthOf5gmmCapabilityContents - (len - 1);
	/* need to find better way for finding the remaining length*/
	memcpy(buffer + len, _5gmmCapability->spare1, spare1Len);
	len += spare1Len;


	*encodedLen = len;
	nasLog("5GMM_capability encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
