#include "nas/NasDatatypes/5GMMCause.h"

int decode5GMMCause(uint8_t *buffer, uint32_t bufLen,
	_5gmmCause_t *_5gmmCause, uint32_t* decodedLen) {
	
    nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || bufLen == 0 ||
		_5gmmCause == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}
	
    if(len + SIZE_OF_5GMM_CAUSE > bufLen) {
	    higLog("Buffer incomplete");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}
	
    uint8_t _5gmmCauseValue = buffer[0];

    /*Refer TS24501 9.11.3.2 anything apart from specified enums
     *would be treated as error_unspecified*/
    _5gmmCause->cause = 
    _5gmmCauseValue >= _5GMM_CAUSE_IEI_PROTOCOL_ERROR_UNSPECIFIED 
     ? _5GMM_CAUSE_IEI_PROTOCOL_ERROR_UNSPECIFIED : _5gmmCauseValue;
    
    len += SIZE_OF_5GMM_CAUSE;

    nasLog("5GMMCause Value = %d | currentLen = %d | decoded %d bytes", 
    	_5gmmCause->cause,len,SIZE_OF_5GMM_CAUSE);

    *decodedLen = len;
	
    nasLogEXIT;
	return SUCCESS;
}

int encode5GMMCause(uint8_t *buffer, uint32_t bufLen,
    _5gmmCause_t *_5gmmCause, uint32_t *encodedLen) {
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || _5gmmCause == NULL || encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

	if(len + SIZE_OF_5GMM_CAUSE > bufLen) {
	    higLog("Buffer insufficient");
        errno = E_NAS_ERROR_BUFF_INSUFF;
	    nasLogEXIT;
	    return FAILURE;
	}
    
    /*Refer TS24501 9.11.3.2 anything apart from specified enums
     *would be treated as error_unspecified*/
    _5gmmCause->cause = 
    _5gmmCause->cause >= _5GMM_CAUSE_IEI_PROTOCOL_ERROR_UNSPECIFIED 
     ? _5GMM_CAUSE_IEI_PROTOCOL_ERROR_UNSPECIFIED : _5gmmCause->cause;
    
    buffer[len] = _5gmmCause->cause;
    
    len += SIZE_OF_5GMM_CAUSE;    
    
    nasLog("5GMMCause Value = %d | currentLen = %d | encoded %d bytes", 
    	_5gmmCause->cause,len,SIZE_OF_5GMM_CAUSE);
    
    *encodedLen = len;

    nasLogEXIT;	
	return SUCCESS;
}



