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

#include "common/include/aka.h"
#include "common/include/datatypes.h"
#include "common/include/utils.h"
#include "aka/include/securityKeysUtil.h"
#include "platform/include/logging.h" //For traces
#include "crypto5GT/include/kdf.h"
#include "openssl/sha.h"

using namespace std;


/*-------------------------------------------------------------------
* Function to compute key Kausf using KDF.
* KDF parameters: Input Key=CK||IK, data input S= P0||L0||P1||L1
* inputs: CK, IK, SQN, AK, snName
* outputs: Kausf
*-----------------------------------------------------------------*/
int computeKausf(uint8_t CK[], uint8_t IK[], uint8_t SQN[], uint8_t AK[], const char snName[], uint8_t (&Kausf)[KAUSF_SIZE])
{
    LOG_ENTRY;
    int rc=FAILURE;
    uint8_t FC = 0x6A;
    kdf_parameter data_input[2]={};
    kdf_parameter result={};
    kdf_parameter key_input={};
    uint8_t temp[KAUSF_SIZE]={};
    int i=0;

    //Step1: Build Key input=> CK || IK
    for (i = 0; i < CK_SIZE; i++)
        temp[i]= CK[i];
    for (int k = 0; k < IK_SIZE; k++)
         temp[i++]= IK[k];

    key_input.param = temp;
    key_input.len = sizeof(temp);

    //Step2: data input string => P0=snName , P1=SQN^AK
    //P0=snName
    data_input[0].param = (unsigned char*)snName;
    data_input[0].len = strlen((const char*)snName);

    //P1=SQN^AK
    uint8_t tempinput[SQN_SIZE]={};
    for(int k=0; k<SQN_SIZE; k++)
        tempinput[k] = SQN[k] ^ AK[k];

    data_input[1].param = tempinput;
    data_input[1].len = SQN_SIZE;

    //Step 3: compute result
    result.param = Kausf;
    result.len = KAUSF_SIZE;//32
    rc = kdf(FC, data_input, 2, &key_input, 1, &result);
    if(rc < 0)
    {
        higLog("%s","kdf failed with rc=%d", rc);
        LOG_EXIT;
        return FAILURE;
    }

    printArray("Computed Kausf::", result.param, KAUSF_SIZE);

    LOG_EXIT;
    return SUCCESS;
}

/*-------------------------------------------------------------------
* Function to compute XRES from RES using KDF
* KDF parameters:data Input: P0||P1||P2; P0=snName, P1=RAND, P2 = RES
* KDF Input Key to KDF: CK||IK
* Inputs: RAND, CK, IK, RES, snName
* Outputs: XRES
*-----------------------------------------------------------------*/
int computeXRES(uint8_t rand[], uint8_t CK[], uint8_t IK[], uint8_t RES[], const char snName[], uint8_t (&XRES)[XRES_SIZE])
{
    LOG_ENTRY;
    int rc=FAILURE;
    uint8_t FC = 0x6B;//for XRES
    kdf_parameter data_input[3]={};
    kdf_parameter result={};
    kdf_parameter key_input={};
    uint8_t temp[CK_SIZE+IK_SIZE]={};
    int i=0;

    //Step1: Build Key input=> CK || IK
    for (i = 0; i < CK_SIZE; i++)
        temp[i]= CK[i];
    for (int k = 0; k < IK_SIZE; k++)
         temp[i++]= IK[k];

    key_input.param = temp;
    key_input.len = sizeof(temp);

    //Step2: data input string => P0=snName , P1=SQN^AK
    //P0=snName
    lowLog("Serving NW Name:%s  with len: %d",snName, strlen(snName));
    data_input[0].param = (unsigned char*)snName;
    data_input[0].len = strlen((const char*)snName);

    //P1=RAND
    data_input[1].param = rand;
    data_input[1].len = RAND_SIZE;

    //P2=RES
    data_input[2].param = RES;
    data_input[2].len = RES_SIZE;

    //Step 3: compute result
    result.param = XRES;
    result.len = 256;//assuming some MAX
    rc = kdf(FC, data_input, 3, &key_input, 1, &result);
    if(rc < 0)
    {
        higLog("%s","kdf failed with rc=%d", rc);
        LOG_EXIT;
        return FAILURE;
    }

    //XRES=> only 128 least significant bits of kdf output
    printArray("Computed XRES::",XRES, XRES_SIZE);

    LOG_EXIT;
    return SUCCESS;
}



