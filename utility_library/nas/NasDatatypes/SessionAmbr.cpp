#include "nas/NasDatatypes/SessionAmbr.h"

int decodeSessionAmbr(uint8_t *buffer, uint32_t bufLen, SessionAmbr_t *sessionAmbr, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || sessionAmbr == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + SESSION_AMBR_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	sessionAmbr->lengthOfSessionAmbrContents = buffer[len++];

	uint8_t ieLen = sessionAmbr->lengthOfSessionAmbrContents;

	if((ieLen > SESSION_AMBR_LEN) || (ieLen < SESSION_AMBR_LEN)) {
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


	sessionAmbr->unitForSessionAmbrForDownlink = buffer[len++];

	sessionAmbr->sessionAmbrForDownlink |= ((uint16_t)buffer[len++]) << 8;
	sessionAmbr->sessionAmbrForDownlink |= ((uint16_t)buffer[len++]);

	sessionAmbr->unitForSessionAmbrForUplink = buffer[len++];

	sessionAmbr->sessionAmbrForUplink |= ((uint16_t)buffer[len++]) << 8;
	sessionAmbr->sessionAmbrForUplink |= ((uint16_t)buffer[len++]);


	*decodedLen = len;
	nasLog("Session_Ambr decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeSessionAmbr(uint8_t *buffer, uint32_t bufLen, SessionAmbr_t *sessionAmbr, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || sessionAmbr == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + SESSION_AMBR_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (sessionAmbr->lengthOfSessionAmbrContents);
	uint8_t ieLen = sessionAmbr->lengthOfSessionAmbrContents;

	if((ieLen > SESSION_AMBR_LEN) || (ieLen < SESSION_AMBR_LEN)) {
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

	buffer[len++] = (sessionAmbr->unitForSessionAmbrForDownlink);
	buffer[len++] = (sessionAmbr->sessionAmbrForDownlink) >> 8;
	buffer[len++] = (sessionAmbr->sessionAmbrForDownlink);
	buffer[len++] = (sessionAmbr->unitForSessionAmbrForUplink);
	buffer[len++] = (sessionAmbr->sessionAmbrForUplink) >> 8;
	buffer[len++] = (sessionAmbr->sessionAmbrForUplink);

	*encodedLen = len;
	nasLog("Session_Ambr encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
