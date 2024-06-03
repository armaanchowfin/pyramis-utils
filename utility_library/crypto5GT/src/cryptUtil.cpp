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

/** @file cryptUtil.cpp
 *  @brief Provides Utility Functions for encryption and integrity
 */

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/hmac.h>
#include <openssl/cmac.h>
#include "openssl/sha.h"
#include <string.h>
#include "../include/cryptUtil.h"
#include "../include/snow3g.h"
#include "platform/include/logging.h" //For traces
#include "common/include/utils.h"

using namespace std;


void generateRAND(uint8_t *buf, int num)
{
    //Choosing Mersenne Twister 19937 i.e.mt19937
    std::random_device rd;
    std::mt19937_64 rng(rd());

    /*uniform distribution in the [0, UINT8_MAX) to ensure maximum possible
    number in the range */
    uniform_int_distribution<uint8_t> dist(0, UINT8_MAX);
    for (int i = 0; i < RAND_SIZE; i++)
        buf[i] = dist(rng);
}

void generateSRAND(uint8_t *buf, int num)
{
    std::chrono::time_point<std::chrono::system_clock> time_pt;
    time_pt = chrono::high_resolution_clock::now();

    //For less randomness, you can switch to std::chrono::hours
    auto seed = std::chrono::duration_cast<std::chrono::seconds>
                                        (time_pt.time_since_epoch()).count();
    lowLog("seconds since epoch: %d",seed);
    //Choosing Mersenne Twister 19937 i.e.mt19937
    std::random_device rd;
    std::mt19937_64 rng(seed);

    /*uniform distribution in the [0, UINT8_MAX) to ensure maximum possible
    number in the range*/
    uniform_int_distribution<uint8_t> dist(0, UINT8_MAX);
    for (int i = 0; i < num; i++)
        buf[i] = dist(rng);
}


/* computes the ciphertext from plaintext and key using AES128-CBC algorithm */
void encrypt_aes_128_cbc(uint8_t* aes_key, uint8_t* aes_input,
    const int inputlength, uint8_t *aes_output)
{
    //CAUTION:Put RAND value only if possible to share IV with receiver
    /*uint8_t iv_enc[AES_BLOCK_SIZE];
    RAND_bytes(iv_enc, AES_BLOCK_SIZE);
    memcpy(iv_dec, iv_enc, AES_BLOCK_SIZE);*/

    AES_KEY enc_key;
    uint8_t iv_enc[AES_BLOCK_SIZE]={};
    memset(iv_enc, 0x00, sizeof(AES_BLOCK_SIZE));

    AES_set_encrypt_key(aes_key, (AES_BITMODE*8), &enc_key);
    AES_cbc_encrypt(aes_input, aes_output, inputlength, &enc_key,
    iv_enc, AES_ENCRYPT);//same IV should be used for decryption
}

/* computes the plaintext from ciphertext and key using AES128-CBC algorithm,
   keeping decrypt a separate function for the sake of simplicity */
void decrypt_aes_128_cbc(uint8_t* aes_key, uint8_t* aes_input,
    const int encslength, uint8_t *aes_output)
{
    /*ideally we should derive first iv_dec from ciphered text block
    but assuming 0 value for IV since receiver can't be aware of random IV*/
    uint8_t iv_dec[AES_BLOCK_SIZE]={};
    memset(iv_dec, 0x00, sizeof(AES_BLOCK_SIZE));

    AES_KEY dec_key;
    AES_set_decrypt_key(aes_key, (AES_BITMODE*8), &dec_key);
    AES_cbc_encrypt(aes_input, aes_output, encslength, &dec_key,
        iv_dec, AES_DECRYPT);
}

/*
 * 128-NEA2 Function for encryption using 128-bit AES block cipher in CTR mode
 * This function encrypts plaintext using openssl EVP APIs
 */
