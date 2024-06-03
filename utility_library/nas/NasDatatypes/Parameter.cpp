#include "nas/NasDatatypes/Parameter.h"

int decodeParameter(uint8_t *buffer, uint32_t bufLen, Parameter_t *parameter, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || parameter == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}


	parameter->parameterIdentifier = buffer[len++];

	parameter->lengthOfParameterContents = buffer[len++];

	memcpy(parameter->parameterContents, buffer + len, parameter->lengthOfParameterContents);
	len += parameter->lengthOfParameterContents;

    std::string logStr = getPrintStr("nas parameterContents: ",
         parameter->parameterContents,
         parameter->lengthOfParameterContents);

    nasLog("parameter parameterIdentifier %u lengthOfParameterContents %u"
            "parameterContents %s",
            parameter->parameterIdentifier,
            parameter->lengthOfParameterContents,
            logStr.c_str()
        );

	*decodedLen = len;
	nasLog("Parameter decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}

int encodeParameter(uint8_t *buffer, uint32_t bufLen, Parameter_t *parameter, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || parameter == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

    std::string logStr = getPrintStr("nas parameterContents: ",
         parameter->parameterContents,
         parameter->lengthOfParameterContents);

    nasLog("parameter parameterIdentifier %u lengthOfParameterContents %u"
            "parameterContents %s",
            parameter->parameterIdentifier,
            parameter->lengthOfParameterContents,
            logStr.c_str()
        );

	buffer[len++] = (parameter->parameterIdentifier);
	buffer[len++] = (parameter->lengthOfParameterContents);
	memcpy(buffer + len, parameter->parameterContents, parameter->lengthOfParameterContents);
	len += parameter->lengthOfParameterContents;

	*encodedLen = len;
	nasLog("Parameter encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;

}
