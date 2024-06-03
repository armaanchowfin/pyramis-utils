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

/*
Test file for crypto functions only from cryptUtil.cpp and snow3g.cpp
Tests available for functions:-
1.encrypt_aes_128_cbc/decrypt_aes_128_cbc
2.encrypt_aes_128_ctr/decrypt_aes_128_ctr
3.AES CMAC
4.f8 snow_3g ciphering algo
5.f9 snow_3g integrity algo

One extra testcase for
6.CRYPTO_ctr128_encrypt openssl function (not an evp function)
*/

#include <iostream>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/cmac.h>
#include "../include/cryptUtil.h"
#include <openssl/modes.h>
#include <openssl/rand.h>
#include "../include/snow3g.h"
//#include "common/include/datatypes.h"
using namespace std;

/* Code example reference:-
http://stackoverflow.com/questions/3141860/aes-ctr-256-encryption-mode-of-operation-on-openssl
*/
struct ctr_state {
    unsigned char ivec[AES_BLOCK_SIZE];  
    unsigned int num; 
    unsigned char ecount[AES_BLOCK_SIZE]; 
}; 

void init_ctr(struct ctr_state *state, const unsigned char iv[8]) {
    /* aes_ctr128_encrypt requires 'num' and 'ecount' set to zero on the
     * first call. */
    state->num = 0;
    memset(state->ecount, 10, AES_BLOCK_SIZE);

    /* Initialise counter in 'ivec' to 0 */
    memset(state->ivec + 8, 0, 8);

    /* Copy IV into 'ivec' */
    memcpy(state->ivec, iv, 8);
}


// a simple hex-print routine. could be modified to print 16 bytes-per-line
void hex_print(const uint8_t* pv, size_t len)
{
    if (NULL == pv)
        printf("NULL");
    else
    {
        size_t i = 0;
        for (; i<len;++i)
            printf("%02X ", *pv++);
    }
    printf("\n");
}