int encrypt_aes_128_ctr(uint8_t *key_data, const int key_data_len,
    const uint8_t *plaintext, int *len, uint8_t *ciphertext, uint8_t *counter )
{
    LOG_ENTRY;
    int rc = FAILURE;
    int c_len = *len;

    /*EVP_CIPHER_CTX is "opaque" decryption ctx structure that libcrypto uses
     to record status of encrypt/decrypt operations */
    EVP_CIPHER_CTX *e_ctx = EVP_CIPHER_CTX_new();

    if(key_data_len <= 0 || key_data_len < AES_BITMODE){
        lowLog("%s","Insufficient AES key length!");
        return rc;
    }

    /* EVP_BytesToKey is optional function which should be called to generate
     * hashed key & IV for AES CTR mode here.
     * A SHA1 digest is used to hash the supplied key material.
     * nrounds is the number of times the we hash the material.
     * More rounds are more secure but slower.
     * Below routine returns size of the derived key in bytes.
     * WARNING: Use below function only if sender and receiver both agree upon
     * generating derived hash key by specified hashing rounds.
     * Else skip this function for simple encryption but then the ciphered text
     * might be easy to break by intruder.
     */
#if 0
    /* Add your own complex/random salt as 3rd arg to EVP_BytesToKey if needed*/
    //uint8_t salt[] = {0x34, 0xff};
    uint8_t IV[AES_BITMODE]={};//To be generated by EVP_BytesToKey
    uint8_t key[AES_BITMODE]={};//To be generated by EVP_BytesToKey
    int genKeySize = 0;
    const int nrounds = 5; //Hashing rounds
    genKeySize = EVP_BytesToKey(EVP_aes_128_ctr(), EVP_sha1(), NULL, key_data,
                        key_data_len, nrounds, key, IV);
    if (genKeySize != AES_BITMODE) {
        lowLog("Key size is %d bits - should be 128 bits\n", genKeySize*8);
        return rc;
    }
#endif

    EVP_CIPHER_CTX_init(e_ctx);

    /*EVP_EncryptInit_ex() sets up cipher context ctx for encryption with given
    cipher type wnd with default(NULL) ENGINE in our case.
    IMPORTANT NOTE: we use Counter as IV here for encryption*/
    EVP_EncryptInit_ex(e_ctx, EVP_aes_128_ctr(), NULL, key_data, counter);

    /*IMPORTANT: We have disabled padding since we need to get ciphertext exact
    as sizeof plaintext */
    EVP_CIPHER_CTX_set_padding(e_ctx, 0);

    /*Update ciphertext, c_len is filled with the length of ciphertext generated,
        *len is the size of plaintext in bytes */
    if(EVP_EncryptUpdate(e_ctx, ciphertext, &c_len, plaintext,
        *len) == EVP_FAILURE)
        return rc;

    /*Refer below DOC NOTE from openssl:-
    *If padding is disabled then EVP_EncryptFinal_ex() will not encrypt any more
    *data and it will return an error if any data remains in a partial block:
    *i.e. if the total data length is not a multiple of the block size.*/
    /*if(EVP_EncryptFinal_ex(e_ctx, ciphertext+c_len, &f_len) == EVP_FAILURE)
        return rc;*/

    /*NOTE:EVP_EncryptInit_ex(), EVP_EncryptUpdate() and EVP_EncryptFinal_ex()
    * return 1 for success and 0 for failure. */
    EVP_CIPHER_CTX_free(e_ctx);
    LOG_EXIT;
    return SUCCESS;
}

/*
 * Decrypt *len bytes of ciphertext using AES CTR
 */
