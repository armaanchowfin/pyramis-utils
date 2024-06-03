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

#ifndef __5GS_TRACKING_AREA_IDENTITY_H
#define __5GS_TRACKING_AREA_IDENTITY_H


#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "common/include/utils.h"

#define TAC_SIZE 3
#define TAI_SIZE 7



typedef struct tAI {

    uint8_t mcc1:4;
    uint8_t mcc2:4;
    uint8_t mcc3:4;
    uint8_t mnc3:4;
    uint8_t mnc1:4;
    uint8_t mnc2:4;
    uint8_t tac[TAC_SIZE];

} tAI_t;

int decodeTAI(uint8_t *buffer, uint32_t bufLen,
                       tAI_t *tAI, uint32_t* decodedLen);
int encodeTAI(uint8_t *buffer, uint32_t bufLen,
                       tAI_t *tAI, uint32_t *encodedLen);

#endif 

