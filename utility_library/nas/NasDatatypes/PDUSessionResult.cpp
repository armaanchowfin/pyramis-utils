#include "nas/NasDatatypes/PDUSessionResult.h"

int decodePDUSessionResult(uint8_t *buffer, uint32_t bufLen,
			           pduSessionResult_t *pduSessionResult, uint32_t* decodedLen) {
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || pduSessionResult == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        errno = E_NAS_ERROR_INVALID_INPUT;
		return FAILURE;
	}

    pduSessionResult->len = *(buffer + len);
    
    len += sizeof(uint8_t);

    if(len + PDU_SESSION_RESULT_LEN > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    /*variable to decode psi*/
    uint16_t var = 0;
    var = *(buffer + len + 1);
    var = (var << 8) | *(buffer + len);
    pduSessionResult->psi = var;
    
    len += PDU_SESSION_RESULT_LEN;
    
    nasLog("decoded pduSessionResult length : %d | " 
           "decoded pduSessionResult psi : %d | "
           "decoded length : %d", pduSessionResult->len, 
           pduSessionResult->psi, len);  
    
    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS; 
}

int encodePDUSessionResult(uint8_t *buffer, uint32_t bufLen,
                       pduSessionResult_t *pduSessionResult, uint32_t *encodedLen) {
	nasLogENTRY;
    uint32_t len = 0;

    if(buffer == NULL || bufLen == 0 || pduSessionResult == NULL || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    *(buffer + len) = pduSessionResult->len;
    
    len += sizeof(uint8_t);
	
    if(len + PDU_SESSION_RESULT_LEN > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}
        
    *(buffer + len) = pduSessionResult->psi;
    *(buffer + len + 1) = (pduSessionResult->psi >> 8);

    len += PDU_SESSION_RESULT_LEN;
    
    nasLog("encoded pduSessionResult length : %d | " 
           "encoded pduSessionResult psi : %d | "
           "encoded length : %d", pduSessionResult->len, 
           pduSessionResult->psi, len);  

    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;

}


