#include "nas/NasDatatypes/ExtendedProtocolConfigurationOptions.h"

int decodeExtendedProtocolConfigurationOptions(uint8_t *buffer, uint32_t bufLen, ExtendedProtocolConfigurationOptions_t *extendedProtocolConfigurationOptions, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || extendedProtocolConfigurationOptions == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents |= ((uint16_t)buffer[len++]) << 8;
	extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents |= ((uint16_t)buffer[len++]);

	uint16_t ieLen = extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents;

	if((ieLen > EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MAX_LEN) || (ieLen < EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MIN_LEN)) {
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


	memcpy(extendedProtocolConfigurationOptions->extendedProtocolConfigurationOptionsContents, buffer + len, extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents);
	len += extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents;

	*decodedLen = len;
	nasLog("Extended_protocol_configuration_options decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeExtendedProtocolConfigurationOptions(uint8_t *buffer, uint32_t bufLen, ExtendedProtocolConfigurationOptions_t *extendedProtocolConfigurationOptions, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || extendedProtocolConfigurationOptions == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents) >> 8;
	buffer[len++] = (extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents);
	uint16_t ieLen = extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents;

	if((ieLen > EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MAX_LEN) || (ieLen < EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MIN_LEN)) {
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

	memcpy(buffer + len, extendedProtocolConfigurationOptions->extendedProtocolConfigurationOptionsContents, extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents);
	len += extendedProtocolConfigurationOptions->lengthOfExtendedProtocolConfigurationOptionsContents;


	*encodedLen = len;
	nasLog("Extended_protocol_configuration_options encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
