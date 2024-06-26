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

/*------------------------------------------------------------------------
* SNOW_3G.cpp
*------------------------------------------------------------------------*/

#include "../include/snow3g.h"


/* LFSR */
uint32_t LFSR_S0 = 0x00;
uint32_t LFSR_S1 = 0x00;
uint32_t LFSR_S2 = 0x00;
uint32_t LFSR_S3 = 0x00;
uint32_t LFSR_S4 = 0x00;
uint32_t LFSR_S5 = 0x00;
uint32_t LFSR_S6 = 0x00;
uint32_t LFSR_S7 = 0x00;
uint32_t LFSR_S8 = 0x00;
uint32_t LFSR_S9 = 0x00;
uint32_t LFSR_S10 = 0x00;
uint32_t LFSR_S11 = 0x00;
uint32_t LFSR_S12 = 0x00;
uint32_t LFSR_S13 = 0x00;
uint32_t LFSR_S14 = 0x00;
uint32_t LFSR_S15 = 0x00;

/* FSM */

uint32_t FSM_R1 = 0x00;
uint32_t FSM_R2 = 0x00;
uint32_t FSM_R3 = 0x00;

/* Rijndael S-box SR */

const uint8_t SR[256] = {
0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16
};

/* S-box SQ */

const uint8_t SQ[256] = {
0x25,0x24,0x73,0x67,0xD7,0xAE,0x5C,0x30,0xA4,0xEE,0x6E,0xCB,0x7D,0xB5,0x82,0xDB,
0xE4,0x8E,0x48,0x49,0x4F,0x5D,0x6A,0x78,0x70,0x88,0xE8,0x5F,0x5E,0x84,0x65,0xE2,
0xD8,0xE9,0xCC,0xED,0x40,0x2F,0x11,0x28,0x57,0xD2,0xAC,0xE3,0x4A,0x15,0x1B,0xB9,
0xB2,0x80,0x85,0xA6,0x2E,0x02,0x47,0x29,0x07,0x4B,0x0E,0xC1,0x51,0xAA,0x89,0xD4,
0xCA,0x01,0x46,0xB3,0xEF,0xDD,0x44,0x7B,0xC2,0x7F,0xBE,0xC3,0x9F,0x20,0x4C,0x64,
0x83,0xA2,0x68,0x42,0x13,0xB4,0x41,0xCD,0xBA,0xC6,0xBB,0x6D,0x4D,0x71,0x21,0xF4,
0x8D,0xB0,0xE5,0x93,0xFE,0x8F,0xE6,0xCF,0x43,0x45,0x31,0x22,0x37,0x36,0x96,0xFA,
0xBC,0x0F,0x08,0x52,0x1D,0x55,0x1A,0xC5,0x4E,0x23,0x69,0x7A,0x92,0xFF,0x5B,0x5A,
0xEB,0x9A,0x1C,0xA9,0xD1,0x7E,0x0D,0xFC,0x50,0x8A,0xB6,0x62,0xF5,0x0A,0xF8,0xDC,
0x03,0x3C,0x0C,0x39,0xF1,0xB8,0xF3,0x3D,0xF2,0xD5,0x97,0x66,0x81,0x32,0xA0,0x00,
0x06,0xCE,0xF6,0xEA,0xB7,0x17,0xF7,0x8C,0x79,0xD6,0xA7,0xBF,0x8B,0x3F,0x1F,0x53,
0x63,0x75,0x35,0x2C,0x60,0xFD,0x27,0xD3,0x94,0xA5,0x7C,0xA1,0x05,0x58,0x2D,0xBD,
0xD9,0xC7,0xAF,0x6B,0x54,0x0B,0xE0,0x38,0x04,0xC8,0x9D,0xE7,0x14,0xB1,0x87,0x9C,
0xDF,0x6F,0xF9,0xDA,0x2A,0xC4,0x59,0x16,0x74,0x91,0xAB,0x26,0x61,0x76,0x34,0x2B,
0xAD,0x99,0xFB,0x72,0xEC,0x33,0x12,0xDE,0x98,0x3B,0xC0,0x9B,0x3E,0x18,0x10,0x3A,
0x56,0xE1,0x77,0xC9,0x1E,0x9E,0x95,0xA3,0x90,0x19,0xA8,0x6C,0x09,0xD0,0xF0,0x86
};

