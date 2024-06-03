#include "nas/NasDatatypes/UplinkDataStatus.h"

int decodeUplinkDataStatus(uint8_t *buffer, uint32_t bufLen,
			           UplinkDataStatus_t *uplinkDataStatus, uint32_t* decodedLen) {
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || uplinkDataStatus == NULL
         || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        errno = E_NAS_ERROR_INVALID_INPUT;
		return FAILURE;
	}

    /* For coding style consistency using assignment instead of memcpy */
    uplinkDataStatus->len = *(buffer + len);
    
    len += sizeof(uint8_t);
    
	if(len + UPLINK_DATA_STATUS_LEN > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}
    
    /*variable to decode psi*/
    uint16_t var;
    var = *(buffer + len + 1);
    var = (var << 8) | *(buffer + len);
    uplinkDataStatus->psi = var;

    len += UPLINK_DATA_STATUS_LEN;

    nasLog("decoded uplinkDataStatus length : %d | " 
           "decoded uplinkDataStatus psi : %d | "
           "decoded length : %d", uplinkDataStatus->len, 
           uplinkDataStatus->psi, len);  

    *decodedLen = len;
    nasLogEXIT;
	return SUCCESS;

}

int encodeUplinkDataStatus(uint8_t *buffer, uint32_t bufLen,
                       UplinkDataStatus_t *uplinkDataStatus, uint32_t *encodedLen) {
	nasLogENTRY;
    uint32_t len = 0;

    if(buffer == NULL || bufLen == 0 || uplinkDataStatus == NULL
         || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    *(buffer + len) = uplinkDataStatus->len;

    len += sizeof(uint8_t);

	if(len + UPLINK_DATA_STATUS_LEN > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

    *(buffer + len) = uplinkDataStatus->psi;
    *(buffer + len + 1) = (uplinkDataStatus->psi >> 8);
	
    len += UPLINK_DATA_STATUS_LEN;
    
    nasLog("decoded uplinkDataStatus length : %d | " 
           "decoded uplinkDataStatus psi : %d | "
           "decoded length : %d", uplinkDataStatus->len, 
           uplinkDataStatus->psi, len);  

    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}