int decrypt_aes_128_ctr( uint8_t *key_data,const int key_data_len,
    const uint8_t *ciphertext, int *len, uint8_t *origtext, uint8_t* counter)
{
    LOG_ENTRY;
    int rc = FAILURE;
    int p_len = *len;

    /*EVP_CIPHER_CTX is "opaque" decryption ctx structure that libcrypto uses
     to record status of encrypt/decrypt operations */
    EVP_CIPHER_CTX *d_ctx = EVP_CIPHER_CTX_new();

    if(key_data_len <= 0 || key_data_len < AES_BITMODE){
        lowLog("%s","Insufficient Key length!");
        return rc;
    }

#if 0
    /* EVP_BytesToKey is optional function which should be called to generate
     hashed key & IV for AES CTR mode here.For more details in refer encrypt_NEA2.*/

    /* Add your own complex/random salt at 3rg arg of EVP_BytesToKey if needed*/
    uint8_t salt[] = {0x34, 0xff};
    uint8_t IV[AES_BITMODE]={};//To be generated by EVP_BytesToKey
    uint8_t key[AES_BITMODE]={};//To be generated by EVP_BytesToKey
    int genKeySize = 0;
    const int nrounds = 5;//Hashing rounds
    genKeySize = EVP_BytesToKey(EVP_aes_128_ctr(), EVP_sha1(), NULL, key_data,
                        key_data_len, nrounds, key, IV);
    if (genKeySize != AES_BITMODE) {
        lowLog("Key size is %d bits - should be 128 bits\n", genKeySize*8);
        return rc;
    }
#endif

    EVP_CIPHER_CTX_init(d_ctx);
    EVP_DecryptInit_ex(d_ctx, EVP_aes_128_ctr(), NULL, key_data, counter);

    /* To enable padding , we must allocate an extra cipher block size of memory */
    /*unsigned char *originaltext = (unsigned char*)malloc(p_len + AES_BLOCK_SIZE);*/
    /*To disable padding, use below macro i.e.if plaintext is exact blocksize or
    multiples of AES_BLOCK_SIZE. Comment below macro if padding is required*/
    EVP_CIPHER_CTX_set_padding(d_ctx, 0);

    if(EVP_DecryptUpdate(d_ctx, origtext, &p_len, ciphertext,
        *len) == EVP_FAILURE)
        return rc;

    /*If padding is disabled then EVP_EncryptFinal_ex() will not encrypt any
    *more data and it will return an error if any data remains in a partial
    *block i.e.if the total data length is not a multiple of the block size.*/
    /*if(EVP_DecryptFinal_ex(d_ctx, originaltext+p_len, &f_len) == EVP_FAILURE)
        return rc;*/

    EVP_CIPHER_CTX_free(d_ctx);
    LOG_EXIT;
    return SUCCESS;
}

int NAS_Integrity_NIA1(nasInputStream_t* nasInput, uint8_t* MAC)
{
    LOG_ENTRY;
    int rc = FAILURE;

    int msglength = nasInput->msglength;
    uint32_t count = *(uint32_t*)nasInput->count;
    //fresh stands for 32-bit random value,but not feasible to use for 5gtestbed
    uint32_t fresh =  0;

    printArray("NAS msg data:",(uint8_t*)nasInput->message, msglength);
    lowLog("\nNAS msg length = %lu bytes (%lu bits)\n", msglength,msglength*8);

    MAC = f9(nasInput->key, count, fresh, nasInput->direction,
                    (uint8_t*)nasInput->message, msglength*8);
    if(MAC == NULL)
    {
        lowLog("%s","NIA1:f9-snow3g function failed!");
        LOG_EXIT;
        return rc;
    }

    printArray("Calculated MAC::", MAC, SIZE_NASMAC);

    LOG_EXIT;
    return SUCCESS;
}