/* MULx.
* Input V: an 8-bit input.
* Input c: an 8-bit input.
* Output : an 8-bit output.
* See section 3.1.1 for details.
*/

uint8_t MULx(uint8_t V, uint8_t c)
{
    if ( V & 0x80 )
        return ( (V << 1) ^ c);
    else
        return ( V << 1);
}

/* MULxPOW.
* Input V: an 8-bit input.
* Input i: a positive integer.
* Input c: an 8-bit input.
* Output : an 8-bit output.
* See section 3.1.2 for details.
*/

uint8_t MULxPOW(uint8_t V, uint8_t i, uint8_t c)
{
    if ( i == 0)
        return V;
    else
        return MULx( MULxPOW( V, i-1, c ), c);
}

/* The function MUL alpha.
* Input c: 8-bit input.
* Output : 32-bit output.
* See section 3.4.2 for details.
*/

uint32_t MULalpha(uint8_t c)
{
    return ( ( ((uint32_t)MULxPOW(c, 23, 0xa9)) << 24 ) |
        ( ((uint32_t)MULxPOW(c, 245, 0xa9)) << 16 ) |
        ( ((uint32_t)MULxPOW(c, 48, 0xa9)) << 8 ) |
        ( ((uint32_t)MULxPOW(c, 239, 0xa9)) ) ) ;
}

/* The function DIV alpha.
* Input c: 8-bit input.
* Output : 32-bit output.
* See section 3.4.3 for details.
*/

uint32_t DIValpha(uint8_t c)
{
    return ( ( ((uint32_t)MULxPOW(c, 16, 0xa9)) << 24 ) |
        ( ((uint32_t)MULxPOW(c, 39, 0xa9)) << 16 ) |
        ( ((uint32_t)MULxPOW(c, 6, 0xa9)) << 8 ) |
        ( ((uint32_t)MULxPOW(c, 64, 0xa9)) ) ) ;
}

/* The 32x32-bit S-Box S1
* Input: a 32-bit input.
* Output: a 32-bit output of S1 box.
* See section 3.3.1.
*/

uint32_t S1(uint32_t w)
{
    uint8_t r0=0, r1=0, r2=0, r3=0;
    uint8_t srw0 = SR[ (uint8_t)((w >> 24) & 0xff) ];
    uint8_t srw1 = SR[ (uint8_t)((w >> 16) & 0xff) ];
    uint8_t srw2 = SR[ (uint8_t)((w >> 8) & 0xff) ];
    uint8_t srw3 = SR[ (uint8_t)((w) & 0xff) ];
    r0 = ( ( MULx( srw0 , 0x1b) ) ^
        ( srw1 ) ^
        ( srw2 ) ^
        ( (MULx( srw3, 0x1b)) ^ srw3 )
    );
    r1 = ( ( ( MULx( srw0 , 0x1b) ) ^ srw0 ) ^
        ( MULx(srw1, 0x1b) ) ^
        ( srw2 ) ^
        ( srw3 )
    );
    r2 = ( ( srw0 ) ^
        ( ( MULx( srw1 , 0x1b) ) ^ srw1 ) ^
        ( MULx(srw2, 0x1b) ) ^
        ( srw3 )
    );
    r3 = ( ( srw0 ) ^
        ( srw1 ) ^
        ( ( MULx( srw2 , 0x1b) ) ^ srw2 ) ^
        ( MULx( srw3, 0x1b) )
    );

    return ( ( ((uint32_t)r0) << 24 ) | ( ((uint32_t)r1) << 16 ) | ( ((uint32_t)r2) << 8 ) |
        ( ((uint32_t)r3) ) );
}

/* The 32x32-bit S-Box S2
* Input: a 32-bit input.
* Output: a 32-bit output of S2 box.
* See section 3.3.2.
*/

