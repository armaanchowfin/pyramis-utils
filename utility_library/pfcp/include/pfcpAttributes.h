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

#include "pfcp/include/pfcpLogging.h"

typedef struct fteid {
    uint8_t CHID:1;
    uint8_t CH:1;
    uint8_t V6:1;
    uint8_t V4:1;
    uint32_t TEID;
    uint8_t ipv4Address[PFCP_IPV4_ADDR_LEN];
    uint8_t ipv6Address[PFCP_IPV6_ADDR_LEN];
    uint8_t chooseId;
} fteid_t;

typedef struct pktRate {
    uint8_t DLPR:1;
    uint8_t ULPR:1;
    uint8_t uplinkTimeUnit:3;
    uint16_t maximumUplinkPacketRate;
    uint8_t downlinkTimeUnit:3;
    uint16_t maximumDownlinkPacketRate;
} pktRate_t;




typedef struct ohr {
    uint8_t description;
    uint8_t gtpUextensionHdrDel;

} ohr_t;

typedef struct ohc {
    uint16_t  description;
    uint32_t  teid;
    uint8_t   ipv4Addr[PFCP_IPV4_ADDR_LEN];
    uint8_t   ipv6Addr[PFCP_IPV6_ADDR_LEN];
    uint16_t  port;
} ohc_t;

typedef struct mbr {
    uint64_t UL_MBR:40;
    uint64_t DL_MBR:40;
} mbr_t;

typedef struct gbr {
    uint64_t UL_GBR:40;
    uint64_t DL_GBR:40;
} gbr_t;

typedef struct qfi {
    uint8_t  value:6;
} qfi_t;

