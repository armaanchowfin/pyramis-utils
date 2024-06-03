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

#ifndef __INTEG_PROT_MAX_DATA_RATE
#define __INTEG_PROT_MAX_DATA_RATE

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"

#define INTEG_PROT_MAX_DATA_RATE_64_KBPS   0
#define INTEG_PROT_MAX_DATA_RATE_FULL      0XFF

#define INTEG_PROT_MAX_DATA_RATE_LEN       2

typedef struct integProtMaxDataRate {
    uint8_t downlinkRate;
    uint8_t uplinkRate;
} integProtMaxDataRate_t;

int decodeIntegProtMaxDataRate(uint8_t *buffer, uint32_t bufLen,
                    integProtMaxDataRate_t * ipmdr, uint32_t* decodedLen);
int encodeIntegProtMaxDataRate(uint8_t *buffer, uint32_t bufLen,
                    integProtMaxDataRate_t* ipmdr, uint32_t *encodedLen);
#endif  

