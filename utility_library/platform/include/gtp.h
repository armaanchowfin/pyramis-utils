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

#ifndef  __GTP_H
#define  __GTP_H

#include <bits/stdc++.h>
#include <netinet/in.h>
#include <map>
#include "platform/include/logging.h"
#include "common/include/datatypes.h"
#include "platform/include/platform.h"
#include "common/include/utils.h"

#define GTP_V1_MASK  0x20
#define PT_MASK      0x10
#define GTP_E_MASK   0x04
#define GTP_S_MASK   0x02
#define GTP_PN_MASK  0x01

#define GTP_PORT 2152
#define UL_PDU_SESSION_INFO 1
#define DL_PDU_SESSION_INFO 0
#define EXT_HDR_OFFSET 2


#define GTP_ECHO_REQUEST 1
#define GTP_ECHO_RESPONSE 2
#define GTP_ERROR_INDICATION 26
#define GTP_END_MARKER 254
#define G_PDU 255

#define GTP_HDR_SEQ_NUM_LENGTH 2
#define GTP_HDR_NPDU_NUM_LENGTH 1
#define GTP_HDR_NXT_EXTN_HDR_FIELD_LENGTH 1
#define GTP_HDR_MANDATORY_FIELD_LENGTH 8 
#define GTP_HDR_OPTIONAL_FIELD_LENGTH 4

using namespace std;
using std::map;

#define MAX_NUM_OF_EXT_HDR_TYPES   8

typedef enum extHdrType {
    E_GTP_EXT_HDR_NO_MORE_EXTENSION_HEADERS =  0b00000000,
    E_GTP_EXT_HDR_SERVICE_CLASS_INDICATOR   =  0b00100000,
    E_GTP_EXT_HDR_UDP_PORT                  =  0b01000000,
    E_GTP_EXT_HDR_RAN_CONTAINER             =  0b10000001,
    E_GTP_EXT_HDR_LONG_PDCP_PDU_NUMBER      =  0b10000010,
    E_GTP_EXT_HDR_XW_RAN_CONTAINER          =  0b10000011,
    E_GTP_EXT_HDR_NR_RAN_CONTAINER          =  0b10000100,
    E_GTP_EXT_HDR_PDU_SESSION_CONTAINER     =  0b10000101,
    E_GTP_EXT_HDR_PDCP_PDU_NUMBER           =  0b11000000
} extHdrType_t;

static std::map<extHdrType_t, std::string> extHdrName
{
    {E_GTP_EXT_HDR_PDU_SESSION_CONTAINER,   "PDU_SESSION_CONTAINER: "},
    {E_GTP_EXT_HDR_SERVICE_CLASS_INDICATOR, "SERVICE_CLASS_INDICATOR: "},
    {E_GTP_EXT_HDR_UDP_PORT,                "UDP_PORT: "},
    {E_GTP_EXT_HDR_RAN_CONTAINER,           "RAN_CONTAINER: "},
    {E_GTP_EXT_HDR_LONG_PDCP_PDU_NUMBER,    "LONG_PDCP_PDU_NUMBER: "},
    {E_GTP_EXT_HDR_XW_RAN_CONTAINER,        "XW_RAN_CONTAINER: "},
    {E_GTP_EXT_HDR_NR_RAN_CONTAINER,        "NR_RAN_CONTAINER: "},
    {E_GTP_EXT_HDR_PDCP_PDU_NUMBER,         "PDCP_PDU_NUMBER: "}
};

const static std::vector<extHdrType_t> extHdrList
{
    E_GTP_EXT_HDR_PDU_SESSION_CONTAINER,
    E_GTP_EXT_HDR_SERVICE_CLASS_INDICATOR,
    E_GTP_EXT_HDR_UDP_PORT,
    E_GTP_EXT_HDR_RAN_CONTAINER,
    E_GTP_EXT_HDR_LONG_PDCP_PDU_NUMBER,
    E_GTP_EXT_HDR_XW_RAN_CONTAINER,
    E_GTP_EXT_HDR_NR_RAN_CONTAINER,
    E_GTP_EXT_HDR_PDCP_PDU_NUMBER
};

typedef struct extHdrContent {
    uint32_t len;
    uint8_t content[MAX_MESSAGE_SIZE];
}extHdrContent_t;

typedef struct gtpDLPduSess
{   
    uint8_t:4; 
    uint8_t pduType:4;                                                         
    uint8_t qfi:6;
    uint8_t rqi:1;
    uint8_t ppp:1;
    uint8_t:4; 
    uint8_t ppi:3;
    
}gtpDLPduSess_t;

