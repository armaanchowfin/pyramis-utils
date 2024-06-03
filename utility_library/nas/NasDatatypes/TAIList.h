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

#ifndef __5GS_TRACKING_AREA_IDENTITY_LIST_H
#define __5GS_TRACKING_AREA_IDENTITY_LIST_H


#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/TAI.h"
#include "common/include/utils.h"

#define TAI_LIST_LEN_MIN 9
#define TAI_LIST_LEN_MAX 114
#define TAI_LIST_OFFSET 2 


#define MAX_NUM_OF_TAI 16

#define PARTIAL_LIST_TYPE_0 0
#define PARTIAL_LIST_TYPE_1 1
#define PARTIAL_LIST_TYPE_2 2


typedef struct partialTAIList {

    uint8_t numOfElmnts:5;
    uint8_t listType:2;
    tAI_t tAI[MAX_NUM_OF_TAI];

} partialTAIList_t;



typedef struct tAIList {

    uint8_t len;
    partialTAIList_t pTAI[MAX_NUM_OF_TAI];
    uint8_t filled; 


    tAIList() {
        memset(this, 0, sizeof(*this));
    }

} tAIList_t;

int addTAIUint(uint16_t mcc, uint16_t mnc, uint8_t mncLen, uint32_t tac,
                                            partialTAIList_t* pTAI);

int addPartialTAIList(partialTAIList_t pTAI, tAIList_t* taiList);

int decodeTAIList(uint8_t *buffer, uint32_t bufLen,
                       tAIList_t *taiList, uint32_t* decodedLen);
int encodeTAIList(uint8_t *buffer, uint32_t bufLen,
                       tAIList_t *taiList, uint32_t *encodedLen);

#endif 

