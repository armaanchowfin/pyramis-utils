/*********************************************************************
*
* IITB CONFIDENTIAL
* __________________
*
* All Rights Reserved © 2018-19 IITB, India
* All Rights Reserved © 2019-20 IITB, India
*
* NOTICE: All information contained in this document/file is, and remains the 
* property of Indian Institute of Technology, Bombay (IITB).The intellectual
* and technical concepts contained herein may be proprietary to IITB and are
* protected by trade secret or copyright laws. Unauthorized use, dissemination, 
* reproduction, duplication, reverse engineering, any form of redistribution, 
* or use in part or in whole, by any means is strictly forbidden unless prior, 
* express, written and signed consent is obtained from IITB.
*
* Institute   : Indian Institute of Technology Bombay (IITB)
*/

#include "nas/NasDatatypes/MaxNoSupportedPackFilters.h"

int decodeMaxNoSupPackFilts(uint8_t *buffer, uint32_t bufLen,
			maxNoSupPackFilts_t *mnspf, uint32_t *decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || bufLen == 0 ||
		mnspf == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}
	if(len + SIZE_OF_MAX_NO_SUPPORTED_PACKET_FILTERS > bufLen) {
	    higLog("Buffer insufficient");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
	    nasLogEXIT;
	    return FAILURE;
	}
	uint16_t supportedPacketFilters=((buffer[len]<<3)|(buffer[len+1]>>5));

	if((supportedPacketFilters<17) || (supportedPacketFilters>1024)){
		higLog("Constraint Check Failed on supportedPacketFilters\n"
			"Value %d not supported",supportedPacketFilters);
		nasLogEXIT;
	    return FAILURE;
	}

	mnspf->supportedPacketFilters =supportedPacketFilters;
    
    len  += SIZE_OF_MAX_NO_SUPPORTED_PACKET_FILTERS;

    *decodedLen = len;
   	
   	nasLog("Max No. of Supported Packet Filters decoded : %d | currentLen = %d |"
   		"decoded %d bytes",mnspf->supportedPacketFilters,len,
   		SIZE_OF_MAX_NO_SUPPORTED_PACKET_FILTERS);
	
	nasLogEXIT;	
	return SUCCESS;
}

int encodeMaxNoSupPackFilts(uint8_t *buffer, uint32_t bufLen,
            maxNoSupPackFilts_t *mnspf, uint32_t *encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || mnspf == NULL || encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}
	if(len + SIZE_OF_MAX_NO_SUPPORTED_PACKET_FILTERS > bufLen) {
	    higLog("Buffer insufficient");
        errno = E_NAS_ERROR_BUFF_INSUFF;
	    nasLogEXIT;
	    return FAILURE;
	}

    buffer[len] = (mnspf->supportedPacketFilters)>>3;
    buffer[len+1] =((mnspf->supportedPacketFilters) & 0x07)<<5;
    len += SIZE_OF_MAX_NO_SUPPORTED_PACKET_FILTERS;    
    
    *encodedLen = len;

    nasLog("Max No. of Supported Packet Filters encoded : %d | currentLen = %d |"
		"encoded %d bytes", mnspf->supportedPacketFilters,len,
		SIZE_OF_MAX_NO_SUPPORTED_PACKET_FILTERS);
    
	nasLogEXIT;	
	return SUCCESS;
}
