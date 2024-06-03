#include "nas/NasDatatypes/IntegrityProtectionMaximumDataRate.h"

int decodeIntegrityProtectionMaximumDataRate(uint8_t *buffer, uint32_t bufLen, IntegrityProtectionMaximumDataRate_t *integrityProtectionMaximumDataRate, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || integrityProtectionMaximumDataRate == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	integrityProtectionMaximumDataRate->maximumDataRatePerUeForUserPlaneIntegrityProtectionForUplink = buffer[len++];

	integrityProtectionMaximumDataRate->maximumDataRatePerUeForUserPlaneIntegrityProtectionForDownlink = buffer[len++];


	*decodedLen = len;
	nasLog("Integrity_protection_maximum_data_rate decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeIntegrityProtectionMaximumDataRate(uint8_t *buffer, uint32_t bufLen, IntegrityProtectionMaximumDataRate_t *integrityProtectionMaximumDataRate, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || integrityProtectionMaximumDataRate == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (integrityProtectionMaximumDataRate->maximumDataRatePerUeForUserPlaneIntegrityProtectionForUplink);
	buffer[len++] = (integrityProtectionMaximumDataRate->maximumDataRatePerUeForUserPlaneIntegrityProtectionForDownlink);

	*encodedLen = len;
	nasLog("Integrity_protection_maximum_data_rate encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
