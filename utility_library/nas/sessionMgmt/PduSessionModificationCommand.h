#ifndef __PDU_SESSION_MODIFICATION_COMMAND_H
#define __PDU_SESSION_MODIFICATION_COMMAND_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/NasDatatypes/_5gsmCause.h"
#include "nas/NasDatatypes/SessionAmbr.h"
#include "nas/NasDatatypes/GprsTimer.h"
#include "nas/NasDatatypes/AlwaysOnPduSessionIndication.h"
#include "nas/NasDatatypes/QosRules.h"
#include "nas/NasDatatypes/QosFlowDescriptions.h"
#include "nas/NasDatatypes/ExtendedProtocolConfigurationOptions.h"

#define PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENT	(1 << 0)
#define PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_PRESENT	(1 << 1)
#define PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT	(1 << 2)
#define PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_PRESENT	(1 << 3)
#define PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT	(1 << 4)
#define PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT	(1 << 5)

typedef enum pduSessionModificationCommandIEI {
	E_PDU_SESSION_MODIFICATION_COMMAND_IEI_SESSION_AMBR	= 0x2A,
	E_PDU_SESSION_MODIFICATION_COMMAND_IEI_RQ_TIMER_VALUE	= 0x56,
	E_PDU_SESSION_MODIFICATION_COMMAND_IEI_ALWAYS_ON_PDU_SESSION_INDICATION	= 0x8,
	E_PDU_SESSION_MODIFICATION_COMMAND_IEI_AUTHORIZED_QOS_RULES	= 0x7A,
	E_PDU_SESSION_MODIFICATION_COMMAND_IEI_AUTHORIZED_QOS_FLOW_DESCRIPTIONS	= 0x79,
	E_PDU_SESSION_MODIFICATION_COMMAND_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS	= 0x7B,

}pduSessionModificationCommandIEI_t;

typedef struct pduSessionModificationCommand {
	

	_5gsmMsgHeader_t 	 smHeader;
	


	

	uint32_t	presenceMask;
	SessionAmbr_t	sessionAmbr;
	GprsTimer_t	rqTimerValue;
	AlwaysOnPduSessionIndication_t	alwaysOnPduSessionIndication;
	QosRules_t	authorizedQosRules;
	QosFlowDescriptions_t	authorizedQosFlowDescriptions;
	ExtendedProtocolConfigurationOptions_t	extendedProtocolConfigurationOptions;
	


}PduSessionModificationCommand_t;

int decodePduSessionModificationCommand(uint8_t *buffer, PduSessionModificationCommand_t *pduSessionModificationCommand, uint32_t decodedLen);

int encodePduSessionModificationCommand(uint8_t *buffer, uint32_t bufLen, PduSessionModificationCommand_t *pduSessionModificationCommand, uint32_t* encodedLen);

#endif 

