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

#ifndef  __PFCP_HDR_UE_IP_ADDRESS
#define  __PFCP_HDR_UE_IP_ADDRESS

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcp/include/pfcpCommon.h"

#define PFCP_UE_IPADDR_MAX_LEN   22


#define PFCP_UE_IPADDR_MIN_LEN   5



#define PFCP_UE_IPADDR_MASK_LEN              1
#define PFCP_UE_IPADDR_IPV6_PD_BITS_LEN      1



#define PFCP_UE_IPADDR_SRC_ADDR   0
#define PFCP_UE_IPADDR_DEST_ADDR  1


typedef struct ueIpaddress {
    uint16_t IEI;
    


    uint8_t IPv6D:1;
    uint8_t SrcOrDest:1;    

    uint8_t V4:1;
    uint8_t V6:1;
    uint8_t ipv4Address[PFCP_IPV4_ADDR_LEN];
    uint8_t ipv6Address[PFCP_IPV6_ADDR_LEN];
    uint8_t ipv6PrefixDelegBits;
} ueIpaddress_t;

int ueIpaddressEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen);
int ueIpaddressDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen);

#endif
