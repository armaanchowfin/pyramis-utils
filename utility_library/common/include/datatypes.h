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
 * This file defines datatypes.
 *
 * It contains the 5gTestBed defined datatypes
 *
*/

#ifndef __DATATYPES_H
#define __DATATYPES_H

#include <arpa/inet.h>

#include "IEs.h"
#include "aka.h"
#include <errno.h>

#define	IP_ADDR_MAX_STRLEN	(INET6_ADDRSTRLEN+1)

#define MCC_HUNDREDS(vALUE) ((vALUE) / 100)


#define MNC_HUNDREDS(vALUE, mNCdIGITlENGTH) ( mNCdIGITlENGTH == 2 ? 15 : (vALUE) / 100)
#define MCC_MNC_TENS(vALUE) (((vALUE) / 10) % 10)
#define MCC_MNC_DIGIT(vALUE) ((vALUE) % 10)



#define INT16_TO_BUFFER(x, buf) \
do {                            \
    (buf)[0] = (x) >> 8;        \
    (buf)[1] = (x);             \
} while(0)

#define INT16_TO_OCTET_STRING(x, aSN)           \
do {                                            \
    (aSN)->buf = (uint8_t *)calloc(2, sizeof(uint8_t));    \
    (aSN)->size = 2;              \
    INT16_TO_BUFFER(x, (aSN)->buf);             \
} while(0)

#define BITCNT_TO_BYTECNT(vALUE) ((vALUE + 7)/8)

#ifndef SUCCESS
#define SUCCESS	(0)
#define FAILURE (-1)
#endif

#define PLMN_SIZE 3
#define SD_SIZE 3
#define SST_SIZE 1
#define MAX_MSG_LEN 3000
#define SIZE_NASMAC      4
#define SIZE_NASCOUNT    4
#define NAS_KEY_SIZE    16

#define N_NAS_ENCRYPTION_ALG 0x01
#define N_NAS_INTEGRITY_ALG 0x02


#define NAS_ENCRYPTION_ALG_NEA0  0b000
#define NAS_ENCRYPTION_ALG_NEA1  0b001
#define NAS_ENCRYPTION_ALG_NEA2  0b010
#define NAS_ENCRYPTION_ALG_NEA3  0b011
#define NAS_ENCRYPTION_ALG_NEA4  0b100
#define NAS_ENCRYPTION_ALG_NEA5  0b101
#define NAS_ENCRYPTION_ALG_NEA6  0b110
#define NAS_ENCRYPTION_ALG_NEA7  0b111
#define INVALID_NAS_ENCRYPTION_ALG 0b1111111

#define NAS_INTEGRITY_ALG_NIA0  0b000
#define NAS_INTEGRITY_ALG_NIA1  0b001
#define NAS_INTEGRITY_ALG_NIA2  0b010
#define NAS_INTEGRITY_ALG_NIA3  0b011
#define NAS_INTEGRITY_ALG_NIA4  0b100
#define NAS_INTEGRITY_ALG_NIA5  0b101
#define NAS_INTEGRITY_ALG_NIA6  0b110
#define NAS_INTEGRITY_ALG_NIA7  0b111
#define INVALID_NAS_INTEGRITY_ALG 0b1111111


#define SECURITY_HEADER_TYPE_NOT_PROTECTED                    0b0000
#define SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED              0b0001
#define SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED     0b0010
#define SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_NEW          0b0011
#define SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED_NEW 0b0100


#define _5GS_MOBILITY_MANAGEMENT_MESSAGE        0x7E
#define _5GS_SESSION_MANAGEMENT_MESSAGE         0x2E



#define  _5GS_MOBILITY_MANAGEMENT_MESSAGES_MASK       0b01111111
                                                       

#define  REGISTRATION_REQUEST                         0b01000001  

#define  REGISTRATION_ACCEPT                          0b01000010  

#define  REGISTRATION_COMPLETE                        0b01000011  

#define  REGISTRATION_REJECT                          0b01000100  

