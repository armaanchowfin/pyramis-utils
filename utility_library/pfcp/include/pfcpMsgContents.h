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

#ifndef   __PFCP_HEADER_MSG_CONTENTS
#define   __PFCP_HEADER_MSG_CONTENTS
/*
 * Contains const data
 *
 * 1. ieData: IEs that can be added to a particular message
 *
 * 2. pfcpIEoperations : encode & decode operations for each IE
 *
 */



#include "pfcp/include/pfcpDefs.h"
#include "pfcp/pfcpDeliver.h"




const static std::map<uint8_t, std::map<uint16_t, pfcpIEdata_t> > ieData = {
    { PFCP_HEARTBEAT_REQUEST,   

        {
            { PFCP_IE_RECOVERY_TIME_STAMP, { PFCP_IED_FLAG_MANDATORY }}
        }
    },

    { PFCP_HEARTBEAT_RESPONSE,   

        {
            { PFCP_IE_RECOVERY_TIME_STAMP, { PFCP_IED_FLAG_MANDATORY }}
        }
    },

    { PFCP_ASSOCIATION_SETUP_REQUEST,   

        {
            { PFCP_IE_NODE_ID, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_RECOVERY_TIME_STAMP, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_UP_FUNCTION_FEATURES, {PFCP_IED_FLAG_CONDITIONAL }},
            { PFCP_IE_CP_FUNCTION_FEATURES, {PFCP_IED_FLAG_CONDITIONAL }},
            { PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION,
                    {PFCP_IED_FLAG_OPTIONAL | PFCP_IED_FLAG_MULTIPLE}}
        }
    },

    { PFCP_ASSOCIATION_SETUP_RESPONSE,   

        {
            { PFCP_IE_NODE_ID, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_CAUSE, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_RECOVERY_TIME_STAMP, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_UP_FUNCTION_FEATURES, {PFCP_IED_FLAG_CONDITIONAL }},
            { PFCP_IE_CP_FUNCTION_FEATURES, {PFCP_IED_FLAG_CONDITIONAL }},
            { PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION,
                    {PFCP_IED_FLAG_OPTIONAL | PFCP_IED_FLAG_MULTIPLE}},
            { PFCP_IE_FAILED_RULE_ID, {PFCP_IED_FLAG_CONDITIONAL}},
        }
    },

    { PFCP_SESSION_ESTABLISHMENT_REQUEST,     

        {
            { PFCP_IE_NODE_ID, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_F_SEID, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_CREATE_PDR,
                    {PFCP_IED_FLAG_MANDATORY | PFCP_IED_FLAG_MULTIPLE}},
            { PFCP_IE_CREATE_FAR,
                    {PFCP_IED_FLAG_MANDATORY | PFCP_IED_FLAG_MULTIPLE}},
            { PFCP_IE_CREATE_QER,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE}},
            { PFCP_IE_FAILED_RULE_ID, {PFCP_IED_FLAG_CONDITIONAL}},
            

            { PFCP_IE_CREATE_BAR, {PFCP_IED_FLAG_OPTIONAL}},
            

        }
    },

    { PFCP_SESSION_ESTABLISHMENT_RESPONSE,     

        {
            { PFCP_IE_NODE_ID, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_F_SEID, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_CAUSE, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_OFFENDING_IE, {PFCP_IED_FLAG_CONDITIONAL}},
            { PFCP_IE_CREATED_PDR,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE}},
            

        }
    },

    { PFCP_SESSION_MODIFICATION_REQUEST,       

        {
            { PFCP_IE_F_SEID, {PFCP_IED_FLAG_CONDITIONAL}},
            { PFCP_IE_REMOVE_PDR,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE }},
            { PFCP_IE_REMOVE_FAR,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE }},
            { PFCP_IE_REMOVE_QER,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE }},
            { PFCP_IE_CREATE_PDR,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE }},
            { PFCP_IE_CREATE_FAR,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE }},
            { PFCP_IE_CREATE_QER,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE }},
            { PFCP_IE_UPDATE_PDR,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE }},
            {PFCP_IE_UPDATE_FAR,
                    {PFCP_IED_FLAG_CONDITIONAL | PFCP_IED_FLAG_MULTIPLE }},
            { PFCP_IE_CREATE_BAR, {PFCP_IED_FLAG_CONDITIONAL }},
            { PFCP_IE_REMOVE_BAR, {PFCP_IED_FLAG_CONDITIONAL }},
            

            

        }
    },

    { PFCP_SESSION_MODIFICATION_RESPONSE,       

        {
            { PFCP_IE_CAUSE, {PFCP_IED_FLAG_MANDATORY}},
            { PFCP_IE_OFFENDING_IE, {PFCP_IED_FLAG_CONDITIONAL}},
            { PFCP_IE_CREATED_PDR, {PFCP_IED_FLAG_CONDITIONAL}},
            { PFCP_IE_UPDATED_PDR, {PFCP_IED_FLAG_CONDITIONAL}},
            

        }
    },

    { PFCP_SESSION_DELETION_REQUEST,            

        {   

        }
    },

    { PFCP_SESSION_DELETION_RESPONSE,           

        {
            { PFCP_IE_CAUSE, {PFCP_IED_FLAG_MANDATORY}},
            { PFCP_IE_OFFENDING_IE, {PFCP_IED_FLAG_CONDITIONAL}},
        }
    },

    { PFCP_SESSION_REPORT_REQUEST,     

        {
            { PFCP_IE_REPORT_TYPE, {PFCP_IED_FLAG_MANDATORY }},
            { PFCP_IE_DOWNLINK_DATA_REPORT, {PFCP_IED_FLAG_CONDITIONAL }},
            

        }
    },

    { PFCP_SESSION_REPORT_RESPONSE,     

        {
            { PFCP_IE_CAUSE, {PFCP_IED_FLAG_MANDATORY }},
            

        }
    },

};






