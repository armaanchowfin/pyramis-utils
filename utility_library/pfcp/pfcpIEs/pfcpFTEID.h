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

#ifndef __PFCP_HDR_PFCP_F_TEID
#define __PFCP_HDR_PFCP_F_TEID

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"
#include "pfcp/include/pfcpCommon.h"

#define PFCP_F_TEID_MIN_LEN     1


#define PFCP_F_TEID_MAX_LEN     25
/* size(CHID,CH,V4,V6) + size(TEID) + size(v4addr) + size(v6addr)
 *      1              +    4       +   4          +   16           = 25 */

#define PFCP_F_TEID_MASK_LEN        1
#define PFCP_F_TEID_TEID_LEN        4
#define PFCP_F_TEID_CHOOSEID_LEN    1


/*
 * CHID | CH | V6 | V4 |  Encode
 *  0      0    x    y    Encode v6 addr if x=1, Encode v4Addr if y=1
 *                        Encode TEID
 *                        if CH=0, CHID must be 0
 *  z      1    x    y    v4, v6 Addr wont be Encoded for any value of x and y
 *                        Dont Encode TEID
 *                        Encode ChooseId if z=1
 *      ( CH is to 1 by CP function to let the UP choose the TEID,
 *        UP should support PDI optimization,
 *        ChooseId functionality unclear.)
 */



typedef struct pfcpFTEID {
    uint16_t IEI;
    


    uint8_t CHID:1;
    uint8_t CH:1;
    uint8_t V6:1;
    uint8_t V4:1;

    uint32_t TEID;
    uint8_t ipv4Address[PFCP_IPV4_ADDR_LEN];
    uint8_t ipv6Address[PFCP_IPV6_ADDR_LEN];
    uint8_t chooseId;
} pfcpFTEID_t;

int pfcpFTEIDencode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *encodedLen);
int pfcpFTEIDdecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                     uint16_t *decodedLen);

#endif
