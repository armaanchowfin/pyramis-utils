#ifndef __QOS_RULES_H
#define __QOS_RULES_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/QosRule.h"

#define QOS_RULES_MIN_LEN	4
#define QOS_RULES_MAX_LEN	65534

#define QOS_RULE_MAX_NO	 4

#define QOS_RULES_NO_QOS_RULE_IDENTIFIER_ASSIGNED	0b00000000
#define QOS_RULES_QRI_1	0b00000001
#define QOS_RULES_QRI_2	0b00000010
#define QOS_RULES_QRI_3	0b00000011
#define QOS_RULES_QRI_255	0b11111111
#define QOS_RULES_SEGREGATION_NOT_REQUEST	0b0
#define QOS_RULES_SEGREGATION_REQUESTED	0b1
#define QOS_RULES_NO_QOS_FLOW_IDENTIFIER_ASSIGNED	0b000000
#define QOS_RULES_QFI_1	0b000001
#define QOS_RULES_QFI_2	0b000010
#define QOS_RULES_QFI_3	0b000011
#define QOS_RULES_QFI_63	0b111111
#define QOS_RULES_NON_DEFAULT_QOS_RULE	0b0
#define QOS_RULES_DEFAULT_QOS_RULE	0b1
#define QOS_RULES_DOWNLINK_ONLY	0b01
#define QOS_RULES_UPLINK_ONLY	0b10
#define QOS_RULES_BIDIRECTIONAL	0b11

#define OFFSET_FOR_LENGTH_OF_QOS_RULES_IE_FIELD 0

typedef struct qosRules {
	uint16_t	lengthOfQosRulesIe;
	QosRule_t	qosRule[QOS_RULE_MAX_NO];

	uint8_t	noOfQosRule;
}QosRules_t;

int decodeQosRules(uint8_t *buffer, uint32_t bufLen, QosRules_t *qosRules, uint32_t* decodedLen);

int encodeQosRules(uint8_t *buffer, uint32_t bufLen, QosRules_t *qosRules, uint32_t* encodedLen);

int addQosRuleIE(QosRules_t &qosRules, QosRule_t qosRuleIE);

#endif 

