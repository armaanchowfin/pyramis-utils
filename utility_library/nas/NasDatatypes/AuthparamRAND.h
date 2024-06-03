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

#ifndef __AUTHENTICATION_PARAMETER_RAND_H
#define __AUTHENTICATION_PARAMETER_RAND_H

#include "common/include/aka.h"

typedef struct RAND_s {
    uint8_t     RAND[RAND_SIZE];
} RAND_t;




int decodeAuthParamRAND(uint8_t *buffer, RAND_t *rand, uint32_t bufLen,
                        uint32_t* consumedLen);
int encodeAuthParamRAND(uint8_t *buffer, uint32_t buflen, RAND_t *rand,
                        uint32_t *encodedLen);

#endif 

