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

#include <bits/stdc++.h>
#include <stdlib.h>
#include "common/include/setup.h"
#include "platform/include/platform.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/RegistrationComplete.h"

/* elements are filled into user allocated nasMessage in the function.
 * returns SUCCESS/FAILURE
 */
int decodeRegComplete(uint8_t *buffer, RegistrationCompleteMsg_t *msgStruct,
		  uint32_t decodLen)
{
    nasLogENTRY;
    // nothing to decode for now
    nasLogEXIT;
    return SUCCESS;
}

/* returns SUCCESS/FAILURE AND Number of bytes encoded is set in encodedLen.
 */
int encodeRegComplete(uint8_t *buffer, uint32_t bufLen,
		  RegistrationCompleteMsg_t *msgStruct, uint32_t *encodedLen)
{
    nasLogENTRY;
    // nothing to decode for now
    *encodedLen = 0;
    nasLog("exit from encodeRegComplete : encoded %d",*encodedLen);
    nasLogEXIT;
    return SUCCESS;
}
