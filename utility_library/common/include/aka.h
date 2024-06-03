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

#ifndef __AKA_H
#define __AKA_H

#include <iostream>


#define RAND_SIZE       16
#define AUTN_SIZE       16
#define AUTS_SIZE       16
#define XRES_SIZE       16 
#define RES_SIZE        8
#define KAUSF_SIZE      32
#define KSEAF_SIZE      32
#define KAMF_SIZE       32
#define KGNB_SIZE       32
#define KN3IWF_SIZE     32
#define NAS_KEY_SIZE    16
#define MAX_SUPI_SIZE   15

#define KA_SIZE 16
#define MAC_SIZE 8
#define SQN_SIZE 6
#define AK_SIZE 6
#define CK_SIZE 16
#define IK_SIZE 16
#define OP_SIZE 16
#define AMF_SIZE 2
#define ALG_BITMODE 16 

typedef struct _5gHeAv {
       uint8_t RAND[RAND_SIZE]; 
       uint8_t AUTN[AUTN_SIZE]; 
       uint8_t XRES[XRES_SIZE]; 
       uint8_t Kausf[KAUSF_SIZE];
} _5gHeAv_t;

typedef struct _5gAv {
       uint8_t RAND[RAND_SIZE]; 
       uint8_t AUTN[AUTN_SIZE]; 
       uint8_t HXRES[XRES_SIZE]; 
       uint8_t Kseaf[KSEAF_SIZE];
} _5gAv_t;

#endif
