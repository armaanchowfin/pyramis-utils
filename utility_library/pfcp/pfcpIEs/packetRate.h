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

#ifndef __PFCP_IE_PACKET_RATE
#define __PFCP_IE_PACKET_RATE

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"

#define PFCP_PACKET_RATE_MIN_LEN  4     
#define PFCP_PACKET_RATE_MAX_LEN  7     
#define PFCP_PACKET_RATE_ULPR_LEN   3
#define PFCP_PACKET_RATE_DLPR_LEN   3
#define PFCP_PACKET_RATE_MAX_UPLINK_PACKET_RATE_LEN   2
#define PFCP_PACKET_RATE_MAX_DOWNLINK_PACKET_RATE_LEN    2
#define PFCP_PACKET_RATE_MASK_LEN   1

typedef struct packetRate {
    uint16_t IEI;
    

    uint32_t value;
    
    uint8_t DLPR:1;
    uint8_t ULPR:1;
    
    uint8_t uplinkTimeUnit:3;
    uint16_t maximumUplinkPacketRate;
    
    uint8_t downlinkTimeUnit:3;
    uint16_t maximumDownlinkPacketRate;
} packetRate_t;

int packetRateEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen);
int packetRateDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen);

#endif
