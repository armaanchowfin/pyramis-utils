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

#ifndef _UTILS_H
#define _UTILS_H
#include <string>
#include "ran/intf/include/ue.h"
#include "common/include/aka.h"
#include "platform/include/logging.h"
#include "nas/NasDatatypes/nssai.h"
using namespace std;

#define utilLog(filename, line, func, arg, ...)
#if (LOGGING_BITMASK & 0b001)
#undef utilLog
#define utilLog(filename, line, func, arg, ...)  \
        platformLog(0, filename, line, func, arg, ##__VA_ARGS__)
#endif



#define printArray(name, inputArray, size)
#if (LOGGING_BITMASK & 0b001)
#undef printArray
#define printArray(name, inputArray, size)    \
        __printArray(name, inputArray, size, __FILENAME__, __LINE__, __func__)
#endif

#define binPrint(name, val)  \
        __binPrint(name, val, __FILENAME__, __LINE__, __func__)
#define print_HEAV(udmHeAv) \
        __print_HEAV(udmHeAv, __FILENAME__, __LINE__, __func__)
#define print_AV(ausfAv) \
        __print_AV(ausfAv,  __FILENAME__, __LINE__, __func__)
#define print_SUPI(supi) \
         __print_SUPI(supi, __FILENAME__, __LINE__, __func__)
void print_SUCI(suci_t suci);

void getPlmnString(uint16_t mcc, uint16_t mnc, uint8_t mnc_digit_length,
                   std::string &str);
void bytecopy(uint8_t *dest, uint8_t *src, int len);
void initialize_SUCI(suci_t &suci);
void hexCopyFromString(uint8_t *arr, uint size, std::string hexS);
void hexCopyToString(std::string &hexS, uint8_t *arr, uint size);
supi_t getSupi();
std::string build_nai_string(imsi_t userName_imsi, std::string domainName);
uint16_t mirrorOctet(uint8_t input);
void copyNSSAI(nssai_t srcNSSAI, nssai_t destNSSAI);
string binaryPrint(uint16_t val);



void __printArray(const char *name, uint8_t* inputArray, size_t size,
                  const char *filename, const int line, const char *func);
void __binPrint(const char *name, uint8_t val,
                const char *filename, const int line, const char *func);
void __print_HEAV(_5gHeAv_t *udmHeAv,
                  const char *filename, const int line, const char *func);
void __print_AV(_5gAv_t* ausfAv,
              const char *filename, const int line, const char *func);
void __print_SUPI(supi_t supi,
                  const char *filename, const int line, const char *func);

#endif
