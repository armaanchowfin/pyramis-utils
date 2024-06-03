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

#ifndef __SSC_MODE_H
#define __SSC_MODE_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define SIZE_OF_SSC_MODE	1

#define SSC_MODE_INVALID    0b000
#define SSC_MODE_1 			0b001
#define SSC_MODE_2  		0b010
#define SSC_MODE_3  		0b011
#define UNUSED_SSC_MODE_1  	0b100
#define UNUSED_SSC_MODE_2  	0b101
#define UNUSED_SSC_MODE_3  	0b110



typedef struct sscMode{
	uint8_t:5;      

	uint8_t ssc_mode_value:3;
}sscMode_t;

int decodeSSCMode(uint8_t *buffer, uint32_t bufLen,
			           sscMode_t *sscMode, uint32_t* decodedLen);
int encodeSSCMode(uint8_t *buffer, uint32_t bufLen,
                       sscMode_t *sscMode, uint32_t *encodedLen);

#endif
