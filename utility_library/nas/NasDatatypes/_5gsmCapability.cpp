#include "nas/NasDatatypes/_5gsmCapability.h"

int decode5gsmCapability(uint8_t *buffer, uint32_t bufLen, _5gsmCapability_t *_5gsmCapability, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gsmCapability == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GSM_CAPABILITY_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	_5gsmCapability->lengthOf5gsmCapabilityContents = buffer[len++];

	uint8_t ieLen = _5gsmCapability->lengthOf5gsmCapabilityContents;

	if((ieLen > _5GSM_CAPABILITY_MAX_LEN) || (ieLen < _5GSM_CAPABILITY_MIN_LEN)) {
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


	_5gsmCapability->spare= (buffer[len] & 0xfc) >> 2;
	_5gsmCapability->mh6Pdu= (buffer[len] & 0x02) >> 1;
	_5gsmCapability->rqos= (buffer[len] & 0x01);
	len++;

	uint32_t spare1Len = _5gsmCapability->lengthOf5gsmCapabilityContents - (len - 1);
	/* need to find better way for finding the remaining length */
	memcpy(_5gsmCapability->spare1, buffer + len, spare1Len);
	len += spare1Len;

	*decodedLen = len;
	nasLog("5GSM_capability decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encode5gsmCapability(uint8_t *buffer, uint32_t bufLen, _5gsmCapability_t *_5gsmCapability, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || _5gsmCapability == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + _5GSM_CAPABILITY_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (_5gsmCapability->lengthOf5gsmCapabilityContents);
	uint8_t ieLen = _5gsmCapability->lengthOf5gsmCapabilityContents;

	if((ieLen > _5GSM_CAPABILITY_MAX_LEN) || (ieLen < _5GSM_CAPABILITY_MIN_LEN)) {
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

	buffer[len] = (_5gsmCapability->spare << 2) | (_5gsmCapability->mh6Pdu << 1) | (_5gsmCapability->rqos);
	len++;

	uint32_t spare1Len = _5gsmCapability->lengthOf5gsmCapabilityContents - (len - 1);
	/* need to find better way for finding the remaining length*/
	memcpy(buffer + len, _5gsmCapability->spare1, spare1Len);
	len += spare1Len;


	*encodedLen = len;
	nasLog("5GSM_capability encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
