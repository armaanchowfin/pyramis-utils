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

#ifndef __PFCP_IE_OUTER_HEADER_CREATION
#define __PFCP_IE_OUTER_HEADER_CREATION

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcp/include/pfcpCommon.h"

#define PFCP_OH_CREATN_GTPU_UDP_IPV4 (1<<8)
#define PFCP_OH_CREATN_GTPU_UDP_IPV6 (1<<9)
#define PFCP_OH_CREATN_UDP_IPV4      (1<<10)
#define PFCP_OH_CREATN_UDP_IPV6      (1<<11)

#define PFCP_OH_CREATN_MIN_LEN        8     

#define PFCP_OH_CREATN_MAX_LEN        22    


#define PFCP_OH_CREATN_DESCRIPTN_LEN  2
#define PFCP_OH_CREATN_TEID_LEN       4
#define PFCP_OH_PORT_LEN              2




typedef struct outHdrCreatn {
    uint16_t IEI;
    

    uint16_t  description;
    uint32_t  teid;
    uint8_t   ipv4Addr[PFCP_IPV4_ADDR_LEN];
    uint8_t   ipv6Addr[PFCP_IPV6_ADDR_LEN];
    uint16_t  port;



} outHdrCreatn_t;

int outHdrCreatnEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen);
int outHdrCreatnDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen);

#endif
