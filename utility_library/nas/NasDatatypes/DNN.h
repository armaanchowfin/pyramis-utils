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

#ifndef __NAS_HEADER_DNN_H
#define __NAS_HEADER_DNN_H

#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"

#define  DNN_IE_MIN_LEN  1
#define  DNN_IE_MAX_LEN  100

typedef struct DNN {
    uint8_t     len;
    uint8_t     value[DNN_IE_MAX_LEN];
} DNN_t;


int decodeDNN(uint8_t *buffer,  uint32_t bufLen, DNN_t *msgStruct_p,
              uint32_t *decodedLen);
int encodeDNN(uint8_t *buffer, uint32_t bufLen, DNN_t *msgStruct,
              uint32_t *encodedLen);



#endif
