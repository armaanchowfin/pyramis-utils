#ifndef __PDU_SESSION_MODIFICATION_REQUEST_H
#define __PDU_SESSION_MODIFICATION_REQUEST_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/NasDatatypes/_5gsmCapability.h"
#include "nas/NasDatatypes/_5gsmCause.h"
#include "nas/NasDatatypes/MaximumNumberOfSupportedPacketFilters.h"
#include "nas/NasDatatypes/AlwaysOnPduSessionRequested.h"
#include "nas/NasDatatypes/IntegrityProtectionMaximumDataRate.h"
#include "nas/NasDatatypes/QosRules.h"
#include "nas/NasDatatypes/QosFlowDescriptions.h"
#include "nas/NasDatatypes/ExtendedProtocolConfigurationOptions.h"

#define PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT	(1 << 0)
#define PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT	(1 << 1)
#define PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT	(1 << 2)
#define PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT	(1 << 3)
#define PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT	(1 << 4)
#define PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT	(1 << 5)
#define PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT	(1 << 6)

typedef enum pduSessionModificationRequestIEI {
	E_PDU_SESSION_MODIFICATION_REQUEST_IEI_5GSM_CAUSE	= 0x59,
	E_PDU_SESSION_MODIFICATION_REQUEST_IEI_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS	= 0x55,
	E_PDU_SESSION_MODIFICATION_REQUEST_IEI_ALWAYS_ON_PDU_SESSION_REQUESTED	= 0xB,
	E_PDU_SESSION_MODIFICATION_REQUEST_IEI_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE	= 0x13,
	E_PDU_SESSION_MODIFICATION_REQUEST_IEI_REQUESTED_QOS_RULES	= 0x7A,
	E_PDU_SESSION_MODIFICATION_REQUEST_IEI_REQUESTED_QOS_FLOW_DESCRIPTIONS	= 0x79,
	E_PDU_SESSION_MODIFICATION_REQUEST_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS	= 0x7B,

}pduSessionModificationRequestIEI_t;

typedef struct pduSessionModificationRequest {
	

	_5gsmMsgHeader_t 	 smHeader;
	


	

	uint32_t	presenceMask;
	_5gsmCause_t	_5gsmCause;
	MaximumNumberOfSupportedPacketFilters_t	maximumNumberOfSupportedPacketFilters;
	AlwaysOnPduSessionRequested_t	alwaysOnPduSessionRequested;
	IntegrityProtectionMaximumDataRate_t	integrityProtectionMaximumDataRate;
	QosRules_t	requestedQosRules;
	QosFlowDescriptions_t	requestedQosFlowDescriptions;
	ExtendedProtocolConfigurationOptions_t	extendedProtocolConfigurationOptions;
	


}PduSessionModificationRequest_t;

int decodePduSessionModificationRequest(uint8_t *buffer, PduSessionModificationRequest_t *pduSessionModificationRequest, uint32_t decodedLen);

int encodePduSessionModificationRequest(uint8_t *buffer, uint32_t bufLen, PduSessionModificationRequest_t *pduSessionModificationRequest, uint32_t* encodedLen);

#endif 

