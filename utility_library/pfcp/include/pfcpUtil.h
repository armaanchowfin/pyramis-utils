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

#ifndef __PFCP_HEADER_UTILS
#define __PFCP_HEADER_UTILS

#include "pfcp/include/pfcpLogging.h"
#include "pfcp/include/pfcpDefs.h"

union pfcpIE;
typedef pfcpIE pfcpIE_t;

#define PFCP_IEI_LEN        2
#define PFCP_IE_LEN_SIZE    2
#define PFCP_IE_HDR_LEN     4   





int inline addIEI(uint8_t *buffer, uint16_t IEI) {
    

    buffer[0] = IEI >> 8;
    buffer[1] = IEI & 0xFF;
    return 2;
}

int inline getIEI(uint8_t *buffer, uint16_t *IEI) {
    

    *IEI = 0 | ((uint16_t)buffer[0] << 8) | ((uint16_t)buffer[1]);
    return 2;
}

int inline addLEN(uint8_t *buffer, uint16_t LEN) {
    

    buffer[0] = LEN >> 8;
    buffer[1] = LEN & 0xFF;
    return 2;
}

int inline getLEN(uint8_t *buffer, uint16_t *LEN) {
    

    *LEN = 0 | (((uint16_t)buffer[0]) << 8) | ((uint16_t)buffer[1]);
    return 2;
}

int inline getU64(uint8_t *buffer, uint64_t *val) {
    

    *val = 0 | ((uint64_t)buffer[0] << 56) |  ((uint64_t)buffer[1] << 48) |
               ((uint64_t)buffer[2] << 40) |  ((uint64_t)buffer[3] << 32) |
               ((uint64_t)buffer[4] << 24) |  ((uint64_t)buffer[5] << 16) |
               ((uint64_t)buffer[6] << 8) | ((uint64_t)buffer[7]);
    return 8;
}

int inline getU32(uint8_t *buffer, uint32_t *val) {
    

    *val = 0 | ((uint32_t)buffer[0] << 24) |  ((uint32_t)buffer[1] << 16 ) | 
              ((uint32_t)buffer[2] << 8) | ((uint32_t)buffer[3]);
    return 4;
}

int inline getU16(uint8_t *buffer, uint16_t *val) {
    

    *val = 0 | ((uint16_t)buffer[0] << 8) | ((uint16_t)buffer[1]);
    return 2;
}

int inline addU64(uint8_t *buffer, uint64_t val) {
    

    buffer[0] = val >> 56;
    buffer[1] = (val >> 48) & 0xFF;
    buffer[2] = (val >> 40) & 0xFF;
    buffer[3] = (val >> 32) & 0xFF;
    buffer[4] = (val >> 24) & 0xFF;
    buffer[5] = (val >> 16) & 0xFF;
    buffer[6] = (val >> 8) & 0xFF;
    buffer[7] = val & 0xFF;
    return 8;
}

int inline addU32(uint8_t *buffer, uint32_t val) {
    

    buffer[0] = val >> 24;
    buffer[1] = (val >> 16) & 0xFF;
    buffer[2] = (val >> 8) & 0xFF;
    buffer[3] = val & 0xFF;
    return 4;
}

int inline addU16(uint8_t *buffer, uint16_t val) {
    

    buffer[0] = val >> 8;
    buffer[1] = val & 0xFF;
    return 2;
}


/* Function to encode/decode a IE which is made up of multiple IEs.
 * e.g. Create PDR IE contains multiple IEs.
 * Fun Fact: Also used by PFCP Message, since it is also a List of IEs :-) */
int pfcpIeEncode(uint8_t *buffer, uint16_t bufLen, std::vector<pfcpIE_t> &ieList,
              const std::map<uint16_t, pfcpIEdata_t> typeData,
              uint16_t *encodedLen);

int pfcpIeDecode(uint8_t *buffer, uint16_t bufLen, std::vector<pfcpIE_t> &ieList,
              const std::map<uint16_t, pfcpIEdata_t> typeData,
              uint16_t *decodedLen);

#endif
