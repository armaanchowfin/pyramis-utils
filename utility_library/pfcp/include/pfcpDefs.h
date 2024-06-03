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

#ifndef __PFCP_ERRORS_HEADER
#define __PFCP_ERRORS_HEADER

#include <errno.h>
#include <bits/stdc++.h>



enum {
    E_PFCP_ERROR_UNKNOWN             =  1,    

    E_PFCP_ERROR_INVALID_INPUT       =  2,
    E_PFCP_ERROR_BUFF_INSUFF         =  3,
    E_PFCP_ERROR_BUFF_INCOMPLETE     =  4,
    E_PFCP_ERROR_CONSTR_CHECK_FAIL   =  5,
    E_PFCP_ERROR_CANNOT_ADD_IE       =  6,
    E_PFCP_ERROR_MAND_IE_MISSING     =  7,

};


 




#define     PFCP_HEARTBEAT_REQUEST                      1
#define     PFCP_HEARTBEAT_RESPONSE                     2
#define     PFCP_PFD_MANAGEMENT_REQUEST                 3
#define     PFCP_PFD_MANAGEMENT_RESPONSE                4
#define     PFCP_ASSOCIATION_SETUP_REQUEST              5
#define     PFCP_ASSOCIATION_SETUP_RESPONSE             6
#define     PFCP_ASSOCIATION_UPDATE_REQUEST             7
#define     PFCP_ASSOCIATION_UPDATE_RESPONSE            8
#define     PFCP_ASSOCIATION_RELEASE_REQUEST            9
#define     PFCP_ASSOCIATION_RELEASE_RESPONSE           10
#define     PFCP_VERSION_NOT_SUPPORTED_RESPONSE         11
#define     PFCP_NODE_REPORT_REQUEST                    12
#define     PFCP_NODE_REPORT_RESPONSE                   13
#define     PFCP_SESSION_SET_DELETION_REQUEST           14
#define     PFCP_SESSION_SET_DELETION_RESPONSE          15



#define     PFCP_SESSION_ESTABLISHMENT_REQUEST          50
#define     PFCP_SESSION_ESTABLISHMENT_RESPONSE         51
#define     PFCP_SESSION_MODIFICATION_REQUEST           52
#define     PFCP_SESSION_MODIFICATION_RESPONSE          53
#define     PFCP_SESSION_DELETION_REQUEST               54
#define     PFCP_SESSION_DELETION_RESPONSE              55
#define     PFCP_SESSION_REPORT_REQUEST                 56
#define     PFCP_SESSION_REPORT_RESPONSE                57


bool inline isNodeRelatedMsg(uint8_t type)
{
    if(type >= PFCP_HEARTBEAT_REQUEST &&
            type <= PFCP_SESSION_SET_DELETION_RESPONSE) {
        return true;
    } else {
        return false;
    }
}
bool inline isSessionRelatedMsg(uint8_t type)
{
    if(type >= PFCP_SESSION_ESTABLISHMENT_REQUEST &&
            type <= PFCP_SESSION_REPORT_RESPONSE) {
        return true;
    } else {
        return false;
    }
}



 

