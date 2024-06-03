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

#ifndef _MILENAGE_H
#define _MILENAGE_H

#include <bits/stdc++.h> 
#include "common/include/datatypes.h"
#include "common/include/utils.h"
#include "common/include/aka.h"

/*List of input and output parameters used in Milenage Algorithm:-
________________________________________________________________________________
 Parameter  |  Size(bits)  |         Description
--------------------------------------------------------------------------------
    K       |     128      |  Subscriber key K[0]...K[127]
    RAND    |     128      |  Random challenge RAND[0]...RAND[127]
    SQN     |      48      |  Sequence number SQN[0]...SQN[47]
    AMF     |      16      |  Authentication management field AMF[0]..AMF[15]
    MAC-A   |      64      |  Network authentication code MAC-A[0]...MAC-A[63]
    MAC-S   |      64      |  Resynch authentication code MAC-S[0]...MAC-S[63]
    RES     |      64      |  Response RES[0]...RES[63]
    CK      |     128      |  Confidentiality key CK[0]...CK[127]
    IK      |     128      |  Integrity key IK[0]...IK[127]
    AK      |      48      |  Anonymity key AK[0]...AK[47]
    OP      |     128      |  Operator Variant Algorithm Configuration Field
    OPc     |     128      |  value derived from OP and K
--------------------------------------------------------------------------------
*/



/*-----------------------------------------------------------------------
* Function for Algorithm f1
* Purpose: Compute Network authentication code MAC-A
* inputs: Subscriber Key K, RAND, SQN and AMF field
* output: MAC-A
*----------------------------------------------------------------------*/
void f1( uint8_t K[], uint8_t OP[], uint8_t rand[],
    uint8_t sqn[], uint8_t amf[],uint8_t (&mac_a)[MAC_SIZE] );

/*-------------------------------------------------------------------
* Function for Algorithms f2-f5
* Purpose: compute response RES and keys AK,IK,CK
* inputs: Subscriber key K, OP, random challenge RAND
* output : Response RES, AK, IK, CK
*-----------------------------------------------------------------*/

void f2345 ( uint8_t K[], uint8_t OP[], uint8_t rand[],
    uint8_t (&RES)[RES_SIZE], uint8_t (&AK)[AK_SIZE],
    uint8_t (&CK)[CK_SIZE], uint8_t (&IK)[IK_SIZE]);

/*-------------------------------------------------------------------
* Function for Algorithm f1*
* Purpose: Compute resynchronisation authentication code MAC-S
*          This needs to be used in case if UE faces SQN synchronisation failure
*          UDM needs to call use this method for re-generation of AV
* TODO pre-requisistes: service to Communicate synchronization failure to UDM
* TODO: f1star empty as of now. It should be implemented same as f1.
        For details,please refer to 35.206.

* inputs: Subscriber key K, random challenge RAND, SQN and field AMF
*-----------------------------------------------------------------*/
void f1star( uint8_t K[], uint8_t OP[], uint8_t rand[],
    uint8_t sqn[], uint8_t amf[], uint8_t (&mac_s)[MAC_SIZE]);

/*-------------------------------------------------------------------
 *Algorithm f5*
* Purpose: Compute resynchronisation anonymity key AK
* inputs: Subscriber Key K ,OP and RAND
* output: AK
*-----------------------------------------------------------------*/
void f5star( uint8_t K[], uint8_t OP[], uint8_t rand[],uint8_t (&AK)[AK_SIZE]);

/*-------------------------------------------------------------------
* Function to compute OPc by encryption of OP with subscriber key KA
*-----------------------------------------------------------------*/
void ComputeOPc( uint8_t roundKeys[11][4][4], uint8_t OP[],
    uint8_t (&OPc)[OP_SIZE] );

/*-------------------------------------------------------------------
* Rijndael key schedule function
* Purpose: create all Rijndael's internal subkeys for encryption
* inputs: 16-byte key for encryption
*-----------------------------------------------------------------*/
void RijndaelKeySchedule( uint8_t key[16], uint8_t (&roundKeys)[11][4][4]);

/*-------------------------------------------------------------------
* Rijndael encryption function
* Purpose: Implelement Rijndael encryption for single block of 16 bytes
*          (using round keys already derived from 16-byte subscriber key)
* inputs: Takes 16-byte input text
* output: 16-byte ciphered output
*-----------------------------------------------------------------*/
void RijndaelEncrypt( uint8_t roundKeys[11][4][4],
    uint8_t input[16], uint8_t (&output)[16] );


#endif
