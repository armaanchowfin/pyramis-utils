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

#ifndef   __PFCP_IE_CREATE_FAR
#define   __PFCP_IE_CREATE_FAR

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcp/pfcpIEs/pfcpAny.h"

union pfcpIE;
typedef pfcpIE pfcpIE_t;



typedef pfcpAny_t createFAR_ANY_t;

typedef struct createFAR {
    std::vector<pfcpIE_t> IElist;
} createFAR_t;

int createFARencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen);
int createFARdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen);
#endif
