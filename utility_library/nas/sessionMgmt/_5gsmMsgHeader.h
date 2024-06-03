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

#ifndef ___5GSM_MSG_HEADER_H
#define ___5GSM_MSG_HEADER_H

#include "common/include/datatypes.h"

#define SIZE_5G_SESSION_MESSAGE_HEADER     4  

typedef struct _5gsmMsgHeader {
    ExtendedProtocolDiscriminator_t         epd;
    uint8_t                                 align[5];
    PduSessionid_t                          PduSessionid;
    Pti_t                                   Pti;
    NasMessageType_t                        msgType;
}_5gsmMsgHeader_t;

#endif