/*-------------------------------------------------------------------
* Function to compute Anchor key, Kseaf from Kausf using KDF
* KDF parameters: Input Key=Kausf, data input S= P0 where P0 is snName
* inputs: Kausf, snName
* outputs: Kseaf
*-----------------------------------------------------------------*/
int computeKseaf(uint8_t Kausf[], char snName[], uint8_t (&Kseaf)[KSEAF_SIZE])
{
    LOG_ENTRY;
    int rc = FAILURE;
    uint8_t FC = 0x6C;//for Kseaf
    kdf_parameter data_input={};
    kdf_parameter result={};
    kdf_parameter key_input={};

    //Step1: Build Key input parameter from Kausf
    key_input.param = &Kausf[0];
    key_input.len = KAUSF_SIZE;

    /*lowLog("%s","key input param:");
    printArray(key_input.param , KAUSF_SIZE);*/

    //Step2: Build input s from snName
    data_input.param = (unsigned char*)snName;
    data_input.len = strlen((const char*)snName);

    //Step 3: compute result
    result.param = Kseaf;
    result.len = KSEAF_SIZE;
    rc = kdf(FC, &data_input, 1, &key_input, 1, &result);
    if(rc < 0)
    {
        higLog("%s","kdf failed!");
        LOG_EXIT;
        return FAILURE;
    }

    //lowLog("%s","result.len::%d",result.len);
    printArray("Computed Kseaf::", result.param, KSEAF_SIZE);

    LOG_EXIT;
    return SUCCESS;
}



/*-------------------------------------------------------------------
* Function to calculate HXRES using SHA-256
* Hash input S= P0||P1 where P0=RAND, P1=XRES
* inputs: RAND, XRES
* outputs:HXRES
*-----------------------------------------------------------------*/
int computeHXRES(uint8_t rand[], uint8_t XRES[], uint8_t (&HXRES)[XRES_SIZE])
{
    LOG_ENTRY;
    //int rc = FAILURE;

    uint8_t temp[RAND_SIZE + XRES_SIZE]={};
    int i;
    //Step1: Build input => RAND and XRES
    for (i = 0; i < RAND_SIZE; i++)
        temp[i]= rand[i];
    for (int k = 0; k < XRES_SIZE; k++)
         temp[i++]= XRES[k];

    //Step 2: Hashing
    uint8_t hashVal[SHA256_DIGEST_LENGTH]={};
    SHA256(temp, RAND_SIZE + XRES_SIZE, hashVal);
    //taking only 128 bits of hashed value (256 bits)
    bytecopy(HXRES, hashVal, XRES_SIZE);
    printArray("Computed HXRES::", HXRES, XRES_SIZE);

    LOG_EXIT;

    return SUCCESS;
}


/*-------------------------------------------------------------------
* Function to compute key Kamf from Kseaf using KDF
* KDF Input Key=Kseaf, data input S= P0||P1 where P0 is supi, P1 is ABBA
* input: Kseaf, supi, ABBA
* output: Kamf
*-----------------------------------------------------------------*/
int computeKamf(uint8_t Kseaf[], string supi_str, uint8_t ABBA, uint8_t (&Kamf)[KAMF_SIZE])
{
    LOG_ENTRY;
    int rc = FAILURE;
    uint8_t FC = 0x6D;//for Kamf
    kdf_parameter data_input[2]={};
    kdf_parameter result={};
    kdf_parameter key_input={};

    //Step1: Build Key input=> Kseaf
    key_input.param = Kseaf;
    key_input.len = KSEAF_SIZE;

    //Step2: data input string => P0=supi as a string
#if 0
    data_input[0].len = 2;//2 bytes including all 4 stucture fields in supi

    uint8_t byte1 = 0x00;
    byte1 |= supi.supi_format;
    byte1 |= supi.imsi.mcc << 1;
    byte1 |= supi.imsi.mnc << 4;
    byte1 |= supi.imsi.msin_high << 7;

    uint8_t temp[2]={};
    temp[0] = byte1;
    temp[1] = supi.imsi.msin_low;
#endif
    uint supi_len = supi_str.length();
    uint8_t supi[MAX_SUPI_SIZE] = {};
    memcpy(supi, supi_str.c_str(), supi_len);
    data_input[0].param =  supi;
    data_input[0].len = supi_len;

    //P1=ABBA
    data_input[1].len = 1;
    data_input[1].param = &ABBA;

    //Step 3: compute result
    result.param = Kamf;
    result.len = KAMF_SIZE;
    rc = kdf(FC, data_input, 2, &key_input, 1, &result);
    if(rc < 0)
    {
        higLog("%s","kdf failed!!");
        LOG_EXIT;
        return FAILURE;
    }

    printArray("Computed Kamf::", Kamf, KAMF_SIZE);

    LOG_EXIT;
    return SUCCESS;
}

#define ACCESS_TYPE_DISTINGUISHER_3GPP_ACCESS       0x01
#define ACCESS_TYPE_DISTINGUISHER_NON_3GPP_ACCESS   0x02

