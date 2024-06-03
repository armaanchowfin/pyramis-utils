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

#ifndef __PFCP_IE_APPLY_ACTION
#define __PFCP_IE_APPLY_ACTION

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"

#define PFCP_APPLY_ACTION_LEN  1
#define PFCP_APPLY_ACTION_DROP_MASK 0x01
#define PFCP_APPLY_ACTION_FORW_MASK 0x02
#define PFCP_APPLY_ACTION_BUFF_MASK 0x04
#define PFCP_APPLY_ACTION_NOCP_MASK 0x08

#define APPLY_ACTION_DROP (1<<0)
#define APPLY_ACTION_FORW (1<<1)
#define APPLY_ACTION_BUFF (1<<2)
#define APPLY_ACTION_NOCP (1<<3)
#define APPLY_ACTION_DUPL (1<<4)
#define APPLY_ACTION_BUFF_NOCP (APPLY_ACTION_BUFF|APPLY_ACTION_NOCP)



typedef struct applyAction {
    uint16_t IEI;
    

    uint8_t  DROP:1;
    uint8_t  FORW:1;
    uint8_t  BUFF:1;
    uint8_t  NOCP:1;
    uint8_t  DUPL:1;

} applyAction_t;

int pfcpApplyActionEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen);
int pfcpApplyActionDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen);

#endif
