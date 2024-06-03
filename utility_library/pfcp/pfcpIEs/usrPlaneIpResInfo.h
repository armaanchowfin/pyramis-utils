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

#ifndef __PFCP_IE_USER_PLAN_IP_RESOURCE_INFO
#define __PFCP_IE_USER_PLAN_IP_RESOURCE_INFO

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcp/include/pfcpCommon.h"
#include "pfcp/pfcpIEs/sdInterface.h"   


#define UP_IP_RES_INFO_MASK_LEN  1
#define UP_IP_RES_INFO_NTW_INST_LEN 100    

#define UP_IP_RES_INFO_SRC_IFACE_LEN 1
#define UP_IP_RES_INFO_TEID_RANGE_LEN 1

#define UP_IP_RES_INFO_MAX_LEN  127
#define UP_IP_RES_INFO_MIN_LEN  5




typedef struct usrPlaneIpResInfo {
    uint16_t IEI;
    

    uint8_t:1;
    uint8_t ASSO_SI:1;
    uint8_t ASSO_NI:1;
    uint8_t TEIDRI:3;
    uint8_t V6:1;
    uint8_t V4:1;

    uint8_t teidRange;
    uint8_t ipv4Address[PFCP_IPV4_ADDR_LEN];
    uint8_t ipv6Address[PFCP_IPV6_ADDR_LEN];
    uint8_t netwInstLen;        

    uint8_t netwInst[UP_IP_RES_INFO_NTW_INST_LEN];
    uint8_t srcIface:4;

} usrPlaneIpResInfo_t;

int usrPlaneIpResInfoEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                            uint16_t *encodedLen);
int usrPlaneIpResInfoDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                            uint16_t *decodedLen);

#endif
