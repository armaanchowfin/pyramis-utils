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

#ifndef __UE_SECURITY_CAPABILITY_H
#define __UE_SECURITY_CAPABILITY_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define UE_SECURITY_CAPABILITY_NAS_LEN      2
#define UE_SECURITY_CAPABILITY_NAS_AS_LEN   4




#define NAS_SEC_ALGO_NEA0 (1<<7)
#define NAS_SEC_ALGO_NEA1 (1<<6)
#define NAS_SEC_ALGO_NEA2 (1<<5)
#define NAS_SEC_ALGO_NEA3 (1<<4)
#define NAS_SEC_ALGO_NEA4 (1<<3)
#define NAS_SEC_ALGO_NEA5 (1<<2)
#define NAS_SEC_ALGO_NEA6 (1<<1)
#define NAS_SEC_ALGO_NEA7 (1<<0)

#define NAS_SEC_ALGO_NIA0 (1<<7)
#define NAS_SEC_ALGO_NIA1 (1<<6)
#define NAS_SEC_ALGO_NIA2 (1<<5)
#define NAS_SEC_ALGO_NIA3 (1<<4)
#define NAS_SEC_ALGO_NIA4 (1<<3)
#define NAS_SEC_ALGO_NIA5 (1<<2)
#define NAS_SEC_ALGO_NIA6 (1<<1)
#define NAS_SEC_ALGO_NIA7 (1<<0)





typedef struct UeSecurityCapability {
    uint8_t      len;                       

    uint8_t     _5gNASEncAlgo;              

    uint8_t     _5gNASIntAlgo;              

    


    

    uint8_t     _5gASEncAlgo;              

    uint8_t     _5gASIntAlgo;              

}UeSecurityCapability_t;


int decodeUESecCap(uint8_t *buffer, uint32_t bufLen,
			           UeSecurityCapability_t* ueSeccap, uint32_t* decodedLen);
int encodeUESecCap(uint8_t *buffer, uint32_t bufLen,
                       UeSecurityCapability_t* ueSeccap, uint32_t *encodedLen);

#endif 

