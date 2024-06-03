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

#ifndef   __PFCP_IE_PACKET_DETECTION_RULE_ID
#define   __PFCP_IE_PACKET_DETECTION_RULE_ID

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"

#define PFCP_PDR_ID_LEN     2



typedef struct pfcpPdrId{
    uint16_t IEI;
    

    uint16_t ruleId;
} pfcpPdrId_t;

int pfcpPdrIDencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen);
int pfcpPdrIDdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen);
#endif
