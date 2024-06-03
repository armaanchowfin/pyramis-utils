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

#ifndef  __NAS_HEADER_PDU_SESSION_ID
#define  __NAS_HEADER_PDU_SESSION_ID

#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"



#define     NO_PDU_SESSION_IDENTITY_ASSIGNED   0b00000000
#define     PDU_SESSION_IDENTITY_VALUE_1       0b00000001
#define     PDU_SESSION_IDENTITY_VALUE_2       0b00000010
#define     PDU_SESSION_IDENTITY_VALUE_3       0b00000011
#define     PDU_SESSION_IDENTITY_VALUE_4       0b00000100
#define     PDU_SESSION_IDENTITY_VALUE_5       0b00000101
#define     PDU_SESSION_IDENTITY_VALUE_6       0b00000110
#define     PDU_SESSION_IDENTITY_VALUE_7       0b00000111
#define     PDU_SESSION_IDENTITY_VALUE_8       0b00001000
#define     PDU_SESSION_IDENTITY_VALUE_9       0b00001001
#define     PDU_SESSION_IDENTITY_VALUE_10      0b00001010
#define     PDU_SESSION_IDENTITY_VALUE_11      0b00001011
#define     PDU_SESSION_IDENTITY_VALUE_12      0b00001100
#define     PDU_SESSION_IDENTITY_VALUE_13      0b00001101
#define     PDU_SESSION_IDENTITY_VALUE_14      0b00001110
#define     PDU_SESSION_IDENTITY_VALUE_15      0b00001111

#define PDU_SESSION_ID_LEN  1



typedef struct PDUSessionId2 {
    uint8_t   value;
} PDUSessionId2_t;

int decodePDUSessionId2(uint8_t *buffer, uint32_t bufLen,
                        PDUSessionId2_t *pduSid, uint32_t* decodedLen);
int encodePDUSessionId2(uint8_t *buffer, uint32_t bufLen,
                       PDUSessionId2_t * pduSid, uint32_t *encodedLen);
#endif
