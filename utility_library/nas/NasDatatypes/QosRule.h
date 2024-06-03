#ifndef __QOS_RULE_H
#define __QOS_RULE_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/PacketFilterListCreate.h"
#include "nas/NasDatatypes/PacketFilterListDelete.h"

#define LENGTH_OF_QOS_RULE_MIN_LEN	1
#define LENGTH_OF_QOS_RULE_MAX_LEN	65532

#define PACKET_FILTER_LIST_MAX_NO	 4

#define QOS_RULE_CREATE_NEW_QOS_RULE	0b001
#define QOS_RULE_DELETE_EXISTING_QOS_RULE	0b010
#define QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS	0b011
#define QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS	0b100
#define QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS	0b101
#define QOS_RULE_MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS	0b110

#define OFFSET_FOR_LENGTH_OF_QOS_RULE_FIELD 1

typedef struct qosRule {
	uint8_t	qosRuleIdentifier;
	uint16_t	lengthOfQosRule;
	uint8_t	ruleOperationCode:3;
	uint8_t	dqrBit:1;
	uint8_t	numberOfPacketFilters:4;
	union packetFilterList{
		PacketFilterListCreate_t	packetFilterListCreate[PACKET_FILTER_LIST_MAX_NO];
		PacketFilterListDelete_t	packetFilterListDelete[PACKET_FILTER_LIST_MAX_NO];
	}packetFilterListChoice;
	uint8_t	qosRulePrecedence;
	uint8_t	spare:1;
	uint8_t	segregation:1;
	uint8_t	qfi:6;
}QosRule_t;

int decodeQosRule(uint8_t *buffer, uint32_t bufLen, QosRule_t *qosRule, uint32_t* decodedLen);

int encodeQosRule(uint8_t *buffer, uint32_t bufLen, QosRule_t *qosRule, uint32_t* encodedLen);

int addPacketFilterCreate(QosRule_t &qosRuleIE, PacketFilterListCreate_t pF);

int addPacketFilterDelete(QosRule_t &qosRuleIE, PacketFilterListDelete_t pF);

#endif 

