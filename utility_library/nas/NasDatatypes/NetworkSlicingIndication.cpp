#include "nas/NasDatatypes/NetworkSlicingIndication.h"

int decodeNetworkSlicingIndication(uint8_t *buffer, uint32_t bufLen, NetworkSlicingIndication_t *networkSlicingIndication, uint32_t* decodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || networkSlicingIndication == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + NETWORK_SLICING_INDICATION_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
		networkSlicingIndication->spare= (buffer[len] & 0xc0) >> 6;
		networkSlicingIndication->dcni= (buffer[len] & 0x20) >> 5;
		networkSlicingIndication->nssci= (buffer[len] & 0x10) >> 4;

	}else{
		networkSlicingIndication->spare= (buffer[len] & 0x0c) >> 2;
		networkSlicingIndication->dcni= (buffer[len] & 0x02) >> 1;
		networkSlicingIndication->nssci= (buffer[len] & 0x01);

	}
	len = NETWORK_SLICING_INDICATION_LEN;
	*decodedLen = len;
	nasLog("Network_slicing_indication decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeNetworkSlicingIndication(uint8_t *buffer, uint32_t bufLen, NetworkSlicingIndication_t *networkSlicingIndication, uint32_t* encodedLen, bool msbAligned /*false*/)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || networkSlicingIndication == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + NETWORK_SLICING_INDICATION_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	if(msbAligned){
	buffer[len] = (networkSlicingIndication->spare << 6) | (networkSlicingIndication->dcni << 5) | (networkSlicingIndication->nssci << 4);


	}else{
	buffer[len] = (networkSlicingIndication->spare << 2) | (networkSlicingIndication->dcni << 1) | (networkSlicingIndication->nssci);


	}
	len = NETWORK_SLICING_INDICATION_LEN;
	*encodedLen = len;
	nasLog("Network_slicing_indication encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
