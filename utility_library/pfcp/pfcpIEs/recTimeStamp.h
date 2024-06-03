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

#ifndef __PFCP_HEADER_RECOVERY_TIME_STAMP
#define __PFCP_HEADER_RECOVERY_TIME_STAMP




#define PFCP_TIME_STAMP_LENGTH          4
#define PFCP_REC_TIME_STAMP_LENGTH      4   


#include "pfcp/include/pfcpDefs.h"

typedef struct recTimeStamp {
    uint16_t IEI;
    

    uint8_t     timestamp[PFCP_REC_TIME_STAMP_LENGTH];
} recTimeStamp_t;

/* timestamp should be filled in network byte order.
 * save htonl(time(NULL) + EPOCH_SHIFT) into a uint32_t,
 * and fill timestamp by using the TIME_STAMP_TO_OCTET macro.
 * */

/* Unix time has EPOCH Jan 1 1970. NTP has EPOCH Jan 1 1990.
The corresponding number of seconds that we need to add when using time() to get recovery time stamp */ 
#define EPOCH_SHIFT 2208988800



#define TIME_STAMP_TO_OCTET(timeStamp, buffer)({        \
                buffer[0] = timeStamp & 0xFF;           \
                buffer[1] = (timeStamp >> 8) & 0xFF;    \
                buffer[2] = (timeStamp >> 16) & 0xFF;   \
                buffer[3] = (timeStamp >> 24) & 0xFF;   \
})

#define OCTET_TO_TIME_STAMP(buffer, timeStamp)({            \
                timeStamp = 0;                              \
                timeStamp = (uint32_t)buffer[0];            \
                timeStamp |= ((uint32_t)buffer[1]) << 8;    \
                timeStamp |= ((uint32_t)buffer[2]) << 16;   \
                timeStamp |= ((uint32_t)buffer[3]) << 24;   \
})

int recTimeStampEncode(uint8_t *buffer, uint16_t bufLen, void *ptr,
                        uint16_t* encodedLen);
int recTimeStampDecode(uint8_t *buffer, uint16_t bufLen, void *ptr,
                        uint16_t* decodedLen);


#endif
