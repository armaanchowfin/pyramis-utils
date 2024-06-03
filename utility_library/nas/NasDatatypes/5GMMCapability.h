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

#ifndef __5GMM_CAPABILITY_H
#define __5GMM_CAPABILITY_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define _5GMM_CAPABILITY_OFFSET    2     

#define _5GMM_CAPABILITY_LEN_MIN   1
#define _5GMM_CAPABILITY_LEN_MAX   13    
 




typedef struct _5gmmCapability {
    uint8_t      len;                       

    uint8_t     hoS1;              

    


    

    

}_5gmmCapability_t;


int decode5GMMCap(uint8_t *buffer, uint32_t bufLen,
			           _5gmmCapability_t* _5gmmcap, uint32_t* decodedLen);
int encode5GMMCap(uint8_t *buffer, uint32_t bufLen,
                       _5gmmCapability_t* _5gmmcap, uint32_t *encodedLen);

#endif 