uint32_t S2(uint32_t w)
{
    uint8_t r0=0, r1=0, r2=0, r3=0;
    uint8_t sqw0 = SQ[ (uint8_t)((w >> 24) & 0xff) ];
    uint8_t sqw1 = SQ[ (uint8_t)((w >> 16) & 0xff) ];
    uint8_t sqw2 = SQ[ (uint8_t)((w >> 8) & 0xff) ];
    uint8_t sqw3 = SQ[ (uint8_t)((w) & 0xff) ];
    r0 = ( ( MULx( sqw0 , 0x69) ) ^
        ( sqw1 ) ^
        ( sqw2 ) ^
        ( (MULx( sqw3, 0x69)) ^ sqw3 )
    );
    r1 = ( ( ( MULx( sqw0 , 0x69) ) ^ sqw0 ) ^
        ( MULx(sqw1, 0x69) ) ^
        ( sqw2 ) ^
        ( sqw3 )
    );
    r2 = ( ( sqw0 ) ^
        ( ( MULx( sqw1 , 0x69) ) ^ sqw1 ) ^
        ( MULx(sqw2, 0x69) ) ^
        ( sqw3 )
    );
    r3 = ( ( sqw0 ) ^
        ( sqw1 ) ^
        ( ( MULx( sqw2 , 0x69) ) ^ sqw2 ) ^
        ( MULx( sqw3, 0x69) )
    );
    return ( ( ((uint32_t)r0) << 24 ) | ( ((uint32_t)r1) << 16 ) | ( ((uint32_t)r2) << 8 ) |
        ( ((uint32_t)r3) ) );
}

/* Clocking LFSR in initialization mode.
* LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
* Input F: a 32-bit word comes from output of FSM.
* See section 3.4.4.
*/

void ClockLFSRInitializationMode(uint32_t F)
{
    uint32_t v = ( ( (LFSR_S0 << 8) & 0xffffff00 ) ^
        ( MULalpha( (uint8_t)((LFSR_S0>>24) & 0xff) ) ) ^
        ( LFSR_S2 ) ^
        ( (LFSR_S11 >> 8) & 0x00ffffff ) ^
        ( DIValpha( (uint8_t)( ( LFSR_S11) & 0xff ) ) ) ^
        ( F )
    );
    LFSR_S0 = LFSR_S1;
    LFSR_S1 = LFSR_S2;
    LFSR_S2 = LFSR_S3;
    LFSR_S3 = LFSR_S4;
    LFSR_S4 = LFSR_S5;
    LFSR_S5 = LFSR_S6;
    LFSR_S6 = LFSR_S7;
    LFSR_S7 = LFSR_S8;
    LFSR_S8 = LFSR_S9;
    LFSR_S9 = LFSR_S10;
    LFSR_S10 = LFSR_S11;
    LFSR_S11 = LFSR_S12;
    LFSR_S12 = LFSR_S13;
    LFSR_S13 = LFSR_S14;
    LFSR_S14 = LFSR_S15;
    LFSR_S15 = v;
}

/* Clocking LFSR in keystream mode.
* LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
* See section 3.4.5.
*/

void ClockLFSRKeyStreamMode(void)
{
    uint32_t v = ( ( (LFSR_S0 << 8) & 0xffffff00 ) ^
        ( MULalpha( (uint8_t)((LFSR_S0>>24) & 0xff) ) ) ^
        ( LFSR_S2 ) ^
        ( (LFSR_S11 >> 8) & 0x00ffffff ) ^
        ( DIValpha( (uint8_t)( ( LFSR_S11) & 0xff ) ) )
    );
    LFSR_S0 = LFSR_S1;
    LFSR_S1 = LFSR_S2;
    LFSR_S2 = LFSR_S3;
    LFSR_S3 = LFSR_S4;
    LFSR_S4 = LFSR_S5;
    LFSR_S5 = LFSR_S6;
    LFSR_S6 = LFSR_S7;
    LFSR_S7 = LFSR_S8;
    LFSR_S8 = LFSR_S9;
    LFSR_S9 = LFSR_S10;
    LFSR_S10 = LFSR_S11;
    LFSR_S11 = LFSR_S12;
    LFSR_S12 = LFSR_S13;
    LFSR_S13 = LFSR_S14;
    LFSR_S14 = LFSR_S15;
    LFSR_S15 = v;
}

