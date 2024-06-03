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

#ifndef __ABBA_H
#define __ABBA_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"

#define ABBA_OFFSET   1 

#define ABBA_LEN_MIN  1 

#define ABBA_LEN_MAX 24 



#define ABBA_CONTENTS_MAX_SIZE  (ABBA_LEN_MAX)

typedef struct ABBA {
    uint8_t len;
    uint8_t contents[ABBA_CONTENTS_MAX_SIZE];
} ABBA_t;

int decodeABBA(uint8_t *buffer, ABBA_t *abba, uint32_t bufLen,
                      uint32_t* consumedLen);
int encodeABBA(uint8_t *buffer, uint32_t buflen, ABBA_t *abba,
                      uint32_t *encodedLen);

#endif 

