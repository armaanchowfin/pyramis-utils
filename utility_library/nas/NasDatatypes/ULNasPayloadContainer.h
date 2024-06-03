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

#ifndef __UL_NAS_PAYLOAD_CONTAINER
#define __UL_NAS_PAYLOAD_CONTAINER

#include <cstdint>
#include <cstdlib>
#include "common/include/aka.h"
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/PDUSessionId2.h"
#include "nas/NasDatatypes/requestType.h"
#include "nas/NasDatatypes/DNN.h"



#define MAX_PAYLOAD_LEN 1000
#define MIN_PAYLOAD_LEN 3
#define PAYLOAD_PARAM_OFFSET 0



#define MAX_PAYLOAD_ENTRIES   10
#define MAX_PAYLOAD_OPT_IES   15

#define NUM_PAYLOADS_SIZE      1   

#define PAYLOAD_ENTRY_OFFSET   2  /* entryLen is 1 octet, plus
                                   * numOfIEs and containerType share a octet*/

#define PAYLOAD_OPT_IEI_LEN_SIZE 2  




#define   PAYLOAD_PDU_SESSION_ID            0x12
#define   PAYLOAD_REQUEST_TYPE               0x8  

#define   PAYLOAD_DNN                       0x25

#if 0 

#define   PAYLOAD_5GMM_CAUSE                0x58
#define   PAYLOAD_ADDITIONAL_INFORMATION    0x24
#define   PAYLOAD_BACK_OFF_TIMER_VALUE      0x37
#define   PAYLOAD_OLD_PDU_SESSION_ID        0x59
#define   PAYLOAD_S_NSSAI                   0x22
#endif

typedef struct payloadOptIE {
    uint8_t IEI;
    union {
        PDUSessionId2_t     pduSessionId;
        requestType_t       requestType;
        DNN_t               dnn;
    }u;
} payloadOptIE_t;

typedef struct payloadEntry {
    uint8_t numOfIEs:4;
    uint8_t containerType:4;
    payloadOptIE_t optIEs[MAX_PAYLOAD_OPT_IES];
    uint8_t contentLen;
    uint8_t content[MAX_PAYLOAD_LEN];
} payloadEntry_t;

typedef struct multiplePayloads {
    int numberOfEntries;
    payloadEntry_t entry[MAX_PAYLOAD_ENTRIES];
} multiplePayloads_t;


typedef struct PayloadContainer {
	uint16_t   Length;
    uint8_t    content[MAX_PAYLOAD_LEN];
}PayloadContainer_t;

int decodePayloadContainer(uint8_t *buffer, PayloadContainer_t *pc,
                        uint32_t bufLen, uint32_t* decodedLen);
int encodePayloadContainer(uint8_t *buffer, uint32_t buflen,
                        PayloadContainer_t *pc, uint32_t *encodedLen);

int decodeMultiPayloadContainer(PayloadContainer_t *pc, multiplePayloads_t *mp);
int encodeMultiPayloadContainer(multiplePayloads_t *mp, PayloadContainer_t *pc);

/* TODO: Add seperate encode/decode functions for N1SM Info and
 *       SOR transparent container. */

#endif 
