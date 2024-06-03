#ifndef __QOS_FLOW_DESCRIPTION_H
#define __QOS_FLOW_DESCRIPTION_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/Parameter.h"


#define PARAMETER_MAX_NO	 10


typedef struct qosFlowDescription {
	uint8_t	spare1:2;
	uint8_t	qfi:6;
	uint8_t	operationCode:3;
	uint8_t	spare2:5;
	uint8_t	spare3:1;
	uint8_t	e:1;
	uint8_t	numberOfParameters:6;
	Parameter_t	parameter[PARAMETER_MAX_NO];

}QosFlowDescription_t;

int decodeQosFlowDescription(uint8_t *buffer, uint32_t bufLen, QosFlowDescription_t *qosFlowDescription, uint32_t* decodedLen);

int encodeQosFlowDescription(uint8_t *buffer, uint32_t bufLen, QosFlowDescription_t *qosFlowDescription, uint32_t* encodedLen);

#endif 

