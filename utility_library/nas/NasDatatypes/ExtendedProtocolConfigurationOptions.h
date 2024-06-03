#ifndef __EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_H
#define __EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MIN_LEN	1
#define EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MAX_LEN	65535

#define EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_CONTENTS_MAX_NO	 64


typedef struct extendedProtocolConfigurationOptions {
	uint16_t	lengthOfExtendedProtocolConfigurationOptionsContents;
	uint8_t	extendedProtocolConfigurationOptionsContents[EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_CONTENTS_MAX_NO];
}ExtendedProtocolConfigurationOptions_t;

int decodeExtendedProtocolConfigurationOptions(uint8_t *buffer, uint32_t bufLen, ExtendedProtocolConfigurationOptions_t *extendedProtocolConfigurationOptions, uint32_t* decodedLen);

int encodeExtendedProtocolConfigurationOptions(uint8_t *buffer, uint32_t bufLen, ExtendedProtocolConfigurationOptions_t *extendedProtocolConfigurationOptions, uint32_t* encodedLen);

#endif 

