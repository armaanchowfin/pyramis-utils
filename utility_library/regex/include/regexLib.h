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

#ifndef __REGEX_LIB_H
#define __REGEX_LIB_H

#include <errno.h>
#include <bits/stdc++.h>
#include <tre/tre.h>
#include "platform/include/logging.h"
#include "common/include/datatypes.h"

using namespace std;

#define MAX_NUM_MATCHES 10
#define MAX_PATTERN_STRING_LENGTH 100

typedef enum
{
    IP_FILTER_RULE_REGEXP = 0,
    IP_FILTER_RULE_NODE_REGEXP = 1,
    BITRATE_REGEXP = 2,
    INTEGER_REGEXP = 3,
    RAND_REGEXP = 4,
    HX_RES_STAR_REGEEXP = 5,
    AUTN_REGEXP = 6,
    K_SEAF_REGEXP = 7,
    SD_REGEXP = 8,
    UUID_REGEXP = 9,
    X_RES_STAR_REGEXP = 10,
    K_AUSF_REGEXP = 11
} REGULAR_EXPRESSION;


static std::map<REGULAR_EXPRESSION, const char * > regexPatternMap = {
    {IP_FILTER_RULE_REGEXP, "(.+)\\s+(.+)\\s+(.+)\\s+from\\s+(.+)\\s+to\\s+(.+)"},
    {IP_FILTER_RULE_NODE_REGEXP, "(!?)(any|assigned|(25[0-6]|2[0-4][0-9]|1[0-9]{1,2}|"
            "[0-9]{1,2}).(25[0-6]|2[0-4][0-9]|1[0-9]{1,2}|[0-9]{1,2}).(25[0-6]"
            "|2[0-4][0-9]|1[0-9]{1,2}|[0-9]{1,2}).(25[0-6]|2[0-4][0-9]|1[0-9]"
            "{1,2}|[0-9]{1,2}))(/[0-9]{1,3})?( [0-9,-]*)?"},
    {BITRATE_REGEXP, "((^\\d+)(\\.\\d+)?) (bps|Kbps|Mbps|Gbps|Tbps)$"},
    {INTEGER_REGEXP, "[0-9]+"},
    {RAND_REGEXP, "^[A-Fa-f0-9]{32}$"},
    {HX_RES_STAR_REGEEXP, "^[A-Fa-f0-9]{32}$"},
    {AUTN_REGEXP, "^[A-Fa-f0-9]{32}$"},
    {K_SEAF_REGEXP, "[A-Fa-f0-9]{64}"},
    {SD_REGEXP, "^[A-Fa-f0-9]{6}$"},
    {UUID_REGEXP, "[a-f0-9]{8}-[a-f0-9]{4}-4[a-f0-9]{3}-[89aAbB][a-f0-9]{3}-[a-f0-9]{12}"},
    {X_RES_STAR_REGEXP, "^[A-Fa-f0-9]{32}$"},
    {K_AUSF_REGEXP, "^[A-Fa-f0-9]{64}$"},
};

extern std::unordered_map<REGULAR_EXPRESSION, regex_t > regexToDfaMap;

int initRegexLib();
int compileRegex(regex_t *patternReg, const char *regexStr, int cflags);
int regexMatchFull(regex_t *patternReg, const char * strToMatch, 
                    int strToMatchLen, size_t nmatch, regmatch_t *pmatch, int eflags);
int regexMatchPartial(regex_t *patternReg, const char * strToMatch, 
                        size_t nmatch, regmatch_t *pmatch, int eflags);
void terminateRegexLib();



#endif
