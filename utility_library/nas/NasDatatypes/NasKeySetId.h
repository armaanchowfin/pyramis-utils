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

#ifndef __NAS_KEY_SET_ID_H
#define __NAS_KEY_SET_ID_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"



typedef struct NaskeysetId {
    uint8_t kya_someth:4;      

    uint8_t         tsc:1;
    uint8_t         ngksi:3;
} NaskeysetId_t;



int decodeNasKeySetId(uint8_t *buffer, NaskeysetId_t *ngksi, uint32_t bufLen,
                      uint32_t* consumedLen, bool msbAligned=false);
int encodeNasKeySetId(uint8_t *buffer, uint32_t buflen, NaskeysetId_t *ngksi,
                      uint32_t *encodedLen, bool msbAligned=false);

#endif 

