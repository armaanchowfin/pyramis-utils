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

#ifndef __5GS_DEREGISTRATION_TYPE_H
#define __5GS_DEREGISTRATION_TYPE_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"



/* Octets   1 & 2 : accessType
 *          3     : regRequired
 *          4     : switchoff
 *          5 - 8 : IEI/spare
 */
typedef struct _5gDeregistrationType {
    uint8_t          accessType:2;    

    uint8_t          regRequired:1;   

    uint8_t          switchoff:1;	  

    uint8_t:4;			  

    
}_5gDeregistrationType_t;

int decode5GSDeregType(uint8_t *buffer, uint32_t bufLen,
			           _5gDeregistrationType_t *deregType, uint32_t* decodedLen);
int encode5GSDeregType(uint8_t *buffer, uint32_t bufLen,
                       _5gDeregistrationType_t *deregType, uint32_t *encodedLen);

#endif 

