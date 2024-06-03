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

#ifndef __PFCP_HDR_FAILED_RULE_ID
#define __PFCP_HDR_FAILED_RULE_ID

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"

#define PFCP_FAILED_RULEID_MIN_LENGTH  3
#define PFCP_FAILED_RULEID_MAX_LENGTH  5

#define PFCP_FAILED_RULE_ID_TYPE_SIZE  1

#define PFCP_FAILED_RULE_ID_TYPE_PDR      0
#define PFCP_FAILED_RULE_ID_PDR_ID_SIZE   2

#define PFCP_FAILED_RULE_ID_TYPE_FAR      1
#define PFCP_FAILED_RULE_ID_FAR_ID_SIZE   4





typedef struct failedRuleId {
    uint16_t IEI;
    


    uint8_t ruleIdType:5;
    union {
        uint16_t pdrId;
        uint32_t farId;
        

    } ruleIdValue;

} failedRuleId_t;

int failedRuleIdencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                       uint16_t *encodedLen);
int failedRuleIddecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                       uint16_t *decodedLen);

#endif
