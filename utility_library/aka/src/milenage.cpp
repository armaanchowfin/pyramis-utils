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

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include "../include/milenage.h"
#include "crypto5GT/include/cryptUtil.h"
#include "platform/include/logging.h" //For traces

/*
Stepwise List of computations inside Milenage Algorithm
1.128-bit value OPC is derived from OP and K as follows:
  OPC = OP ^ E[OP]K

2.Intermediate 128-bit value TEMP is computed as follows:
  TEMP = E[RAND ^ OPC ]K.

3.128-bit value IN1 is constructed as follows:
    IN1[0] .. IN1[47] = SQN[0] .. SQN[47]
    IN1[48] .. IN1[63] = AMF[0] .. AMF[15]
    IN1[64] .. IN1[111] = SQN[0] .. SQN[47]
    IN1[112] .. IN1[127] = AMF[0] .. AMF[15]

4.Five 128-bit constants c1, c2, c3, c4, c5 are defined as follows:
    c1[i] = 0 for 0<=i<=127
    c2[i] = 0 for 0<=i<=127, except that c2[127] = 1
    c3[i] = 0 for 0<=i<=127, except that c3[126] = 1
    c4[i] = 0 for 0<=i<=127, except that c4[125] = 1
    c5[i] = 0 for 0<=i<=127, except that c5[124] = 1

5.Five integers r1, r2, r3, r4, r5 are defined as follows:
    r1 = 64; r2 = 0; r3 = 32; r4 = 64; r5 = 96

6.Five 128-bit blocks OUT1, OUT2, OUT3, OUT4, OUT5 are computed as follows:
    OUT1 = E[TEMP ^rotate(IN1 ^ OPC, r1) ^c1]K  ^ OPc
    OUT2 = E[rotate(TEMP ^ OP C, r2) ^ c2]K ^ OPc
    OUT3 = E[rotate(TEMP ^ OP C, r3) ^ c3]K ^ OPc
    OUT4 = E[rotate(TEMP ^ OP C, r4) ^ c4]K ^ OPc
    OUT5 = E[rotate(TEMP ^ OP C, r5) ^ c5]K ^ OPc

7.The outputs of the various functions are then defined as follows:
    Output of f1 = MAC-A, where MAC-A[0] .. MAC-A[63] = OUT1[0] .. OUT1[63]
    Output of f1* = MAC-S, where MAC-S[0] .. MAC-S[63] = OUT1[64] .. OUT1[127]
    Output of f2 = RES, where RES[0] .. RES[63] = OUT2[64] .. OUT2[127]
    Output of f3 = CK, where CK[0] .. CK[127] = OUT3[0] .. OUT3[127]
    Output of f4 = IK, where IK[0] .. IK[127] = OUT4[0] .. OUT4[127]
    Output of f5 = AK, where AK[0] .. AK[47] = OUT2[0] .. OUT2[47]
    Output of f5* = AK, where AK[0] .. AK[47] = OUT5[0] .. OUT5[47]
    NOTE:Implementation of f1* and f5* is not present in this file. but its
    same as f1 and f5; it needs to be taken as it is from 3GPP 35.206.

*/

#define TRA_DEBUG

//Global constants for Rijndael Algo
/*--------------------- Rijndael S box table ----------------------*/
const uint8_t Sbox[256] = {
    99,124,119,123,242,107,111,197, 48, 1,103, 43,254,215,171,118,
    202,130,201,125,250, 89, 71,240,173,212,162,175,156,164,114,192,
    183,253,147, 38, 54, 63,247,204, 52,165,229,241,113,216, 49, 21,
    4,199, 35,195, 24,150, 5,154, 7, 18,128,226,235, 39,178,117,
    9,131, 44, 26, 27,110, 90,160, 82, 59,214,179, 41,227, 47,132,
    83,209, 0,237, 32,252,177, 91,106,203,190, 57, 74, 76, 88,207,
    208,239,170,251, 67, 77, 51,133, 69,249, 2,127, 80, 60,159,168,
    81,163, 64,143,146,157, 56,245,188,182,218, 33, 16,255,243,210,
    205, 12, 19,236, 95,151, 68, 23,196,167,126, 61,100, 93, 25,115,
    96,129, 79,220, 34, 42,144,136, 70,238,184, 20,222, 94, 11,219,
    224, 50, 58, 10, 73, 6, 36, 92,194,211,172, 98,145,149,228,121,
    231,200, 55,109,141,213, 78,169,108, 86,244,234,101,122,174, 8,
    186,120, 37, 46, 28,166,180,198,232,221,116, 31, 75,189,139,138,
    112, 62,181,102, 72, 3,246, 14, 97, 53, 87,185,134,193, 29,158,
    225,248,152, 17,105,217,142,148,155, 30,135,233,206, 85, 40,223,
    140,161,137, 13,191,230, 66,104, 65,153, 45, 15,176, 84,187, 22,
    };


