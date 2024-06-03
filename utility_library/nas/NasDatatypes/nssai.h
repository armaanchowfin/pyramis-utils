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

#ifndef __NSSAI_H
#define __NSSAI_H

#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"

#define NSSAI_IEI_SIZE          1
#define NSSAI_LEN_SIZE          1


#define NSSAI_LEN_MIN   4      
#define NSSAI_LEN_MAX   146    
#define MAX_S_NSSAI     8      
#define SIZE_S_NSSAI    5      
#define MAX_SST 255            

#define MAX_SD ((((long)1)<<32)-1)
#define SST_SIZE 1
#define SS_SIZE 3


#define LEN_ONLY_SST                  0b00000001
#define LEN_SST_AND_MAPPED_CFG_SST    0b00000010
#define LEN_SST_AND_SD                0b00000100
#define LEN_SST_SD_MAPPED_CFG_SST     0b00000101
#define LEN_SST_SD_MAPPED_CFG_SST_SD  0b00001000


#define SST_eMBB    1
#define SST_URLLC   2
#define SST_MIoT    3

template< typename T >
std::string int_to_hex( T x ,int len = 6) {
    std::stringstream stream;
    stream << std::setfill ('0') << std::setw(len) 
            << std::hex << x;
    return stream.str();
}

/*NOTE from 23.501 5.15.2.1
An S-NSSAI can have standard values (i.e. such S-NSSAI is only comprised of SST 
with a standardised SST value,and no SD) or non-standard values 
(i.e.such S-NSSAI is comprised of either both SST and SD or only SST without 
 a standardised SST value and no SD).
*/

/*(Single) S-NSSAI value:
len_s_nssai: Possible values range from LEN_ONLY_SST to LEN_SST_SD_MAPPED_CFG_SST_SD
sST: 1 octet with possible values SST_eMBB,SST_URLLC,SST_MIoT
sD: 3 octets with non-standardised values
*/
typedef struct s_nssai {
    uint8_t len_s_nssai;
    
    uint8_t sST;
    uint32_t sD:24;
    
    

    operator std::string() {
        return std::to_string(sST) + int_to_hex(sD);
    }
    

    std::string getSd() {
        return int_to_hex(sD);;
    }
    

    std::string getSst() {
        return std::to_string(sST);
    }
} s_nssai_t;

typedef struct nssai
{
    /*NOTE: NSSAI Total len in octets will be in range 4 <= len <= 146 
    NSSAI len is taken care while encode/decode in library,but not part of struct*/

    uint8_t no_of_slices;
    s_nssai_t Nssai[MAX_S_NSSAI];
    
    /*nssai()
    {
        this->len_nssai = 0;
        this->Nssai[MAX_S_NSSAI] = {};
    }*/
}nssai_t;


int decodeSnssai(uint8_t *buffer, uint32_t bufLen,
                       s_nssai_t* _snssai, uint32_t* decodedLen);
int encodeSnssai(uint8_t *buffer, uint32_t bufLen,
                       s_nssai_t* _snssai, uint32_t *encodedLen);                       

int decodeNssai(uint8_t *buffer, uint32_t bufLen,
                       nssai_t* _nssai, uint32_t* decodedLen);
int encodeNssai(uint8_t *buffer, uint32_t bufLen,
                       nssai_t* _nssai, uint32_t *encodedLen);


#endif
