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
#include "common/include/setup.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/DeregistrationAccept.h"

int decodeDeregAccept(uint8_t *buffer, DeregistrationAccept_t *msgStruct,
                 uint32_t bufLen)
{ 
    nasLogENTRY;
    nasLog("exit from decodeDeregAccept");
    nasLogEXIT;
    return SUCCESS;
}

int encodeDeregAccept(uint8_t *buffer, uint32_t bufLen,
                 DeregistrationAccept_t *msgStruct,uint32_t* encodedLen)
{
    nasLogENTRY;
    nasLog("exit from encodeDeregAccept");
    nasLogEXIT;
    return SUCCESS;
}
