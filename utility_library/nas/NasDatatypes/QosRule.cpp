#include "nas/NasDatatypes/QosRule.h"

int decodeQosRule(uint8_t *buffer, uint32_t bufLen, QosRule_t *qosRule, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || qosRule == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}


	qosRule->qosRuleIdentifier = buffer[len++];

	qosRule->lengthOfQosRule |= ((uint16_t)buffer[len++]) << 8;
	qosRule->lengthOfQosRule |= ((uint16_t)buffer[len++]);

	uint32_t lengthOfQosRule = qosRule->lengthOfQosRule;
	if((lengthOfQosRule > LENGTH_OF_QOS_RULE_MAX_LEN) || (lengthOfQosRule < LENGTH_OF_QOS_RULE_MIN_LEN)) {
		higLog("Invalid Length %d", lengthOfQosRule);
		errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
		nasLogEXIT;
		return FAILURE;
	}

	if((len + lengthOfQosRule) > bufLen) {
		higLog("Buffer insufficient to read %d bytes", lengthOfQosRule);
		errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

	qosRule->ruleOperationCode= (buffer[len] & 0xe0) >> 5;
	qosRule->dqrBit= (buffer[len] & 0x10) >> 4;
	qosRule->numberOfPacketFilters= (buffer[len] & 0x0f);
	len++;

	switch(qosRule->ruleOperationCode) {
		case QOS_RULE_CREATE_NEW_QOS_RULE:
		case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS:
		{
		    if ((qosRule->numberOfPacketFilters < 0) || (qosRule->numberOfPacketFilters > 15)) {
				higLog("Invalid numberOfPacketFilters %d", qosRule->numberOfPacketFilters);
				errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
				nasLogEXIT;
				return FAILURE;
		    }
		}
		break;
		case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS:
		case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS:
		{
		    if ((qosRule->numberOfPacketFilters <= 0) || (qosRule->numberOfPacketFilters > 15)) {
				higLog("Invalid numberOfPacketFilters %d", qosRule->numberOfPacketFilters);
				errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
				nasLogEXIT;
				return FAILURE;
		    }
		}
		break;
		case QOS_RULE_DELETE_EXISTING_QOS_RULE:
		case QOS_RULE_MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS:
		{
		    if (qosRule->numberOfPacketFilters != 0) {
				higLog("Invalid numberOfPacketFilters %d", qosRule->numberOfPacketFilters);
				errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
				nasLogEXIT;
				return FAILURE;
			}
		}
		break;
		default:{
			higLog("InvalidRule operation code");
			nasLogEXIT;
			return FAILURE;
		}
	}

	for(uint32_t itr = 0; itr < qosRule->numberOfPacketFilters; itr++) {
		uint32_t consumedLen=0;
		switch(qosRule->ruleOperationCode) {
			case QOS_RULE_CREATE_NEW_QOS_RULE:
			case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS:
			case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS:
			{
				if(decodePacketFilterListCreate(buffer + len, bufLen - len,
					&(qosRule->packetFilterListChoice.packetFilterListCreate[itr]), &consumedLen) == FAILURE) {
					higLog("Packet filter list at %u in list failed to decode",itr);
					nasLogEXIT;
					return FAILURE;
				}
			}
			break;
			case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS:
			{
				if(decodePacketFilterListDelete(buffer + len, bufLen - len,
					&(qosRule->packetFilterListChoice.packetFilterListDelete[itr]), &consumedLen) == FAILURE) {
					higLog("Packet filter list at %u in list failed to decode",itr);
					nasLogEXIT;
					return FAILURE;
				}
			}
			break;
			case QOS_RULE_DELETE_EXISTING_QOS_RULE:
			case QOS_RULE_MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS:
			{
				midLog("No special opeartion specified");
			}
			break;
			default:{
				higLog("InvalidRule operation code");
				nasLogEXIT;
				return FAILURE;
			}
		}
		len += consumedLen;
		nasLog("Decode itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
		if(len > bufLen) {
			higLog("Decoding has crossed boundary of buffer meant to be decoded");
			nasLogEXIT;
			return FAILURE;
		}
	}
	if (qosRule->ruleOperationCode != QOS_RULE_DELETE_EXISTING_QOS_RULE) {
	    qosRule->qosRulePrecedence = buffer[len++];

	    qosRule->spare= (buffer[len] & 0x80) >> 7;
	    qosRule->segregation= (buffer[len] & 0x40) >> 6;
	    qosRule->qfi= (buffer[len] & 0x3f);
	    len++;
	}

    nasLog("QosRule qosRuleIdentifier %u lengthOfQosRule %u ruleOperationCode %u "
            "dqrBit %u numberOfPacketFilters %u qosRulePrecedence %u "
            "spare %u segregation %u qfi %u",
            qosRule->qosRuleIdentifier,
            qosRule->lengthOfQosRule,
            qosRule->ruleOperationCode,
            qosRule->dqrBit,
            qosRule->numberOfPacketFilters,
            qosRule->qosRulePrecedence,
            qosRule->spare,
            qosRule->segregation,
            qosRule->qfi
        );

	*decodedLen = len;
	nasLog("Qos_rule decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeQosRule(uint8_t *buffer, uint32_t bufLen, QosRule_t *qosRule, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	qosRule->lengthOfQosRule = 0;

	if(buffer == NULL || qosRule == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[len++] = (qosRule->qosRuleIdentifier);
	len += sizeof(qosRule->lengthOfQosRule);

	buffer[len] = (qosRule->ruleOperationCode << 5) | (qosRule->dqrBit << 4) | (qosRule->numberOfPacketFilters);
	len++;

	switch(qosRule->ruleOperationCode) {
		case QOS_RULE_CREATE_NEW_QOS_RULE:
		case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS:
		{
		    if ((qosRule->numberOfPacketFilters < 0) || (qosRule->numberOfPacketFilters > 15)) {
				higLog("Invalid numberOfPacketFilters %d", qosRule->numberOfPacketFilters);
				errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
				nasLogEXIT;
				return FAILURE;
		    }
		}
		break;
		case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS:
		case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS:
		{
		    if ((qosRule->numberOfPacketFilters <= 0) || (qosRule->numberOfPacketFilters > 15)) {
				higLog("Invalid numberOfPacketFilters %d", qosRule->numberOfPacketFilters);
				errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
				nasLogEXIT;
				return FAILURE;
		    }
		}
		break;
		case QOS_RULE_DELETE_EXISTING_QOS_RULE:
		case QOS_RULE_MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS:
		{
		    if (qosRule->numberOfPacketFilters != 0) {
				higLog("Invalid numberOfPacketFilters %d", qosRule->numberOfPacketFilters);
				errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
				nasLogEXIT;
				return FAILURE;
		    }
		}
		break;
		default:{
			higLog("InvalidRule operation code");
			nasLogEXIT;
			return FAILURE;
		}
	}

	for(uint8_t itr = 0; itr < qosRule->numberOfPacketFilters; itr++) {
		uint32_t consumedLen=0;
		switch(qosRule->ruleOperationCode) {
			case QOS_RULE_CREATE_NEW_QOS_RULE:
			case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS:
			case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS:
			{
				if(encodePacketFilterListCreate(buffer + len, bufLen - len,
					&(qosRule->packetFilterListChoice.packetFilterListCreate[itr]), &consumedLen) == FAILURE) {
					higLog("Packet filter list at %u in list failed to encode",itr);
					nasLogEXIT;
					return FAILURE;
				}
				len += consumedLen;
				nasLog("Encoding itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
			}
			break;
			case QOS_RULE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS:
			{
				if(encodePacketFilterListDelete(buffer + len, bufLen - len,
					&(qosRule->packetFilterListChoice.packetFilterListDelete[itr]), &consumedLen) == FAILURE) {
					higLog("Packet filter list at %u in list failed to encode",itr);
					nasLogEXIT;
					return FAILURE;
				}
				len += consumedLen;
				nasLog("Encoding itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
			}
			break;
			case QOS_RULE_DELETE_EXISTING_QOS_RULE:
			case QOS_RULE_MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS:
			{
				midLog("No special opeartion specified");
			}
			break;
			default:{
				higLog("InvalidRule operation code");
				nasLogEXIT;
				return FAILURE;
			}
		}
	}

	if (qosRule->ruleOperationCode != QOS_RULE_DELETE_EXISTING_QOS_RULE) {
	    buffer[len++] = (qosRule->qosRulePrecedence);
	    buffer[len] = (qosRule->spare << 7) | (qosRule->segregation << 6) | (qosRule->qfi);
	    len++;
	}

	qosRule->lengthOfQosRule = (len - sizeof(qosRule->lengthOfQosRule) - sizeof(qosRule->qosRuleIdentifier));
	buffer[OFFSET_FOR_LENGTH_OF_QOS_RULE_FIELD] = (qosRule->lengthOfQosRule) >> 8;
	buffer[OFFSET_FOR_LENGTH_OF_QOS_RULE_FIELD+1] = (qosRule->lengthOfQosRule);
	uint32_t lengthOfQosRule = qosRule->lengthOfQosRule;
	if((lengthOfQosRule > LENGTH_OF_QOS_RULE_MAX_LEN) || (lengthOfQosRule < LENGTH_OF_QOS_RULE_MIN_LEN)) {
		higLog("Invalid Length %d", lengthOfQosRule);
		errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
		nasLogEXIT;
		return FAILURE;
	}

	nasLog("QosRule qosRuleIdentifier %u lengthOfQosRule %u ruleOperationCode %u "
            "dqrBit %u numberOfPacketFilters %u qosRulePrecedence %u "
            "spare %u segregation %u qfi %u",
            qosRule->qosRuleIdentifier,
            qosRule->lengthOfQosRule,
            qosRule->ruleOperationCode,
            qosRule->dqrBit,
            qosRule->numberOfPacketFilters,
            qosRule->qosRulePrecedence,
            qosRule->spare,
            qosRule->segregation,
            qosRule->qfi
        );

	*encodedLen = len;
	nasLog("Qos_rule encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int addPacketFilterCreate(QosRule_t &qosRuleIE, PacketFilterListCreate_t pF)
{
	if(qosRuleIE.numberOfPacketFilters == PACKET_FILTER_LIST_MAX_NO)
	{
		higLog("Reached max allowed packet filter list");
		return FAILURE;
	}
	qosRuleIE.packetFilterListChoice.packetFilterListCreate[qosRuleIE.numberOfPacketFilters] = pF;
	qosRuleIE.numberOfPacketFilters++;
	return SUCCESS;
	
}

int addPacketFilterDelete(QosRule_t &qosRuleIE, PacketFilterListDelete_t pF)
{
	if(qosRuleIE.numberOfPacketFilters == PACKET_FILTER_LIST_MAX_NO)
	{
		higLog("Reached max allowed packet filter list");
		return FAILURE;
	}
	qosRuleIE.packetFilterListChoice.packetFilterListDelete[qosRuleIE.numberOfPacketFilters] = pF;
	qosRuleIE.numberOfPacketFilters++;
	return SUCCESS;
}