/*------- This array does the multiplication by x in GF(2^8) ------*/
const uint8_t Xtime[256] = {
    0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
    32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62,
    64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
    96, 98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,
    128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,
    160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,
    192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,
    224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,
    27, 25, 31, 29, 19, 17, 23, 21, 11, 9, 15, 13, 3, 1, 7, 5,
    59, 57, 63, 61, 51, 49, 55, 53, 43, 41, 47, 45, 35, 33, 39, 37,
    91, 89, 95, 93, 83, 81, 87, 85, 75, 73, 79, 77, 67, 65, 71, 69,
    123,121,127,125,115,113,119,117,107,105,111,109, 99, 97,103,101,
    155,153,159,157,147,145,151,149,139,137,143,141,131,129,135,133,
    187,185,191,189,179,177,183,181,171,169,175,173,163,161,167,165,
    219,217,223,221,211,209,215,213,203,201,207,205,195,193,199,197,
    251,249,255,253,243,241,247,245,235,233,239,237,227,225,231,229
    };

// functions for computation/transformation in each round of Rijndael algo ++
/* Round key addition function */
void KeyAdd(uint8_t state[4][4], uint8_t roundKeys[11][4][4], int round)
{
    int i, j;
    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
            state[i][j] ^= roundKeys[round][i][j];
    return;
}

/* Byte substitution transformation */
int ByteSub(uint8_t state[4][4])
{
    int i, j;
    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
            state[i][j] = Sbox[state[i][j]];

    return 0;
}

/* Row shift transformation */
void ShiftRow(uint8_t state[4][4])
{
    uint8_t temp;
    /* left rotate row 1 by 1 */
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
    /* left rotate row 2 by 2 */
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    /* left rotate row 3 by 3 */
    temp = state[3][0];
    state[3][0] = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = temp;

    return;
}

/* MixColumn transformation*/
void MixColumn(uint8_t state[4][4])
{
    uint8_t temp, tmp, tmp0;
    int i;


    /* do one column at a time */
    for (i=0; i<4;i++)
    {
        temp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
        tmp0 = state[0][i];

        /* Xtime array does multiply by x in GF2^8 */
        tmp = Xtime[state[0][i] ^ state[1][i]];
        state[0][i] ^= temp ^ tmp;
        tmp = Xtime[state[1][i] ^ state[2][i]];
        state[1][i] ^= temp ^ tmp;
        tmp = Xtime[state[2][i] ^ state[3][i]];
        state[2][i] ^= temp ^ tmp;
        tmp = Xtime[state[3][i] ^ tmp0];
        state[3][i] ^= temp ^ tmp;
    }
    return;
}

//End of Rijndael computation/transformation functions--

