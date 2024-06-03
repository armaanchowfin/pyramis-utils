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

#ifndef __5GS_REGISTRATION_RESULT_H
#define __5GS_REGISTRATION_RESULT_H
#define REG_RESULT_SIZE 1

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"



typedef struct _5gRegistrationResult {
    uint8_t          len;
    uint8_t          value:3;
    uint8_t          smsAllowed:1;   

    uint8_t          spare:4;          

}_5gRegistrationResult_t;



typedef enum _5gRegistrationResultValue {
    E_3GPP_ACCESS                    = 0b001,
    E_NON_3GPP_ACCESS                = 0b010,
    E_3GPP_NON_3GPP_ACCESS           = 0b011,
    E_RESERVED                       = 0b111
}_5gRegistrationResultValue_t;

int decode5GSRegResult(uint8_t *buffer, uint32_t bufLen,
			           _5gRegistrationResult_t *regResult, uint32_t* decodedLen);
int encode5GSRegResult(uint8_t *buffer, uint32_t bufLen,
                       _5gRegistrationResult_t *regResult, uint32_t *encodedLen);

#endif 

