#include "nas/NasDatatypes/QosFlowDescription.h"

int decodeQosFlowDescription(uint8_t *buffer, uint32_t bufLen, QosFlowDescription_t *qosFlowDescription, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || qosFlowDescription == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}


	qosFlowDescription->spare1= (buffer[len] & 0xc0) >> 6;
	qosFlowDescription->qfi= (buffer[len] & 0x3f);
	len++;

	qosFlowDescription->operationCode= (buffer[len] & 0xe0) >> 5;
	qosFlowDescription->spare2= (buffer[len] & 0x1f);
	len++;

	qosFlowDescription->spare3= (buffer[len] & 0x80) >> 7;
	qosFlowDescription->e= (buffer[len] & 0x40) >> 6;
	qosFlowDescription->numberOfParameters= (buffer[len] & 0x3f);
	len++;

	for(uint32_t itr = 0; itr < qosFlowDescription->numberOfParameters; itr++) {
		uint32_t consumedLen=0;
			if(decodeParameter(buffer + len, bufLen - len,
				&(qosFlowDescription->parameter[itr]), &consumedLen) == FAILURE) {
				higLog("Parameter at %u in list failed to decode",itr);
				nasLogEXIT;
				return FAILURE;
			}
		len += consumedLen;
		nasLog("Decode itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
		if(len > bufLen) {
		higLog("Decoding has crossed boundary of buffer meant to be decoded");
		nasLogEXIT;
		return FAILURE;
		}
	}

    nasLog("qosFlowDescription spare1 %u qfi %u operationCode %u spare2 %u "
            "spare3 %u e %u numberOfParameters %u",
        qosFlowDescription->spare1,
        qosFlowDescription->qfi,
        qosFlowDescription->operationCode,
        qosFlowDescription->spare2,
        qosFlowDescription->spare1,
        qosFlowDescription->e,
        qosFlowDescription->numberOfParameters
        );

	*decodedLen = len;
	nasLog("QoS_flow_description decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeQosFlowDescription(uint8_t *buffer, uint32_t bufLen, QosFlowDescription_t *qosFlowDescription, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || qosFlowDescription == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

    nasLog("qosFlowDescription spare1 %u qfi %u operationCode %u spare2 %u "
            "spare3 %u e %u numberOfParameters %u",
        qosFlowDescription->spare1,
        qosFlowDescription->qfi,
        qosFlowDescription->operationCode,
        qosFlowDescription->spare2,
        qosFlowDescription->spare1,
        qosFlowDescription->e,
        qosFlowDescription->numberOfParameters
        );

	buffer[len] = (qosFlowDescription->spare1 << 6) | (qosFlowDescription->qfi);
	len++;

	buffer[len] = (qosFlowDescription->operationCode << 5) | (qosFlowDescription->spare2);
	len++;

	buffer[len] = (qosFlowDescription->spare3 << 7) | (qosFlowDescription->e << 6) | (qosFlowDescription->numberOfParameters);
	len++;

	for(uint8_t itr = 0; itr < qosFlowDescription->numberOfParameters; itr++) {
		uint32_t consumedLen=0;
			if(encodeParameter(buffer + len, bufLen - len,
				&(qosFlowDescription->parameter[itr]), &consumedLen) == FAILURE) {
				higLog("Parameter at %u in list failed to encode",itr);
				nasLogEXIT;
				return FAILURE;
			}
			len += consumedLen;
			nasLog("Encoding itr %d ongoing | currentLen %u | decoded %u bytes", itr, len, consumedLen);
	}


	*encodedLen = len;
	nasLog("QoS_flow_description encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