/*-----------------------------------------------------------------------------
* Algorithm f1: Computes MAC-A
**----------------------------------------------------------------------------*/
void f1( uint8_t KA[], uint8_t OP[], uint8_t rand[],
    uint8_t sqn[], uint8_t amf[],uint8_t (&mac_a)[MAC_SIZE] )
 {
    LOG_ENTRY;

    uint8_t OPc[OP_SIZE]={};
    uint8_t temp[ALG_BITMODE]={};
    uint8_t in1[ALG_BITMODE]={};
    uint8_t out1[ALG_BITMODE]={};
    uint8_t rijndaelInput[ALG_BITMODE]={};
    int i;
    uint8_t roundKeys[11][4][4]={};

    RijndaelKeySchedule( KA , roundKeys);
/*
    //Trace just to ensure round keys are same across UE and UDM
#ifdef TRA_DEBUG
    for (int i=0;i<11;i++)
    {
        lowLog("Facet:%d:",i);
        for(int j=0;j<4;j++)
            printArray("roundKeys::", roundKeys[i][j], 4);
    }
#endif
*/
    ComputeOPc( roundKeys, OP , OPc );
    for (i=0; i<ALG_BITMODE; i++)
        rijndaelInput[i] = rand[i] ^ OPc[i];

    /*CAUTION: Using raw encryption routine as per 35.206.
    Be careful to ensure this is being used same way at UE side and UDM both*/
    RijndaelEncrypt(roundKeys, rijndaelInput, temp );

#if 0
    /*Below code snippet is for Rijndael(AES) encryption with CTR mode
    using openssl evp functions. This is option against raw encryption
    function which internally does the same process.
    */
    int rijndaelInputLength = sizeof(rijndaelInput);
    rc = encrypt_NEA2(KA, KA_SIZE, rijndaelInput, &rijndaelInputLength, temp);
    if(rc == FAILURE)
    {
        higLog("%s","Encryption failed!!");
        LOG_EXIT;
        return rc;
    }
#endif

    for (i=0; i<SQN_SIZE; i++)
    {
        in1[i] = sqn[i];
        in1[i+8] = sqn[i];
    }
    for (i=0; i<AMF_SIZE; i++)
    {
        in1[i+6] = amf[i];
        in1[i+14] = amf[i];
    }

    /* XOR op_c and in1, rotate by r1=64, and XOR on the constant c1
    (which is all zeroes)
    Below we copy XOR output (in1^OPc) to array rijndaelInput after
    cyclic rotation to left by 8 byes*/
    for (i=0; i<ALG_BITMODE; i++)
        rijndaelInput[(i+8) % 16] = in1[i] ^ OPc[i];
    // XOR on the value temp computed before
    for (i=0; i<ALG_BITMODE; i++)
        rijndaelInput[i] ^= temp[i];

    /*CAUTION: Using raw encryption routine as per 35.206.
    Be careful to ensure this is being used same way at UE side and UDM both*/
    RijndaelEncrypt(roundKeys, rijndaelInput, out1);

#if 0
    /*Below snippet is for Rijndael(AES) encryption with CTR mode
    using openssl evp functions. This is option against raw encryption
    function which internally does the same process.
    */
    rijndaelInputLength = sizeof(rijndaelInput);
    rc = encrypt_NEA2( KA, KA_SIZE, rijndaelInput, &rijndaelInputLength, out1 );
    if(rc == FAILURE)
    {
        higLog("%s","Encryption failed!!");
        LOG_EXIT;
        return rc;
    }
#endif

    for (i=0; i<OP_SIZE; i++)
        out1[i] ^= OPc[i];
    for (i=0; i<MAC_SIZE; i++)
        mac_a[i] = out1[i];
#ifdef TRA_DEBUG
    printArray("Computed MAC_A::",mac_a, sizeof(mac_a));
#endif
    LOG_EXIT;
    //return rc;

 }

