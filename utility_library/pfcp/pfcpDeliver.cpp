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
 * Contains functions to start a timer, stop it and the function to handle
 * retransmissions.
 *
 * NF                   PFCP layer
 * encode()             starts a timer to retransmit the message if necesary.
 *
 *  --  response arrives  --
 *
 * decode()             timer disarmed.
 *
 *  --  timer expires before the response arrives
 *                      retransmit()
 *
 *
 */
#include "pfcpDeliver.h"

pfcpRelInstance_t *gRelInst = NULL;	/* a global var to access CB*/

/* TODO: Q below implies Pfcp sequence number is per NF(SMF, UPF)
 *       It is better if it is per UDP connection:
 *       local Ip, local port and peer Ip (peer server port is fixed) */
/* NOTE: Should be called only once at start in a single thread */
int initPfcp()
{
    pfcpLogENTRY;

    std::lock_guard<std::mutex> guard(pfcpDlvMutex);

    if(!pfcpSeqNumQ.empty()) {
        pfcpLog("pfcp sequence number Q has already been initialized");
        pfcpLogEXIT;
        return SUCCESS;
    }

    for(uint32_t i=1; i< 10000; i++) {
        pfcpSeqNumQ.push(i);
    }

    pfcpSeqNumMap.clear();

    if(setupTimerFd()==FAILURE)                                                 
    {                                                                           
       higLog("Error in Setting up Timer");
       pfcpLogEXIT;
       return FAILURE;
    } 

    /* register on expiry call back function */
    gRelInst = new struct pfcpRelInstance;

    pfcpLogEXIT;
    return SUCCESS;
}

/**
 * @brief Method for registering the callbacks
 *
 * @param relInst
 */
void registerCallback(pfcpRelInstance_t relInst) {
    gRelInst->pfcpTimeoutCB = relInst.pfcpTimeoutCB;
    gRelInst->pfcpRespCB = relInst.pfcpRespCB;
}

/* NOTE: if sequence number Q has to be maintained per SEID
 *       then it makes sense to implement get put functions in NF itself
 *       as the Q will be also maintained per SEID in NF */
