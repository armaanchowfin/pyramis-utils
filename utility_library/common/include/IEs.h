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


#ifndef __IES_H
#define __IES_H

struct MessageType {
    uint8_t                 procedureCode;
    uint32_t                choice;
};

typedef struct PlmnId_internal {                         
    
    uint8_t mcc_digit2:4;
    uint8_t mcc_digit1:4;               
    uint8_t mnc_digit3:4;
    uint8_t mcc_digit3:4;               
    uint8_t mnc_digit2:4;
    uint8_t mnc_digit1:4;               
}PlmnId_t;
typedef struct Tai_internal {                            
    PlmnId_t                plmnId;     
    uint16_t                tac;        
} Tai_t;
typedef struct NrCellId {                       
    PlmnId_t                plmnId;
    
} NrCellId;

/*  5G doc 2018 : 38.473 : page 90
    NRCGI ::= SEQUENCE {                
    pLMN-Identity PLMN-Identity,
    nRCellIdentity NRCellIdentity,
    iE-Extensions ProtocolExtensionContainer { {NRCGI-ExtIEs} } OPTIONAL,
...
} */

typedef struct NrCgi {                          
    PlmnId_t                plmnId;     
    NrCellId                nrCellId;   
} NrCgi;
typedef struct NrUserLocationInformation {
    Tai_t                   tai;
    NrCgi                   nrCgi;
}NrUserLocationInformation;
struct UserLocationInformation_i {
    NrUserLocationInformation           nrUserLocationInformation;
};

#endif
