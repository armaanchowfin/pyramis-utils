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

#ifndef __SECURITY_MODE_COMMAND_H
#define __SECURITY_MODE_COMMAND_H

#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/UESecCapability.h"
#include "nas/NasDatatypes/NasKeySetId.h"

typedef struct SecurityModeCommand {
    

    _5gmmMsgHeader_t                           mmHeader;
    NasSecurityAlgorithm_t                     nasSecurityAlgorithm;
    NaskeysetId_t                              ngKsi;
    UeSecurityCapability_t                     ueSecuCapability;

    

    
    hashAmf_t                                  hashAmf;

}SecurityModeCommand_t;

/* DO NOT USE THESE FUNCTIONS DIRECTLY,
 * use nasMessageEncode & nasMessageDecode */
int decodeSecuModeCommand(uint8_t *buffer, SecurityModeCommand_t *msgStruct,
                          uint32_t decodLen);
int encodeSecuModeCommand(uint8_t *buffer, uint32_t bufLen,
                          SecurityModeCommand_t *msgStruct,
                          uint32_t *encodedLen);


#endif
