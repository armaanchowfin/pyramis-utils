#include <bits/stdc++.h>
#include "common/include/setup.h"
#include <stdlib.h>
#include "platform/include/platform.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/PduSessionModificationCommand.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/sessionMgmt/_5gsmMsgApi.h"

int decodePduSessionModificationCommand(uint8_t *buffer, PduSessionModificationCommand_t *pduSessionModificationCommand, uint32_t decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t decodeLen = 0;

	if(buffer == NULL || pduSessionModificationCommand == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	pduSessionModificationCommand->presenceMask = 0;
	while(len < decodedLen) {
		uint8_t iei = *(buffer + len);
		len++;
		switch(iei) {
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_COMMAND_IEI_SESSION_AMBR: {
				decodeLen = 0;
				if(decodeSessionAmbr(buffer + len, decodeLen - len, 
					&pduSessionModificationCommand->sessionAmbr, &decodeLen) == FAILURE)
				{
					higLog("decode Session-AMBR Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationCommand->presenceMask |= E_PDU_SESSION_MODIFICATION_COMMAND_IEI_SESSION_AMBR;
				nasLog("Session AMBR decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_COMMAND_IEI_RQ_TIMER_VALUE: {
				decodeLen = 0;
				if(decodeGprsTimer(buffer + len, decodeLen - len, 
					&pduSessionModificationCommand->rqTimerValue, &decodeLen) == FAILURE)
				{
					higLog("decode GPRS timer Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationCommand->presenceMask |= E_PDU_SESSION_MODIFICATION_COMMAND_IEI_RQ_TIMER_VALUE;
				nasLog("RQ timer value decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_COMMAND_IEI_AUTHORIZED_QOS_RULES: {
				decodeLen = 0;
				if(decodeQosRules(buffer + len, decodeLen - len, 
					&pduSessionModificationCommand->authorizedQosRules, &decodeLen) == FAILURE)
				{
					higLog("decode QoS rules Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationCommand->presenceMask |= E_PDU_SESSION_MODIFICATION_COMMAND_IEI_AUTHORIZED_QOS_RULES;
				nasLog("Authorized QoS rules decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_COMMAND_IEI_AUTHORIZED_QOS_FLOW_DESCRIPTIONS: {
				decodeLen = 0;
				if(decodeQosFlowDescriptions(buffer + len, decodeLen - len, 
					&pduSessionModificationCommand->authorizedQosFlowDescriptions, &decodeLen) == FAILURE)
				{
					higLog("decode QoS flow descriptions Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationCommand->presenceMask |= E_PDU_SESSION_MODIFICATION_COMMAND_IEI_AUTHORIZED_QOS_FLOW_DESCRIPTIONS;
				nasLog("Authorized QoS flow descriptions decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			/* Decoding Optional field*/
			case E_PDU_SESSION_MODIFICATION_COMMAND_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS: {
				decodeLen = 0;
				if(decodeExtendedProtocolConfigurationOptions(buffer + len, decodeLen - len, 
					&pduSessionModificationCommand->extendedProtocolConfigurationOptions, &decodeLen) == FAILURE)
				{
					higLog("decode Extended protocol configuration options Failed");
					nasLogEXIT;
					return FAILURE;
				}
				len += decodeLen;
				pduSessionModificationCommand->presenceMask |= E_PDU_SESSION_MODIFICATION_COMMAND_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS;
				nasLog("Extended protocol configuration options decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
			}
			break;
			default : {
				/*Might be a half octet IEI */
				uint8_t iei2 = iei >> 4;
				len--;  /* IE will be within this octet */
				switch(iei2) {
					case E_PDU_SESSION_MODIFICATION_COMMAND_IEI_ALWAYS_ON_PDU_SESSION_INDICATION: {
					decodeLen = 0;
					if(decodeAlwaysOnPduSessionIndication(buffer + len, decodeLen - len, 
						&pduSessionModificationCommand->alwaysOnPduSessionIndication, &decodeLen) == FAILURE)
					{
						higLog("decode Always-on PDU session indication Failed");
						nasLogEXIT;
						return FAILURE;
					}
					len += decodeLen;
					pduSessionModificationCommand->presenceMask |= E_PDU_SESSION_MODIFICATION_COMMAND_IEI_ALWAYS_ON_PDU_SESSION_INDICATION;
					nasLog("Always-on PDU session indication decoded | currentLen = %d | decoded %d bytes",len, decodeLen);
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

int encodePduSessionModificationCommand(uint8_t *buffer, uint32_t bufLen, PduSessionModificationCommand_t *pduSessionModificationCommand, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	uint32_t encodeLen = 0;

	if(buffer == NULL || pduSessionModificationCommand == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	/* Optional field*/
	if((pduSessionModificationCommand->presenceMask & PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENT)
			==PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_COMMAND_IEI_SESSION_AMBR;
		len++;
		encodeLen = 0;
		if(encodeSessionAmbr(buffer + len, bufLen - len, 
			&pduSessionModificationCommand->sessionAmbr, &encodeLen) == FAILURE)
		{
			higLog("encode Session-AMBR Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Session AMBR is not set");
	}
	/* Optional field*/
	if((pduSessionModificationCommand->presenceMask & PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_PRESENT)
			==PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_COMMAND_IEI_RQ_TIMER_VALUE;
		len++;
		encodeLen = 0;
		if(encodeGprsTimer(buffer + len, bufLen - len, 
			&pduSessionModificationCommand->rqTimerValue, &encodeLen) == FAILURE)
		{
			higLog("encode GPRS timer Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("RQ timer value is not set");
	}
	/* Optional field*/
	if((pduSessionModificationCommand->presenceMask & PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT)
			==PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT) {
		buffer[len] = (E_PDU_SESSION_MODIFICATION_COMMAND_IEI_ALWAYS_ON_PDU_SESSION_INDICATION << 4);
		/*length not incremented since half octect IEI*/
		encodeLen = 0;
		if(encodeAlwaysOnPduSessionIndication(buffer + len, bufLen - len, 
			&pduSessionModificationCommand->alwaysOnPduSessionIndication, &encodeLen) == FAILURE)
		{
			higLog("encode Always-on PDU session indication Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Always-on PDU session indication is not set");
	}
	/* Optional field*/
	if((pduSessionModificationCommand->presenceMask & PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_PRESENT)
			==PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_COMMAND_IEI_AUTHORIZED_QOS_RULES;
		len++;
		encodeLen = 0;
		if(encodeQosRules(buffer + len, bufLen - len, 
			&pduSessionModificationCommand->authorizedQosRules, &encodeLen) == FAILURE)
		{
			higLog("encode QoS rules Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Authorized QoS rules is not set");
	}
	/* Optional field*/
	if((pduSessionModificationCommand->presenceMask & PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT)
			==PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_COMMAND_IEI_AUTHORIZED_QOS_FLOW_DESCRIPTIONS;
		len++;
		encodeLen = 0;
		if(encodeQosFlowDescriptions(buffer + len, bufLen - len, 
			&pduSessionModificationCommand->authorizedQosFlowDescriptions, &encodeLen) == FAILURE)
		{
			higLog("encode QoS flow descriptions Failed");
			nasLogEXIT;
			return FAILURE;
		}
		len += encodeLen;
	} else {
		nasLog("Authorized QoS flow descriptions is not set");
	}
	/* Optional field*/
	if((pduSessionModificationCommand->presenceMask & PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
			==PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
		buffer[len] = E_PDU_SESSION_MODIFICATION_COMMAND_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS;
		len++;
		encodeLen = 0;
		if(encodeExtendedProtocolConfigurationOptions(buffer + len, bufLen - len, 
			&pduSessionModificationCommand->extendedProtocolConfigurationOptions, &encodeLen) == FAILURE)
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
