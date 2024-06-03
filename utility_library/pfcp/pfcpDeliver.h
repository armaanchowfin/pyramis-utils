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

#ifndef __PFCP_DELIVER_H
#define __PFCP_DELIVER_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "pfcpMessage.h"
#include "platform/include/platform.h"
#include "pfcp/include/pfcpCommon.h"
#include "pfcp/include/pfcpAttributes.h"
#include "common/include/utils.h"
#include "pfcp/include/pfcpLogging.h"



using namespace std;

#define TIMER_PFCP_REL_DURATION     3
#define TIMER_PFCP_REL_RETRIES      3

static std::queue<uint32_t> pfcpSeqNumQ = {};
static std::mutex pfcpDlvMutex;

static std::map<uint32_t, bool>pfcpSeqNumMap = {};

typedef struct pfcpDlvWS {
    uint32_t seid;
    uint8_t buffer[MAX_MSG_LEN];
    uint16_t encodedLen;
    uint pfcpClientId;
    std::string peerIpAddr;
    struct sockaddr_in sin;
    int msgType;

    pfcpDlvWS() {
        seid = 0;
        memset(buffer, 0, MAX_MSG_LEN);
        encodedLen = 0;
        pfcpClientId = 0;
        peerIpAddr = "";
        sin = {};
        msgType = 0;
    }
} pfcpDlvWS_t;

static std::map<uint32_t, pfcpDlvWS_t> pfcpDlvMap = {};

typedef struct pfcpRelInstance {
    void (*pfcpTimeoutCB)(uint32_t seqN, uint32_t smContextId);
    void (*pfcpRespCB)(pfcpMessage_t * msg, uint Id, uint16_t streamId);

} pfcpRelInstance_t;




int initPfcp();
int getSeqNum(uint32_t &seqN);
int putSeqNum(uint32_t seqN);
int getMessageType(uint32_t seqN);
int sendPfcpMsg(uint32_t seid, uint id, pfcpMessage_t pfcpMsg,
     std::string peerIpAddr, uint32_t seqN);
void recievePfcpMsg(void *msgBlob, int len, uint Id, uint16_t peerToken);
int getSockAddr(std::string ipAddr, sockaddr_in *sin);
int delFromDlWSMap(uint32_t seqN);
void pfcpRelTimeOutFunction(timerInfo relTimer);
void registerCallback(pfcpRelInstance_t relInst);
void pfcpStopTimer(uint32_t seqN);
#endif 