#define  DEREGISTRATION_REQUEST_UE_ORIGINATING        0b01000101
#define  DEREGISTRATION_ACCEPT_UE_ORIGINATING         0b01000110
#define  DEREGISTRATION_REQUEST_UE_TERMINATED         0b01000111
#define  DEREGISTRATION_ACCEPT_UE_TERMINATED          0b01001000
#define  SERVICE_REQUEST                              0b01001100
#define  SERVICE_REJECT                               0b01001101
#define  SERVICE_ACCEPT                               0b01001110
#define  CONFIGURATION_UPDATE_COMMAND                 0b01010100
#define  CONFIGURATION_UPDATE_COMPLETE                0b01010101
#define  AUTHENTICATION_REQUEST                       0b01010110 

#define  AUTHENTICATION_RESPONSE                      0b01010111 

#define  AUTHENTICATION_REJECT                        0b01011000 

#define  AUTHENTICATION_FAILURE                       0b01011001 

#define  AUTHENTICATION_RESULT                        0b01011010
#define  IDENTITY_REQUEST                             0b01011011
#define  IDENTITY_RESPONSE                            0b01011100
#define  SECURITY_MODE_COMMAND                        0b01011101
#define  SECURITY_MODE_COMPLETE                       0b01011110
#define  SECURITY_MODE_REJECT                         0b01011111
#define  _5GMM_STATUS                                 0b01100100
#define  NOTIFICATION                                 0b01100101
#define  NOTIFICATION_RESPONSE                        0b01100110
#define  UL_NAS_TRANSPORT                             0b01100111 

#define  DL_NAS_TRANSPORT                             0b01101000



#define SESSION_ESTABLISHMENT_REQUEST                 0b11000001 

#define SESSION_ESTABLISHMENT_ACCEPT                  0b11000010 

#define SESSION_ESTABLISHMENT_REJECT                  0b11000011
#define SESSION_AUTHENTICATION_COMMAND                0b11000101
#define SESSION_AUTHENTICATION_COMPLETE               0b11000110
#define SESSION_AUTHENTICATION_RESULT                 0b11000111
#define SESSION_MODIFICATION_REQUEST                  0b11001001 

#define SESSION_MODIFICATION_REJECT                   0b11001010
#define SESSION_MODIFICATION_COMMAND                  0b11001011 

#define SESSION_MODIFICATION_COMPLETE                 0b11001100 

#define SESSION_MODIFICATION_COMMAND_REJECT           0b11001101
#define SESSION_RELEASE_REQUEST                       0b11010001 

#define SESSION_RELEASE_REJECT                        0b11010010
#define SESSION_RELEASE_COMMAND                       0b11010011 

#define SESSION_RELEASE_COMPLETE                      0b11010100 

#define _5GSM_STATUS                                  0b11010110


#define SECU_DIRECTION_UPLINK   0
#define SECU_DIRECTION_DOWNLINK 1

#define _5G_MOBILE_IDENTITY_EVEN  0
#define _5G_MOBILE_IDENTITY_ODD   1



#define SMS_SUPPORTED       1
#define SMS_NOT_SUPPORTED   0


/* NOTE: Below fields are temporary.
(TODO: Not decided on keeping Security params in Config or here itself)
   ABBA
   BEARER ID
*/
#define ABBA_PARAM          0x00
#define BEARER_ID_3GPP      0x01
#define BEARER_ID_NON3GPP     0x02
#define BEARER_ID_INVALID     0x00

typedef struct securityAlgo {

 uint8_t nasEncAlgo;
 uint8_t nasIntAlgo;
 uint8_t asEncAlgo;
 uint8_t asIntAlgo;

}securityAlgo_t;

typedef enum NgapTypeofMessage {
    A,
    B,
    C
}NgapTypeofMessage_t;

typedef enum IdentityType {
    E_NO_IDENTITY                    = 0b000,
    E_SUCI                           = 0b001,
    E_5G_GUTI                        = 0b010
}IdentityType_t;



typedef enum SUPIFormat {
    E_IMSI                          = 0b000,
    E_NETWORK_SPECIFIC_IDENTIFIER   = 0b001
}SUPIFormat_t;



