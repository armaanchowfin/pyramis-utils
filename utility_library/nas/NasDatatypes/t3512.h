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

#ifndef __t3512_H
#define __t3512_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define REG_ACCEPT_T3512_IEI_OFFSET  0

typedef struct _t3512 {
    uint8_t          len;
    uint8_t          unit:3;
    uint8_t          value:5;
}t3512_t;

int decodet3512(uint8_t *buffer, uint32_t bufLen,
			           t3512_t* t3512, uint32_t* decodedLen);
int encodet3512(uint8_t *buffer, uint32_t bufLen,
                       t3512_t* t3512, uint32_t *encodedLen);

#endif 

