#ifndef __PDU_SESSION_MODIFICATION_REJECT_H
#define __PDU_SESSION_MODIFICATION_REJECT_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/NasDatatypes/_5gsmCause.h"
#include "nas/NasDatatypes/GprsTimer3.h"
#include "nas/NasDatatypes/ExtendedProtocolConfigurationOptions.h"

#define PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT	(1 << 0)
#define PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT	(1 << 1)

typedef enum pduSessionModificationRejectIEI {
	E_PDU_SESSION_MODIFICATION_REJECT_IEI_BACK_OFF_TIMER_VALUE	= 0x37,
	E_PDU_SESSION_MODIFICATION_REJECT_IEI_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS	= 0x7B,

}pduSessionModificationRejectIEI_t;

typedef struct pduSessionModificationReject {
	

	_5gsmMsgHeader_t 	 smHeader;
    _5gsmCause_t    _5gsmCause;
	


	

	uint32_t	presenceMask;
	GprsTimer3_t	backOffTimerValue;
	ExtendedProtocolConfigurationOptions_t	extendedProtocolConfigurationOptions;
	


}PduSessionModificationReject_t;

int decodePduSessionModificationReject(uint8_t *buffer, PduSessionModificationReject_t *pduSessionModificationReject, uint32_t decodedLen);

int encodePduSessionModificationReject(uint8_t *buffer, uint32_t bufLen, PduSessionModificationReject_t *pduSessionModificationReject, uint32_t* encodedLen);

#endif 

