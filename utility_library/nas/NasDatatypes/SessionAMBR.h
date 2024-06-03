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

#ifndef  __NAS_HEADER_SESSION_AMBR
#define  __NAS_HEADER_SESSION_AMBR

#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"

#define VALUE_IS_NOT_USED                              0b00000000
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_KBPS    0b00000001
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_KBPS    0b00000010
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_KBPS   0b00000011
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_KBPS   0b00000100
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_KBPS  0b00000101
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_MBPS    0b00000110
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_MBPS    0b00000111
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_MBPS   0b00001000
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_MBPS   0b00001001
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_MBPS  0b00001010
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_GBPS    0b00001011
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_GBPS    0b00001100
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_GBPS   0b00001101
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_GBPS   0b00001110
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_GBPS  0b00001111
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_TBPS    0b00010000
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_TBPS    0b00010001
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_TBPS   0b00010010
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_TBPS   0b00010011
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_TBPS  0b00010100
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_PBPS    0b00010101
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4_PBPS    0b00010110
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16_PBPS   0b00010111
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64_PBPS   0b00011000
#define VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256_PBPS  0b00011001

#define STR_NOT_USED "Invalid Unit of Session AMBR"
#define STR_1_KBPS   " x 1 Kbps"
#define STR_4_KBPS   " x 4 Kbps"
#define STR_16_KBPS  " x 16 Kbps"
#define STR_64_KBPS  " x 64 Kbps"
#define STR_256_KBPS " x 256 Kbps"
#define STR_1_MBPS   " x 1 Mbps"
#define STR_4_MBPS   " x 4 Mbps"
#define STR_16_MBPS  " x 16 Mbps"
#define STR_64_MBPS  " x 64 Mbps"
#define STR_256_MBPS " x 256 Mbps"
#define STR_1_GBPS   " x 1 Gbps"
#define STR_4_GBPS   " x 4 Gbps"
#define STR_16_GBPS  " x 16 Gbps"
#define STR_64_GBPS  " x 64 Gbps"
#define STR_256_GBPS " x 256 Gbps"
#define STR_1_TBPS   " x 1 Tbps"
#define STR_4_TBPS   " x 4 Tbps"
#define STR_16_TBPS  " x 16 Tbps"
#define STR_64_TBPS  " x 64 Tbps"
#define STR_256_TBPS " x 256 Tbps"
#define STR_1_PBPS   " x 1 Pbps"
#define STR_4_PBPS   " x 4 Pbps"
#define STR_16_PBPS  " x 16 Pbps"
#define STR_64_PBPS  " x 64 Pbps"
#define STR_256_PBPS " x 256 Pbps"

const std::string session_AMBR_log_str[UCHAR_MAX] = {
   STR_NOT_USED,
   STR_1_KBPS,
   STR_4_KBPS,
   STR_16_KBPS,
   STR_64_KBPS,
   STR_256_KBPS,
   STR_1_MBPS,
   STR_4_MBPS,
   STR_16_MBPS,
   STR_64_MBPS,
   STR_256_MBPS,
   STR_1_GBPS,
   STR_4_GBPS,
   STR_16_GBPS,
   STR_64_GBPS,
   STR_256_GBPS,
   STR_1_TBPS,
   STR_4_TBPS,
   STR_16_TBPS,
   STR_64_TBPS,
   STR_256_TBPS,
   STR_1_PBPS,
   STR_4_PBPS,
   STR_16_PBPS,
   STR_64_PBPS,
   {STR_256_PBPS} /* Other values shall be interpreted as
                   * multiples of 256 Pbps in this version of the
                   * protocol. */
};

#define NAS_DATATYPE_SESSION_AMBR_LEN    6



typedef struct SessionAMBR {
    uint8_t   len;
    uint8_t   downlinkUnit;
    uint16_t  downlinkValue;
    uint8_t   uplinkUnit;
    uint16_t  uplinkValue;
} SessionAMBR_t;

int decodeSessionAMBR(uint8_t *buffer, uint32_t bufLen,
                        SessionAMBR_t *ambr, uint32_t* decodedLen);
int encodeSessionAMBR(uint8_t *buffer, uint32_t bufLen,
                        SessionAMBR_t *ambr, uint32_t *encodedLen);
#endif