int NAS_Integrity_NIA2(nasInputStream_t* nasInput, uint8_t* MAC)
{
    LOG_ENTRY;
    int rc = FAILURE;
    uint8_t MACT[CMAC_BLOCK_SIZE] = {};//CMAC always produces 16 bytes MAC code
    int blength = nasInput->msglength;
    int mlength = blength + CMAC_MLENGTH;
    int key_length = NAS_KEY_SIZE;

    uint8_t *m = NULL;
    lowLog("\nOriginal msg length = %lu bytes (%lu bits)\n", blength,blength*8);

    //Forming input message for CMAC algorithm
    m = new uint8_t[mlength]();//dynamic array initialized to 0
    /*Input Message for NIA2/EIA2.Refer 33.401 B.2.3 128-EIA2
    * M0..M31 = COUNT[0]..COUNT[31]
    * Forming single byte from bearer = 5 bits, direction= 1 bit
    * M32 .. M36 = BEARER[0]...BEARER[4]
    * M37        = DIRECTION
    * Total 26 bits after M38 to M63, so m[4] is a byte which has 2 zero bits.
    * i.e. m[5] to m[6] are 24 bits with zero values because m is initialized.
    */
    memcpy(&m[0], nasInput->count, 4);
    m[4] = ((nasInput->bearer & 0x1F) << 3)|((nasInput->direction & 0x01) << 2);

    //NAS message added from M64 to blength+64
    memcpy(&m[8], nasInput->message, blength);
    printArray("Constructed complete message for Integrity::", m, mlength);
    rc = AES_CMAC(nasInput->key, key_length, m, mlength, MACT);
    if(rc == FAILURE)
    {
        lowLog("%s","AES-CMAC routine failed!");
        LOG_EXIT;
        return rc;
    }
    printArray("Full MACT::", MACT, CMAC_BLOCK_SIZE);

    memcpy(MAC, MACT, SIZE_NASMAC); //32 bit MAC
    printArray("Truncated MACT to required 4 bytes MAC::", MAC, SIZE_NASMAC);

    //memory cleaning
    if(m)
    {
        delete [] m;
        m=NULL;
    }

    LOG_EXIT;
    return rc;
}

int NASIntegrityAlgo(nasInputStream_t* nasInput, uint8_t algorithm,uint8_t* MAC)
{
    LOG_ENTRY;
    int rc = FAILURE;
    int blength = nasInput->msglength;
    int key_length = nasInput->key_length;

    //key must be exactly 16 bytes of block size
    if(key_length <= 0 || key_length != NAS_KEY_SIZE){
        lowLog("%s","Key must be exactly 16 bytes length!");
        return rc;
    }
    if(blength <= 0 ){
        lowLog("%s","Input message length is insufficient!");
        return rc; //insufficient length
    }

    switch(algorithm)
    {
        case NAS_INTEGRITY_ALG_NIA1:
        {
            lowLog("%s","Using Integrity Algorithm: 128-NIA1:Snow 3g");
            rc = NAS_Integrity_NIA1(nasInput, MAC);
            break;
        }
        case NAS_INTEGRITY_ALG_NIA2:
        {
            lowLog("%s","Using Integrity Algorithm: 128-NIA2:AES CMAC");
            rc = NAS_Integrity_NIA2(nasInput, MAC);
            break;
        }
        default:
        {
            lowLog("Provided Integrity algo is not supported = %u\n", algorithm);
            rc = FAILURE;
            break;
        }
    }
    if(rc == FAILURE)
    {
        lowLog("%s","Could not calculate MAC for the NAS Input!");
        LOG_EXIT;
        return rc;
    }

    LOG_EXIT;
    return SUCCESS;
}

/* AES 128 bit CMAC which is using key twisted with AES CBC cipher*/
int AES_CMAC(uint8_t* key, const int key_length, uint8_t* message,
    int mlength, uint8_t* MACT)
{
    int rc = FAILURE;
    CMAC_CTX *ctx = CMAC_CTX_new();
    size_t mactlen=0;//CMAC_Final requires len in size_t type
    //NOTE: CMAC functions return 1 on success or 0 on error
    if(CMAC_Init(ctx, key, key_length, EVP_aes_128_cbc(), NULL) == EVP_FAILURE)
    {
        higLog("%s", "CMAC_Init failed");
        return rc;
    }

    if(CMAC_Update(ctx, message, mlength) == EVP_FAILURE)
    {
        higLog("%s","CMAC Update failed");
        return rc;
    }

    if(CMAC_Final(ctx, MACT, &mactlen) == EVP_FAILURE)
    {
        higLog("%s", "CMAC_Final failed!");
        return rc;
    }

    //memory cleaning
    CMAC_CTX_free(ctx);
    return SUCCESS;
}