/*------------------------------------------------------------------------------
* Algorithms f2-f5: Computes RES,AK,CK,IK
* NOTE: Please refer to caution notes in f1() for detail on similar code snippet
*-----------------------------------------------------------------------------*/
void f2345 ( uint8_t KA[], uint8_t OP[], uint8_t rand[],
    uint8_t (&RES)[RES_SIZE], uint8_t (&AK)[AK_SIZE],
    uint8_t (&CK)[CK_SIZE], uint8_t (&IK)[IK_SIZE])
{
    LOG_ENTRY;

    //int rc = FAILURE;
    uint8_t OPc[ALG_BITMODE]={};
    uint8_t temp[ALG_BITMODE]={};
    uint8_t out[ALG_BITMODE]={};
    uint8_t rijndaelInput[ALG_BITMODE]={};
    int i;
    uint8_t roundKeys[11][4][4]={};

    RijndaelKeySchedule( KA , roundKeys);
/*
#ifdef TRA_DEBUG
    for (int i=0;i<11;i++)
    {
        lowLog("Facet:%d:",i);
        for(int j=0;j<4;j++)
            printArray("roundKeys::", roundKeys[i][j], 4);
    }
#endif
*/
    ComputeOPc( roundKeys, OP, OPc );

    for (i=0; i<ALG_BITMODE; i++)
        rijndaelInput[i] = rand[i] ^ OPc[i];
    RijndaelEncrypt(roundKeys, rijndaelInput, temp);
#if 0
    int rijndaelInputLength = sizeof(rijndaelInput);
    rc = encrypt_NEA2( KA, KA_SIZE, rijndaelInput, &rijndaelInputLength,temp);
    if(rc == FAILURE)
    {
        higLog("%s","Encryption failed!!");
        LOG_EXIT;
        return rc;
    }
#endif
    /* To obtain output block OUT2: XOR OPc and TEMP, rotate by r2=0, and XOR
    * on the constant c2 (which is all zeroes except that the last bit is 1).
    * i.e.c2[i] = 0 for 0<=i<=127, except that c2[127] = 1
    */
    for (i=0; i<ALG_BITMODE; i++)
        rijndaelInput[i] = temp[i] ^ OPc[i];
    rijndaelInput[15] ^= 1;//XOR with c2
    RijndaelEncrypt(roundKeys, rijndaelInput, out);
#if 0
    rijndaelInputLength = sizeof(rijndaelInput);
    rc = encrypt_NEA2( KA, KA_SIZE, rijndaelInput, &rijndaelInputLength, out);
    if(rc == FAILURE)
    {
        higLog("%s","Encryption failed!!");
        LOG_EXIT;
        return rc;
    }
#endif
    //forming response RES
    for (i=0; i<ALG_BITMODE; i++)
        out[i] ^= OPc[i];
    for (i=0; i<RES_SIZE; i++)
        RES[i] = out[i+8];

    //forming Anonymity key AK
    for (i=0; i<AK_SIZE; i++)
        AK[i] = out[i];

    /* To obtain output block OUT3: XOR OPc and TEMP,
     * rotate by r3=32, and XOR on the constant c3 (which
     * is all zeroes except that the next to last bit is 1)
     * c3[i] = 0 for 0<=i<=127, except that c3[126] = 1
    */

    /*Below we copy XOR output (temp^OPc) to array rijndaelInput after
    cyclic rotation to left by 4 byes i.e.32 bits*/
    for (i=0; i<ALG_BITMODE; i++)
        rijndaelInput[(i+12) % 16] = temp[i] ^ OPc[i];

    /*TODO: Confirm this step for correctness and UE must use the same way
    Need to XOR with c3.c3[15] is 0100 0000 as per 35.206.
    Below step to XOR.with 2 i.e.0000 0010, seems as per little endian format*/
    rijndaelInput[15] ^= 2;

    RijndaelEncrypt(roundKeys, rijndaelInput, out);
#if 0
    rijndaelInputLength = sizeof(rijndaelInput);
    rc = encrypt_NEA2( KA, KA_SIZE, rijndaelInput, &rijndaelInputLength, out);
    if(rc == FAILURE)
    {
        higLog("%s","Encryption failed!!");
        LOG_EXIT;
        return rc;
    }
#endif

    //forming cipher key CK
    for (i=0; i<CK_SIZE; i++)
        out[i] ^= OPc[i];
    for (i=0; i<CK_SIZE; i++)
        CK[i] = out[i];

    /* To obtain output block OUT4: XOR OPc and TEMP,rotate by r4=64,
    * and XOR on the constant c4 (which is all zeroes except that the 2nd
    * from last bit is 1).
    * i.e. c4[i] = 0 for 0<=i<=127, except that c4[125] = 1
    */
    for (i=0; i<ALG_BITMODE; i++)
        rijndaelInput[(i+8) % 16] = temp[i] ^ OPc[i];

    /*TODO: Confirm below step for correctness and UE must use the same way.
    Need to XOR with c4;Note that c4[15] is 0010 0000 as per 35.206.
    Below step to XOR.with 2 i.e.0000 0100, seems as per little endian format*/
    rijndaelInput[15] ^= 4;

    RijndaelEncrypt(roundKeys, rijndaelInput, out);
#if 0
    rijndaelInputLength = sizeof(rijndaelInput);
    rc = encrypt_NEA2( KA, KA_SIZE, rijndaelInput, &rijndaelInputLength, out);
    if(rc == FAILURE)
    {
        higLog("%s","Encryption failed!!");
        LOG_EXIT;
        return rc;
    }
#endif
    //Forming integrity key IK
    for (i=0; i<IK_SIZE; i++)
        out[i] ^= OPc[i];
    for (i=0; i<IK_SIZE; i++)
        IK[i] = out[i];

#ifdef TRA_DEBUG
    printArray("RES::", RES, RES_SIZE);
    printArray("AK::", AK, sizeof(AK));
    printArray("CK::", CK, sizeof(CK));
    printArray("IK::", IK, sizeof(IK));
#endif

    LOG_EXIT; 
    return;
}
/* end of function f2345 */

/*------------------------------------------------------------------------------
* Algorithm f1* :Computes resynch authentication code MAC-S
* -----------------------------------------------------------------------------*/
void f1star(uint8_t KA[], uint8_t OP[], uint8_t rand[],
    uint8_t sqn[], uint8_t amf[], uint8_t (&mac_s)[MAC_SIZE])
{
    LOG_ENTRY;
    uint8_t temp[]={0x65,0xe8,0x2f,0x56,0xbc,0x06,0xac,0xee};
    bytecopy(mac_s, temp, sizeof(temp));
    LOG_EXIT;
    return;//TODO manage rc value after implementation
}