typedef enum ProtectionScheme {
    E_NULL_SCHEME       = 0b0000,
    E_ECIES_PROFILE_A   = 0b0001,
    E_ECIES_PROFILE_B   = 0b0010
}ProtectionScheme_t;


#define SERVING_NETWORK_NAME_LEN	(128)
typedef uint8_t     NasMessageType_t;
typedef uint8_t     PduSessionid_t;
typedef uint8_t     Pti_t;


typedef uint8_t     ExtendedProtocolDiscriminator_t;
typedef uint8_t     SecurityHeaderType_t;
typedef unsigned long    AmfUeNgapId_t;
typedef unsigned long    RanUeNgapId_t;
#define INVALID_RANUENGAPID ULONG_MAX
typedef uint8_t     NgapProcedureCode_t;


/*typedef struct bstring {
    uint32_t    len;
    uint8_t *   data;
}bnstring_t;
typedef bnstring_t* AuthenticationParameterRand_t;
typedef bnstring_t* AuthenticationParameterAutn_t;*/

typedef struct hashAmf {
    uint8_t     iei;
    uint8_t     len;
    uint64_t    _hash;
}hashAmf_t;


typedef struct nasInputStream{
  uint8_t  *key;
  int    key_length;
  uint8_t  *count;
  uint8_t   bearer;
  uint8_t   direction;
  const uint8_t  *message;
  int    msglength;  

  nasInputStream()
  {
    key = NULL;
    
    count = NULL;
    bearer = 0;
    direction = 0;
    message = NULL;
    msglength = 0;
  }
} nasInputStream_t;

typedef struct NasSecurityAlgorithm {
    uint8_t encryptionAlgo:4;
    uint8_t integrityAlgo:4;
}NasSecurityAlgorithm_t;

typedef struct AllowedNssai {

}AllowedNssai_t;

typedef struct Guami_internal {
    PlmnId_t        plmnId;
    uint16_t        amfRegionId;
    uint8_t         amfSetId:4;
    uint8_t         amfPointer:4;
}Guami_t;

typedef struct SecurityKey_i {
    uint64_t            K_gnodeB;
}SecurityKey_i_t;

typedef struct NgapMessageType {
    NgapProcedureCode_t         procedureCode;
    NgapTypeofMessage_t         typeofMessage;
}NgapMessageType_t;



typedef enum timerUnit {
    E_10MINUTE      = 0b000,
    E_1HOUR         = 0b001,
    E_10HOUR        = 0b010,
    E_2SECOND       = 0b011,
    E_30SECOND      = 0b100,
    E_1MINUTE       = 0b101,
    E_320HOUR       = 0b110,
    E_DEACTIVATED   = 0b111
}timerUnit_t;






