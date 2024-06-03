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

#ifndef __PFCP_HELPER_H
#define __PFCP_HELPER_H

#include "include/pfcpLogging.h"
#include "platform/include/logging.h"
#include "pfcp/pfcpIEs/pfcpNodeId.h"
#include "pfcp/pfcpIEs/pfcpFSEID.h"
#include "pfcp/pfcpIEs/pfcpFTEID.h"
#include "pfcp/pfcpIEs/SDFFilter.h"
#include "pfcp/include/pfcpCommon.h"
#include <bits/stdc++.h>
#include <arpa/inet.h>

using namespace std;

struct pfcpMessage;
typedef struct pfcpMessage pfcpMessage_t;

struct fteid;
typedef struct fteid fteid_t;

union pfcpIE;
typedef union pfcpIE pfcpIE_t;

int getPfcpIE(uint16_t ieType, pfcpMessage_t *pfcpMsg, pfcpIE_t *pfcpIE);
int getPfcpIEList(uint16_t ieType, std::vector<pfcpIE_t> inIEList, 
                                            std::vector<pfcpIE_t> &outIEList);
int getNodeIdFrmKeyStr(string keyStr, nodeId_t *nId, uint8_t nIdType);
int getKeyStrFrmNodeId(nodeId_t *nodeId, string &keyStr);
int getIpStrFrmIpv4(uint8_t* ipAddr, string &keyStr);
int getIPAddrFrmStr(string ipAddrStr, uint8_t *ipAddr, uint8_t ipType);
int getIPAddrFrmFseid(pfcpFSEID_t fseid, string &ipAddr);
int getPfcpIEFrmList(uint16_t ieType, std::vector<pfcpIE_t> ieList, pfcpIE_t *
                                                                    pfcpIE);




pfcpIE_t formPdrIdIE(uint16_t ruleId);
pfcpIE_t formPrecedenceIE(uint32_t precedence);
pfcpIE_t formDestIfaceIE(uint8_t interfaceValue);
pfcpIE_t formSrcIfaceIE(uint8_t interfaceValue);
pfcpIE_t formUeIpv4AddrIE(uint32_t ueIpAddr, uint8_t SrcOrDest);
pfcpIE_t formFarIdIE(uint32_t farIdValue);
pfcpIE_t formQerIdIE(uint32_t qerIdValue);
pfcpIE_t formApacBuffNocpIE();
pfcpIE_t formApacBuffIE();
pfcpIE_t formApacDropIE();
pfcpIE_t formApacDuplIE();
pfcpIE_t formApacForwIE();
pfcpIE_t formGateStatusIE(uint8_t dlGate, uint8_t ulGate);
pfcpIE_t formMbrIE(uint64_t ulMbr, uint64_t dlMbr);
pfcpIE_t formGbrIE(uint64_t ulGbr, uint64_t dlGbr);
pfcpIE_t formQerCorIdIE(uint32_t value);

pfcpIE_t formOhcIE(uint16_t  description, pfcpFTEID_t tunnelInfo);
pfcpIE_t formFteidIE(fteid_t tunnelInfo);
pfcpIE_t formQfiIE(uint8_t qfi);
pfcpIE_t formPacketRateIE(uint8_t ulPR, uint8_t dlPR, uint8_t ulTimeUnit,
        uint16_t maxUlPR, uint8_t dlTimeUnit, uint16_t maxDlPR);
pfcpIE_t formDLDataNotificationDelayIE(uint8_t delay);
pfcpIE_t formSuggestedBufPktCnt(uint8_t pktCnt);
pfcpIE_t formBarIdIE(uint32_t barIdValue);
pfcpIE_t formApplyActionIE(uint8_t applyAction);

#endif