/* Clocking FSM.
* Produces a 32-bit word F.
* Updates FSM registers R1, R2, R3.
* See Section 3.4.6.
*/

uint32_t ClockFSM(void)
{
    uint32_t F = ( ( LFSR_S15 + FSM_R1 ) & 0xffffffff ) ^ FSM_R2 ;
    uint32_t r = ( FSM_R2 + ( FSM_R3 ^ LFSR_S5 ) ) & 0xffffffff ;
    FSM_R3 = S2(FSM_R2);
    FSM_R2 = S1(FSM_R1);
    FSM_R1 = r;
    return F;
}

/* Initialization.
* Input k[4]: Four 32-bit words making up 128-bit key.
* Input IV[4]: Four 32-bit words making 128-bit initialization variable.
* Output: All the LFSRs and FSM are initialized for key generation.
* See Section 4.1.
*/

void Initialize(uint32_t k[4], uint32_t IV[4])
{
    uint8_t i=0;
    uint32_t F = 0x0;
    LFSR_S15 = k[3] ^ IV[0];
    LFSR_S14 = k[2];
    LFSR_S13 = k[1];
    LFSR_S12 = k[0] ^ IV[1];
    LFSR_S11 = k[3] ^ 0xffffffff;
    LFSR_S10 = k[2] ^ 0xffffffff ^ IV[2];
    LFSR_S9 = k[1] ^ 0xffffffff ^ IV[3];
    LFSR_S8 = k[0] ^ 0xffffffff;
    LFSR_S7 = k[3];
    LFSR_S6 = k[2];
    LFSR_S5 = k[1];
    LFSR_S4 = k[0];
    LFSR_S3 = k[3] ^ 0xffffffff;
    LFSR_S2 = k[2] ^ 0xffffffff;
    LFSR_S1 = k[1] ^ 0xffffffff;
    LFSR_S0 = k[0] ^ 0xffffffff;
    FSM_R1 = 0x0;
    FSM_R2 = 0x0;
    FSM_R3 = 0x0;
    for(i=0;i<32;i++)
    {
        F = ClockFSM();
        ClockLFSRInitializationMode(F);
    }
}

/* Generation of Keystream.
* input n: number of 32-bit words of keystream.
* input z: space for the generated keystream, assumes
* memory is allocated already.
* output: generated keystream which is filled in z
* See section 4.2.
*/

void GenerateKeystream(uint32_t n, uint32_t *ks)
{
    uint32_t t = 0;
    uint32_t F = 0x0;
    ClockFSM(); /* Clock FSM once. Discard the output. */
    ClockLFSRKeyStreamMode(); /* Clock LFSR in keystream mode once. */
    for ( t=0; t<n; t++)
    {
        F = ClockFSM(); /* STEP 1 */
        ks[t] = F ^ LFSR_S0; /* STEP 2 */
        /* Note that ks[t] corresponds to z_{t+1} in section 4.2
        */
        ClockLFSRKeyStreamMode(); /* STEP 3 */
    }
}

/*-----------------------------------------------------------------------*/
/* end of SNOW_3G Algo Methods*/

/*-----------------------------------------------------------------------*/
/* f8: For Confidentiality (Ciphering) protection
* Input key: 128 bit Confidentiality Key.
* Input count:32-bit Count, Frame dependent input.
* Input bearer: 5-bit Bearer identity (in the LSB side).
* Input dir:1 bit, direction of transmission.
* Input data: length number of bits, input bit stream.
* Input length: 32 bit Length, i.e., the number of bits to be encrypted or
* decrypted.
* Output data: Output bit stream. Assumes data is suitably memory
* allocated.
* Encrypts/decrypts blocks of data between 1 and 2^32 bits in length as
* defined in Section 3.
*/

