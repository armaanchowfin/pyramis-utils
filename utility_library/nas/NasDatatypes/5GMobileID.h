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

#ifndef __5G_MOBILE_ID_H
#define __5G_MOBILE_ID_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define REG_REQ_MIN_5G_MOBILE_ID_LEN 4





#if 0
#define _5G_MOBILE_IDENTITY_EVEN  0
#define _5G_MOBILE_IDENTITY_ODD   1
#endif

#define MSIN_LENGTH 5 

#define GUTI_LENGTH 11
#define SUCI_LENGTH 16
#define SUCI_LENGTH_MIN 8



#if 0
#define SMS_SUPPORTED       1
#define SMS_NOT_SUPPORTED   0
#endif

typedef struct guti_5gMobileId {
    uint8_t          reserved:4;
    uint8_t          spare:1;
    uint8_t          identityType:3;
    uint8_t          mccDigit1:4;
    uint8_t          mccDigit2:4;
    uint8_t          mccDigit3:4;
    uint8_t          mncDigit3:4;
    uint8_t          mncDigit2:4;
    uint8_t          mncDigit1:4;
    uint32_t         _5gTMSI;
    uint8_t          amfRegionId;   
    uint16_t         amfSetId:10;      
    uint16_t         amfPointer:6;
    guti_5gMobileId() {
        reserved = 0;
        spare = 0;
        identityType = 0;
        mccDigit1 = 0;
        mccDigit2 = 0;
        mccDigit3 = 0;
        mncDigit3 = 0;
        mncDigit2 = 0;
        mncDigit1 = 0;
        _5gTMSI = 0;
        amfRegionId = 0;
        amfSetId = 0;
        amfPointer = 0;
    }
    
}guti_5gMobileId_t;



typedef struct suci_5gMobileId {
    uint8_t          spare1:1;
    uint8_t          supiFormat:3;
    uint8_t          spare:1;
    uint8_t          identityType:3;
    uint8_t          mccDigit1:4;
    uint8_t          mccDigit2:4;
    uint8_t          mccDigit3:4;
    uint8_t          mncDigit3:4;
    uint8_t          mncDigit2:4;
    uint8_t          mncDigit1:4;
    uint8_t          routingInc1:4;   
    uint8_t          routingInc2:4; 
    uint8_t          routingInc3:4;
    uint8_t          routingInc4:4;
    uint8_t          spare2:4;
    uint8_t          protectionSchId:4;
    uint8_t          homeNtwrkPKI;
    uint8_t          schemeOutput[MSIN_LENGTH];

}suci_5gMobileId_t;



typedef struct _5gMobileId {
    uint16_t          len;
    union {
    guti_5gMobileId_t guti5gMobileId;
    suci_5gMobileId_t suci5gMobileId;};   
     
}_5gMobileId_t;

int decode5GGuti(uint8_t *buffer, uint32_t bufLen,
			           guti_5gMobileId_t* guti, uint32_t* decodedLen);
int encode5GGuti(uint8_t *buffer, uint32_t bufLen,
                       guti_5gMobileId_t* guti, uint32_t *encodedLen);
int decodeSuci(uint8_t *buffer, uint32_t bufLen,
                       suci_5gMobileId_t* suci, uint32_t* decodedLen);
int encodeSuci(uint8_t *buffer, uint32_t bufLen,
                       suci_5gMobileId_t* suci, uint32_t *encodedLen);

#endif 

