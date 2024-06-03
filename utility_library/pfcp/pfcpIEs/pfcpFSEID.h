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

#ifndef  __PFCP_IE_F_SEID_HEADER
#define  __PFCP_IE_F_SEID_HEADER

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcp/include/pfcpCommon.h"

#define PFCP_FSEID_SEID_LENGTH 8
#define PFCP_FSEID_IPV4_ONLY   0x02
#define PFCP_FSEID_IPV6_ONLY   0x01
#define PFCP_FSEID_IPV4_IPV6   0x03
#define PFCP_FSEID_MIN_LEN 13


#define PFCP_FSEID_MAX_LEN 33


#define PFCP_FSEID_MAX_LEN 33

#define PFCP_FSEID_MASK_LEN  1



typedef struct fseid {
    uint16_t IEI;
    

    uint8_t  v4v6:2;
    uint64_t seid;
    uint8_t ipv4Address[PFCP_IPV4_ADDR_LEN];
    uint8_t ipv6Address[PFCP_IPV6_ADDR_LEN];

} pfcpFSEID_t;


int fseidEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen);
int fseidDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen);


#endif
