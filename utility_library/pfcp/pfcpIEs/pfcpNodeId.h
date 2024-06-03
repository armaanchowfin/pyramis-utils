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

#ifndef  __PFCP_IE_NODE_ID_HEADER
#define  __PFCP_IE_NODE_ID_HEADER

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcp/include/pfcpCommon.h"

#define PFCP_NODE_ID_MIN_LEN            5
#define PFCP_NODE_ID_MAX_LEN            17  

#define PFCP_NODE_ID_TYPE_LEN           1

#define PFCP_NODE_ID_TYPE_IPV4         0
#define PFCP_NODE_ID_TYPE_IPV6         1
#define PFCP_NODE_ID_TYPE_FQDN         2




typedef struct nodeId {
    uint16_t IEI;
    

    uint8_t  nodeIdType:4;
    union {
        uint8_t ipv4Address[PFCP_IPV4_ADDR_LEN];
        uint8_t ipv6Address[PFCP_IPV6_ADDR_LEN];
        

    }u;
} nodeId_t;



int nodeIdEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen);
int nodeIdDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen);


#endif