void f8(uint8_t *key, uint32_t count, uint32_t bearer, uint32_t dir,
    uint8_t *input, uint32_t length, uint8_t* output)
{
    uint32_t K[4]={},IV[4]={};
    /*NOTE: f8 code given in ETSI document:
    Document 1: UEA2 and UIA2 Specification, does not handle data whose length
    is not in multiple of 32 bit size.So with current code as taken from above
    document, we add those remaining bytes as it is in ciphertext/plaintext.
    If we want to ensure encryption of remaining bytes, there is other possible
    option :-
        Add/pad remaining bytes with 0s to complete block size;
        after encrypt,decrypt remove padded bytes.These are called as zero_bits.
    But note that this would again need to ge agreed at other receiver side.
    So as of now keeping code as per ETSI. If needed use zero_bits concept*/

    /*n=> no of blocks of 4 bytes i.e. 32 bits in size*/
    int n = (length*8)/32;
    int i=0;
    uint32_t *KS=nullptr;
#if 0
    //printf("n inside f8:%d\n", n);
    int extra_bytes = length % 4;//length in bytes % snow 3g block length
    printf("extra_bytes:%d\n",extra_bytes);
    int zero_bytes_reqd = 4 -extra_bytes;
    printf("zero_bytes_reqd:%d\n",zero_bytes_reqd);
#endif
    /*Initialisation*/
    /* Load the confidentiality key for SNOW 3G initialization as in section
    3.4. */
    for (i=0; i<4; i++)
        K[3-i] = (key[4*i] << 24) ^ (key[4*i+1] << 16)
               ^ (key[4*i+2] << 8) ^ (key[4*i+3]);

    /* Prepare the initialization vector (IV) for SNOW 3G initialization as in
    section 3.4. */
    IV[3] = count;
    IV[2] = (bearer << 27) | ((dir & 0x1) << 26);
    IV[1] = IV[3];
    IV[0] = IV[2];

    /* Run SNOW 3G algorithm to generate sequence of key stream bits KS*/
    Initialize(K,IV);
    KS = (uint32_t *)malloc(4*n);
    GenerateKeystream(n,(uint32_t*)KS);

    /* Exclusive-OR the input data with keystream to generate the output bit
    stream */
    memcpy(output, input, length);
    for (i=0; i<n; i++)
    {
        output[4*i+0] ^= (uint8_t) (KS[i] >> 24) & 0xff;
        output[4*i+1] ^= (uint8_t) (KS[i] >> 16) & 0xff;
        output[4*i+2] ^= (uint8_t) (KS[i] >> 8) & 0xff;
        output[4*i+3] ^= (uint8_t) (KS[i] ) & 0xff;
    }

    free(KS);
    //LOG_EXIT;
}
/* End of f8*/


/*---------------------------------------------------------
 *                  Functions for f9 Integrity algorithm
 *---------------------------------------------------------*/

/* MUL64x.
 * Input V: a 64-bit input.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling 
 * function.
 * See section 4.3.2 for details.
 */
uint64_t MUL64x(uint64_t V, uint64_t c)
{
    if ( V & 0x8000000000000000 )
        return (V << 1) ^ c;
    else
        return V << 1;
}

/* MUL64xPOW.
 * Input V: a 64-bit input.
 * Input i: a positive integer.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling function.
 * See section 4.3.3 for details.
 */
uint64_t MUL64xPOW(uint64_t V, uint8_t i, uint64_t c)
{
    if ( i == 0)
        return V;
    else
        return MUL64x( MUL64xPOW(V,i-1,c) , c);
}

/* MUL64.
 * Input V: a 64-bit input.
 * Input P: a 64-bit input.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling
 * function.
 * See section 4.3.4 for details.
 */
uint64_t MUL64(uint64_t V, uint64_t P, uint64_t c)
{
    uint64_t result = 0;
    int i = 0;

    for ( i=0; i<64; i++)
    {
        if( ( P>>i ) & 0x1 )
            result ^= MUL64xPOW(V,i,c);
    }
    return result;
}

/* mask8bit.
 * Input n: an integer in 1-7.
 * Output : an 8 bit mask.
 * Prepares an 8 bit mask with required number of 1 bits on the MSB side.
 */
uint8_t mask8bit(int n)
{
    return 0xFF ^ ((1<<(8-n)) - 1);
}