typedef struct gtpULPduSess                                                    
{   
    uint8_t:4;
    uint8_t pduType:4;                                                         
    uint8_t qfi:6;
    uint8_t:4; 

}gtpULPduSess_t;

typedef struct gtpPduSess{

    union {
        gtpDLPduSess_t ulContainer;
        gtpULPduSess_t dlContainer;
    };

}gtpPduSess_t;

typedef struct gtpHeader {

    

    /*
     * 0                   1                   2                   3
     * 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * | Ver |P|R|E|S|N| Message Type|               Length            |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |               Tunnel Endpoint Identifier                      |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |      Sequence Number        |   N-PDU Number  | Next-Ext-Hdr  |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     *
     *  o Ver: Version field (Set to ’1’)
     *  o P: Protocol Type (Set to ’1’)
     *  o R: Reserved bit (Set to ’0’)
     *  o E: Extension Header Flag (Set to ’1’ if extension header exists)
     *  o S: Sequence Number Flag (Set to ’1’ if sequence number exists)
     *  o N: N-PDU Number Flag (Set to ’1’ if N-PDU number exists)
     *  o Message Type: Indicates the type of GTP-U message
     *  o Length: Indicates the length in octets of the payload
     *  o Tunnel Endpoint Identifier (TEID)
     *  o Sequence Number: Indicates increasing sequence number for T-PDUs
     *       is transmitted via GTP-U tunnels
     *  o N-PDU Number: It is used only for inter SGSN, 2G-3G handover case,
     *       etc.
     *  o Next-Ext-Hdr: Indicates following extension header type
     *
     *
     * 0                   1                   2                   3
     * 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * | Ext-Hdr Length|                                               |
     * +-+-+-+-+-+-+-+-+                                               |
     * |                  Extension Header Content                     |
     * .                                                               .
     * .                                               +-+-+-+-+-+-+-+-+
     * |                                                | Next-Ext-Hdr |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     *
     * o No more extension headers (Next-Ext-Hdr = 00000000)
     * o Service Class Indicator (Next-Ext-Hdr = 00100000)
     * o UDP Port (Next-Ext-Hdr = 01000000)
     * o RAN Container (Next-Ext-Hdr = 10000001)
     * o Long PDCP PDU Number (Next-Ext-Hdr = 10000010)
     * o Xw RAN Container (Next-Ext-Hdr = 10000011)
     * o NR RAN Container (Next-Ext-Hdr = 10000100)
     * o PDU Session Container (Next-Ext-Hdr = 10000101)
     * o PDCP PDU Number (Next-Ext-Hdr = 11000000)
     */

    uint8_t flags;
    uint8_t msgType;
    uint16_t length; 
    uint32_t teid;

    

    uint16_t seqNo;
    uint8_t npduNum;
    uint8_t nxtExtnHdrType;
    extHdrContent_t extHdrs[MAX_NUM_OF_EXT_HDR_TYPES];

    

    gtpPduSess_t pduSessContainer;

    uint32_t gtpHdrLen;

    gtpHeader() {
        memset(this, 0, sizeof(*this));
    }
}gtpHeader_t;

typedef struct gtpMessage
{
    gtpHeader_t gtp_header;
    uint32_t payloadLength;
    uint8_t payload[MAX_MESSAGE_SIZE];

}gtpMessage_t;

typedef struct gtpPayload 
{
    uint32_t len;
    uint8_t buffer[MAX_MESSAGE_SIZE];

}gtpPayload_t;

typedef struct nxtExtHdrOps {
    int (*encode) (gtpHeader_t *);
    int (*decode) (gtpHeader_t *);
}nxtExtHdrOps_t;


int encodeGtpHeader(uint8_t *buffer, gtpHeader_t *msgStruct, uint32_t bufLen,
                            uint32_t *encodedLen);

int decodeGtpHeader(uint8_t *buffer, gtpHeader_t *msgStruct, uint32_t bufLen,
                            uint32_t *decodedLen);

int encodeGtpMessage(uint8_t *buffer, uint32_t bufLen, gtpMessage_t *msgStruct,
                     uint32_t *encodedLen);

int decodeGtpMessage(uint8_t *buffer,gtpMessage_t *msgStruct,uint32_t bufLen);

int pduSessionContainerEncode(gtpHeader_t *msgStruct);

int pduSessionContainerDecode(gtpHeader_t *msgStruct);

static std::map<extHdrType_t, nxtExtHdrOps_t> nxtExtHdrOperations =
{
    { E_GTP_EXT_HDR_PDU_SESSION_CONTAINER,
        { pduSessionContainerEncode, pduSessionContainerDecode }
    },
    
};

#endif  

