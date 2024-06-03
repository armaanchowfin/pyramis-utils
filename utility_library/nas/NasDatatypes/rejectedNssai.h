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

#ifndef __REJ_NSSAI_H
#define __REJ_NSSAI_H

#include <bits/stdc++.h>
#include "nas/NasDatatypes/nssai.h"



#define CAUSE_S_NSSAI_NA_FOR_PLMN      0b0000
#define CAUSE_S_NSSAI_NA_FOR_REGAREA   0b0001

/*NOTE: Most of macros and sizes used for rejected NSSAI are referred from  
(Single) Rejected S-NSSAI value:
len_s_nssai: Possible values range from LEN_ONLY_SST to LEN_SST_SD_MAPPED_CFG_SST_SD
reject_cause: and value part of S-NSSAI information
element as specified in subclause 9.11.2.8 starting with the second octet.
*/
typedef struct rejected_s_nssai {
    uint8_t len_s_nssai:4;
    uint8_t reject_cause:4;
    
    uint8_t sST;
    uint32_t sD:24;
} rejected_s_nssai_t;

typedef struct rejectedNssai
{
    /*NOTE: NSSAI Total len in octets will be in range 4 <= len <= 146 
    NSSAI len is taken care while encode/decode in library,but not part of struct*/

    uint8_t no_of_slices;
    rejected_s_nssai_t Nssai[MAX_S_NSSAI];
}rejectedNssai_t;


int decodeRejectedNssai(uint8_t *buffer, uint32_t bufLen,
                       rejectedNssai_t* _nssai, uint32_t* decodedLen);
int encodeRejectedNssai(uint8_t *buffer, uint32_t bufLen,
                       rejectedNssai_t* _nssai, uint32_t *encodedLen);


#endif