/* f9.: For Integrity protection
 * Input key: 128 bit Integrity Key.
 * Input count:32-bit Count, Frame dependent input.
 * Input fresh: 32-bit Random number.
 * Input dir:1 bit, direction of transmission (in the LSB).
 * Input data: length number of bits, input bit stream.
 * Input length: 64 bit Length, i.e., the number of bits to be MAC'd.
 * Output  : 32 bit block used as MAC
 * Generates 32-bit MAC using UIA2 algorithm as defined in Section 4.
 */
uint8_t* f9( uint8_t* key, uint32_t count, uint32_t fresh, uint32_t dir, 
    uint8_t *data, uint64_t length)
{
    uint32_t K[4],IV[4], z[5];
    uint32_t i=0;
    static uint8_t MAC_I[4] = {0,0,0,0}; /* static memory for the result */
    uint64_t D;
    uint64_t EVAL;
    uint64_t V;
    uint64_t P;
    uint64_t Q;
    uint64_t c;

    uint64_t M_D_2;
    int rem_bits = 0;

    /* Load the Integrity Key for SNOW3G initialization as in section 4.4. */
    for (i=0; i<4; i++)
        K[3-i] = (key[4*i] << 24) ^ (key[4*i+1] << 16) ^
                 (key[4*i+2] << 8) ^ (key[4*i+3]);

    /* Prepare the Initialization Vector (IV) for SNOW3G initialization as
       in section 4.4. */
    IV[3] = count;
    IV[2] = fresh;
    IV[1] = count ^ ( dir << 31 ) ;
    IV[0] = fresh ^ (dir << 15);

    z[0] = z[1] = z[2] = z[3] = z[4] = 0;

    /* Run SNOW 3G to produce 5 keystream words z_1, z_2, z_3, z_4 and z_5. */
    Initialize(K, IV);
    GenerateKeystream(5, z);
    
    P = (uint64_t)z[0] << 32 | (uint64_t)z[1];
    Q = (uint64_t)z[2] << 32 | (uint64_t)z[3];

    /* Calculation */
    if ((length % 64) == 0)
        D = (length>>6) + 1;
    else
        D = (length>>6) + 2;
    EVAL = 0;
    c = 0x1b;

    /* for 0 <= i <= D-3 */
    for (i=0; i<D-2; i++)
    {
        V = EVAL ^ ( (uint64_t)data[8*i  ]<<56 | (uint64_t)data[8*i+1]<<48 |
                     (uint64_t)data[8*i+2]<<40 | (uint64_t)data[8*i+3]<<32 |
                     (uint64_t)data[8*i+4]<<24 | (uint64_t)data[8*i+5]<<16 |
                     (uint64_t)data[8*i+6]<< 8 | (uint64_t)data[8*i+7] );
        EVAL = MUL64(V,P,c);
    }

    /* for D-2 */
    rem_bits = length % 64;
    if (rem_bits == 0)
        rem_bits = 64;
#if 0
    printf("msg length in bits:%d\n",(int)length);
    printf("rem bits:%d\n",rem_bits);
#endif

    M_D_2 = 0;
    i = 0;
    while (rem_bits > 7)
    {
        M_D_2 |= (uint64_t)data[8*(D-2)+i] << (8*(7-i));
        rem_bits -= 8;
        i++;
    }
    if (rem_bits > 0)
        M_D_2 |= (uint64_t)(data[8*(D-2)+i] & mask8bit(rem_bits)) << (8*(7-i));

    V = EVAL ^ M_D_2;
    EVAL = MUL64(V,P,c);

    /* for D-1 */
    EVAL ^= length;

    /* Multiply by Q */
    EVAL = MUL64(EVAL,Q,c);

    /* XOR with z_5: this is a modification to the reference C code,
       which forgot to XOR z[5] */
        /*
        MAC_I[i] = (mac32 >> (8*(3-i))) & 0xff;
        */
    for (i=0; i<4; i++)
        MAC_I[i] = ((EVAL >> (56-(i*8))) ^ (z[4] >> (24-(i*8)))) & 0xff;

    return MAC_I;
}

/* End of f9.c */
/*------------------------------------------------------------------------*/


