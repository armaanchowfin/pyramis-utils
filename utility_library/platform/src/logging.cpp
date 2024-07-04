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

/* code for logging */
#include <bits/stdc++.h>
#include <cstdio>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include "../include/logging.h"

#ifdef  COLOURED_LOGS		//Enable from Makefile
                                            /* logMask, Y = dont care */
#define STYLE_RED           "\x1B[31m"      /*    0x1Y   or   0xY2 */
#define STYLE_YELLOW        "\x1B[33m"      /*    0x2Y   or   0xY1 */
#define STYLE_GREEN         "\x1b[32m"      /*    0x3Y */
#define STYLE_BLUE          "\x1b[34m"      /*    0x4Y */
#define STYLE_MAGENTA       "\x1b[35m"      /*    0x5Y */
#define STYLE_CYAN          "\x1b[36m"      /*    0x6Y */
#define STYLE_BOLD          "\033[1m"
#define STYLE_RESET         "\033[0m"
#else
#define STYLE_RED           ""
#define STYLE_YELLOW        ""
#define STYLE_GREEN         ""
#define STYLE_BLUE          ""
#define STYLE_MAGENTA       ""
#define STYLE_CYAN          ""
#define STYLE_BOLD          ""
#define STYLE_RESET         ""
#endif  /*COLOURED_LOGS*/

#define STYLE_BUF_LEN   30

#define LOG_FILENAME_LEN	200
FILE *logFp = NULL;

#define NAMEBUFF_LEN    100
#define PRINTBUFF_LEN   5000

std::string getPrintStr(const char *name, uint8_t* inputArray, size_t size)
{
    std::string out = "";
    int counter = 0, len = PRINTBUFF_LEN;
    char printBuff[PRINTBUFF_LEN] = {};
    char nameBuff[NAMEBUFF_LEN] = {};
    /* "%s :" is being used while populating nameBuff
     * hence NAMEBUFF_LEN-2 check */
    if(strlen(name) >= (NAMEBUFF_LEN-2)) {
        higLog("Internal Error: resource: unable to get array contents");
        return out;
    }
    if(size >= PRINTBUFF_LEN) {
        higLog("Internal Error: resource: unable to get array contents");
        return out;
    }
    sprintf(nameBuff, "%s :", name);
    out = nameBuff;
    for(uint i=0;i<size;i++)
    {
        //print each byte as hex
        counter += snprintf(printBuff + counter, len - counter, "%02X ",
                            inputArray[i]);
    }
    out += printBuff;
    return out;
}

#undef NAMEBUFF_LEN
#undef PRINTBUFF_LEN

/* This function should be called only once. */
/* It should be the first func called in main */
void setTag(const char *tag)
{
	char timeBuff[80] = {};
	struct tm timeInfo = {};
    struct timeval tv = {};
    gettimeofday(&tv, NULL);

	localtime_r((time_t*)(&tv.tv_sec), &timeInfo); /* seconds since epoch */
	strftime(timeBuff, 80, "%d-%b-%Y_%H:%M:%S", &timeInfo);

    if(logFp) {
        printf("ERROR: logfile tag can be set only once\n");
        exit(-1);
    }

	char logFileName[LOG_FILENAME_LEN] = {};
	sprintf(logFileName, "%s_%s_%d_log", tag, timeBuff, getpid());
	logFp = fopen(logFileName, "a+");
	if(logFp == NULL) {
		perror("ERROR: In opening log file\t");
		exit(-1);
	}
}

void logFlush()
{
    fflush(logFp);
}

/* main logging function, DONT USE DIRECTLY, use wrappers */
void platformLog(int logMask, const char* file, int line,
		 const char* func, const char *logMsg, ...)
{
    int logColour = logMask >> 4;
    int logLevel = logMask & 0xF;   /* The last four bits */
    /* Removed runtime check for LOGGING_BITMASK.
     * The decision whether to execute a particular loglevel should be done
     * preprocessor time.
     * Persisted with loglevel variable as yellow and red colors for mid
     * and hig Logs are determined using loglevel */

    /*first get time before anything else */
    struct timeval tv = {};
    gettimeofday(&tv, NULL);

	va_list arg;
	struct tm timeInfo = {};
	char timeBuff[20] = {};
	char logLevelChars[] = "LMH"; /*Low, Mid & High*/
    char styleBuff[STYLE_BUF_LEN] = {};
    int rc = 0;

	if(logLevel > 2)
		return;
	
	/* 

	if(logLevel == 4) {
		std::ofstream logFile;
		logFile.open("ranLog.txt");
		logFile << getpid() <<" "<<timeBuff <<" "<<file<<" "<<line<<" "<<func;
		
		va_start (arg, logMsg);
		vfprintf (logFile, logMsg, arg);
		va_end (arg);
	
	}else if(logLevel == 5) {

	} 
	
	*/
	localtime_r((time_t*)(&tv.tv_sec), &timeInfo); /* seconds since epoch */
	strftime(timeBuff, 20, "%H:%M:%S", &timeInfo);

    switch(logColour) {
        case 1: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s", STYLE_RED);
                break;
        case 2: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s", STYLE_YELLOW);
                break;
        case 3: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s", STYLE_GREEN);
                break;
        case 4: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s", STYLE_BLUE);
                break;
        case 5: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s", STYLE_MAGENTA);
                break;
        case 6: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s", STYLE_CYAN);
                break;
        /* and finally no special colour, just white */
        case 0xF: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s", STYLE_RESET);
                break;
        default: {
            switch(logLevel) {  /* logColour takes precedence over logLevel */
                case 1: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s", 
                                      STYLE_YELLOW);
                        break;
                case 2: rc = snprintf(styleBuff, STYLE_BUF_LEN, "%s",STYLE_RED);
                        break;
            }
        }
    }
    if(rc > STYLE_BUF_LEN) {   /* impossible */
        printf("%s\n", STYLE_RESET);
        return;
    }

    char *printfBuff;
    size_t size = 0;
    FILE *filep = open_memstream(&printfBuff, &size);
    if(filep == NULL) {
        printf("ERROR:open_memstream failed");
        return;
    }

    fprintf(filep, "%s%c  TID:%ld  %s:%03ld    %s:%d %s:   ",
                        styleBuff, logLevelChars[logLevel], syscall(SYS_gettid),
                        timeBuff, tv.tv_usec/1000, file, line, func);

	va_start (arg, logMsg);
	vfprintf(filep, logMsg, arg);
	va_end (arg);

    fclose(filep);
    printf("%s%s\n", printfBuff, STYLE_RESET);
    if(logFp ==  NULL) {
        printf("setTag(\"NFname\") should be called"
            " as first step in application main");
        printf("Since setTag() was not called by application"
            " Calling it here with default tag 'output'");
        setTag("output");
	}
    fprintf(logFp, "%s%s\n", printfBuff, STYLE_RESET);
    free(printfBuff);
}
