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

#include "pfcp/pfcpDeliver.h"
#include "pfcpHelper.h"


/* Retrieve PFCP IE from PFCP message based on IE type */
int getPfcpIE(uint16_t ieType, pfcpMessage_t *pfcpMsg, pfcpIE_t *pfcpIE){
    
    pfcpLogENTRY;

    for(pfcpIE_t pIE : pfcpMsg->body){
        if(pIE.IEI == ieType){
            *pfcpIE = pIE;
            pfcpLogEXIT;
            return SUCCESS;
        }
    }
    pfcpLogEXIT;
    return FAILURE;
}

/* Retrieve PFCP IE from a list of PFCP IEs based on IE type */
int getPfcpIEFrmList(uint16_t ieType, std::vector<pfcpIE_t> ieList, pfcpIE_t *
                                                                    pfcpIE) {
    pfcpLogENTRY;

    for(pfcpIE_t pIE : ieList){

        if(pIE.IEI == ieType) {
            *pfcpIE = pIE;
            pfcpLogEXIT;
            return SUCCESS;          
        }
    }
    pfcpLogEXIT;
    return FAILURE;
}

/* Function to get a PFCP IE list from PFCP message. For example createFAR_ANY_t or createPDR_ANY_t is again a list of PFCP IEs of create PDR or create FAR type. This function can be used to retrieve all create PDRs or create FARs so that we can loop over multiple PDRs or FARs. This function should not be used to update existing IE list. So ensure ieList is empty before calling this function */
int getPfcpIEList(uint16_t ieType, std::vector<pfcpIE_t> inIEList, 
                                            std::vector<pfcpIE_t> &outIEList) {
    pfcpLogENTRY;
    int itr = 0;
    
    for(pfcpIE_t pIE : inIEList){
        if(pIE.IEI == ieType){
            outIEList.push_back(pIE);
            itr++;
        }
    }
    if(outIEList.size()==0){
        higLog("PFCP IE missing");
        pfcpLogEXIT;
        return FAILURE;
    } 
    pfcpLogEXIT;
    return SUCCESS;
}

