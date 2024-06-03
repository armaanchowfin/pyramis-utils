#include "nas/NasDatatypes/QosRules.h"

int decodeQosRules(uint8_t *buffer, uint32_t bufLen, QosRules_t *qosRules, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || qosRules == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + QOS_RULES_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	qosRules->lengthOfQosRulesIe |= ((uint16_t)buffer[len++]) << 8;
	qosRules->lengthOfQosRulesIe |= ((uint16_t)buffer[len++]);

	uint16_t ieLen = qosRules->lengthOfQosRulesIe;

	if((ieLen > QOS_RULES_MAX_LEN) || (ieLen < QOS_RULES_MIN_LEN)) {
		higLog("Invalid Length %d", ieLen);
		errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
		nasLogEXIT;
		return FAILURE;
	}

	if((len + ieLen) > bufLen) {
		higLog("Buffer insufficient to read %d bytes", ieLen);
		errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}


	uint32_t qosRuleLen = qosRules->lengthOfQosRulesIe + len;
	uint32_t itr = 0;
	while(len < qosRuleLen) {
		uint32_t consumedLen=0;
			if(decodeQosRule(buffer + len, bufLen - len,
				&(qosRules->qosRule[itr]), &consumedLen) == FAILURE) {
				higLog("QoS rule at %u in list failed to decode",itr);
				nasLogEXIT;
				return FAILURE;
			}
		qosRules->noOfQosRule++;
		len += consumedLen;
		nasLog("Decode itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
		itr++;
		if(len > qosRuleLen) {
		higLog("Decoding has crossed boundary of buffer meant to be decoded");
		nasLogEXIT;
		return FAILURE;
		}
	}

    nasLog("QosRules lengthOfQosRulesIe %u noOfQosRule %u",
            qosRules->lengthOfQosRulesIe,
            qosRules->noOfQosRule
        );

	*decodedLen = len;
	nasLog("QoS_rules decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeQosRules(uint8_t *buffer, uint32_t bufLen, QosRules_t *qosRules, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
	qosRules->lengthOfQosRulesIe=0;

	if(buffer == NULL || qosRules == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + QOS_RULES_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

    len += sizeof(qosRules->lengthOfQosRulesIe);

	uint8_t itr = 0;
	for(itr = 0; itr < qosRules->noOfQosRule; itr++) {
		uint32_t consumedLen=0;
			if(encodeQosRule(buffer + len, bufLen - len,
				&(qosRules->qosRule[itr]), &consumedLen) == FAILURE) {
				higLog("QoS rule at %u in list failed to encode",itr);
				nasLogEXIT;
				return FAILURE;
			}
			len += consumedLen;
			qosRules->lengthOfQosRulesIe += consumedLen;
			nasLog("Encoding itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
	}

    uint16_t ieLen = qosRules->lengthOfQosRulesIe;

	if((ieLen > QOS_RULES_MAX_LEN) || (ieLen < QOS_RULES_MIN_LEN)) {
		higLog("Invalid Length %d", ieLen);
		errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
		nasLogEXIT;
		return FAILURE;
	}

	buffer[OFFSET_FOR_LENGTH_OF_QOS_RULES_IE_FIELD] = (qosRules->lengthOfQosRulesIe) >> 8;
	buffer[OFFSET_FOR_LENGTH_OF_QOS_RULES_IE_FIELD+1] = (qosRules->lengthOfQosRulesIe);

	nasLog("QosRules lengthOfQosRulesIe %u noOfQosRule %u",
            qosRules->lengthOfQosRulesIe,
            qosRules->noOfQosRule
        );

	*encodedLen = len;
	nasLog("QoS_rules encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int addQosRuleIE(QosRules_t &qosRules, QosRule_t qosRuleIE)
{
	if(qosRules.noOfQosRule == QOS_RULE_MAX_NO)
	{
		higLog("Reached max allowed QosRule");
		return FAILURE;
	}
	memcpy(&qosRules.qosRule[qosRules.noOfQosRule], &qosRuleIE, sizeof(qosRuleIE));
	qosRules.noOfQosRule++;
	return SUCCESS;
}

