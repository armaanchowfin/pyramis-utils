#ifndef __PDU_SESSION_MODIFICATION_COMPLETE_H
#define __PDU_SESSION_MODIFICATION_COMPLETE_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/sessionMgmt/_5gsmMsgHeader.h"
#include "nas/NasDatatypes/ExtendedProtocolConfigurationOptions.h"
#include "nas/NasDatatypes/_5gsmCause.h"

#define PDU_SESSION_MODIFICATION_COMPLETE_5GSM_CAUSE_PRESENT	(1 << 0)

typedef enum pduSessionModificationCompleteIEI {
	E_PDU_SESSION_MODIFICATION_COMPLETE_IEI_5GSM_CAUSE	= 0x59,

}pduSessionModificationCompleteIEI_t;

typedef struct pduSessionModificationComplete {
	

	_5gsmMsgHeader_t 	 smHeader;
	


	

	uint32_t	presenceMask;
	_5gsmCause_t	_5gsmCause;
	


}PduSessionModificationComplete_t;

int decodePduSessionModificationComplete(uint8_t *buffer, PduSessionModificationComplete_t *pduSessionModificationComplete, uint32_t decodedLen);

int encodePduSessionModificationComplete(uint8_t *buffer, uint32_t bufLen, PduSessionModificationComplete_t *pduSessionModificationComplete, uint32_t* encodedLen);

#endif 