/* Convert IPv4 and IPv6 addresses from text to binary form and fill the Node Id struct */ 
int getNodeIdFrmKeyStr(string keyStr, nodeId_t *nId, uint8_t nIdType){

    pfcpLogENTRY;

    int addrFamily;
    void *ipAddr;

    nId->IEI = PFCP_IE_NODE_ID;
    nId->nodeIdType = nIdType;

    if(nIdType == PFCP_NODE_ID_TYPE_IPV4){
        addrFamily = AF_INET;
        ipAddr = (void*)nId->u.ipv4Address;
    }
    else if(nIdType == PFCP_NODE_ID_TYPE_IPV6){
        addrFamily = AF_INET6;
        ipAddr = (void*)nId->u.ipv6Address;
    }

    if(inet_pton(addrFamily, keyStr.c_str(), ipAddr) <= 0){
        higLog("inet_pton - Conversion from string to IP Address failed %s",
                                                            strerror(errno));
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLogEXIT;
    return SUCCESS;
    
}

/* Convert IPv4 and IPv6 addresses from binary to text form */
int getKeyStrFrmNodeId(nodeId_t *nodeId, string &keyStr){

    pfcpLogENTRY;
    int addrFamily;
    socklen_t addrSize;
    void *ipAddr;

    if(nodeId->nodeIdType == PFCP_NODE_ID_TYPE_IPV4){
        
        addrFamily = AF_INET;
        addrSize = INET_ADDRSTRLEN;
        ipAddr = (void*)nodeId->u.ipv4Address;
    }
    else if(nodeId->nodeIdType == PFCP_NODE_ID_TYPE_IPV6){
        
        addrFamily = AF_INET6;
        addrSize = INET6_ADDRSTRLEN;
        ipAddr = (void*)nodeId->u.ipv6Address;
    }
    
    char *str_ipAddr = (char*)calloc(addrSize, sizeof(char));

    if(inet_ntop(addrFamily, ipAddr, str_ipAddr, addrSize) == NULL){
        higLog("inet_ntop - Conversion to string failed %s", strerror(errno));
        free(str_ipAddr);
        pfcpLogEXIT;
        return FAILURE;
    }
    keyStr = str_ipAddr;
    free(str_ipAddr);
    pfcpLogEXIT;
    return SUCCESS; 
}

/* Convert IPv4 from binary(uint32_t) to text form */
int getIpStrFrmIpv4(uint8_t* ipAddr, string &keyStr){

    pfcpLogENTRY;
    
    char *str_ipAddr = (char*)calloc(INET_ADDRSTRLEN, sizeof(char));

    if(inet_ntop(AF_INET, (void*)ipAddr, str_ipAddr, INET_ADDRSTRLEN) == NULL){
        higLog("inet_ntop - Conversion to string failed %s", strerror(errno));
        free(str_ipAddr);
        pfcpLogEXIT;
        return FAILURE;
    }
    keyStr = str_ipAddr;
    free(str_ipAddr);
    pfcpLogEXIT;
    return SUCCESS; 
}

/* Construct IP Address from string */
int getIPAddrFrmStr(string ipAddrStr, uint8_t *ipAddr, uint8_t ipType){

    pfcpLogENTRY;

    int addrFamily;

    if(ipType == PFCP_IP_TYPE_IPV4){
        addrFamily = AF_INET;
    }
    else if(ipType == PFCP_IP_TYPE_IPV6){
        addrFamily = AF_INET6;
    }

    if(inet_pton(addrFamily, ipAddrStr.c_str(), (void *)ipAddr) <= 0){
        higLog("inet_pton - Conversion from string to IP Address failed %s", 
                                                            strerror(errno));
        pfcpLogEXIT;
        return FAILURE;
    }

    pfcpLogEXIT;
    return SUCCESS; 

}

int getIPAddrFrmFseid(pfcpFSEID_t fseid, string &strIPAddr){

    pfcpLogENTRY;
    int addrFamily;
    socklen_t addrSize;
    void *ipAddr;

    if(fseid.v4v6 == PFCP_FSEID_IPV4_ONLY){
        addrFamily = AF_INET;
        addrSize = INET_ADDRSTRLEN;
        ipAddr = (void*)fseid.ipv4Address;
    }
    else if(fseid.v4v6 == PFCP_FSEID_IPV6_ONLY){
        addrFamily = AF_INET6;
        addrSize = INET6_ADDRSTRLEN;
        ipAddr = (void*)fseid.ipv6Address;
    }
    else if(fseid.v4v6 == PFCP_FSEID_IPV4_IPV6){
        /* TODO */
    }
    char *str_ipAddr = (char*)calloc(addrSize, sizeof(char));

    if(inet_ntop(addrFamily, ipAddr, str_ipAddr, addrSize) == NULL){
        higLog("inet_ntop - Conversion to string failed %s", strerror(errno));
        free(str_ipAddr);
        pfcpLogEXIT;
        return FAILURE;
    }
    strIPAddr = str_ipAddr;
    free(str_ipAddr);
    pfcpLogEXIT;
    return SUCCESS; 
}

pfcpIE_t formPdrIdIE(uint16_t ruleId) {

    pfcpIE_t pdrId_IE = {};
    pdrId_IE.IEI = PFCP_IE_PACKET_DETECTION_RULE_ID;
    pdrId_IE.pdrId.ruleId = ruleId;
    return pdrId_IE;
}

pfcpIE_t formPrecedenceIE(uint32_t precedence) {
    pfcpIE_t prec_IE = {};
    prec_IE.IEI = PFCP_IE_PRECEDENCE;
    prec_IE.precedence.precedence = precedence;
    return prec_IE;
}

pfcpIE_t formDestIfaceIE(uint8_t interfaceValue) {
    pfcpIE_t dstIface_IE = {};
    dstIface_IE.IEI = PFCP_IE_DESTINATION_INTERFACE;
    dstIface_IE.destIface.interfaceValue = interfaceValue;
    return dstIface_IE;
}

pfcpIE_t formSrcIfaceIE(uint8_t interfaceValue) {
    pfcpIE_t srcIface_IE = {};
    srcIface_IE.IEI = PFCP_IE_SOURCE_INTERFACE;
    srcIface_IE.srcIface.interfaceValue = interfaceValue;
    return srcIface_IE;
}

pfcpIE_t formUeIpv4AddrIE(uint32_t ueIpAddr, uint8_t SrcOrDest) {
    pfcpIE_t ipAddr_IE = {};
    ipAddr_IE.IEI = PFCP_IE_UE_IP_ADDRESS;
    ipAddr_IE.ueIpAddr.V4 = 1;
    memcpy(ipAddr_IE.ueIpAddr.ipv4Address, &ueIpAddr,
                PFCP_IPV4_ADDR_LEN);
    ipAddr_IE.ueIpAddr.SrcOrDest = SrcOrDest;
    return ipAddr_IE;
}

pfcpIE_t formFteidIE(fteid_t tunnelInfo) {
    pfcpIE_t fteid_IE = {};
    fteid_IE.IEI = PFCP_IE_F_TEID;
    fteid_IE.fteid.CHID = tunnelInfo.CHID;
    fteid_IE.fteid.CH = tunnelInfo.CH;
    fteid_IE.fteid.V6 = tunnelInfo.V6;
    fteid_IE.fteid.V4 = tunnelInfo.V4;
    fteid_IE.fteid.TEID = tunnelInfo.TEID;
    memcpy(fteid_IE.fteid.ipv4Address, tunnelInfo.ipv4Address,
                PFCP_IPV4_ADDR_LEN);
    memcpy(fteid_IE.fteid.ipv6Address, tunnelInfo.ipv6Address,
                PFCP_IPV6_ADDR_LEN);
    fteid_IE.fteid.chooseId = tunnelInfo.chooseId;

    return fteid_IE;
}

pfcpIE_t formFarIdIE(uint32_t farIdValue) {

    pfcpIE_t farID_IE = {};
    farID_IE.IEI = PFCP_IE_FAR_ID;
    farID_IE.farId.value = farIdValue;
    return farID_IE;
}

pfcpIE_t formQerIdIE(uint32_t qerIdValue) {
    pfcpIE_t qerId_IE = {};
    qerId_IE.IEI = PFCP_IE_QER_ID;
    qerId_IE.qerId.value = qerIdValue;
    return qerId_IE;
}

pfcpIE_t formBarIdIE(uint32_t barIdValue) {
    pfcpIE_t barId_IE = {};
    barId_IE.IEI = PFCP_IE_BAR_ID;
    barId_IE.barId.value = barIdValue;
    return barId_IE;
}

pfcpIE_t formApacBuffNocpIE() {
    pfcpIE_t apac_IE = {};
    apac_IE.IEI = PFCP_IE_APPLY_ACTION;
    apac_IE.applyAction.BUFF = 1;
    apac_IE.applyAction.NOCP = 1;
    return apac_IE;
}

pfcpIE_t formApacBuffIE() {
    pfcpIE_t apac_IE = {};
    apac_IE.IEI = PFCP_IE_APPLY_ACTION;
    apac_IE.applyAction.BUFF = 1;
    return apac_IE;
}

pfcpIE_t formApacDropIE() {
    pfcpIE_t apac_IE = {};
    apac_IE.IEI = PFCP_IE_APPLY_ACTION;
    apac_IE.applyAction.DROP = 1;
    return apac_IE;
}

pfcpIE_t formApacDuplIE() {
    pfcpIE_t apac_IE = {};
    apac_IE.IEI = PFCP_IE_APPLY_ACTION;
    apac_IE.applyAction.DUPL = 1;
    return apac_IE;
}

pfcpIE_t formApacForwIE() {
    pfcpIE_t apac_IE = {};
    apac_IE.IEI = PFCP_IE_APPLY_ACTION;
    apac_IE.applyAction.FORW = 1;
    return apac_IE;
}

pfcpIE_t formGateStatusIE(uint8_t dlGate, uint8_t ulGate) {
    pfcpIE_t gateStatus_IE = {};
    gateStatus_IE.IEI = PFCP_IE_GATE_STATUS;
    gateStatus_IE.gateStatus.DL_GATE = dlGate;
    gateStatus_IE.gateStatus.UL_GATE = ulGate;
    return gateStatus_IE;
}

pfcpIE_t formMbrIE(uint64_t ulMbr, uint64_t dlMbr) {
    pfcpIE_t mbr_IE = {};
    mbr_IE.IEI = PFCP_IE_MBR;
    if(ulMbr) 
        mbr_IE.mbr.UL_MBR = ulMbr;
    if(dlMbr) 
        mbr_IE.mbr.DL_MBR = dlMbr;
    return mbr_IE;
}

pfcpIE_t formGbrIE(uint64_t ulGbr, uint64_t dlGbr) {
    pfcpIE_t gbr_IE = {};
    gbr_IE.IEI = PFCP_IE_GBR;
    if(ulGbr) 
        gbr_IE.gbr.UL_GBR = ulGbr;
    if(dlGbr) 
        gbr_IE.gbr.DL_GBR = dlGbr;
    return gbr_IE;
}

pfcpIE_t formQerCorIdIE(uint32_t value) {
    pfcpIE_t qerCorrelationId_IE = {};
    qerCorrelationId_IE.IEI = PFCP_IE_QER_CORRELATION_ID;
    qerCorrelationId_IE.qerCorrelationId.value = value;
    return qerCorrelationId_IE;
}

// TODO(adheeba): check whether it is for ranTunnelInfo alone
pfcpIE_t formOhcIE(uint16_t  description, pfcpFTEID_t tunnelInfo) {
    pfcpIE_t ohr_IE = {};
    ohr_IE.IEI = PFCP_IE_OUTER_HEADER_CREATION;
    ohr_IE.outerHdrCreatn.description = description;
    ohr_IE.outerHdrCreatn.teid = tunnelInfo.TEID;
    memcpy(ohr_IE.outerHdrCreatn.ipv4Addr, tunnelInfo.ipv4Address,
            PFCP_IPV4_ADDR_LEN);
    return ohr_IE;
}

pfcpIE_t formQfiIE(uint8_t qfi) {
        pfcpIE_t qfi_IE = {};
        qfi_IE.IEI = PFCP_IE_QFI;
        qfi_IE.qfi.value = qfi;
    return qfi_IE;
}

pfcpIE_t formPacketRateIE(uint8_t ulPR, uint8_t dlPR, uint8_t ulTimeUnit,
        uint16_t maxUlPR, uint8_t dlTimeUnit, uint16_t maxDlPR) {
        pfcpIE_t packetRate_IE = {};
        packetRate_IE.IEI = PFCP_IE_PACKET_RATE;
        packetRate_IE.packetRate.ULPR = ulPR;
        packetRate_IE.packetRate.DLPR = ulPR;
        packetRate_IE.packetRate.uplinkTimeUnit = ulTimeUnit;
        packetRate_IE.packetRate.maximumUplinkPacketRate = maxUlPR;
        packetRate_IE.packetRate.downlinkTimeUnit = dlTimeUnit;
        packetRate_IE.packetRate.maximumDownlinkPacketRate = maxDlPR;
    return packetRate_IE;
}

pfcpIE_t formDLDataNotificationDelayIE(uint8_t delay) {
        pfcpIE_t delay_IE = {};
        delay_IE.IEI = PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY;
        delay_IE.dlDataNotifDelay.value = delay;
    return delay_IE;
}

pfcpIE_t formSuggestedBufPktCnt(uint8_t pktCnt) {
        pfcpIE_t pktCnt_IE = {};
        pktCnt_IE.IEI = PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT;
        pktCnt_IE.sugBufPktCnt.value = pktCnt;
    return pktCnt_IE;
}

pfcpIE_t formApplyActionIE(uint8_t applyAction) { 
    pfcpIE_t apac_IE = {}; 
    apac_IE.IEI = PFCP_IE_APPLY_ACTION; 
    apac_IE.applyAction.DROP = applyAction; 
    apac_IE.applyAction.FORW = applyAction >> 1; 
    apac_IE.applyAction.BUFF = applyAction >> 2; 
    apac_IE.applyAction.NOCP = applyAction >> 3; 
    apac_IE.applyAction.DUPL = applyAction >> 4; 
    return apac_IE;
}

