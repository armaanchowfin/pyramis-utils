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

#ifndef __PFCP_IE_SUGGESTED_BUF_PACKET_COUNT
#define __PFCP_IE_SUGGESTED_BUF_PACKET_COUNT

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"

#define PFCP_SUG_BUF_PKT_COUNT_LEN                  1
#define PFCP_SUG_BUF_PKT_COUNT_MAX                  255

typedef struct pfcpSuggestedBufPktCount {
    uint16_t IEI;
    

    uint8_t value;

} pfcpSuggestedBufPktCount_t;

int pfcpSuggestedBufPktCountEncode(uint8_t *buffer, uint16_t bufLen,
                                            void* ptr, uint16_t *encodedLen);
int pfcpSuggestedBufPktCountDecode(uint8_t *buffer, uint16_t bufLen,
                                            void* ptr, uint16_t *decodedLen);

#endif