/*------------------------------------------------------------------------------
* Algorithm f5*: Computes resynch anonymity key AK
* ----------------------------------------------------------------------------*/
void f5star(uint8_t KA[], uint8_t OP[],uint8_t rand[],uint8_t (&AK)[AK_SIZE])
{
    LOG_ENTRY;
    uint8_t temp[] ={0x46,0xfc,0xcb,0x23,0x2f,0x1d};
    bytecopy(AK, temp, sizeof(temp));
    LOG_EXIT;
    return;//TODO manage rc value after implementation
}
 

/* -----------------------------------------------------------------------------
* Function to compute OPc from OP and K
* (Assume Key schedule has already been performed)
------------------------------------------------------------------------------*/
void ComputeOPc(uint8_t roundKeys[11][4][4], uint8_t OP[],
    uint8_t (&OPc)[OP_SIZE])
{
    LOG_ENTRY;
#ifdef TRA_DEBUG
    printArray("original OP:",OP, OP_SIZE);
#endif

    RijndaelEncrypt( roundKeys, OP, OPc );
#if 0
    int len = OP_SIZE;
    rc = encrypt_NEA2(KA, KA_SIZE, OP, &len, OPc );
    if(rc == FAILURE)
    {
        higLog("%s","Encryption failed!!");
        LOG_EXIT;
        return rc;
    }
#endif

    for (int i=0; i<OP_SIZE; i++)
        OPc[i] ^= OP[i];

    LOG_EXIT;
    return;
}

/*-------------------------------------------------------------------
* Rijndael key schedule function. Takes 16-byte key and creates
* all Rijndael's internal subkeys ready for encryption.
*-------------------------------------------------------------------*/
void RijndaelKeySchedule( uint8_t key[16], uint8_t (&roundKeys)[11][4][4])
{
    LOG_ENTRY;
    uint8_t roundConst;
    int i, j;
    /* first round key equals key */
    for (i=0; i<16; i++)
        roundKeys[0][i & 0x03][i>>2] = key[i];
    roundConst = 1;
    /* now calculate round keys */
    for (i=1; i<11; i++)
    {
        roundKeys[i][0][0] = Sbox[roundKeys[i-1][1][3]]
        ^ roundKeys[i-1][0][0] ^ roundConst;
        roundKeys[i][1][0] = Sbox[roundKeys[i-1][2][3]]
        ^ roundKeys[i-1][1][0];
        roundKeys[i][2][0] = Sbox[roundKeys[i-1][3][3]]
        ^ roundKeys[i-1][2][0];
        roundKeys[i][3][0] = Sbox[roundKeys[i-1][0][3]]
        ^ roundKeys[i-1][3][0];
        for (j=0; j<4; j++)
        {
            roundKeys[i][j][1] = roundKeys[i-1][j][1] ^ roundKeys[i][j][0];
            roundKeys[i][j][2] = roundKeys[i-1][j][2] ^ roundKeys[i][j][1];
            roundKeys[i][j][3] = roundKeys[i-1][j][3] ^ roundKeys[i][j][2];
        }
        /* update round constant */
        roundConst = Xtime[roundConst];
    }

    LOG_EXIT;
    return;
}

/*-------------------------------------------------------------------
* Rijndael encryption function. Takes 16-byte input and creates
* 16-byte output (using round keys already derived from 16-byte key).
*-----------------------------------------------------------------*/
void RijndaelEncrypt( uint8_t roundKeys[11][4][4],
    uint8_t input[16], uint8_t (&output)[16] )
{
    LOG_ENTRY;
    uint8_t state[4][4];
    int i, r;
    /* initialise state array from input byte string */
    for (i=0; i<16; i++)
        state[i & 0x3][i>>2] = input[i];

    /* add first round_key */
    KeyAdd(state, roundKeys, 0);

    /* do lots of full rounds */
    for (r=1; r<=9; r++)
    {
        ByteSub(state);
        ShiftRow(state);
        MixColumn(state);
        KeyAdd(state, roundKeys, r);
    }

    /* final round */
    ByteSub(state);
    ShiftRow(state);
    KeyAdd(state, roundKeys, r);

    /* produce output byte string from state array */
    for (i=0; i<16; i++)
    {
        output[i] = state[i & 0x3][i>>2];
    }

    LOG_EXIT;
    return;
}
/* end of function RijndaelEncrypt */

