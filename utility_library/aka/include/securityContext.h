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

#ifndef __SECURITY_CONTEXT_H
#define __SECURITY_CONTEXT_H

#include "ran/intf/include/ue.h"
#include "common/include/aka.h"
#include "nas/NasDatatypes/NasKeySetId.h"
#include "nas/NasDatatypes/UESecCapability.h"

/* Security context with common elements that both AMF and UE can use.
 */

typedef struct secContext {
    uint8_t Kamf[KAMF_SIZE];
    uint8_t Kgnb[KGNB_SIZE];
    uint8_t Kn3iwf[KN3IWF_SIZE];
    uint8_t K_NASint[NAS_KEY_SIZE];	

    uint8_t K_NASenc[NAS_KEY_SIZE];
    NaskeysetId_t ngKSI; 
    _5gAv_t _5gAv;
    uint8_t direction;
    uint8_t bearer;
    struct count_s{
        uint8_t spare;
        uint16_t overflow;
        uint8_t seq_num;
    } dl_count, ul_count;
    uint8_t   direction_encode; 
    uint8_t   direction_decode; 
    UeSecurityCapability_t ueSecurityCapability;
    securityAlgo_t secAlgo;
    secContext() { 

        this->Kamf[KAMF_SIZE] = {};
        this->K_NASint[NAS_KEY_SIZE]={};
        this->K_NASenc[NAS_KEY_SIZE]={};
        this->ngKSI={};
        this->_5gAv={};
        this->bearer = BEARER_ID_INVALID;
        this->dl_count = {0,0,0};
        this->ul_count = {0,0,0};
        /* on init it is set to AMF's encode/decode directions.
         * UE should set direction_decode to SECU_DIRECTION_DOWNLINK
         * and direction_encode SECU_DIRECTION_UPLINK */
        this->direction_decode = SECU_DIRECTION_UPLINK;
        this->direction_encode = SECU_DIRECTION_DOWNLINK;
    }
} secContext_t;

#endif
