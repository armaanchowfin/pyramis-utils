#include <bits/stdc++.h>
#include "common/include/setup.h"
#include <stdlib.h>
#include "platform/include/platform.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/PduSessionModificationReject.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/sessionMgmt/_5gsmMsgApi.h"

int decodePduSessionModificationReject(uint8_t *buffer, PduSessionModificationReject_t *pduSessionModificationReject, uint32_t decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t decodeLen = 0;

	if(buffer == NULL || pduSessionModificationReject == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	pduSessionModificationReject->presenceMask = 0;
	while(len < decodedLen) {
		uint8_t iei = *(buffer + len);
		len++;
		switch(iei) {
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_REJECT_IEI_BACK_OFF_TIMER_VALUE: {
				decodeLen = 0;
				if(decodeGprsTimer3(buffer + len, decodeLen - len, 
					&pduSessionModificationReject->backOffTimerValue, &decodeLen) == FAILURE)
				{
					higLog("decode GPRS timer 3 Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationReject->presenceMask |= E_PDU_SESSION_MODIFICATION_REJECT_IEI_BACK_OFF_TIMER_VALUE;
				nasLog("Back-off timer value decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_REJECT_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS: {
				decodeLen = 0;
				if(decodeExtendedProtocolConfigurationOptions(buffer + len, decodeLen - len, 
					&pduSessionModificationReject->extendedProtocolConfigurationOptions, &decodeLen) == FAILURE)
				{
					higLog("decode Extended protocol configuration options Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationReject->presenceMask |= E_PDU_SESSION_MODIFICATION_REJECT_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS;
				nasLog("Extended protocol configuration options decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
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

int encodePduSessionModificationReject(uint8_t *buffer, uint32_t bufLen, PduSessionModificationReject_t *pduSessionModificationReject, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t encodeLen = 0;

	if(buffer == NULL || pduSessionModificationReject == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	/* Optional field*/
	if((pduSessionModificationReject->presenceMask & PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT)
			==PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_REJECT_IEI_BACK_OFF_TIMER_VALUE;
		len++;
		encodeLen = 0;
		if(encodeGprsTimer3(buffer + len, bufLen - len, 
			&pduSessionModificationReject->backOffTimerValue, &encodeLen) == FAILURE)
		{
			higLog("encode GPRS timer 3 Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Back-off timer value is not set");
	}
	/* Optional field*/
	if((pduSessionModificationReject->presenceMask & PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
			==PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_REJECT_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS;
		len++;
		encodeLen = 0;
		if(encodeExtendedProtocolConfigurationOptions(buffer + len, bufLen - len, 
			&pduSessionModificationReject->extendedProtocolConfigurationOptions, &encodeLen) == FAILURE)
		{
			higLog("encode Extended protocol configuration options Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Extended protocol configuration options is not set");
	}

*encodedLen = len;
	nasLog("successfully encoded len = %d", len);
	nasLogEXIT;
	return SUCCESS;

}
