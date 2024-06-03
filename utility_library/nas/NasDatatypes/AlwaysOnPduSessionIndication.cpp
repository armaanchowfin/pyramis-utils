#include "nas/NasDatatypes/AlwaysOnPduSessionIndication.h"

int decodeAlwaysOnPduSessionIndication(uint8_t *buffer, uint32_t bufLen, AlwaysOnPduSessionIndication_t *alwaysOnPduSessionIndication, uint32_t* decodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || alwaysOnPduSessionIndication == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + ALWAYS_ON_PDU_SESSION_INDICATION_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
		alwaysOnPduSessionIndication->spare= (buffer[len] & 0xe0) >> 5;
		alwaysOnPduSessionIndication->apsi= (buffer[len] & 0x10) >> 4;

	}else{
		alwaysOnPduSessionIndication->spare= (buffer[len] & 0x0e) >> 1;
		alwaysOnPduSessionIndication->apsi= (buffer[len] & 0x01);

	}
	len = ALWAYS_ON_PDU_SESSION_INDICATION_LEN;
	*decodedLen = len;
	nasLog("Always-on_PDU_session_indication decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeAlwaysOnPduSessionIndication(uint8_t *buffer, uint32_t bufLen, AlwaysOnPduSessionIndication_t *alwaysOnPduSessionIndication, uint32_t* encodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || alwaysOnPduSessionIndication == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + ALWAYS_ON_PDU_SESSION_INDICATION_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
	buffer[len] = (alwaysOnPduSessionIndication->spare << 5) | (alwaysOnPduSessionIndication->apsi << 4);


	}else{
	buffer[len] = (alwaysOnPduSessionIndication->spare << 1) | (alwaysOnPduSessionIndication->apsi);


	}
	len = ALWAYS_ON_PDU_SESSION_INDICATION_LEN;
	*encodedLen = len;
	nasLog("Always-on_PDU_session_indication encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