#define    PFCP_IE_CREATE_PDR                                          1
#define    PFCP_IE_PDI                                                 2
#define    PFCP_IE_CREATE_FAR                                          3
#define    PFCP_IE_FORWARDING_PARAMETERS                               4
#define    PFCP_IE_DUPLICATING_PARAMETERS                              5
#define    PFCP_IE_CREATE_URR                                          6
#define    PFCP_IE_CREATE_QER                                          7
#define    PFCP_IE_CREATED_PDR                                         8
#define    PFCP_IE_UPDATE_PDR                                          9
#define    PFCP_IE_UPDATE_FAR                                          10
#define    PFCP_IE_UPDATE_FORWARDING_PARAMETERS                        11
#define    PFCP_IE_UPDATE_BAR_PFCP_SESSION_REPORT_RESPONSE             12
#define    PFCP_IE_UPDATE_URR                                          13
#define    PFCP_IE_UPDATE_QER                                          14
#define    PFCP_IE_REMOVE_PDR                                          15
#define    PFCP_IE_REMOVE_FAR                                          16
#define    PFCP_IE_REMOVE_URR                                          17
#define    PFCP_IE_REMOVE_QER                                          18
#define    PFCP_IE_CAUSE                                               19
#define    PFCP_IE_SOURCE_INTERFACE                                    20
#define    PFCP_IE_F_TEID                                              21
#define    PFCP_IE_NETWORK_INSTANCE                                    22
#define    PFCP_IE_SDF_FILTER                                          23
#define    PFCP_IE_APPLICATION_ID                                      24
#define    PFCP_IE_GATE_STATUS                                         25
#define    PFCP_IE_MBR                                                 26
#define    PFCP_IE_GBR                                                 27
#define    PFCP_IE_QER_CORRELATION_ID                                  28
#define    PFCP_IE_PRECEDENCE                                          29
#define    PFCP_IE_TRANSPORT_LEVEL_MARKING                             30
#define    PFCP_IE_VOLUME_THRESHOLD                                    31
#define    PFCP_IE_TIME_THRESHOLD                                      32
#define    PFCP_IE_MONITORING_TIME                                     33
#define    PFCP_IE_SUBSEQUENT_VOLUME_THRESHOLD                         34
#define    PFCP_IE_SUBSEQUENT_TIME_THRESHOLD                           35
#define    PFCP_IE_INACTIVITY_DETECTION_TIME                           36
#define    PFCP_IE_REPORTING_TRIGGERS                                  37
#define    PFCP_IE_REDIRECT_INFORMATION                                38
#define    PFCP_IE_REPORT_TYPE                                         39
#define    PFCP_IE_OFFENDING_IE                                        40
#define    PFCP_IE_FORWARDING_POLICY                                   41
#define    PFCP_IE_DESTINATION_INTERFACE                               42
#define    PFCP_IE_UP_FUNCTION_FEATURES                                43
#define    PFCP_IE_APPLY_ACTION                                        44
#define    PFCP_IE_DOWNLINK_DATA_SERVICE_INFORMATION                   45
#define    PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY                    46
#define    PFCP_IE_DL_BUFFERING_DURATION                               47
#define    PFCP_IE_DL_BUFFERING_SUGGESTED_PACKET_COUNT                 48
#define    PFCP_IE_PFCPSMREQ_FLAGS                                     49
#define    PFCP_IE_PFCPSRRSP_FLAGS                                     50
#define    PFCP_IE_LOAD_CONTROL_INFORMATION                            51
#define    PFCP_IE_SEQUENCE_NUMBER                                     52
#define    PFCP_IE_METRIC                                              53
#define    PFCP_IE_OVERLOAD_CONTROL_INFORMATION                        54
#define    PFCP_IE_TIMER                                               55
#define    PFCP_IE_PACKET_DETECTION_RULE_ID                            56
#define    PFCP_IE_F_SEID                                              57
#define    PFCP_IE_APPLICATION_IDS_PFDS                                58
#define    PFCP_IE_PFD_CONTEXT                                         59
#define    PFCP_IE_NODE_ID                                             60
#define    PFCP_IE_PFD_CONTENTS                                        61
#define    PFCP_IE_MEASUREMENT_METHOD                                  62
#define    PFCP_IE_USAGE_REPORT_TRIGGER                                63
#define    PFCP_IE_MEASUREMENT_PERIOD                                  64
#define    PFCP_IE_FQ_CSID                                             65
#define    PFCP_IE_VOLUME_MEASUREMENT                                  66
#define    PFCP_IE_DURATION_MEASUREMENT                                67
#define    PFCP_IE_APPLICATION_DETECTION_INFORMATION                   68
#define    PFCP_IE_TIME_OF_FIRST_PACKET                                69
#define    PFCP_IE_TIME_OF_LAST_PACKET                                 70
#define    PFCP_IE_QUOTA_HOLDING_TIME                                  71
#define    PFCP_IE_DROPPED_DL_TRAFFIC_THRESHOLD                        72
#define    PFCP_IE_VOLUME_QUOTA                                        73
#define    PFCP_IE_TIME_QUOTA                                          74
#define    PFCP_IE_START_TIME                                          75
#define    PFCP_IE_END_TIME                                            76
#define    PFCP_IE_QUERY_URR                                           77
#define    PFCP_IE_USAGE_REPORT_IN_SESSION_MODIFICATION_RESPONSE       78
#define    PFCP_IE_USAGE_REPORT_SESSION_DELETION_RESPONSE              89
#define    PFCP_IE_USAGE_REPORT_SESSION_REPORT_REQUEST                 80
#define    PFCP_IE_URR_ID                                              81
#define    PFCP_IE_LINKED_URR_ID                                       82
#define    PFCP_IE_DOWNLINK_DATA_REPORT                                83
#define    PFCP_IE_OUTER_HEADER_CREATION                               84
#define    PFCP_IE_CREATE_BAR                                          85
#define    PFCP_IE_UPDATE_BAR_SESSION_MODIFICATION_REQUEST             86
#define    PFCP_IE_REMOVE_BAR                                          87
#define    PFCP_IE_BAR_ID                                              88
#define    PFCP_IE_CP_FUNCTION_FEATURES                                89
#define    PFCP_IE_USAGE_INFORMATION                                   90
#define    PFCP_IE_APPLICATION_INSTANCE_ID                             91
#define    PFCP_IE_FLOW_INFORMATION                                    92
#define    PFCP_IE_UE_IP_ADDRESS                                       93
#define    PFCP_IE_PACKET_RATE                                         94
#define    PFCP_IE_OUTER_HEADER_REMOVAL                                95
#define    PFCP_IE_RECOVERY_TIME_STAMP                                 96
#define    PFCP_IE_DL_FLOW_LEVEL_MARKING                               97
#define    PFCP_IE_HEADER_ENRICHMENT                                   98
#define    PFCP_IE_ERROR_INDICATION_REPORT                             99
#define    PFCP_IE_MEASUREMENT_INFORMATION                             100
#define    PFCP_IE_NODE_REPORT_TYPE                                    101
#define    PFCP_IE_USER_PLANE_PATH_FAILURE_REPORT                      102
#define    PFCP_IE_REMOTE_GTP_U_PEER                                   103
#define    PFCP_IE_UR_SEQN                                             104
#define    PFCP_IE_UPDATE_DUPLICATING_PARAMETERS                       105
#define    PFCP_IE_ACTIVATE_PREDEFINED_RULES                           106
#define    PFCP_IE_DEACTIVATE_PREDEFINED_RULES                         107
#define    PFCP_IE_FAR_ID                                              108
#define    PFCP_IE_QER_ID                                              109
#define    PFCP_IE_OCI_FLAGS                                           110
#define    PFCP_IE_PFCP_ASSOCIATION_RELEASE_REQUEST                    111
#define    PFCP_IE_GRACEFUL_RELEASE_PERIOD                             112
#define    PFCP_IE_PDN_TYPE                                            113
#define    PFCP_IE_FAILED_RULE_ID                                      114
#define    PFCP_IE_TIME_QUOTA_MECHANISM                                115
#define    PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION                  116
#define    PFCP_IE_USER_PLANE_INACTIVITY_TIMER                         117
#define    PFCP_IE_AGGREGATED_URRS                                     118
#define    PFCP_IE_MULTIPLIER                                          119
#define    PFCP_IE_AGGREGATED_URR_ID                                   120
#define    PFCP_IE_SUBSEQUENT_VOLUME_QUOTA                             121
#define    PFCP_IE_SUBSEQUENT_TIME_QUOTA                               122
#define    PFCP_IE_RQI                                                 123
#define    PFCP_IE_QFI                                                 124
#define    PFCP_IE_QUERY_URR_REFERENCE                                 125
#define    PFCP_IE_ADDITIONAL_USAGE_REPORTS_INFORMATION                126
#define    PFCP_IE_CREATE_TRAFFIC_ENDPOINT                             127
#define    PFCP_IE_CREATED_TRAFFIC_ENDPOINT                            128
#define    PFCP_IE_UPDATE_TRAFFIC_ENDPOINT                             129
#define    PFCP_IE_REMOVE_TRAFFIC_ENDPOINT                             130
#define    PFCP_IE_TRAFFIC_ENDPOINT_ID                                 131
#define    PFCP_IE_ETHERNET_PACKET_FILTER                              132
#define    PFCP_IE_MAC_ADDRESS                                         133
#define    PFCP_IE_C_TAG                                               134
#define    PFCP_IE_S_TAG                                               135
#define    PFCP_IE_ETHERTYPE                                           136
#define    PFCP_IE_PROXYING                                            137
#define    PFCP_IE_ETHERNET_FILTER_ID                                  138
#define    PFCP_IE_ETHERNET_FILTER_PROPERTIES                          139
#define    PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT                   140
#define    PFCP_IE_USER_ID                                             141
#define    PFCP_IE_ETHERNET_PDU_SESSION_INFORMATION                    142
#define    PFCP_IE_ETHERNET_TRAFFIC_INFORMATION                        143
#define    PFCP_IE_MAC_ADDRESSES_DETECTED                              144
#define    PFCP_IE_MAC_ADDRESSES_REMOVED                               145
#define    PFCP_IE_ETHERNET_INACTIVITY_TIMER                           146
#define    PFCP_IE_ADDITIONAL_MONITORING_TIME                          147
#define    PFCP_IE_EVENT_INFORMATION                                   148
#define    PFCP_IE_EVENT_REPORTING                                     149
#define    PFCP_IE_EVENT_ID                                            150
#define    PFCP_IE_EVENT_THRESHOLD                                     151
#define    PFCP_IE_TRACE_INFORMATION                                   152
#define    PFCP_IE_FRAMED_ROUTE                                        153
#define    PFCP_IE_FRAMED_ROUTING                                      154
#define    PFCP_IE_FRAMED_IPV6_ROUTE                                   155
#define    PFCP_IE_DESTINATION_INTERFACE_TYPE                          160
#define    PFCP_IE_UPDATED_PDR                                         256




#define   PFCP_IED_FLAG_MANDATORY                   1<<0
#define   PFCP_IED_FLAG_CONDITIONAL                 1<<1
#define   PFCP_IED_FLAG_OPTIONAL                    1<<2
#define   PFCP_IED_FLAG_MULTIPLE                    1<<3

typedef struct  pfcpIEdata {
    uint16_t     flags;
} pfcpIEdata_t;

#endif
