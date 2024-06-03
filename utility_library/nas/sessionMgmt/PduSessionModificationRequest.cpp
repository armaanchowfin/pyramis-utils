#include <bits/stdc++.h>
#include "common/include/setup.h"
#include <stdlib.h>
#include "platform/include/platform.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/PduSessionModificationRequest.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/sessionMgmt/_5gsmMsgApi.h"

int decodePduSessionModificationRequest(uint8_t *buffer, PduSessionModificationRequest_t *pduSessionModificationRequest, uint32_t decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t decodeLen = 0;

	if(buffer == NULL || pduSessionModificationRequest == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	pduSessionModificationRequest->presenceMask = 0;
	while(len < decodedLen) {
		uint8_t iei = *(buffer + len);
		len++;
		switch(iei) {
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_REQUEST_IEI_5GSM_CAUSE: {
				decodeLen = 0;
				if(decode5gsmCause(buffer + len, decodeLen - len, 
					&pduSessionModificationRequest->_5gsmCause, &decodeLen) == FAILURE)
				{
					higLog("decode 5GSM cause Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationRequest->presenceMask |= PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT;
				nasLog("5GSM cause decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_REQUEST_IEI_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS: {
				decodeLen = 0;
				if(decodeMaximumNumberOfSupportedPacketFilters(buffer + len, decodeLen - len, 
					&pduSessionModificationRequest->maximumNumberOfSupportedPacketFilters, &decodeLen) == FAILURE)
				{
					higLog("decode Maximum number of supported packet filters Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationRequest->presenceMask |= PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT;
				nasLog("Maximum number of supported packet filters decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_REQUEST_IEI_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE: {
				decodeLen = 0;
				if(decodeIntegrityProtectionMaximumDataRate(buffer + len, decodeLen - len, 
					&pduSessionModificationRequest->integrityProtectionMaximumDataRate, &decodeLen) == FAILURE)
				{
					higLog("decode Integrity protection maximum data rate Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationRequest->presenceMask |= PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT;
				nasLog("Integrity protection maximum data rate decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_REQUEST_IEI_REQUESTED_QOS_RULES: {
				decodeLen = 0;
				if(decodeQosRules(buffer + len, decodeLen - len, 
					&pduSessionModificationRequest->requestedQosRules, &decodeLen) == FAILURE)
				{
					higLog("decode QoS rules Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationRequest->presenceMask |= PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT;
				nasLog("Requested QoS rules decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_REQUEST_IEI_REQUESTED_QOS_FLOW_DESCRIPTIONS: {
				decodeLen = 0;
				if(decodeQosFlowDescriptions(buffer + len, decodeLen - len, 
					&pduSessionModificationRequest->requestedQosFlowDescriptions, &decodeLen) == FAILURE)
				{
					higLog("decode QoS flow descriptions Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationRequest->presenceMask |= PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT;
				nasLog("Requested QoS flow descriptions decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_REQUEST_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS: {
				decodeLen = 0;
				if(decodeExtendedProtocolConfigurationOptions(buffer + len, decodeLen - len, 
					&pduSessionModificationRequest->extendedProtocolConfigurationOptions, &decodeLen) == FAILURE)
				{
					higLog("decode Extended protocol configuration options Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationRequest->presenceMask |= PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
				nasLog("Extended protocol configuration options decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			default : {
				/*Might be a half octet IEI */
				uint8_t iei2 = iei >> 4;
				len--;  /* IE will be within this octet */
				switch(iei2) {
					case E_PDU_SESSION_MODIFICATION_REQUEST_IEI_ALWAYS_ON_PDU_SESSION_REQUESTED: {
					decodeLen = 0;
					if(decodeAlwaysOnPduSessionRequested(buffer + len, decodeLen - len, 
						&pduSessionModificationRequest->alwaysOnPduSessionRequested, &decodeLen) == FAILURE)
					{
						higLog("decode Always-on PDU session requested Failed");
						nasLogEXIT;
						return FAILURE;
					}
					len += decodeLen;
					pduSessionModificationRequest->presenceMask |= E_PDU_SESSION_MODIFICATION_REQUEST_IEI_ALWAYS_ON_PDU_SESSION_REQUESTED;
					nasLog("Always-on PDU session requested decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
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

int encodePduSessionModificationRequest(uint8_t *buffer, uint32_t bufLen, PduSessionModificationRequest_t *pduSessionModificationRequest, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t encodeLen = 0;

	if(buffer == NULL || pduSessionModificationRequest == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	/* Optional field*/
	if((pduSessionModificationRequest->presenceMask & PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT)
			==PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_REQUEST_IEI_5GSM_CAUSE;
		len++;
		encodeLen = 0;
		if(encode5gsmCause(buffer + len, bufLen - len, 
			&pduSessionModificationRequest->_5gsmCause, &encodeLen) == FAILURE)
		{
			higLog("encode 5GSM cause Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("5GSM cause is not set");
	}
	/* Optional field*/
	if((pduSessionModificationRequest->presenceMask & PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT)
			==PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_REQUEST_IEI_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS;
		len++;
		encodeLen = 0;
		if(encodeMaximumNumberOfSupportedPacketFilters(buffer + len, bufLen - len, 
			&pduSessionModificationRequest->maximumNumberOfSupportedPacketFilters, &encodeLen) == FAILURE)
		{
			higLog("encode Maximum number of supported packet filters Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Maximum number of supported packet filters is not set");
	}
	/* Optional field*/
	if((pduSessionModificationRequest->presenceMask & PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT)
			==PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT) {
		buffer[len] = (E_PDU_SESSION_MODIFICATION_REQUEST_IEI_ALWAYS_ON_PDU_SESSION_REQUESTED << 4);
		/*length not incremented since half octect IEI*/
		encodeLen = 0;
		if(encodeAlwaysOnPduSessionRequested(buffer + len, bufLen - len, 
			&pduSessionModificationRequest->alwaysOnPduSessionRequested, &encodeLen) == FAILURE)
		{
			higLog("encode Always-on PDU session requested Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Always-on PDU session requested is not set");
	}
	/* Optional field*/
	if((pduSessionModificationRequest->presenceMask & PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT)
			==PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_REQUEST_IEI_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE;
		len++;
		encodeLen = 0;
		if(encodeIntegrityProtectionMaximumDataRate(buffer + len, bufLen - len, 
			&pduSessionModificationRequest->integrityProtectionMaximumDataRate, &encodeLen) == FAILURE)
		{
			higLog("encode Integrity protection maximum data rate Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Integrity protection maximum data rate is not set");
	}
	/* Optional field*/
	if((pduSessionModificationRequest->presenceMask & PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT)
			==PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_REQUEST_IEI_REQUESTED_QOS_RULES;
		len++;
		encodeLen = 0;
		if(encodeQosRules(buffer + len, bufLen - len, 
			&pduSessionModificationRequest->requestedQosRules, &encodeLen) == FAILURE)
		{
			higLog("encode QoS rules Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Requested QoS rules is not set");
	}
	/* Optional field*/
	if((pduSessionModificationRequest->presenceMask & PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT)
			==PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_REQUEST_IEI_REQUESTED_QOS_FLOW_DESCRIPTIONS;
		len++;
		encodeLen = 0;
		if(encodeQosFlowDescriptions(buffer + len, bufLen - len, 
			&pduSessionModificationRequest->requestedQosFlowDescriptions, &encodeLen) == FAILURE)
		{
			higLog("encode QoS flow descriptions Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Requested QoS flow descriptions is not set");
	}
	/* Optional field*/
	if((pduSessionModificationRequest->presenceMask & PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
			==PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_REQUEST_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS;
		len++;
		encodeLen = 0;
		if(encodeExtendedProtocolConfigurationOptions(buffer + len, bufLen - len, 
			&pduSessionModificationRequest->extendedProtocolConfigurationOptions, &encodeLen) == FAILURE)
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
