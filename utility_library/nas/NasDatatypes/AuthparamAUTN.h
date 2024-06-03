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

#ifndef __AUTHENTICATION_PARAMETER_AUTN_H
#define __AUTHENTICATION_PARAMETER_AUTN_H

#include "common/include/aka.h"

/* Since AUTN is always used as a optional field, encode & decodes funcs
 * assume it to be optional, i.e. IEI length is included in autn->len */


#define AUTH_PARAM_AUTN_LEN_MIN       1   

#define AUTH_PARAM_AUTN_LEN_MAX  AUTN_SIZE   



typedef struct AUTN_s {
    uint8_t len;
    uint8_t AUTN[AUTN_SIZE];
} AUTN_t;

int decodeAuthParamAUTN(uint8_t *buffer, AUTN_t *autn, uint32_t bufLen,
                        uint32_t* consumedLen);
int encodeAuthParamAUTN(uint8_t *buffer, uint32_t buflen, AUTN_t *autn,
                        uint32_t *encodedLen);


#endif 

