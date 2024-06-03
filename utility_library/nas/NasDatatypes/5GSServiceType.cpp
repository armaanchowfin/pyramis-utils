#include "nas/NasDatatypes/5GSServiceType.h"

int decode5GSServType(uint8_t *buffer, uint32_t bufLen,
			           _5gServiceType_t *servType, uint32_t* decodedLen) {
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || servType == NULL || decodedLen == NULL)
    {
		higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        errno = E_NAS_ERROR_INVALID_INPUT;
		return FAILURE;
	}

    if(len + _5G_SERV_TYPE_LEN > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    /* For coding style consistency in service request datatypes
     * using assignment instead of memcpy*/
    servType->value = *buffer >> 4;
    
    /* According to the spec TS 124 501 v 15.2.1 Table 9.11.3.50.1
     * any service type from E_UNUSED_1 to E_UNUSED_3 should be taken as 
     * signaling type and from E_UNUSED_4 to E_UNUSED_6 should be taken as data
     * type of service request if fetched by network*/
    uint8_t _val = servType->value;
 
    _val = (_val >= E_UNUSED_1 && _val <= E_UNUSED_3) ? E_SIGNALLING : _val;
    _val = (_val >= E_UNUSED_4 && _val <= E_UNUSED_6) ? E_DATA : _val;
    
    servType->value = _val;
    
    len += _5G_SERV_TYPE_LEN;
    
    nasLog("5gservType : value %d", servType->value);

    *decodedLen = len;
    nasLogEXIT;
	return SUCCESS;

}

int encode5GSServType(uint8_t *buffer, uint32_t bufLen,
                       _5gServiceType_t *servType, uint32_t *encodedLen) {
	nasLogENTRY;
	uint32_t len = 0;

    if(buffer == NULL || bufLen == 0 || servType == NULL || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

	if(len + _5G_SERV_TYPE_LEN > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

    /* For coding style consistency in service request datatypes
     * using assignment instead of memcpy*/
    uint8_t value = servType->value; 
    
    *buffer = value << 4; 
	
    nasLog("5gservType : value %d", servType->value );
    
    len += _5G_SERV_TYPE_LEN;

    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}

