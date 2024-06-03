#include "nas/NasDatatypes/PDUSessionResultCause.h"

int decodePDUSessionResultCause(uint8_t *buffer,uint32_t bufLen
     ,pduSessionResultCause_t *msgStruct, uint32_t *decodedLen) {

	nasLogENTRY;
    uint32_t len = 0;
    uint32_t decLen = 0;
    int rc = 0;

    if(buffer == NULL || bufLen == 0 ||
    msgStruct == NULL || decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    msgStruct->length = *(buffer + len); 
    msgStruct->length = ((msgStruct->length << 8) | *(buffer + len + 1));
    
    if(msgStruct->length > MAX_NO_OF_PSI_CAUSE) {
        higLog("%s", "Invalid Length");
        nasLogEXIT;
        return FAILURE;
    }
    
    len = len + sizeof(uint16_t);

    if(len + (msgStruct->length) > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }
    
    nasLog("length %d bytes",msgStruct->length);


    for(uint8_t count = 0; count < msgStruct->length/2; count++) {
    
        msgStruct->psiCause[count].psi  = *(buffer + len);
        len++;
        
        if(msgStruct->psiCause[count].psi > PDU_SESSION_IDENTITY_VALUE_15) {
            higLog("%s", "Invalid PDU Session ID");
            nasLogEXIT;
            return FAILURE;
        }

        if(decode5GMMCause((buffer + len),bufLen - len,
            &(msgStruct->psiCause[count]._5gmmCause),&decLen) == FAILURE) {
            higLog("Decode 5GSMCause Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += decLen;
        nasLog("PDU Session ID : %d Cause : %d",msgStruct->psiCause[count].psi
        ,msgStruct->psiCause[count]._5gmmCause);

    }
    
	
    *decodedLen += len;
    nasLog("pduSessionResultCause Decoded | decoded %d bytes",*decodedLen);
	nasLogEXIT;
        return SUCCESS;
}

int encodePDUSessionResultCause(uint8_t *buffer, uint32_t bufLen
     ,pduSessionResultCause_t *msgStruct,uint32_t* encodedLen) {

	nasLogENTRY;
    uint32_t len = 0;
    uint32_t encLen = 0;
    int rc = 0;
	
    if(bufLen == 0 || buffer == NULL ||
	msgStruct == NULL || encodedLen == NULL) {
		higLog("%s", "Failure Encoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	*(buffer + len) = (msgStruct->length >> 8);
	*(buffer + len + 1) = (msgStruct->length);
    
    if(msgStruct->length > MAX_NO_OF_PSI_CAUSE) {
        higLog("%s", "Invalid Length");
        nasLogEXIT;
        return FAILURE;
    }
    
    len = len + sizeof(uint16_t);

	if (len + msgStruct->length > bufLen) {
		higLog("%s","Buffer Capacity Insfficient");
		errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

    nasLog("length %d bytes",msgStruct->length);


    for(uint8_t count = 0; count < msgStruct->length/2; count++) {
    
        *(buffer + len)  = msgStruct->psiCause[count].psi;
        len++;
        
        if(msgStruct->psiCause[count].psi > PDU_SESSION_IDENTITY_VALUE_15) {
            higLog("%s", "Invalid PDU Session ID");
            nasLogEXIT;
            return FAILURE;
        }

        if(encode5GMMCause((buffer + len),bufLen - len,
            &(msgStruct->psiCause[count]._5gmmCause),&encLen) == FAILURE) {
            higLog("Encode 5GSMCause Failed");
            nasLogEXIT;
            return FAILURE;
        }
        len += encLen;
        nasLog("PDU Session ID : %d Cause : %d",msgStruct->psiCause[count].psi
        ,msgStruct->psiCause[count]._5gmmCause);

    }

    *encodedLen += len;

    nasLogEXIT;
	return SUCCESS;
}


