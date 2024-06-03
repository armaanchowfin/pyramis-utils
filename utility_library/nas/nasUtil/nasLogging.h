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

#ifndef __NASLOGGING_H
#define __NASLOGGING_H

#include "platform/include/logging.h"

#define nasLog(arg, ...)
#if (LOGGING_BITMASK & 0b001)
#undef nasLog


#define nasLog(arg, ...)       platformLog(0x60, __FILENAME__, __LINE__, __func__, arg, ##__VA_ARGS__)
#endif

#define nasLogENTRY            nasLog("%s", "ENTRY")
#define nasLogEXIT             nasLog("%s", "EXIT")

#endif

