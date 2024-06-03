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

/** @file cryptUtil.h
 *  @brief Provides Utility Functions for encryption and integrity
 *
 *  Purpose: ONLY for Authentication and NAS security in 5G test bed project
 *  Important points:-
 *  1. Support for symmetric cryptographic algorithms
 *  2. Supported algorithms in cryptUtil uses 128 bit keys at present
 *  3. Use of openssl high level EVP APIs.
       Functions in this library are wrappers around these APIs and do not
       directly return values from EVP functions.They return values:-
       rc= 0 on success
       rc=-1 on failure
 *
 *  @author Manasi Kolhe
 *  @bug No known bugs yet
 */
#ifndef _CRYPTUTIL_H
#define _CRYPTUTIL_H

#include <bits/stdc++.h>
#include <string.h>
#include <openssl/aes.h>
#include "common/include/datatypes.h"

/*33.501 5.1.3 The 5GC and 5G-RAN shall allow for use of encryption and
integrity protection algorithms for AS and NAS protection having keys of
length 128 bits as of now*/
#define AES_BITMODE     16

/*Defining separate macro since evp functions return different values;
We stick to usual values:-success=0, failure=1 */
#define EVP_SUCCESS     1
#define EVP_FAILURE     0
#define CMAC_MLENGTH    8   


#define CMAC_BLOCK_SIZE 16  


/** @brief API to generate random number using Mersenne twister engine
*           with default seed (analogous to rand)
*           Using merscenne twister engine can generate longest non-repeating
*           sequence with the most desirable spectral characteristics
*
*   @param buf array/pointer for output random value,
*   @param num Output size
*   @return void.
*/
void generateRAND(uint8_t *buf, int num);

/** @brief API to generate random number using Mersenne twister engine
*           with current time seed (analogous to srand)
*
*   @param buf array/pointer for output random value,
*   @param num Output size
*   @return void.
*/
void generateSRAND(uint8_t *buf, int num);

/** @brief API to encrypt using AES Counter mode in current AES_BITMODE.
*           This wrapper API uses set of openssl EVP functions for encryption
*
*   @param key_data,key_data_len:- AES key for algorithm & its length
*   @param plaintext,len:- input plain text & its len
*   @param ciphertext:-generated ciphertext (length same as plaintext length)
*   @param counter: IV/Nonce for AES CTR, default is NULL
*   @return SUCCESS or FAILURE. Failure happens if :-
*           1.Key size is not 128 bits
*           2.ciphertext input has insufficient size

*/

int encrypt_aes_128_ctr( uint8_t *key_data,const int key_data_len,
    const uint8_t *plaintext, int *len,
    uint8_t *ciphertext, uint8_t *counter = NULL);

/** @brief API to decrypt using AES Counter mode in current AES_BITMODE
*           This wrapper API uses set of openssl EVP functions for decryption
*
*   @param  key_data,key_data_len:- AES key for algorithm & its length
*   @param  plaintext,len:- cipher text & its len
*   @param  ciphertext:-generated original text
*   @param  counter: IV/Nonce for AES CTR
*   @return SUCCESS or FAILURE.Failure happens if :-
*           1.Key size is not 128 bits
*           2.ciphertext input has insufficient size
*/
int decrypt_aes_128_ctr(uint8_t *key_data,const int key_data_len,
    const uint8_t *ciphertext, int *len,
    uint8_t *originaltext, uint8_t *counter = NULL);

/** @brief API to compute Message Authentication code (MAC)
*           This wrapper API uses set of openssl EVP functions for AES-CMAC
*
*   @param key_data,key_length:- AES key for algorithm & its length
*   @param message,mlength:- input message & its length
*   @param MACT: computed Message Authentication Code (MACT)
*   @return SUCCESS or FAILURE.Failure happens if :-
*           1.Integrity Key size is not 128 bits
*           2.MACT pointer has insufficient size
*/
int AES_CMAC(uint8_t* key, const int key_length, uint8_t* message,
    int mlength, uint8_t* MACT);