#define  _5GMM_CAUSE_IEI_ILLEGAL_UE                                  0b00000011
#define  _5GMM_CAUSE_IEI_PEI_NOT_ACCEPTED                            0b00000101
#define  _5GMM_CAUSE_IEI_ILLEGAL_ME                                  0b00000110
#define  _5GMM_CAUSE_IEI_5GS_SERVICES_NOT_ALLOWED                    0b00000111
#define  _5GMM_CAUSE_IEI_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETW   0b00001001
#define  _5GMM_CAUSE_IEI_IMPLICITLY_DE_REGISTERED                    0b00001010
#define  _5GMM_CAUSE_IEI_PLMN_NOT_ALLOWED                            0b00001011
#define  _5GMM_CAUSE_IEI_TRACKING_AREA_NOT_ALLOWED                   0b00001100
#define  _5GMM_CAUSE_IEI_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA   0b00001101
#define  _5GMM_CAUSE_IEI_NO_SUITABLE_CELLS_IN_TRACKING_AREA          0b00001111
#define  _5GMM_CAUSE_IEI_MAC_FAILURE                                 0b00010100
#define  _5GMM_CAUSE_IEI_SYNCH_FAILURE                               0b00010101
#define  _5GMM_CAUSE_IEI_CONGESTION                                  0b00010110
#define  _5GMM_CAUSE_IEI_UE_SECURITY_CAPABILITIES_MISMATCH           0b00010111
#define  _5GMM_CAUSE_IEI_SECURITY_MODE_REJECTED_UNSPECIFIED          0b00011000
#define  _5GMM_CAUSE_IEI_NON_5G_AUTH_UNACCEPTABLE                    0b00011010
#define  _5GMM_CAUSE_IEI_N1_MODE_NOT_ALLOWED                         0b00011011
#define  _5GMM_CAUSE_IEI_RESTRICTED_SERVICE_AREA                     0b00011100
#define  _5GMM_CAUSE_IEI_LADN_NOT_AVAILABLE                          0b00101011
#define  _5GMM_CAUSE_IEI_MAX_NUM_OF_PDU_SESSIONS_REACHED             0b01000001
#define  _5GMM_CAUSE_IEI_INSUFFI_RESFOR_SPECIFIC_SLICE_AND_DNN       0b01000011
#define  _5GMM_CAUSE_IEI_INSUFFI_RESFOR_SPECIFIC_SLICE               0b01000101
#define  _5GMM_CAUSE_IEI_NGKSI_ALREADY_IN_USE                        0b01000111
#define  _5GMM_CAUSE_IEI_NON_3GPP_ACCESS_TO_5GCN_NOT_ALLOWED         0b01001000
#define  _5GMM_CAUSE_IEI_SERVING_NETWORK_NOT_AUTHORIZED              0b01001001
#define  _5GMM_CAUSE_IEI_PAYLOAD_WAS_NOT_FORWARDED                   0b01011010
#define  _5GMM_CAUSE_IEI_DNN_NOT_SUPPORTED                           0b01011011
#define  _5GMM_CAUSE_IEI_INSUFFICIENT_USER_PLANE_RESOURCES           0b01011100
#define  _5GMM_CAUSE_IEI_SEMANTICALLY_INCORRECT_MESSAGE              0b01011111
#define  _5GMM_CAUSE_IEI_INVALID_MANDATORY_INFORMATION               0b01100000
#define  _5GMM_CAUSE_IEI_MESSAGE_TYPE_NON_EXISTENT                   0b01100001
#define  _5GMM_CAUSE_IEI_MESSAGE_TYPE_NOT_COMPATIBLE                 0b01100010
#define  _5GMM_CAUSE_IEI_INFORMATION_ELEMENT_NON_EXISTENT            0b01100011
#define  _5GMM_CAUSE_IEI_CONDITIONAL_IE_ERROR                        0b01100100
#define  _5GMM_CAUSE_IEI_MESSAGE_NOT_COMPATIBLE                      0b01100101
#define  _5GMM_CAUSE_IEI_PROTOCOL_ERROR_UNSPECIFIED                  0b01101111



enum {
    E_NAS_ERROR_UNKNOWN             =  1,    

    E_NAS_ERROR_INVALID_INPUT       =  2,
    E_NAS_ERROR_BUFF_INSUFF         =  3,
    E_NAS_ERROR_BUFF_INCOMPLETE     =  4,
    E_NAS_ERROR_SECCTX_NULL         =  5,
    E_NAS_ERROR_INVALID_PDU         =  6,
    E_NAS_ERROR_OLD_SEQ_NUM         =  7,
    E_NAS_ERROR_NOT_SEC_MODE_CMD    =  8,
    E_NAS_ERROR_MAC_MISMATCH        =  9,
    E_NAS_ERROR_DECRYPT_FAIL        = 10,
    E_NAS_ERROR_ENCRYPT_FAIL        = 11,
    E_NAS_ERROR_INTEG_ALGO_FAIL     = 12,
    E_NAS_ERROR_CONSTR_CHECK_FAIL   = 13,
    E_NAS_ERROR_SEC_CAP_MISMATCH    = 14,
    E_NAS_ERROR_INVALID_PDUADDR_TYPE= 15,
    E_NAS_SUBSCRIPTION_ID_NOT_FOUND = 16

};

#endif 