int getSeqNum(uint32_t &seqN)
{
    std::lock_guard<std::mutex> guard(pfcpDlvMutex);

    if(pfcpSeqNumQ.empty()) {
        higLog("pfcp sequence numbers exhausted");
        pfcpLogEXIT;
        return FAILURE;
    }

    seqN = pfcpSeqNumQ.front();
    pfcpSeqNumQ.pop();

    if(pfcpSeqNumMap.find(seqN) != pfcpSeqNumMap.end()) {
        higLog("ERROR: seqN %llu already under use. Popped value conflicts", seqN);
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpSeqNumMap[seqN] = true;

    pfcpLogEXIT;
    return SUCCESS;
}

int putSeqNum(uint32_t seqN)
{
    std::lock_guard<std::mutex> guard(pfcpDlvMutex);

    if(pfcpSeqNumMap.find(seqN) == pfcpSeqNumMap.end()) {
        higLog("ERROR: stray seqN %llu being pushed", seqN);
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpSeqNumMap.erase(seqN);
    pfcpSeqNumQ.push(seqN);
    pfcpDlvMap.erase(seqN);
    lowLog("pfcp sequence Number %d replenished into queue", seqN);

    pfcpLogEXIT;
    return SUCCESS;
}

int getMessageType(uint32_t seqN) {
    LOG_ENTRY;
    if(pfcpDlvMap.find(seqN) == pfcpDlvMap.end()) {
        pfcpLogEXIT;
        return 0;
    }
    LOG_EXIT;
    return pfcpDlvMap[seqN].msgType;
}

int sendPfcpMsg(uint32_t seid, uint id, pfcpMessage_t pfcpMsg,
     std::string peerIpAddr, uint32_t seqN)
{
    pfcpLogENTRY;

    if(!seqN) {
        higLog("pfcp sequence number is invalid");
        pfcpLogEXIT;
        return FAILURE;
    }

    uint8_t buffer[MAX_MSG_LEN] = {};
    uint16_t encodedLen = 0;
    if(pfcpEncode(buffer, MAX_MSG_LEN, &pfcpMsg, &encodedLen) == FAILURE){
        higLog("PFCP encode failed");
        LOG_EXIT;
        return FAILURE;
    }
    printArray("buffer",  buffer, encodedLen);

    struct sockaddr_in sin = {};
    if(getSockAddr(peerIpAddr, &sin) == FAILURE){
        higLog("getSockAddr failed");
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpDlvWS_t work = {};
    work.seid = seid;
    memcpy(work.buffer, buffer, MAX_MSG_LEN);
    work.encodedLen = encodedLen;
    work.pfcpClientId = id;
    work.peerIpAddr = peerIpAddr;
    work.sin = sin;
    work.msgType = pfcpMsg.header.MessageType;

    midLog("Sending request to message type : %d", work.msgType);
    pfcpDlvMutex.lock();
    pfcpDlvMap[seqN] = work;
    pfcpDlvMutex.unlock();
    
    /* start Timer */
    timerInfo relTimer = {};
    relTimer.Id = seqN;
    relTimer.duration = TIMER_PFCP_REL_DURATION;
    relTimer.retries = TIMER_PFCP_REL_RETRIES;
    relTimer.timeoutFunction = pfcpRelTimeOutFunction;
    startTimer(relTimer);  

    if(platformSendToPeer(id, (char*)buffer, encodedLen,
            (struct sockaddr *)&sin, 0) < 0) {
        higLog("platformSendToPeer failed seid %llu", seid);
        pfcpLogEXIT;
        return FAILURE;
    }
    pfcpLogEXIT;
    return SUCCESS;
}

/**
 * @brief Function to decode the recieved PFCP message and calls the 
 * corresponding function to handle the message in NF.
 *
 * @param msgBlob  message recieved
 * @param len   length of message
 * @param Id
 * @param peerToken
 */
void recievePfcpMsg(void *msgBlob, int len, uint Id, uint16_t peerToken)
{
    /* Decode PFCP messages and call a function within smf */
    LOG_ENTRY;

    pfcpMessage_t pfcpMsg = {};

    if(pfcpDecode((uint8_t*)msgBlob, len, &pfcpMsg) == FAILURE){
        higLog("PFCP Decode Failed");
        platformClrPeerTkn(peerToken);
        free(msgBlob);
        LOG_EXIT;
        return;
    }
    free(msgBlob);
    uint8_t msgType = pfcpMsg.header.MessageType;
    uint32_t seqN = pfcpMsg.header.SequenceNumber;

    pfcpDlvMutex.lock();
    if(!peerToken && (pfcpSeqNumMap.find(seqN) == pfcpSeqNumMap.end())) {
        pfcpDlvMutex.unlock();
        higLog("ERROR: seqN %llu absent. Rcvd resp from peer", seqN);
        pfcpLogEXIT;
        return;
    }

    pfcpDlvMutex.unlock();

    /* peerToken is 0 when CLIENT. Stop timer is applicable when CLIENT.*/
    if (peerToken == 0)
        stopTimer(seqN);

    gRelInst->pfcpRespCB(&pfcpMsg, Id, peerToken);

    if (peerToken == 0)
        putSeqNum(seqN);
    LOG_EXIT;
}

/**
 * @brief Function to handle timeout of pfcp reliability timer
 *
 * @param relTimer
 */
void pfcpRelTimeOutFunction(timerInfo relTimer) {
    LOG_ENTRY;
    relTimer.retries += -1;
    uint32_t seqNo = relTimer.Id;
    
    pfcpDlvMutex.lock();
    std::map<uint32_t, pfcpDlvWS_t>::iterator itr =
                                        pfcpDlvMap.find(seqNo);
    if(itr == pfcpDlvMap.end()) {
        pfcpDlvMutex.unlock();
        higLog("Unknown sequence Number, %lu", seqNo);
        LOG_EXIT;
        return;
    }
    
    pfcpDlvWS_t work = itr->second;
    pfcpDlvMutex.unlock();
    if (relTimer.retries <= 0) {
       /* CB to tell AMF */
       higLog("PFCP_REL timer retries exhausted for seqNo %d and SEID %d",
               seqNo, work.seid);
       gRelInst->pfcpTimeoutCB(seqNo, work.seid);
       pfcpLogEXIT;
       return;
    }
    
    higLog("PFCP_REL timer timeout for seqNo %d and SEID %d, timer attempts remaining: %d"
            ,seqNo, work.seid, relTimer.retries);

    platformSendToPeer(work.pfcpClientId, (char*)work.buffer,
     work.encodedLen, (struct sockaddr *)&work.sin, 0);
    
    startTimer(relTimer);

    LOG_EXIT;
    return;
}
int getSockAddr(std::string ipAddr, sockaddr_in *sin){

    LOG_ENTRY;

    sin->sin_family = AF_INET;
    sin->sin_port = htons(PFCP_SERVER_PORT);
    if(inet_pton(AF_INET, ipAddr.c_str(), (void *)&sin->sin_addr.s_addr) <= 0)
    {
        higLog("inet_pton - Conversion from string to IP Address failed");
        LOG_EXIT;
        return FAILURE;
    }

    LOG_EXIT;
    return SUCCESS;

    /* TODO: Support for IPv6 */

}

void pfcpStopTimer(uint32_t seqN) {
    pfcpLogENTRY;
    stopTimer(seqN);
    if (delFromDlWSMap(seqN) == FAILURE) {
        higLog("delFromPfcpDlvWSMap Failed");
    }
    pfcpLogEXIT;
    return;
}
/**
 * @brief Function to remove the pfcpDlvWS entry corresponding to the
 *        seqN 
 *
 * @param seqN
 *
 * @return 
 */
int delFromDlWSMap(uint32_t seqN)
{
	pfcpLogENTRY;
    std::lock_guard<std::mutex> guard(pfcpDlvMutex);

    std::map<uint32_t, pfcpDlvWS_t>::iterator itr =
                                    pfcpDlvMap.find(seqN);
    if(itr == pfcpDlvMap.end()) {
        higLog("seqNo %lu not found in pfcpDlWSMap", seqN);
        pfcpLogEXIT;
        return FAILURE;
    }

	pfcpDlvMap.erase(seqN);
    lowLog("Deleting from pfcpDlvWS Map entry for seqNo %d", seqN);
    pfcpLogEXIT;
    return FAILURE;
}
