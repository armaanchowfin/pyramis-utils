#include "nas/NasDatatypes/5G-S-TMSI.h"

int decode5gsTmsi(uint8_t *buffer, uint32_t bufLen,
			           _5gsTmsi_t *_5gsTmsi, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint32_t decLen = 0;

    if(buffer == NULL || bufLen == 0 || _5gsTmsi == NULL || decodedLen == NULL)
    {
		higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        errno = E_NAS_ERROR_INVALID_INPUT;
		return FAILURE;
	}

    if(len + _5G_S_TMSI_LEN > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }
   
    _5gsTmsi->len = *(buffer);
    _5gsTmsi->len = ((_5gsTmsi->len << 8) | *(buffer + 1));
    
    len += sizeof(uint16_t);

    /*set bits must be one*/
    _5gsTmsi->setBits = 1;
    _5gsTmsi->idType = *(buffer + len);

    /*Checking if ID type is TMSI type or not and if not return failure*/
	if (_5gsTmsi->idType != _5G_S_TMSI_ID_TYPE) {
		higLog("%s", "Identity type incorrect must be 5G-S-TMSI type");
		errno = E_NAS_SUBSCRIPTION_ID_NOT_FOUND;
		nasLogEXIT;
		return FAILURE;
	}

    len += sizeof(uint8_t);

    /*variable to store and set bit fields value accordingly*/
    uint16_t var;
    var = *(buffer + len);
    var = (var << 8 | *(buffer + len + 1));
    _5gsTmsi->amfSetID = (var >> 6);
    _5gsTmsi->amfPointer = (var & 0x3F);
    
    len += sizeof(uint16_t);

    if(decodeTmsi(buffer + len, bufLen - len,&(_5gsTmsi->_5gTmsi), &decLen)
                                                                == FAILURE) {
            higLog("Decoding 5gTmsi Failed");
            nasLogEXIT;
            return FAILURE;
    }

    len += decLen;

    nasLog("5G_S_TMSI decoded | len %d |  idType  %d | amfSetID %d | amfPointer %d"
           " | 5gTMSI value %d", _5gsTmsi->len , _5gsTmsi->idType ,
           _5gsTmsi->amfSetID , _5gsTmsi->amfPointer , _5gsTmsi->_5gTmsi);

    *decodedLen = len;
    nasLogEXIT;
	return SUCCESS;

}

int encode5gsTmsi(uint8_t *buffer, uint32_t bufLen,
                       _5gsTmsi_t *_5gsTmsi, uint32_t *encodedLen)
{
	nasLogENTRY;
    unsigned int len = 0;
    uint32_t encLen = 0;


    if(buffer == NULL || bufLen == 0 || _5gsTmsi == NULL || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

	if(len + _5G_S_TMSI_LEN > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
		errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

    *(buffer + len) = (_5gsTmsi->len >> 8); 
    *(buffer + len + 1) = (_5gsTmsi->len); 
  
    len += sizeof(uint16_t);

    *(buffer + len) = ((_5gsTmsi->idType) | 0xF0);/*since set bits must be 1*/

    len += sizeof(uint8_t);

    /*Checking if ID type is TMSI type or not and if not return failure*/
	if(_5gsTmsi->idType != _5G_S_TMSI_ID_TYPE) {
		higLog("%s", "Identity type incorrect must be 5G-S-TMSI type");
		errno = E_NAS_SUBSCRIPTION_ID_NOT_FOUND;
		nasLogEXIT;
		return FAILURE;
	}

    *(buffer + len) = (_5gsTmsi->amfSetID >> 2); 
    *(buffer + len + 1) = ((_5gsTmsi->amfSetID & 0x03) << 6) | (_5gsTmsi->amfPointer);
    
    len += sizeof(uint16_t);
    
    if(encodeTmsi(buffer + len, bufLen - len,&(_5gsTmsi->_5gTmsi), &encLen) 
                                                                == FAILURE) {
            higLog("Encoding 5gTmsi Failed");
            nasLogEXIT;
            return FAILURE;
    }

    len += encLen;

    nasLog("5G_S_TMSI encoded | len %d |  idType  %d | amfSetID %d | amfPointer %d"
           "| 5gTMSI value %d", _5gsTmsi->len , _5gsTmsi->idType
           , _5gsTmsi->amfSetID , _5gsTmsi->amfPointer , _5gsTmsi->_5gTmsi);

    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}

