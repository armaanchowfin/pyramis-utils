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

#ifndef __MAX_NO_SUPPORTED_PACKET_FILTERS_H
#define __MAX_NO_SUPPORTED_PACKET_FILTERS_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "nas/nasUtil/nasCommon.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define SIZE_OF_MAX_NO_SUPPORTED_PACKET_FILTERS   2     



typedef struct maxNoSupPackFilts{
	uint16_t	supportedPacketFilters:11;
	uint8_t		spare:5;
}maxNoSupPackFilts_t;


int decodeMaxNoSupPackFilts(uint8_t *buffer, uint32_t bufLen,
			maxNoSupPackFilts_t *mnspf, uint32_t *decodedLen);
int encodeMaxNoSupPackFilts(uint8_t *buffer, uint32_t bufLen,
            maxNoSupPackFilts_t *_mnspf, uint32_t *encodedLen);

#endif
