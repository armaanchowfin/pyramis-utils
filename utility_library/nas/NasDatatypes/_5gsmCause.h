#ifndef ___5GSM_CAUSE_H
#define ___5GSM_CAUSE_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define _5GSM_CAUSE_LEN	1


#define _5GSM_CAUSE_INSUFFICIENT_RESOURCES	0b00011010
#define _5GSM_CAUSE_MISSING_OR_UNKNOWN_DNN	0b00011011
#define _5GSM_CAUSE_UNKNOWN_PDU_SESSION_TYPE	0b00011100
#define _5GSM_CAUSE_USER_AUTHENTICATION_OR_AUTHORIZATION_FAILED	0b00011101
#define _5GSM_CAUSE_REQUEST_REJECTED_UNSPECIFIED	0b00011111
#define _5GSM_CAUSE_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER	0b00100010
#define _5GSM_CAUSE_PTI_ALREADY_IN_USE	0b00100011
#define _5GSM_CAUSE_REGULAR_DEACTIVATION	0b00100100
#define _5GSM_CAUSE_REACTIVATION_REQUESTED	0b00100111
#define _5GSM_CAUSE_INVALID_PDU_SESSION_IDENTITY	0b00101011
#define _5GSM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER(S)	0b00101100
#define _5GSM_CAUSE_SYNTACTICAL_ERROR_IN_PACKET_FILTER(S)	0b00101101
#define _5GSM_CAUSE_OUT_OF_LADN_SERVICE_AREA	0b00101110
#define _5GSM_CAUSE_PTI_MISMATCH	0b00101111
#define _5GSM_CAUSE_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED	0b00110010
#define _5GSM_CAUSE_PDU_SESSION_TYPE_IPV6_ONLY_ALLOWED	0b00110011
#define _5GSM_CAUSE_PDU_SESSION_DOES_NOT_EXIST	0b00110110
#define _5GSM_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_AND_DNN	0b01000011
#define _5GSM_CAUSE_NOT_SUPPORTED_SSC_MODE	0b01000100
#define _5GSM_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE	0b01000101
#define _5GSM_CAUSE_MISSING_OR_UNKNOWN_DNN_IN_A_SLICE	0b01000110
#define _5GSM_CAUSE_INVALID_PTI_VALUE	0b01010001
#define _5GSM_CAUSE_MAXIMUM_DATA_RATE_PER_UE_FOR_USER_PLANE_INTEGRITY_PROTECTION_IS_TOO_LOW	0b01010010
#define _5GSM_CAUSE_SEMANTIC_ERROR_IN_THE_QOS_OPERATION	0b01010011
#define _5GSM_CAUSE_SYNTACTICAL_ERROR_IN_THE_QOS_OPERATION	0b01010100
#define _5GSM_CAUSE_INVALID_MAPPED_EPS_BEARER_IDENTITY	0b01010101
#define _5GSM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE	0b01011111
#define _5GSM_CAUSE_INVALID_MANDATORY_INFORMATION	0b01100000
#define _5GSM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED	0b01100001
#define _5GSM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE	0b01100010
#define _5GSM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED	0b01100011
#define _5GSM_CAUSE_CONDITIONAL_IE_ERROR	0b01100100
#define _5GSM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE	0b01100101
#define _5GSM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED	0b01101111

typedef struct _5gsmCause {
	uint8_t	causeValue;
}_5gsmCause_t;

int decode5gsmCause(uint8_t *buffer, uint32_t bufLen, _5gsmCause_t *_5gsmCause, uint32_t* decodedLen);

int encode5gsmCause(uint8_t *buffer, uint32_t bufLen, _5gsmCause_t *_5gsmCause, uint32_t* encodedLen);

#endif 

