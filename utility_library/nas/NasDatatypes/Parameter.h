#ifndef __PARAMETER_H
#define __PARAMETER_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"


#define PARAMETER_CONTENTS_MAX_NO	 64


typedef struct parameter {
	uint8_t	parameterIdentifier;
	uint8_t	lengthOfParameterContents;
	uint8_t	parameterContents[PARAMETER_CONTENTS_MAX_NO];
}Parameter_t;

int decodeParameter(uint8_t *buffer, uint32_t bufLen, Parameter_t *parameter, uint32_t* decodedLen);

int encodeParameter(uint8_t *buffer, uint32_t bufLen, Parameter_t *parameter, uint32_t* encodedLen);

#endif 