int NAS_Cipher_NEA1(nasInputStream_t* nasInput,uint8_t* output)
{
    LOG_ENTRY;

    int inputlength = nasInput->msglength;
    uint32_t count = *(uint32_t*)nasInput->count;
    lowLog("original NAS msg length:%d\n",inputlength);
    printArray("original NAS Msg:", (uint8_t*)nasInput->message, inputlength);

    f8(nasInput->key,count , nasInput->bearer, nasInput->direction,
        (uint8_t*)nasInput->message, inputlength ,output);

    if(output== NULL)
    {
        lowLog("Snow 3g routine failed!");
        LOG_EXIT;
        return FAILURE;
    }
    LOG_EXIT;
    return SUCCESS;
}


int NAS_Cipher_NEA2(nasInputStream_t* nasInput,int cipherMode, uint8_t* output)
{
    LOG_ENTRY;
    int rc=FAILURE;
    /*
    Forming input message for ciphering NEA algorithm (33.401 B.1.3 128-EEA2)
    M0..M31 = COUNT[0]..COUNT[31]
    Forming single byte from bearer = 5 bits, direction= 1 bit
     M32 .. M36 = BEARER[0] .. BEARER[4]
     M37        = DIRECTION
     M38 to M127 already set to 0 since AEScounter is well intialized*/
    uint8_t AEScounter[AES_BITMODE]={};
    memcpy(&AEScounter[0], nasInput->count, SIZE_NASCOUNT);
    AEScounter[4] = ((nasInput->bearer & 0x1F) << 3) |
                    ((nasInput->direction & 0x01) << 2);

    printArray("Constructed counter::", AEScounter, AES_BITMODE);

    if (cipherMode == AES_ENCRYPT)
        rc = encrypt_aes_128_ctr( nasInput->key, nasInput->key_length,
                   nasInput->message, &nasInput->msglength ,output, AEScounter);
    else if (cipherMode == AES_DECRYPT)
        rc = decrypt_aes_128_ctr( nasInput->key, nasInput->key_length,
                   nasInput->message, &nasInput->msglength, output, AEScounter);
    if(rc == FAILURE)
    {
        lowLog("%s","AES-encrypt_NEA2 routine failed!");
        LOG_EXIT;
        return rc;
    }
    LOG_ENTRY;
    return rc;
}


int NASCipheringAlgo(nasInputStream_t* nasInput, uint8_t algorithm,
    int cipherMode, uint8_t* output)
{
    LOG_ENTRY;
    int rc = FAILURE;
    int inputlength = nasInput->msglength;
    int key_length = nasInput->key_length;
    //key must be exactly 16 bytes of block size
    if(key_length <= 0 || key_length != NAS_KEY_SIZE){
        lowLog("%s","Key must be exactly 16 bytes length!");
        return rc;
    }
    if(inputlength <= 0 ){
        lowLog("%s","Input message length is insufficient!");
        return rc; //insufficient length
    }

    lowLog("\nOriginal message length = %lu bytes \n", inputlength );

    switch(algorithm)
    {
        case NAS_ENCRYPTION_ALG_NEA1:
        {
            lowLog("%s","Using Ciphering Algorithm: 128-NEA1:Snow 3g");
            rc = NAS_Cipher_NEA1(nasInput, output);
            break;
        }
        case NAS_ENCRYPTION_ALG_NEA2:
        {
            lowLog("%s","Using Ciphering Algorithm: 128-NEA2:AES in CTR mode");
            rc = NAS_Cipher_NEA2(nasInput, cipherMode, output);
            break;
        }
        default:
        {
            lowLog("Provided ciphering algo is not supported = %u\n", algorithm);
            rc = FAILURE;
            break;
        }
    }
    if(rc == FAILURE)
    {
        lowLog("%s","Could not cipher the NAS Input!");
        LOG_EXIT;
        return rc;
    }
    string str = "Input: ";
    str += cipherMode?"Plain NAS Message::" :"Encrypted Input";
    printArray(str.c_str(), (uint8_t*)nasInput->message, inputlength);

    str = "Output: ";
    str += cipherMode?"Encrypted NAS Message::" :"Decrypted NAS Message";
    printArray(str.c_str(), output, inputlength);;

    LOG_EXIT;
    return SUCCESS;
}