/** @brief API to compute Message Authentication code (MAC) using NIA1
*                   i.e f9 function which uses snow 3g algorithm.
*
*   @param nasInput:- NAS input structure which includes:-
                        Encryption key, key size, COUNT (4 bytes),
                        Bearer (5 bits in 1 byte), direction (1 bit in byte)
                        NAS message, NAS msg length
*   @param output:- MAC (32 bits)
*   @return SUCCESS or FAILURE
*/
int NAS_Integrity_NIA1(nasInputStream_t* nasInput, uint8_t* MAC);

/** @brief API to compute Message Authentication code (MAC) using AES CMAC algo
*
*   @param nasInput:- NAS input structure which includes:-
                        Encryption key, key size, COUNT (4 bytes),
                        Bearer (5 bits in 1 byte), direction (1 bit in byte)
                        NAS message, NAS msg length
*   @param output:- MAC (32 bits)
*   @return SUCCESS or FAILURE
*/
int NAS_Integrity_NIA2(nasInputStream_t* nasInput, uint8_t* MAC);

/** @brief API to compute Message Authentication code (MAC) using
*           user specified 5G algorithm to verify integrity
*           NOTE:MAC will be same if algorithm choice and
            nas input stream matches at sender receiver
*   @param nasInput:- structure which includes:-
                        Integrity key, key size, COUNT (4 bytes),
                        Bearer (5 bits in 1 byte), direction (1 bit in byte)
                        NAS message, NAS msg length
*   @param MAC:- computed NAS MAC (4 bytes)
*   @return SUCCESS or FAILURE
*/
int NASIntegrityAlgo(nasInputStream_t* nasInput, uint8_t algorithm,
    uint8_t* MAC);

/** @brief API to call NEA1 i.e. Snow 3g encrypt/decrypt functions
*
*   @param nasInput:- NAS input structure which includes:-
                        Encryption key, key size, COUNT (4 bytes),
                        Bearer (5 bits in 1 byte), direction (1 bit in byte)
                        NAS message, NAS msg length
*   @param output:- Ciphertext (length same as input text length)
*   @return SUCCESS or FAILURE
*/
int NAS_Cipher_NEA1(nasInputStream_t* nasInput,uint8_t* output);

/** @brief API to call NEA2 encrypt/decrypt functions
*
*   @param nasInput:- NAS input structure which includes:-
                        Encryption key, key size, COUNT (4 bytes),
                        Bearer (5 bits in 1 byte), direction (1 bit in byte)
                        NAS message, NAS msg length
*   @param cipherMode:- AES_ENCRYPT OR AES_DECRYPT
*   @param output:- Ciphertext (length same as input text length)
*   @return SUCCESS or FAILURE
*/
int NAS_Cipher_NEA2(nasInputStream_t* nasInput,int cipherMode, uint8_t* output);

/** @brief API to compute cipher using user specified 5G ciphering algorithm
*
*   @param nasInput:- NAS input structure which includes:-
                        Encryption key, key size, COUNT (4 bytes),
                        Bearer (5 bits in 1 byte), direction (1 bit in byte)
                        NAS message, NAS msg length
*   @param algorithm:- NAS Ciphering Algorithm id (NEA1/NEA2)
*   @param cipherMode:- AES_ENCRYPT OR AES_DECRYPT
*   @param output:- Ciphertext (length same as input text length)
*   @return SUCCESS or FAILURE
*/
int NASCipheringAlgo(nasInputStream_t* nasInput, uint8_t algorithm,
    int cipherMode, uint8_t* output);


/** @brief Simple API wrapper around aes_128_cbc(non-evp) i.e.CBC mode.
*           Note that CBC mode does add padding by default.Thus generated
            ciphertext is more than input text length
**/

void encrypt_aes_128_cbc(uint8_t* aes_key, uint8_t* aes_input,
    const int inputlength, uint8_t *aes_output);
void decrypt_aes_128_cbc(uint8_t* aes_key, uint8_t* aes_input,
    const int encslength, uint8_t *aes_output);

#endif
