#include <bits/stdc++.h>
#include "common/include/setup.h"
#include <stdlib.h>
#include "platform/include/platform.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/PduSessionModificationComplete.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/sessionMgmt/_5gsmMsgApi.h"

int decodePduSessionModificationComplete(uint8_t *buffer, PduSessionModificationComplete_t *pduSessionModificationComplete, uint32_t decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t decodeLen = 0;

	if(buffer == NULL || pduSessionModificationComplete == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	pduSessionModificationComplete->presenceMask = 0;
	while(len < decodedLen) {
		uint8_t iei = *(buffer + len);
		len++;
		switch(iei) {
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_COMPLETE_IEI_5GSM_CAUSE: {
				decodeLen = 0;
				if(decode5gsmCause(buffer + len, decodeLen - len, 
					&pduSessionModificationComplete->_5gsmCause, &decodeLen) == FAILURE)
				{
					higLog("decode 5GSM cause Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationComplete->presenceMask |= E_PDU_SESSION_MODIFICATION_COMPLETE_IEI_5GSM_CAUSE;
				nasLog("5GSM cause decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			default : {
				higLog("Unknown Optional Field IEI 0x%02x", iei);
				errno = E_NAS_ERROR_INVALID_PDU;
				nasLogEXIT;
				return FAILURE;
			}
		}
	}
	if( len > decodedLen) {
		higLog("ERROR: crossed buffer boundaries");
		errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

	nasLog("successfully decoded len = %d", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodePduSessionModificationComplete(uint8_t *buffer, uint32_t bufLen, PduSessionModificationComplete_t *pduSessionModificationComplete, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t encodeLen = 0;

	if(buffer == NULL || pduSessionModificationComplete == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	/* Optional field*/
	if((pduSessionModificationComplete->presenceMask & PDU_SESSION_MODIFICATION_COMPLETE_5GSM_CAUSE_PRESENT)
			==PDU_SESSION_MODIFICATION_COMPLETE_5GSM_CAUSE_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_COMPLETE_IEI_5GSM_CAUSE;
		len++;
		encodeLen = 0;
		if(encode5gsmCause(buffer + len, bufLen - len, 
			&pduSessionModificationComplete->_5gsmCause, &encodeLen) == FAILURE)
		{
			higLog("encode 5GSM cause Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("5GSM cause is not set");
	}

*encodedLen = len;
	nasLog("successfully encoded len = %d", len);
	nasLogEXIT;
	return SUCCESS;

}
