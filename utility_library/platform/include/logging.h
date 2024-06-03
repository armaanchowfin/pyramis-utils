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

#ifndef  __LOGGING_H
#define  __LOGGING_H

#include <cstdio>
#include <stdarg.h>
#include <cstring>
#include <string>



#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define alwaysOnLog(log, ...)    platformLog(0x52, __FILENAME__, __LINE__, __func__, log, ##__VA_ARGS__)
#define lowLog(log, ...)
#define midLog(log, ...)
#define higLog(log, ...)


#define successLog(log, ...)
#define specialLog(log, ...)

#if (LOGGING_BITMASK & 0b001)
#undef lowLog
#define lowLog(log, ...)	platformLog(0, __FILENAME__, __LINE__, __func__, log, ##__VA_ARGS__)
#endif

#if (LOGGING_BITMASK & 0b010)
#undef midLog
#define midLog(log, ...)	platformLog(1, __FILENAME__, __LINE__, __func__, log, ##__VA_ARGS__)
#endif

#if (LOGGING_BITMASK & 0b100)
#undef higLog
#undef successLog
#undef specialLog
#define higLog(log, ...)	platformLog(2, __FILENAME__, __LINE__, __func__, log, ##__VA_ARGS__)


#define successLog(log, ...)    platformLog(0x32, __FILENAME__, __LINE__, __func__, log, ##__VA_ARGS__)
#define specialLog(log, ...)    platformLog(0x52, __FILENAME__, __LINE__, __func__, log, ##__VA_ARGS__)
#endif

#define sLOG_ENTRY		lowLog("%s", "ENTRY")
#define LOG_EXIT		lowLog("%s", "EXIT")

std::string getPrintStr(const char *name, uint8_t* inputArray, size_t size);
void setTag(const char *tag);
void platformLog(int logLevel, const char* file, const int line,
		 const char* func, const char *logMsg, ...);
void logFlush();

#endif  


