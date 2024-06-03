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

#ifndef __PFCP_HDR_OUTER_HDR_REMOVAL
#define __PFCP_HDR_OUTER_HDR_REMOVAL

#define PFCP_OH_REM_GTPU_UDP_IPV4       0
#define PFCP_OH_REM_GTPU_UDP_IPV6       1
#define PFCP_OH_REM_UDP_IPV4            2
#define PFCP_OH_REM_UDP_IPV6            3
#define PFCP_OH_REM_IPV4                4
#define PFCP_OH_REM_IPV6                5
#define PFCP_OH_REM_GTPU_UDP_IP         6
#define PFCP_OH_REM_VLAN_STAG           7
#define PFCP_OH_REM_STAG_AND_CTAG       8

#define PFCP_OHR_GTPU_EXT_REM_PDU_SESSION_CONTAINER    (1<<0)

#define PFCP_OH_REM_MAX_LEN             2
#define PFCP_OH_REM_MIN_LEN             1


#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"



typedef struct outerHdrRemoval {
    uint16_t IEI;
    


    uint8_t description;
    uint8_t gtpUextensionHdrDel;

} outerHdrRemoval_t;

int outerHdrRemovalEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                          uint16_t *encodedLen);
int outerHdrRemovalDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                          uint16_t *decodedLen);

#endif
