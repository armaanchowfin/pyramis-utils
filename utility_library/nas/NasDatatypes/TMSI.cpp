#include "nas/NasDatatypes/TMSI.h"

int decodeTmsi(uint8_t *buffer, uint32_t bufLen,
			           _5gTmsi_t *_5gTmsi, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || _5gTmsi == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        nasLogEXIT;
        errno = E_NAS_ERROR_INVALID_INPUT;
		return FAILURE;
	}

    if(len + _5G_TMSI_LEN > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    _5gTmsi->value = *(buffer);
    _5gTmsi->value = ((_5gTmsi->value << 8) | *(buffer + 1));
    _5gTmsi->value = ((_5gTmsi->value << 8) | *(buffer + 2));
    _5gTmsi->value = ((_5gTmsi->value << 8) | *(buffer + 3));
    
    len += sizeof(*_5gTmsi);
    nasLog("Tmsi : value %d", _5gTmsi->value);

    *decodedLen = len;
    nasLogEXIT;
	return SUCCESS;

}

int encodeTmsi(uint8_t *buffer, uint32_t bufLen,
                       _5gTmsi_t *_5gTmsi, uint32_t *encodedLen)
{
	nasLogENTRY;
    unsigned int len = 0;

    if(buffer == NULL || bufLen == 0 || _5gTmsi == NULL || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

	if(len + _5G_TMSI_LEN > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}
    
    *(buffer)     = (_5gTmsi->value >> 24);
    *(buffer + 1) = (_5gTmsi->value >> 16);
    *(buffer + 2) = (_5gTmsi->value >> 8);
    *(buffer + 3) = (_5gTmsi->value);

    len += sizeof(*_5gTmsi);
	
	nasLog("Tmsi : value %d", _5gTmsi->value);
	
	*encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}