typedef struct pfcpIEops {
    int (*encode) (uint8_t *buffer, uint16_t bufLen, void* ptr,
                    uint16_t* encodedLen);
    int (*decode) (uint8_t *buffer, uint16_t bufLen, void* ptr,
                    uint16_t* decodedLen);
} pfcpIEops_t;

const static std::map<uint16_t, pfcpIEops_t> pfcpIEoperations = {
    { PFCP_IE_RECOVERY_TIME_STAMP,
        { recTimeStampEncode, recTimeStampDecode }
    },
    { PFCP_IE_NODE_ID,
        { nodeIdEncode, nodeIdDecode }
    },
    { PFCP_IE_CAUSE,
        { pfcpCauseEncode, pfcpCauseDecode }
    },
    { PFCP_IE_CREATE_PDR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_CREATED_PDR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_UPDATED_PDR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_PACKET_DETECTION_RULE_ID,
        { pfcpPdrIDencode, pfcpPdrIDdecode }
    },
    { PFCP_IE_PRECEDENCE,
        { precedenceEncode, precedenceDecode }
    },
    { PFCP_IE_PDI,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_F_SEID,
        { fseidEncode, fseidDecode }
    },
    { PFCP_IE_FAR_ID,
        { pfcpFarIDEncode, pfcpFarIDDecode}
    },
    { PFCP_IE_QER_ID,
        { pfcpQerIDEncode, pfcpQerIDDecode}
    },
    { PFCP_IE_APPLY_ACTION,
        { pfcpApplyActionEncode, pfcpApplyActionDecode }
    },
    { PFCP_IE_CREATE_FAR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_FORWARDING_PARAMETERS,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_UPDATE_FORWARDING_PARAMETERS,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_OUTER_HEADER_CREATION,
        { outHdrCreatnEncode, outHdrCreatnDecode}
    },
    { PFCP_IE_SOURCE_INTERFACE,
        { sdIfaceEncode, sdIfaceDecode}
    },
    { PFCP_IE_DESTINATION_INTERFACE,
        { sdIfaceEncode, sdIfaceDecode}
    },
    { PFCP_IE_F_TEID,
        {pfcpFTEIDencode,  pfcpFTEIDdecode}
    },
    { PFCP_IE_UE_IP_ADDRESS,
        {ueIpaddressEncode, ueIpaddressDecode}
    },
    { PFCP_IE_SDF_FILTER,
        {sdfFilterEncode, sdfFilterDecode}
    },
    { PFCP_IE_OUTER_HEADER_REMOVAL,
        {outerHdrRemovalEncode, outerHdrRemovalDecode}
    },
    { PFCP_IE_CREATE_QER,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_GATE_STATUS,
        { pfcpGateStatusEncode, pfcpGateStatusDecode }
    },
    { PFCP_IE_QER_CORRELATION_ID,
        { QerCorrelationIDEncode, QerCorrelationIDDecode }
    },
    { PFCP_IE_MBR,
        { MBREncode, MBRDecode }
    },
    { PFCP_IE_GBR,
        { GBREncode, GBRDecode }
    },
    { PFCP_IE_PACKET_RATE,
        { packetRateEncode, packetRateDecode }
    },
    { PFCP_IE_QFI,
        { QFIEncode, QFIDecode }
    },
    { PFCP_IE_REMOVE_PDR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_REMOVE_FAR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_REMOVE_QER,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_REMOVE_BAR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_CREATE_BAR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_BAR_ID,
        { pfcpBarIDEncode, pfcpBarIDDecode }
    },
    { PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT,
        { pfcpSuggestedBufPktCountEncode, pfcpSuggestedBufPktCountDecode }
    },
    { PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY,
        { pfcpDLDataNotificationDelayEncode, pfcpDLDataNotificationDelayDecode }
    },
    { PFCP_IE_UPDATE_PDR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_UPDATE_FAR,
        { pfcpAnyEncode, pfcpAnyDecode }
    },
    { PFCP_IE_OFFENDING_IE,
        { offendingIEencode, offendingIEdecode }
    },
    { PFCP_IE_FAILED_RULE_ID,
        { failedRuleIdencode, failedRuleIddecode}
    },
    { PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION,
        { usrPlaneIpResInfoEncode, usrPlaneIpResInfoDecode}
    },
    { PFCP_IE_DOWNLINK_DATA_REPORT,
        { pfcpAnyEncode, pfcpAnyDecode}
    },
    { PFCP_IE_REPORT_TYPE,
        { reportTypeEncode, reportTypeDecode}
    },
};




#endif
