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

#ifndef __PFCP_IE_CAUSE_HEADER
#define __PFCP_IE_CAUSE_HEADER

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"



#define PFCP_CAUSE_REQUEST_ACCEPTED                         1
#define PFCP_CAUSE_REQUEST_REJECTED                        64
#define PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND               65
#define PFCP_CAUSE_MANDATORY_IE_MISSING                    66
#define PFCP_CAUSE_CONDITIONAL_IE_MISSING                  67
#define PFCP_CAUSE_INVALID_LENGTH                          68
#define PFCP_CAUSE_MANDATORY_IE_INCORRECT                  69
#define PFCP_CAUSE_INVALID_FORWARDING_POLICY               70
#define PFCP_CAUSE_INVALID_F_TEID_ALLOCATION               71
#define PFCP_CAUSE_NO_ESTABLISHED_PFCP                     72
#define PFCP_CAUSE_RULE_CREATION_MODIFICATION              73
#define PFCP_CAUSE_PFCP_ENTITY_IN_CONGESTION               74
#define PFCP_CAUSE_NO_RESOURCES_AVAILABLE                  75
#define PFCP_CAUSE_SERVICE_NOT_SUPPORTED                   76
#define PFCP_CAUSE_SYSTEM_FAILURE                          77





#define PFCP_CAUSE_LEN          1   


typedef struct pfcpCause {
    uint16_t IEI;
    

    uint8_t  cause;
} pfcpCause_t;

int pfcpCauseEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen);
int pfcpCauseDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen);

#endif
