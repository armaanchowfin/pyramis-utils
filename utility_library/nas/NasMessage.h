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

/*
    TODO : refine the arguments : optimize in in the best possibel way
    This file consists of the top level structs and unions
    It contains top level NAS encode and decoder as well as some
    helper functions
*/
#ifndef __NAS_MESSAGE_H
#define __NAS_MESSAGE_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "common/include/setup.h"
#include "common/include/aka.h"
#include "platform/include/platform.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/sessionMgmt/_5gsmMsgApi.h"
#include "nas/nasUtil/nasLogging.h"
#include "crypto5GT/include/cryptUtil.h"
#define SIZE_EPD_AND_SECU_HEADER   2  

#define SIZE_MM_SECU_HEADER        7  

#define SIZE_NASMAC_AND_SQN_NUM    5  

#define SIZE_OF_IEI                1
/*

NasMessageType_t structure:

+----------------------+----------------------------------------------------+
| header        Bytes  | plain              Bytes     plain                 |
| |                    | L_ _5gmmMsg                 L_ _5gsmMsg  TODO      |
| |                    |    L_ mmheader            |                        |
| L_ epd          1    |    |  L_ epd         1    |                        |
| L_ secuHeader   1    |    |  L_ secuHeader  1    |                        |
| L_ macCode      4    |    |  L_ align       5    |                        |
| L_ sqnNum       1    |    |  |                   |                        |
|                      |    |  L_ msgType     1    |                        |
+----------------------+    |                      |                        |
                       |    | (based on msgType)   |                        |
                       |    L_ regReqMsg           |                        |
                       |    L_ authReqMsg          |                        |
                       |       . . .               |                        |
                       +---------------------------+------------------------+

*/

typedef struct nasMessageSecurityHeader {
    ExtendedProtocolDiscriminator_t         epd;
    SecurityHeaderType_t                    secuHeader;
    uint32_t                                macCode;
    uint8_t                                 sqnNum;
    uint8_t                                 pduSessionId;
    uint8_t                                 pti;
}nasMessageSecurityHeader_t;

typedef union {
    _5gMMMsg_t          _5gmmMsg;   
    _5GSMMsg_t          _5gsmMsg;   
}nasMessagePlain_t;

typedef union nasMessage{
    nasMessageSecurityHeader_t          header;
    nasMessagePlain_t                   plain;
    nasMessage() {
        memset(this, 0, sizeof(*this));
    }
}nasMessage_t;

/* Messages to encode and decode NAS mesages.
 * - return SUCCESS (=0) or FAILURE(=-1)
 *   check errno for additional failure details. (values defined in datatypes.h)
 * - void *security : because these function will be used by the amf
 *   as well as UE both may different context of security
 */

/* Auxillary function which will just extract plain msg part of the nasMsg.
 * The call of this function does not amount to successful 'Decoding' */
/* Useful in cases where Integrity Protected Msgs can not be decoded without
 * availability of Sec Cntxt */
/* Note: This is an auxillary function which can be called from out of nasLib
 * just like nasMessageDecode. Unlike internal functions of nasMessageDecrypt */
int nasMessagePeepAtPlain(uint8_t *buffer, uint32_t bufLen,
                             nasMessagePlain_t *plainMsg);

/* encode into buffer for max size bufLen, using nasMessage msgStruct.
 * set the number of bytes encoded at encodedLen. */
int nasMessageEncode(uint8_t *buffer, uint32_t bufLen, nasMessage_t *msgStruct,
                     uint32_t *encodedLen, void *security );

/* decode from buffer of size decodedLen, into nasMessage msgStruct.
 * msgStruct must be allocated by the caller. */
int nasMessageDecode(uint8_t *buffer, nasMessage_t *msgStruct,
    uint32_t decodedLen,void *security);

#endif