/*-------------------------------------------------------------------
* Function to compute key Kgnb from Kamf using KDF
* KDF Input Key=Kamf, data input S= P0||P1 where P0 is uplink NAS COUNT,
* P1 is Access type distinguisher
* input: Kamf, uplink NAS COUNT
* output: Kgnb
*-----------------------------------------------------------------*/
int computeKgnb(uint8_t Kamf[], uint8_t *uplinkNASCount, uint8_t (&Kgnb)[KGNB_SIZE])
{
    LOG_ENTRY;
    int rc = FAILURE;
    uint8_t FC = 0x6E;//for Kgnb
    kdf_parameter data_input[2]={};
    kdf_parameter result={};
    kdf_parameter key_input={};

    //Step1: Build Key input=> Kamf
    key_input.param = Kamf;
    key_input.len = KAMF_SIZE;

    //Step2: data input string => P0=uplink NAS count
    data_input[0].param = uplinkNASCount;
    data_input[0].len = sizeof(uint32_t);

    //P1=Access type distiguisher
    data_input[1].len = 1;
    uint8_t accessType = ACCESS_TYPE_DISTINGUISHER_3GPP_ACCESS;
    data_input[1].param = &accessType;

    //Step 3: compute result
    result.param = Kgnb;
    result.len = KGNB_SIZE;
    rc = kdf(FC, data_input, 2, &key_input, 1, &result);
    if(rc < 0)
    {
        higLog("%s","kdf failed!!");
        LOG_EXIT;
        return FAILURE;
    }
    printArray("Computed Kgnb::", Kgnb, KGNB_SIZE);

    LOG_EXIT;
    return SUCCESS;
}

/*-------------------------------------------------------------------
* Function to compute key Kn3iwf from Kamf using KDF
* KDF Input Key=Kamf, data input S= P0||P1 where P0 is uplink NAS COUNT,
* P1 is Access type distinguisher
* input: Kamf, uplink NAS COUNT
* output: Kn3iwf
*-----------------------------------------------------------------*/
int computeKn3iwf(uint8_t Kamf[], uint8_t *uplinkNASCount, uint8_t (&Kn3iwf)[KN3IWF_SIZE])
{
    LOG_ENTRY;
    int rc = FAILURE;
    uint8_t FC = 0x6E;//for Kgnb
    kdf_parameter data_input[2]={};
    kdf_parameter result={};
    kdf_parameter key_input={};

    //Step1: Build Key input=> Kamf
    key_input.param = Kamf;
    key_input.len = KAMF_SIZE;

    //Step2: data input string => P0=uplink NAS count
    data_input[0].param = uplinkNASCount;
    data_input[0].len = sizeof(uint32_t);

    //P1=Access type distiguisher
    data_input[1].len = 1;
    uint8_t accessType = ACCESS_TYPE_DISTINGUISHER_NON_3GPP_ACCESS;
    data_input[1].param = &accessType;

    //Step 3: compute result
    result.param = Kn3iwf;
    result.len = KN3IWF_SIZE;
    rc = kdf(FC, data_input, 2, &key_input, 1, &result);
    if(rc < 0)
    {
        higLog("%s","kdf failed!!");
        LOG_EXIT;
        return FAILURE;
    }
    printArray("Computed Kn3iwf::", Kn3iwf, KN3IWF_SIZE);

    LOG_EXIT;
    return SUCCESS;
}

/*-------------------------------------------------------------------
* Function compute KNASint key for NAS integrity algorithms using KDF
* KDF input S: algo-type, algo-id; KDF key: Kamf
* Input  argument: Kamf, algo type distinguisher, algo id
* Output argument: KNASint or KNASenc(128-bit key)
                   (TRUNCATED from 256 bit KDF output i.e. 128 least significant bits used)
-------------------------------------------------------------------*/
int computeNasSecurityKey(uint8_t Kamf[], uint8_t algo_type, uint8_t algo_id, uint8_t (&NAS_key)[NAS_KEY_SIZE])
{
    LOG_ENTRY;
    int rc = FAILURE;
    //temporary traces
    binPrint("Verifying algo_type:", algo_type);
    binPrint("Verifying algo_id:", algo_id);

    uint8_t FC = 0x69;//Common for both NAS integrity and ciphering keys

    kdf_parameter data_input[2]={};
    kdf_parameter result={};
    kdf_parameter key_input={};
    uint8_t temp[KAMF_SIZE]={};//taking max value 256 bits same as key size value for kdf output

    //Step1: Build Key input parameter from Kamf
    key_input.param = &Kamf[0];
    key_input.len = KAMF_SIZE;

    /*lowLog("%s","key input param:");
    printArray(key_input.param , KAMF_SIZE);*/

    //Step2: Build input
    //P0- algorithm type distinguisher
    data_input[0].param = &algo_type;
    data_input[0].len = 1;

    //P1- algorithm id
    data_input[1].param = &algo_id;
    data_input[1].len = 1;

    //Step 3: compute result
    result.param = temp;
    result.len = sizeof(temp);
    rc = kdf(FC, data_input, 2, &key_input, 1, &result);
    if(rc < 0)
    {
        higLog("%s","kdf failed!");
        LOG_EXIT;
        return FAILURE;
    }

    //printArray(temp, result.len);

    //truncate result/temp and use 128 least significant bits as K_NASint
    bytecopy(NAS_key , temp, NAS_KEY_SIZE);
    printArray("Computed NAS Key::", NAS_key, NAS_KEY_SIZE);

    LOG_EXIT;
    return SUCCESS;
}
