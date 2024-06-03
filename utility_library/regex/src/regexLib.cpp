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

#include "regex/include/regexLib.h"

std::unordered_map<REGULAR_EXPRESSION, regex_t > regexToDfaMap;

int initRegexLib()
{
    LOG_ENTRY;
    int cflags = REG_EXTENDED;
    std::map<REGULAR_EXPRESSION, const char * >::iterator itr;
    for(itr=regexPatternMap.begin();itr!=regexPatternMap.end();itr++)
    {
        regex_t patternReg;
        if(compileRegex(&patternReg, itr->second, cflags) !=SUCCESS)
        {
            printf("Compile failed\n");
            LOG_EXIT;
            return FAILURE;
        }
        regexToDfaMap[itr->first] = patternReg;
    }
    successLog("Regex library successfully initialized. All required regex compiled.");
    LOG_EXIT;
    return SUCCESS;
}


int compileRegex(regex_t *patternReg, const char *regexStr, int cflags)
{
    LOG_ENTRY;
    if (tre_regcomp(patternReg, regexStr, cflags) != SUCCESS)
    {
        higLog("Rule regex compilation failed");
        LOG_EXIT;
        return FAILURE;    
    }
    midLog("Rule regex compilation success");
    LOG_EXIT;
    return SUCCESS;
}

int regexMatchFull(regex_t *patternReg, const char * strToMatch, 
                    int strToMatchLen, size_t nmatch, regmatch_t *pmatch, int eflags)
{
    LOG_ENTRY;
    if (tre_regexec(patternReg, strToMatch, nmatch, pmatch, eflags) != SUCCESS
     && pmatch[0].rm_eo-pmatch[0].rm_so != strToMatchLen)
    {   
        higLog("\"%s\" does not match fully.", strToMatch);
        LOG_EXIT;
        return FAILURE;
    }
    midLog("Regex matched fully.");
    LOG_EXIT;
    return SUCCESS;
}

int regexMatchPartial(regex_t *patternReg, const char * strToMatch, 
                        size_t nmatch, regmatch_t *pmatch, int eflags)
{
    LOG_ENTRY;
    if (tre_regexec(patternReg, strToMatch, nmatch, pmatch, eflags) != SUCCESS)
    {   
        higLog("\"%s\" is not a partial match.", strToMatch);
        LOG_EXIT;
        return FAILURE;
    }
    midLog("Regex matched partially.");
    LOG_EXIT;
    return SUCCESS;
}

void terminateRegexLib()
{
    std::unordered_map<REGULAR_EXPRESSION, regex_t >::iterator itr = regexToDfaMap.begin();
    for(;itr!=regexToDfaMap.end();itr++)
    {
        regex_t preg = itr->second;
        tre_regfree(&preg);
    }
    regexToDfaMap.clear();
    successLog("Succesfully freed all regex_t vars");
    successLog("RegexLib unInit success");
}