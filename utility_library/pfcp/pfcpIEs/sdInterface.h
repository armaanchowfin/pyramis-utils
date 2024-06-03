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

#ifndef __PFCP_IE_SOURCE_INTERFACE
#define __PFCP_IE_SOURCE_INTERFACE

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"





#define PFCP_SOURCE_IFACE_VALUE_ACCESS              0
#define PFCP_SOURCE_IFACE_VALUE_CORE                1
#define PFCP_SOURCE_IFACE_VALUE_SGI_OR_N6_LAN       2
#define PFCP_SOURCE_IFACE_VALUE_CP_FUNC             3



#define PFCP_DEST_IFACE_VALUE_ACCESS              0
#define PFCP_DEST_IFACE_VALUE_CORE                1
#define PFCP_DEST_IFACE_VALUE_SGI_OR_N6_LAN       2
#define PFCP_DEST_IFACE_VALUE_CP_FUNC             3
#define PFCP_DEST_IFACE_VALUE_LI_FUNC             4

#define PFCP_SOURCE_IFACE_LEN 1





typedef struct sdIface {
    uint16_t IEI;
    


    uint8_t interfaceValue:4;
} sdIface_t;

int sdIfaceEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                         uint16_t *encodedLen);
int sdIfaceDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                         uint16_t *decodedLen);


#endif
