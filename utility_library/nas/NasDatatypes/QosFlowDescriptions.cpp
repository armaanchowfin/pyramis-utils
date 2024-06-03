#include "nas/NasDatatypes/QosFlowDescriptions.h"

int decodeQosFlowDescriptions(uint8_t *buffer, uint32_t bufLen, QosFlowDescriptions_t *qosFlowDescriptions, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || qosFlowDescriptions == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + QOS_FLOW_DESCRIPTIONS_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

	qosFlowDescriptions->lengthOfQosFlowDescriptionsContents |= ((uint16_t)buffer[len++]) << 8;
	qosFlowDescriptions->lengthOfQosFlowDescriptionsContents |= ((uint16_t)buffer[len++]);

	uint16_t ieLen = qosFlowDescriptions->lengthOfQosFlowDescriptionsContents;

	if((ieLen > QOS_FLOW_DESCRIPTIONS_MAX_LEN) || (ieLen < QOS_FLOW_DESCRIPTIONS_MIN_LEN)) {
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

    nasLog("qosFlowDescriptions->lengthOfQosFlowDescriptionsContents %u",
        qosFlowDescriptions->lengthOfQosFlowDescriptionsContents);

	uint32_t qosFlowDescriptionLen = qosFlowDescriptions->lengthOfQosFlowDescriptionsContents + len;
	uint32_t itr = 0;
	while(len < qosFlowDescriptionLen) {
		uint32_t consumedLen=0;
			if(decodeQosFlowDescription(buffer + len, bufLen - len,
				&(qosFlowDescriptions->qosFlowDescription[itr]), &consumedLen) == FAILURE) {
				higLog("QoS flow description at %u in list failed to decode",itr);
				nasLogEXIT;
				return FAILURE;
			}
		qosFlowDescriptions->noOfQosFlowDescription++;
		len += consumedLen;
		nasLog("Decode itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
		itr++;
		if(len > qosFlowDescriptionLen) {
		higLog("Decoding has crossed boundary of buffer meant to be decoded");
		nasLogEXIT;
		return FAILURE;
		}
	}

	*decodedLen = len;
	nasLog("QoS_flow_descriptions decoded | decoded %d bytes | noOfQosFlowDescription %u",
        len, qosFlowDescriptions->noOfQosFlowDescription);
	nasLogEXIT;
	return SUCCESS;

}

int encodeQosFlowDescriptions(uint8_t *buffer, uint32_t bufLen, QosFlowDescriptions_t *qosFlowDescriptions, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || qosFlowDescriptions == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(len + QOS_FLOW_DESCRIPTIONS_MIN_LEN > bufLen) {
		higLog("Buffer insufficient");
		errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

    nasLog("qosFlowDescriptions->lengthOfQosFlowDescriptionsContents %u",
        qosFlowDescriptions->lengthOfQosFlowDescriptionsContents);

	buffer[len++] = (qosFlowDescriptions->lengthOfQosFlowDescriptionsContents) >> 8;
	buffer[len++] = (qosFlowDescriptions->lengthOfQosFlowDescriptionsContents);
	uint16_t ieLen = qosFlowDescriptions->lengthOfQosFlowDescriptionsContents;

	if((ieLen > QOS_FLOW_DESCRIPTIONS_MAX_LEN) || (ieLen < QOS_FLOW_DESCRIPTIONS_MIN_LEN)) {
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

    nasLog("qosFlowDescriptions->noOfQosFlowDescription %u",
        qosFlowDescriptions->noOfQosFlowDescription);

	uint8_t itr = 0;
	for(itr = 0; itr < qosFlowDescriptions->noOfQosFlowDescription; itr++) {
		uint32_t consumedLen=0;
			if(encodeQosFlowDescription(buffer + len, bufLen - len,
				&(qosFlowDescriptions->qosFlowDescription[itr]), &consumedLen) == FAILURE) {
				higLog("QoS flow description at %u in list failed to encode",itr);
				nasLogEXIT;
				return FAILURE;
			}
			len += consumedLen;
			nasLog("Encoding itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
	}


	*encodedLen = len;
	nasLog("QoS_flow_descriptions encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