int test_aes_128_cbc()
{
    int rc = FAILURE;
    cout<<"Testing for AES 128 bit CBC cipher"<<endl;
    unsigned char *ciphertext=nullptr;
    unsigned char *plaintext=nullptr;
    unsigned char *decryptedtext=nullptr;

    string str = "Successfully decrypted my data!!";
    const int inputlength = str.length();
    plaintext = new unsigned char[inputlength];
    strcpy((char*)plaintext, str.c_str());

    //to encrypt using aes 128 cbc 
    unsigned char cipher_key[AES_BITMODE] =
                              {0x63, 0xbf, 0xa5, 0x0e, 0xe6, 0x52, 0x33, 0x65,
                               0xff, 0x14, 0xc1, 0xf4, 0x5f, 0x88, 0x73, 0x7d};

    //encrypt
    size_t encslength = 
    ((inputlength + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE; //to ensure padding
    ciphertext = (unsigned char*)malloc(encslength*sizeof(unsigned char));
    encrypt_aes_128_cbc(cipher_key, plaintext, inputlength , ciphertext);

    //decrypt
    decryptedtext = (unsigned char*)malloc(inputlength*sizeof(unsigned char));
    decrypt_aes_128_cbc(cipher_key, ciphertext, encslength, decryptedtext);
    //ciphertext to be sent further


    printf("original:\t");
    hex_print(plaintext, inputlength);

    printf("encrypt:\t");
    hex_print(ciphertext, encslength);

    printf("decrypt:\t");
    hex_print(decryptedtext, inputlength);

    stringstream ss;
    for(int i=0;i<inputlength;i++)//tried to print outside range, but still did not get original string
        ss << decryptedtext[i];
    cout<<"original string:"<<ss.str()<<endl;

    if(strncmp((const char*)plaintext, (const char*)decryptedtext, inputlength) == 0)
    {
        cout<<"test aes_128_cbc OK!!"<<endl;
        rc = SUCCESS;
    }
    else
        cout<<"FAILED. aes-128-cbc Enc/dec failed!"<<endl;

    //release memory here...
    delete[] plaintext;
    plaintext =nullptr;
    free(ciphertext);
    ciphertext=nullptr;
    free(decryptedtext);
    decryptedtext=nullptr;
    return rc;
}

/*test Function for calling openssl EVP function for AES with CTR mode*/
int test_EVP_aes_128_ctr()
{
    int rc = FAILURE;
    uint8_t origtext[AES_BITMODE]={};
    uint8_t ciphertext[AES_BITMODE]={};
    uint8_t key_data[AES_BITMODE]={0x34, 0xb7, 0xd5, 0xee, 0xe1, 0x1a, 0x21, 0x62,
                        0xbb, 0x20, 0xf3, 0xc4, 0xe5, 0x77, 0x36, 0x74};

    uint8_t input[AES_BITMODE] ={0x63, 0xbf, 0xa5, 0x0e, 0xe6, 0x52, 0x33, 0x65,
                        0xff, 0x14, 0xc1, 0xf4, 0x5f, 0x88, 0x73, 0x7d};

    int cipherlen=0,origtextlen=0;
    int len = sizeof(input)/sizeof(uint8_t);

    cipherlen = len;
    cout<<"original text length::"<<len<<endl;

    //encrypt with Counter as NULL
    cout<<"cipherlen before encryption::"<<cipherlen<<endl;
    rc = encrypt_aes_128_ctr(key_data, AES_BITMODE, input, 
            &cipherlen, ciphertext, NULL);
    if(rc == FAILURE)
        return rc;
    cout<<"cipherlen after encryption::"<<cipherlen<<endl;

    //decrypt
    origtextlen = len;
    rc = decrypt_aes_128_ctr(key_data, AES_BITMODE, ciphertext,
            &origtextlen, origtext, NULL);
    if(rc == FAILURE)
        return rc;
    cout<<"origtextlen after decryption::"<<origtextlen<<endl;

    //printing all the texts to ensure successful test
    cout<<"original text::"<<endl;
    hex_print(input, len);
    cout<<"encrypted text::"<<endl;
    hex_print(ciphertext, cipherlen);
    cout<<"decrypted text::"<<endl;
    hex_print(origtext, origtextlen);

    if(strncmp((const char*)input, (const char*)origtext, origtextlen) == 0)
    {
        cout<<"test EVP_aes_128_ctr OK!!"<<endl;
        rc = SUCCESS;
    }
    else
        cout<<"FAILED. EVP Enc/dec failed!"<<endl;

    return rc;
}

int test_aes_cmac()
{
    //Inputs to algo
    uint8_t key[CMAC_BLOCK_SIZE] = { 0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
                                 0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c };

    uint8_t message[]={0x33,0x41,0x1d,0xaa,0xf7,0x15,0x88,0x09};

    int inputlength = sizeof(message);
    cout<<"message length:"<<inputlength<<endl;
    hex_print(message, inputlength);

    //output
    unsigned char MAC[4] = {};

    int rc = AES_CMAC(key, CMAC_BLOCK_SIZE, message, inputlength, MAC);
    if(rc == FAILURE)
        cout<<"AES-CMAC routine failed!"<<endl;
    else
    {
        cout<<"CMAC test successful"<<endl;
        cout<<"Calculated MAC:";
        hex_print(MAC, 4);
    }

    return 0;
}

/*AES Cipher with CTR mode using openssl 1.0.1c function CRYPTO_ctr128_encrypt
(old one- AES_ctr128_encrypt). Both are not EVP functions*/
int test_aes_128_ctr()
{
    struct ctr_state state;
    unsigned char iv[8];
    AES_KEY aes_key;
    unsigned const char *key = (unsigned const char*)"abcdefgh87654321";
    
    //printf("length of key: %d\n", strlen(key));
    
    const char *plaintext = "I have no idea what I am doing";
    int textlen = strlen(plaintext);
    unsigned char ciphertext[textlen]={};
    unsigned char origtext[textlen]={};
    
    if(!RAND_bytes(iv, 8)) {
        printf("Error generating random bytes.\n");
        exit(1);
    }
    
    init_ctr(&state, iv);
    
    if (AES_set_encrypt_key(key, 128, &aes_key) < 0) {
        fprintf(stderr, "Set encryption key error!\n");
        exit(1);
    }
    
    cout<<"Clean text:"<<plaintext<<endl;

    CRYPTO_ctr128_encrypt((const unsigned char*)plaintext, ciphertext, textlen,
        &aes_key, state.ivec, state.ecount, &state.num,(block128_f)AES_encrypt);
    //AES_ctr128_encrypt((const unsigned char*)plaintext, ciphertext, textlen, &aes_key, state.ivec, state.ecount, &state.num);
    cout<<"Ciphered text:"<<ciphertext<<endl;
    
    init_ctr(&state, iv);

    CRYPTO_ctr128_encrypt((const unsigned char*)ciphertext, origtext, textlen,
        &aes_key, state.ivec, state.ecount, &state.num,(block128_f)AES_encrypt);
    //AES_ctr128_encrypt((const unsigned char*)ciphertext, origtext, textlen, &aes_key, state.ivec, state.ecount, &state.num);
    cout<<"Decrypted text:"<<origtext<<endl;
    
    return 0;
}

//Test Vector 1 for UEA2
void test_snow_3g_ciphering()
{
    //Inputs to ciphering algo f8 which uses snow3g as of now
    uint8_t key[16] = { 0x2b,0xd6,0x45,0x9f,0x82,0xc5,0xb3,0x00,
                        0x95,0x2c,0x49,0x10,0x48,0x81,0xff,0x48 };

    uint8_t plaintext[]={0x7e,0xc6,0x12,0x72,0x74,0x3b,0xf1,0x61,
                         0x47};//,0x26,0x44,0x6a,0x6c,0x38,0xce,0xd1};

    uint32_t len = sizeof(plaintext);
    uint8_t ciphertext[len]={};
    uint8_t origtext[len]={};
    uint32_t count = 0x72A4F20F;
    uint32_t bearer =  0x0C;
    uint32_t direction = 0x1;

    cout<<"original data:  ";
    hex_print(plaintext, len);

    f8(key,count, bearer, direction, plaintext, len, ciphertext);
    cout<<"ciphered data:  ";
    hex_print(ciphertext, len);

    f8(key,count, bearer, direction, ciphertext, len, origtext);
    cout<<"deciphered data:";
    hex_print(origtext, len);

}

//Test Vector1 for UIA2
void test_snow_3g_integrity()
{
  //Inputs to Integrity algo f8 which uses snow3g as of now
    uint8_t key[16] = { 0x2b,0xd6,0x45,0x9f,0x82,0xc5,0xb3,0x00,
                        0x95,0x2c,0x49,0x10,0x48,0x81,0xff,0x48};

    uint8_t plaintext[]={0x6b,0x22,0x77,0x37,0x29,0x6f,0x39,0x3c,
                        0x80,0x79,0x35,0x3e,0xdc,0x87,0xe2,0xe8,
                        0x05,0xd2,0xec,0x49,0xa4,0xf2,0xd8,0xe0};


    int len = sizeof(plaintext);
    uint32_t count = 0x38A6F056;
    uint32_t direction = 0;
    uint8_t *MAC = NULL;
#if 0
    //One can test with actual random value by lib function generateSRAND
    uint8_t temp[4]={};
    generateSRAND(temp, 4);
    cout<<endl<<"SRAND:";
    hex_print(temp, 4);
    uint32_t freshrand = *(uint32_t*)temp;
#endif
    //Below number is as per test vector1 in Document3: UEA2 & UIA2
    uint32_t fresh = 0x05D2EC49;
    cout<<"original data:";
    hex_print(plaintext, len);

    MAC = f9(key, count, fresh, direction, plaintext, len*8);
    cout<<"MAC:";
    hex_print(MAC, 4);

}

int main(int argc, char *argv[])
{
    cout<<"-------------------------------------------------------------------"<<endl;
    //Running OK!
    test_aes_128_cbc();
    cout<<"-------------------------------------------------------------------"<<endl;
    //Failing, reason not sure
    test_EVP_aes_128_ctr();
    cout<<"-------------------------------------------------------------------"<<endl;
    //Working fine with NIA0
    test_aes_cmac();
    cout<<"-------------------------------------------------------------------"<<endl;
    test_aes_128_ctr();
    cout<<"-------------------------------------------------------------------"<<endl;
    test_snow_3g_ciphering();
    cout<<"-------------------------------------------------------------------"<<endl;
    test_snow_3g_integrity();

    return 0;
}



