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

#ifndef __PFCP_MESSAGE_HEADER
#define __PFCP_MESSAGE_HEADER

#define PFCP_HDR_MSG_LEN      2
#define PFCP_HDR_SEID_LEN     8
#define PFCP_HDR_SEQNUM_LEN   3

#include <bits/stdc++.h>
#include "pfcp/include/pfcpDefs.h"
#include "pfcp/pfcpHelper.h"

#include "pfcp/pfcpIEs/recTimeStamp.h"
#include "pfcp/pfcpIEs/pfcpNodeId.h"
#include "pfcp/pfcpIEs/pfcpCause.h"
#include "pfcp/pfcpIEs/createPDR.h"
#include "pfcp/pfcpIEs/createdPDR.h"
#include "pfcp/pfcpIEs/updatedPDR.h"
#include "pfcp/pfcpIEs/packetDetectionRuleId.h"
#include "pfcp/pfcpIEs/precedence.h"
#include "pfcp/pfcpIEs/pfcpPDI.h"
#include "pfcp/pfcpIEs/pfcpAny.h"
#include "pfcp/pfcpIEs/pfcpFSEID.h"
#include "pfcp/pfcpIEs/pfcpFTEID.h"
#include "pfcp/pfcpIEs/sdInterface.h"
#include "pfcp/pfcpIEs/ueIpaddress.h"
#include "pfcp/pfcpIEs/SDFFilter.h"
#include "pfcp/pfcpIEs/outerHdrRemoval.h"
#include "pfcp/pfcpIEs/createFAR.h"
#include "pfcp/pfcpIEs/fwdParam.h"
#include "pfcp/pfcpIEs/pfcpFARId.h"
#include "pfcp/pfcpIEs/pfcpApplyAct.h"
#include "pfcp/pfcpIEs/outHeadCreatn.h"
#include "pfcp/pfcpIEs/createQER.h"
#include "pfcp/pfcpIEs/updateQER.h"
#include "pfcp/pfcpIEs/pfcpQERId.h"
#include "pfcp/pfcpIEs/gateStatus.h"
#include "pfcp/pfcpIEs/QERCorrelationId.h"
#include "pfcp/pfcpIEs/MBR.h"
#include "pfcp/pfcpIEs/GBR.h"
#include "pfcp/pfcpIEs/packetRate.h"
#include "pfcp/pfcpIEs/QFI.h"

#include "pfcp/pfcpIEs/removePDR.h"
#include "pfcp/pfcpIEs/removeFAR.h"
#include "pfcp/pfcpIEs/removeQER.h"
#include "pfcp/pfcpIEs/removeBAR.h"
#include "pfcp/pfcpIEs/pfcpOffendingIE.h"
#include "pfcp/pfcpIEs/pfcpFailedRuleId.h"
#include "pfcp/pfcpIEs/updatePDR.h"
#include "pfcp/pfcpIEs/updateFAR.h"
#include "pfcp/pfcpIEs/usrPlaneIpResInfo.h"
#include "pfcp/pfcpIEs/createBAR.h"
#include "pfcp/pfcpIEs/pfcpBARId.h"
#include "pfcp/pfcpIEs/pfcpDLDataNotificationDelay.h"
#include "pfcp/pfcpIEs/pfcpSuggestedBufPktCnt.h"
#include "pfcp/pfcpIEs/updateFwdParam.h"
#include "pfcp/pfcpIEs/downlinkDataReport.h"
#include "pfcp/pfcpIEs/reportType.h"

#define PFCP_VERSION         1
#define PFCP_HDR_MIN_LEN     4
    



    typedef struct pfcpHeader {
        uint8_t    version:3;
        uint8_t    spare:3;
        uint8_t    MP:1;        

        uint8_t    S:1;         


        uint8_t    MessageType;
        

        uint64_t   SEID;
        uint32_t    SequenceNumber:24;
        /* For request Seq Num (3 Bytes) will be filled before sending the msg,
         * for a response user should fill in the request's seq number */
        uint8_t    MessagePriority:4;
                   

        pfcpHeader() {
            version = PFCP_VERSION;
            spare = 0;
            MP = 0;
            S = 0;
            MessageType = 0;
            SEID = 0;
            SequenceNumber = 0;
            MessagePriority = 0;
        };
    } pfcpHeader_t;

    typedef union pfcpIE {
        uint16_t IEI; 

        recTimeStamp_t                  recTimeStamp;
        nodeId_t                        nodeId;
        pfcpCause_t                     cause;
        createPDR_ANY_t                 createPDRany;
        createdPDR_ANY_t                createdPDRany;
        updatedPDR_ANY_t                updatedPDRany;
        pfcpPdrId_t                     pdrId;
        precedence_t                    precedence;
        pfcpPDI_ANY_t                   pdiAny;
        pfcpFSEID_t                     fseid;
        pfcpFTEID_t                     fteid;
        sdIface_t                       srcIface;
        sdIface_t                       destIface;
        ueIpaddress_t                   ueIpAddr;
        sdfFilter_t                     sdfFilter;
        outerHdrRemoval_t               outerHdrRem;
        pfcpFarID_t                     farId;
        pfcpQerID_t                     qerId;
        pfcpBarID_t                     barId;
        createFAR_ANY_t                 createFARany;
        applyAction_t                   applyAction;
        fwdParam_ANY_t                  fwdParamAny;
        updateFwdParam_ANY_t            updateFwdParamany;
        outHdrCreatn_t                  outerHdrCreatn;
        createQER_ANY_t                 createQERany;
        updateQER_ANY_t                 updateQERany;
        gateStatus_t                    gateStatus;
        QerCorrelationID_t              qerCorrelationId;
        MBR_t                           mbr;
        GBR_t                           gbr;
        packetRate_t                    packetRate;
        QFI_t                           qfi;
        
        createBAR_ANY_t                 createBARany;
        pfcpSuggestedBufPktCount_t      sugBufPktCnt;
        pfcpDLDataNotificationDelay_t  dlDataNotifDelay;
        removePDR_ANY_t                 removePDRany;
        removeFAR_ANY_t                 removeFARany;
        removeQER_ANY_t                 removeQERany;
        removeBAR_ANY_t                 removeBARany;
        offendingIE_t                   offendingIE;
        failedRuleId_t                  failedRuleId;
        updateFAR_ANY_t                 updateFARany;
        updatePDR_ANY_t                 updatePDRany;
        usrPlaneIpResInfo_t             upIpResInfo;
        reportType_t                    reportType;
        dlDataReport_ANY_t              dlDataReportany;
} pfcpIE_t;



typedef struct pfcpMessage {
    pfcpHeader_t           header;
    std::vector<pfcpIE_t>  body;
    pfcpMessage() {
        header = {};
        body = {};
    };
} pfcpMessage_t;





/* encode pfcpMsg into buffer of max size bufLen. encodedLen is the filled up
 * portion */
int pfcpEncode(uint8_t *buffer, uint16_t bufLen, pfcpMessage_t *pfcpMsg,
                uint16_t* encodedLen);



int pfcpDecode(uint8_t *buffer, uint16_t bufLen, pfcpMessage_t *pfcpMsg);

#endif
