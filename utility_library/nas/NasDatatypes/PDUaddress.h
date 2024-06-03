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

#ifndef __NAS_SM_PDU_ADDRESS
#define __NAS_SM_PDU_ADDRESS

#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasCommon.h"

#define PDU_ADDRESS_TYPE_IPV4       0b001
#define PDU_ADDRESS_TYPE_IPV6       0b010
#define PDU_ADDRESS_TYPE_IPV4v6     0b011

#define PDU_ADDRESS_IPV4_LEN        4
#define PDU_ADDRESS_LL_IPV6_LEN     8



#define PDU_ADDRESS_IE_IPV4_LEN      5
#define PDU_ADDRESS_IE_IPV6_LEN      9
#define PDU_ADDRESS_IE_IPV4V6_LEN    13

#define PDU_ADDRESS_CONTENT_LEN_SIZE     1
#define PDU_ADDRESS_CONTENT_TYPE_SIZE    1

typedef struct pduAddress {
    uint8_t:5;
    uint8_t type:3;
    uint8_t ipv6Address[PDU_ADDRESS_LL_IPV6_LEN];
    uint8_t ipv4Address[PDU_ADDRESS_IPV4_LEN];
} pduAddress_t;

int decodePduAddress(uint8_t *buffer,  uint32_t bufLen,
                     pduAddress_t *pduAddr, uint32_t *decodedLen);
int encodePduAddress(uint8_t *buffer, uint32_t bufLen,
                     pduAddress_t *pduAddr, uint32_t *encodedLen0);


#endif
