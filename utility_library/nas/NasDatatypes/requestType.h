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

#ifndef __NAS_HEADER_REQUEST_TYPE_H
#define __NAS_HEADER_REQUEST_TYPE_H

#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"

#define REQUEST_TYPE_INVALID_VALUE                      0b000



#define REQUEST_TYPE_INITIAL_REQUEST                    0b001
#define REQUEST_TYPE_EXISTING_PDU_SESSION               0b010
#define REQUEST_TYPE_INITIAL_EMERGENCY_REQUEST          0b011
#define REQUEST_TYPE_EXISTING_EMERGENCY_PDU_SESSION     0b100
#define REQUEST_TYPE_MODIFICATION_REQUEST               0b101
#define REQUEST_TYPE_RESERVED                           0b111




typedef struct requestType {
    

    uint8_t value:3;
} requestType_t;

int decodeRequestType(uint8_t *buffer,  uint32_t bufLen,
                      requestType_t *msgStruct, uint32_t *decodedLen);
int encodeRequestType(uint8_t *buffer, uint32_t bufLen,
                      requestType_t *msgStruct, uint32_t *encodedLen);

#endif
