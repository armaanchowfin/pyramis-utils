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

#ifndef ___5GMM_MSG_HEADER_H
#define ___5GMM_MSG_HEADER_H

#include "common/include/datatypes.h"

#define SIZE_5G_MOBILITY_MESSAGE_HEADER     3  


typedef struct _5gmmMsgHeader {
    ExtendedProtocolDiscriminator_t         epd;
    SecurityHeaderType_t                    secuHeader;
    uint8_t                                 align[7];
    /* 5 bytes to align with nasMessageSecurityHeader_t. DONOT use directly
     * use header to fill macCode and seqNum */
    NasMessageType_t                        msgType;
}_5gmmMsgHeader_t;

#endif
