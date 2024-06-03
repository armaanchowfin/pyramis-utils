#include "nas/NasDatatypes/PDUSessionStatus.h"

int decodePDUSessionStatus(uint8_t *buffer, uint32_t bufLen,
			           pduSessionStatus_t *pduSessionStatus, uint32_t* decodedLen) {
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || pduSessionStatus == NULL
         || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        errno = E_NAS_ERROR_INVALID_INPUT;
		return FAILURE;
	}

    pduSessionStatus->len = *(buffer + len);
    
    len += sizeof(uint8_t);
    
    if(len + PDU_SESSION_STATUS_LEN > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }
    
    /*variable to decode psi*/
    uint16_t var;
    var = *(buffer + len + 1);
    var = (var << 8) | *(buffer + len);
    pduSessionStatus->psi = var;
    
    len += PDU_SESSION_STATUS_LEN;
    
    nasLog("decoded pduSessionStatus length : %d | " 
           "decoded pduSessionStatus psi : %d | "
           "decoded length : %d", pduSessionStatus->len, 
           pduSessionStatus->psi, len);  

    
    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}

int encodePDUSessionStatus(uint8_t *buffer, uint32_t bufLen,
                       pduSessionStatus_t *pduSessionStatus, uint32_t *encodedLen) {
	nasLogENTRY;
    uint32_t len = 0;

    if(buffer == NULL || bufLen == 0 || pduSessionStatus == NULL || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    *(buffer + len) = pduSessionStatus->len;
    
    len += sizeof(uint8_t);
    
	if(len + PDU_SESSION_STATUS_LEN > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}
    
    *(buffer + len) = pduSessionStatus->psi;
    *(buffer + len + 1) = (pduSessionStatus->psi >> 8);
    
    len += PDU_SESSION_STATUS_LEN;

    nasLog("encoded pduSessionStatus length : %d | " 
           "encoded pduSessionStatus psi : %d | "
           "encoded length : %d", pduSessionStatus->len, 
           pduSessionStatus->psi, len);  

    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;

}

