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

#ifndef __NGAP_HELPER_H
#define __NGAP_HELPER_H
#include <map>
#include "platform/include/logging.h"
#include "ngap/include/RAN-UE-NGAP-ID.h"
#include "ngap/include/AMF-UE-NGAP-ID.h"
#include "ngap/include/NAS-PDU.h"
#include "ngap/include/GlobalRANNodeID.h"
#include "ngap/include/RANNodeName.h"
#include "ngap/include/PDUSessionResourceSetupListSURes.h"
#include "ngap/include/Cause.h"
#include "ngap/include/GUAMI.h"
#include "ngap/include/UESecurityCapabilities.h"
#include "ngap/include/UE-NGAP-IDs.h"
#include "ngap/include/NGAP-PDU.h"
#include "ngap/include/SupportedTAList.h"
#include "ngap/include/PLMNSupportList.h"
#include "ngap/include/PLMNSupportItem.h"
#include "ngap/include/UE-NGAP-ID-pair.h"
#include "ngap/include/GlobalGNB-ID.h"
#include "ngap/include/GlobalN3IWF-ID.h"
#include "ngap/include/SliceSupportItem.h"
#include "ngap/include/SupportedTAItem.h"
#include "ngap/include/BroadcastPLMNItem.h"
#include "ngap/include/ServedGUAMIItem.h"
#include "ngap/include/ServedGUAMIList.h"
#include "ngap/include/AMFName.h"
#include "ngap/include/TAC.h"
#include "ngap/include/S-NSSAI.h"
#include "ngap/include/InitiatingMessage.h"
#include "ngap/include/SuccessfulOutcome.h"
#include "ngap/include/UnsuccessfulOutcome.h"
#include "ngap/include/ProtocolIE-ID.h"
#include "ngap/include/PLMNIdentity.h"
#include "ngap/include/ProtocolIE-Field.h"
#include "ngap/include/BIT_STRING.h"
#include "ngap/include/NGSetupResponse.h"
#include "ngap/include/UserLocationInformationNR.h"
#include "ngap/include/UserLocationInformationN3IWF.h"
#include "ngap/include/AllowedNSSAI-Item.h"
#include "ngap/include/PDUSessionResourceSetupRequestTransfer.h"
#include "ngap/include/UPTransportLayerInformation.h"
#include "ngap/include/GTPTunnel.h"
#include "ngap/include/PDUSessionResourceSetupItemSUReq.h"
#include "ngap/include/PDUSessionResourceSetupItemSURes.h"
#include "ngap/include/PDUSessionResourceSetupResponseTransfer.h"
#include "ngap/include/QosFlowPerTNLInformation.h"
#include "ngap/include/AssociatedQosFlowItem.h"
#include "ngap/include/PDUSessionResourceReleaseResponseTransfer.h"
#include "ngap/include/PDUSessionResourceReleasedItemRelRes.h"
#include "ngap/include/PDUSessionResourceReleaseCommandTransfer.h"
#include "ngap/include/QosFlowSetupRequestItem.h"
#include "ngap/include/NonDynamic5QIDescriptor.h"
#include "ngap/include/PDUSessionResourceItemCxtRelReq.h"
#include "ngap/include/PDUSessionResourceToReleaseItemRelCmd.h"
#include "ngap/include/PDUSessionResourceSetupItemCxtReq.h"
#include "ngap/include/PDUSessionResourceSetupItemCxtRes.h"
#include "ngap/include/PDUSessionResourceModifyListModReq.h"
#include "ngap/include/PDUSessionResourceModifyRequestTransfer.h"
#include "ngap/include/PDUSessionResourceModifyItemModRes.h"
#include "ngap/include/PDUSessionResourceModifyResponse.h"
#include "ngap/include/PDUSessionResourceModifyItemModReq.h"
#include "ngap/include/PDUSessionResourceModifyResponseTransfer.h"
#include "ngap/include/QosFlowAddOrModifyResponseItem.h"
#include "ngap/include/TAIListForPagingItem.h"

using namespace std;
using std::map;

/* 38.413 15.2.0 9.3.1.2 Cause
 * The purpose of the Cause IE is to indicate the reason for a particular
 * event for the NGAP protocol.
 * APER asn ngap library has support to encode deocde this Casue IE
 * However for error propogation or for logging appropriate logs
 * we need to maintain this enum and map of enum to logStr */
typedef enum {
 E_RADIO_NETWORK_LAYER_UNSPECIFIED = 1, 

 E_RADIO_NETWORK_LAYER_TXNRELOCOVERALL_EXPIRY,
 E_RADIO_NETWORK_LAYER_SUCCESSFUL_HANDOVER,
 E_RADIO_NETWORK_LAYER_RELEASE_DUE_TO_NG_RAN_GENERATED_REASON,
 E_RADIO_NETWORK_LAYER_RELEASE_DUE_TO_5GC_GENERATED_REASON,
 E_RADIO_NETWORK_LAYER_HANDOVER_CANCELLED,
 E_RADIO_NETWORK_LAYER_PARTIAL_HANDOVER,
 E_RADIO_NETWORK_LAYER_HANDOVER_FAILURE_IN_TARGET_5GC_NG_RAN_NODE_OR_TARGET_SYSTEM,
 E_RADIO_NETWORK_LAYER_HANDOVER_TARGET_NOT_ALLOWED,
 E_RADIO_NETWORK_LAYER_TNGRELOCOVERALL_EXPIRY,
 E_RADIO_NETWORK_LAYER_TNGRELOCPREP_EXPIRY,
 E_RADIO_NETWORK_LAYER_CELL_NOT_AVAILABLE,
 E_RADIO_NETWORK_LAYER_UNKNOWN_TARGET_ID,
 E_RADIO_NETWORK_LAYER_NO_RADIO_RESOURCES_AVAILABLE_IN_TARGET_CELL,
 E_RADIO_NETWORK_LAYER_UNKNOWN_LOCAL_UE_NGAP_ID,
 E_RADIO_NETWORK_LAYER_INCONSISTENT_REMOTE_UE_NGAP_ID,
 E_RADIO_NETWORK_LAYER_HANDOVER_DESIRABLE_FOR_RADIO_REASONS,
 E_RADIO_NETWORK_LAYER_TIME_CRITICAL_HANDOVER,
 E_RADIO_NETWORK_LAYER_RESOURCE_OPTIMISATION_HANDOVER,
 E_RADIO_NETWORK_LAYER_REDUCE_LOAD_IN_SERVING_CELL,
 E_RADIO_NETWORK_LAYER_USER_INACTIVITY,
 E_RADIO_NETWORK_LAYER_RADIO_CONNECTION_WITH_UE_LOST,
 E_RADIO_NETWORK_LAYER_RADIO_RESOURCES_NOT_AVAILABLE,
 E_RADIO_NETWORK_LAYER_INVALID_QOS_COMBINATION,
 E_RADIO_NETWORK_LAYER_FAILURE_IN_THE_RADIO_INTERFACE_PROCEDURE,
 E_RADIO_NETWORK_LAYER_INTERACTION_WITH_OTHER_PROCEDURE,
 E_RADIO_NETWORK_LAYER_UNKNOWN_PDU_SESSION_ID,
 E_RADIO_NETWORK_LAYER_UNKNOWN_QOS_FLOW_ID,
 E_RADIO_NETWORK_LAYER_MULTIPLE_PDU_SESSION_ID_INSTANCES,
 E_RADIO_NETWORK_LAYER_MULTIPLE_QOS_FLOW_ID_INSTANCES,
 E_RADIO_NETWORK_LAYER_ENCRYPTION_AND_INTEGRITY_PROTECTION_ALGORITHMS_NOT_SUPPORTED,
 E_RADIO_NETWORK_LAYER_NG_INTRA_SYSTEM_HANDOVER_TRIGGERED,
 E_RADIO_NETWORK_LAYER_NG_INTER_SYSTEM_HANDOVER_TRIGGERED,
 E_RADIO_NETWORK_LAYER_XN_HANDOVER_TRIGGERED,
 E_RADIO_NETWORK_LAYER_NOT_SUPPORTED_5QI_VALUE,
 E_RADIO_NETWORK_LAYER_UE_CONTEXT_TRANSFER,
 E_RADIO_NETWORK_LAYER_IMS_VOICE_EPS_FALLBACK_OR_RAT_FALLBACK_TRIGGERED,
 E_RADIO_NETWORK_LAYER_UP_INTEGRITY_PROTECTION_NOT_POSSIBLE,
 E_RADIO_NETWORK_LAYER_UP_CONFIDENTIALITY_PROTECTION_NOT_POSSIBLE,
 E_RADIO_NETWORK_LAYER_SLICE_NOT_SUPPORTED,
 E_RADIO_NETWORK_LAYER_UE_IN_RRC_INACTIVE_STATE_NOT_REACHABLE,
 E_RADIO_NETWORK_LAYER_REDIRECTION,
 E_RADIO_NETWORK_LAYER_RESOURCES_NOT_AVAILABLE_FOR_THE_SLICES,
 E_RADIO_NETWORK_LAYER_UE_MAXIMUM_INTEGRITY_PROTECTED_DATA_RATE_REASON,
 E_RADIO_NETWORK_LAYER_RELEASE_DUE_TO_CN_DETECTED_MOBILITY,

 E_TRANSPORT_LAYER_TRANSPORT_RESOURCE_UNAVAILABLE,
 E_TRANSPORT_LAYER_UNSPECIFIED,

 E_NAS_NORMAL_RELEASE,
 E_NAS_AUTHENTICATION_FAILURE,
 E_NAS_DEREGISTER,
 E_NAS_UNSPECIFIED,

 E_PROTOCOL_TRANSFER_SYNTAX_ERROR,
 E_PROTOCOL_ABSTRACT_SYNTAX_ERROR_REJECT,
 E_PROTOCOL_ABSTRACT_SYNTAX_ERROR_IGNORE_AND_NOTIFY,
 E_PROTOCOL_MESSAGE_NOT_COMPATIBLE_WITH_RECEIVER_STATE,
 E_PROTOCOL_SEMANTIC_ERROR,
 E_PROTOCOL_ABSTRACT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MESSAGE,
 E_PROTOCOL_UNSPECIFIED,

 E_MISCELLANEOUS_CONTROL_PROCESSING_OVERLOAD,
 E_MISCELLANEOUS_NOT_ENOUGH_USER_PLANE_PROCESSING,
 E_MISCELLANEOUS_RESOURCES,
 E_MISCELLANEOUS_HARDWARE_FAILURE,
 E_MISCELLANEOUS_OM_INTERVENTION,
 E_MISCELLANEOUS_UNKNOWN_PLMN,
 E_MISCELLANEOUS_UNSPECIFIED,

 E_NGAP_UNKOWN_CAUSE
} ngap_error_cause_t;

static std::map<ngap_error_cause_t, std::string> ngapErrlogMap = {
  {E_RADIO_NETWORK_LAYER_UNSPECIFIED,
    "radio_network_layer_unspecified"},
  {E_RADIO_NETWORK_LAYER_TXNRELOCOVERALL_EXPIRY,
    "radio_network_layer_txnrelocoverall_expiry"},
  {E_RADIO_NETWORK_LAYER_SUCCESSFUL_HANDOVER,
    "radio_network_layer_successful_handover"},
  {E_RADIO_NETWORK_LAYER_RELEASE_DUE_TO_NG_RAN_GENERATED_REASON,
    "radio_network_layer_release_due_to_ng_ran_generated_reason"},
  {E_RADIO_NETWORK_LAYER_RELEASE_DUE_TO_5GC_GENERATED_REASON,
    "radio_network_layer_release_due_to_5gc_generated_reason"},
  {E_RADIO_NETWORK_LAYER_HANDOVER_CANCELLED,
    "radio_network_layer_handover_cancelled"},
  {E_RADIO_NETWORK_LAYER_PARTIAL_HANDOVER,
    "radio_network_layer_partial_handover"},
  {E_RADIO_NETWORK_LAYER_HANDOVER_FAILURE_IN_TARGET_5GC_NG_RAN_NODE_OR_TARGET_SYSTEM,
    "radio_network_layer_handover_failure_in_target_5gc_ng_ran_node_or_target_system"},
  {E_RADIO_NETWORK_LAYER_HANDOVER_TARGET_NOT_ALLOWED,
    "radio_network_layer_handover_target_not_allowed"},
  {E_RADIO_NETWORK_LAYER_TNGRELOCOVERALL_EXPIRY,
    "radio_network_layer_tngrelocoverall_expiry"},
  {E_RADIO_NETWORK_LAYER_TNGRELOCPREP_EXPIRY,
    "radio_network_layer_tngrelocprep_expiry"},
  {E_RADIO_NETWORK_LAYER_CELL_NOT_AVAILABLE,
    "radio_network_layer_cell_not_available"},
  {E_RADIO_NETWORK_LAYER_UNKNOWN_TARGET_ID,
    "radio_network_layer_unknown_target_id"},
  {E_RADIO_NETWORK_LAYER_NO_RADIO_RESOURCES_AVAILABLE_IN_TARGET_CELL,
    "radio_network_layer_no_radio_resources_available_in_target_cell"},
  {E_RADIO_NETWORK_LAYER_UNKNOWN_LOCAL_UE_NGAP_ID,
    "radio_network_layer_unknown_local_ue_ngap_id"},
  {E_RADIO_NETWORK_LAYER_INCONSISTENT_REMOTE_UE_NGAP_ID,
    "radio_network_layer_inconsistent_remote_ue_ngap_id"},
  {E_RADIO_NETWORK_LAYER_HANDOVER_DESIRABLE_FOR_RADIO_REASONS,
    "radio_network_layer_handover_desirable_for_radio_reasons"},
  {E_RADIO_NETWORK_LAYER_TIME_CRITICAL_HANDOVER,
    "radio_network_layer_time_critical_handover"},
  {E_RADIO_NETWORK_LAYER_RESOURCE_OPTIMISATION_HANDOVER,
    "radio_network_layer_resource_optimisation_handover"},
  {E_RADIO_NETWORK_LAYER_REDUCE_LOAD_IN_SERVING_CELL,
    "radio_network_layer_reduce_load_in_serving_cell"},
  {E_RADIO_NETWORK_LAYER_USER_INACTIVITY,
    "radio_network_layer_user_inactivity"},
  {E_RADIO_NETWORK_LAYER_RADIO_CONNECTION_WITH_UE_LOST,
    "radio_network_layer_radio_connection_with_ue_lost"},
  {E_RADIO_NETWORK_LAYER_RADIO_RESOURCES_NOT_AVAILABLE,
    "radio_network_layer_radio_resources_not_available"},
  {E_RADIO_NETWORK_LAYER_INVALID_QOS_COMBINATION,
    "radio_network_layer_invalid_qos_combination"},
  {E_RADIO_NETWORK_LAYER_FAILURE_IN_THE_RADIO_INTERFACE_PROCEDURE,
    "radio_network_layer_failure_in_the_radio_interface_procedure"},
  {E_RADIO_NETWORK_LAYER_INTERACTION_WITH_OTHER_PROCEDURE,
    "radio_network_layer_interaction_with_other_procedure"},
  {E_RADIO_NETWORK_LAYER_UNKNOWN_PDU_SESSION_ID,
    "radio_network_layer_unknown_pdu_session_id"},
  {E_RADIO_NETWORK_LAYER_UNKNOWN_QOS_FLOW_ID,
    "radio_network_layer_unknown_qos_flow_id"},
  {E_RADIO_NETWORK_LAYER_MULTIPLE_PDU_SESSION_ID_INSTANCES,
    "radio_network_layer_multiple_pdu_session_id_instances"},
  {E_RADIO_NETWORK_LAYER_MULTIPLE_QOS_FLOW_ID_INSTANCES,
    "radio_network_layer_multiple_qos_flow_id_instances"},
  {E_RADIO_NETWORK_LAYER_ENCRYPTION_AND_INTEGRITY_PROTECTION_ALGORITHMS_NOT_SUPPORTED,
    "radio_network_layer_encryption_and_integrity_protection_algorithms_not_supported"},
  {E_RADIO_NETWORK_LAYER_NG_INTRA_SYSTEM_HANDOVER_TRIGGERED,
    "radio_network_layer_ng_intra_system_handover_triggered"},
  {E_RADIO_NETWORK_LAYER_NG_INTER_SYSTEM_HANDOVER_TRIGGERED,
    "radio_network_layer_ng_inter_system_handover_triggered"},
  {E_RADIO_NETWORK_LAYER_XN_HANDOVER_TRIGGERED,
    "radio_network_layer_xn_handover_triggered"},
  {E_RADIO_NETWORK_LAYER_NOT_SUPPORTED_5QI_VALUE,
    "radio_network_layer_not_supported_5qi_value"},
  {E_RADIO_NETWORK_LAYER_UE_CONTEXT_TRANSFER,
    "radio_network_layer_ue_context_transfer"},
  {E_RADIO_NETWORK_LAYER_IMS_VOICE_EPS_FALLBACK_OR_RAT_FALLBACK_TRIGGERED,
    "radio_network_layer_ims_voice_eps_fallback_or_rat_fallback_triggered"},
  {E_RADIO_NETWORK_LAYER_UP_INTEGRITY_PROTECTION_NOT_POSSIBLE,
    "radio_network_layer_up_integrity_protection_not_possible"},
  {E_RADIO_NETWORK_LAYER_UP_CONFIDENTIALITY_PROTECTION_NOT_POSSIBLE,
    "radio_network_layer_up_confidentiality_protection_not_possible"},
  {E_RADIO_NETWORK_LAYER_SLICE_NOT_SUPPORTED,
    "radio_network_layer_slice_not_supported"},
  {E_RADIO_NETWORK_LAYER_UE_IN_RRC_INACTIVE_STATE_NOT_REACHABLE,
    "radio_network_layer_ue_in_rrc_inactive_state_not_reachable"},
  {E_RADIO_NETWORK_LAYER_REDIRECTION,
    "radio_network_layer_redirection"},
  {E_RADIO_NETWORK_LAYER_RESOURCES_NOT_AVAILABLE_FOR_THE_SLICES,
    "radio_network_layer_resources_not_available_for_the_slices"},
  {E_RADIO_NETWORK_LAYER_UE_MAXIMUM_INTEGRITY_PROTECTED_DATA_RATE_REASON,
    "radio_network_layer_ue_maximum_integrity_protected_data_rate_reason"},
  {E_RADIO_NETWORK_LAYER_RELEASE_DUE_TO_CN_DETECTED_MOBILITY,
    "radio_network_layer_release_due_to_cn_detected_mobility"},

  {E_TRANSPORT_LAYER_TRANSPORT_RESOURCE_UNAVAILABLE,
    "transport_layer_transport_resource_unavailable"},
  {E_TRANSPORT_LAYER_UNSPECIFIED,
    "transport_layer_unspecified"},

  {E_NAS_NORMAL_RELEASE,
    "nas_normal_release"},
  {E_NAS_AUTHENTICATION_FAILURE,
    "nas_authentication_failure"},
  {E_NAS_DEREGISTER,
    "nas_deregister"},
  {E_NAS_UNSPECIFIED,
    "nas_unspecified"},

  {E_PROTOCOL_TRANSFER_SYNTAX_ERROR,
    "protocol_transfer_syntax_error"},
  {E_PROTOCOL_ABSTRACT_SYNTAX_ERROR_REJECT,
    "protocol_abstract_syntax_error_reject"},
  {E_PROTOCOL_ABSTRACT_SYNTAX_ERROR_IGNORE_AND_NOTIFY,
    "protocol_abstract_syntax_error_ignore_and_notify"},
  {E_PROTOCOL_MESSAGE_NOT_COMPATIBLE_WITH_RECEIVER_STATE,
    "protocol_message_not_compatible_with_receiver_state"},
  {E_PROTOCOL_SEMANTIC_ERROR,
    "protocol_semantic_error"},
  {E_PROTOCOL_ABSTRACT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MESSAGE,
    "protocol_abstract_syntax_error_falsely_constructed_message"},
  {E_PROTOCOL_UNSPECIFIED,
    "protocol_unspecified"},

  {E_MISCELLANEOUS_CONTROL_PROCESSING_OVERLOAD,
    "miscellaneous_control_processing_overload"},
  {E_MISCELLANEOUS_NOT_ENOUGH_USER_PLANE_PROCESSING,
    "miscellaneous_not_enough_user_plane_processing"},
  {E_MISCELLANEOUS_RESOURCES,
    "miscellaneous_resources"},
  {E_MISCELLANEOUS_HARDWARE_FAILURE,
    "miscellaneous_hardware_failure"},
  {E_MISCELLANEOUS_OM_INTERVENTION,
    "miscellaneous_o&m_intervention"},
  {E_MISCELLANEOUS_UNKNOWN_PLMN,
    "miscellaneous_unknown_plmn"},
  {E_MISCELLANEOUS_UNSPECIFIED,
    "miscellaneous_unspecified"},

  {E_NGAP_UNKOWN_CAUSE,
    "ngap_unkown_cause"}
};

typedef enum causeRadioNetwork {
    E_CAUSERADIONETWORK_UNSPECIFIED = 0,
    E_CAUSERADIONETWORK_TXNRELOCOVERALL_EXPIRY,
    E_CAUSERADIONETWORK_SUCCESSFUL_HANDOVER,
    E_CAUSERADIONETWORK_RELEASE_DUE_TO_NGRAN_GENERATED_REASON,
    E_CAUSERADIONETWORK_RELEASE_DUE_TO_5GC_GENERATED_REASON,
    E_CAUSERADIONETWORK_HANDOVER_CANCELLED,
    E_CAUSERADIONETWORK_PARTIAL_HANDOVER,
    E_CAUSERADIONETWORK_HO_FAILURE_IN_TARGET_5GC_NGRAN_NODE_OR_TARGET_SYSTEM,
    E_CAUSERADIONETWORK_HO_TARGET_NOT_ALLOWED,
    E_CAUSERADIONETWORK_TNGRELOCOVERALL_EXPIRY,
    E_CAUSERADIONETWORK_TNGRELOCPREP_EXPIRY,
    E_CAUSERADIONETWORK_CELL_NOT_AVAILABLE,
    E_CAUSERADIONETWORK_UNKNOWN_TARGETID,
    E_CAUSERADIONETWORK_NO_RADIO_RESOURCES_AVAILABLE_IN_TARGET_CELL,
    E_CAUSERADIONETWORK_UNKNOWN_LOCAL_UE_NGAP_ID,
    E_CAUSERADIONETWORK_INCONSISTENT_REMOTE_UE_NGAP_ID,
    E_CAUSERADIONETWORK_HANDOVER_DESIRABLE_FOR_RADIO_REASON,
    E_CAUSERADIONETWORK_TIME_CRITICAL_HANDOVER,
    E_CAUSERADIONETWORK_RESOURCE_OPTIMISATION_HANDOVER,
    E_CAUSERADIONETWORK_REDUCE_LOAD_IN_SERVING_CELL,
    E_CAUSERADIONETWORK_USER_INACTIVITY,
    E_CAUSERADIONETWORK_RADIO_CONNECTION_WITH_UE_LOST,
    E_CAUSERADIONETWORK_RADIO_RESOURCES_NOT_AVAILABLE,
    E_CAUSERADIONETWORK_INVALID_QOS_COMBINATION,
    E_CAUSERADIONETWORK_FAILURE_IN_RADIO_INTERFACE_PROCEDURE,
    E_CAUSERADIONETWORK_INTERACTION_WITH_OTHER_PROCEDURE,
    E_CAUSERADIONETWORK_UNKNOWN_PDU_SESSION_ID,
    E_CAUSERADIONETWORK_UNKOWN_QOS_FLOW_ID,
    E_CAUSERADIONETWORK_MULTIPLE_PDU_SESSION_ID_INSTANCES,
    E_CAUSERADIONETWORK_MULTIPLE_QOS_FLOW_ID_INSTANCES,
    E_CAUSERADIONETWORK_ENCRYPTION_AND_OR_INTEGRITY_PROTECTION_ALGORITHMS_NOT_SUPPORTED,
    E_CAUSERADIONETWORK_NG_INTRA_SYSTEM_HANDOVER_TRIGGERED,
    E_CAUSERADIONETWORK_NG_INTER_SYSTEM_HANDOVER_TRIGGERED,
    E_CAUSERADIONETWORK_XN_HANDOVER_TRIGGERED,
    E_CAUSERADIONETWORK_NOT_SUPPORTED_5QI_VALUE,
    E_CAUSERADIONETWORK_UE_CONTEXT_TRANSFER,
    E_CAUSERADIONETWORK_IMS_VOICE_EPS_FALLBACK_OR_RAT_FALLBACK_TRIGGERED,
    E_CAUSERADIONETWORK_UP_INTEGRITY_PROTECTION_NOT_POSSIBLE,
    E_CAUSERADIONETWORK_UP_CONFIDENTIALITY_PROTECTION_NOT_POSSIBLE,
    E_CAUSERADIONETWORK_SLICE_NOT_SUPPORTED,
    E_CAUSERADIONETWORK_UE_IN_RRC_INACTIVE_STATE_NOT_REACHABLE,
    E_CAUSERADIONETWORK_REDIRECTION,
    E_CAUSERADIONETWORK_RESOURCES_NOT_AVAILABLE_FOR_THE_SLICE,
    E_CAUSERADIONETWORK_UE_MAX_INTEGRITY_PROTECTED_DATA_RATE_REASON,
    E_CAUSERADIONETWORK_RELEASE_DUE_TO_CN_DETECTED_MOBILITY
    /*,
     * Enumeration is extensible,
     */
} e_causeRadioNetwork_t;

typedef enum causeMisc {
    E_CAUSEMISC_CONTROL_PROCESSING_OVERLOAD = 0,
    E_CAUSEMISC_NOT_ENOUGH_USER_PLANE_PROCESSING_RESOURCES,
    E_CAUSEMISC_HARDWARE_FAILURE,
    E_CAUSEMISC_OM_INTERVENTION,
    E_CAUSEMISC_UNKNOWN_PLMN,
    E_CAUSEMISC_UNSPECIFIED,

} e_causeMisc_t;

typedef enum causeTransport {
    E_CAUSETRANSPORT_TRANSPORT_RESOURCE_UNAVAILABLE = 0,
    E_CAUSETRANSPORT_UNSPECIFIED,

} e_causeTransport_t;

typedef enum causeProtocol {
    E_CAUSEPROTOCOL_TRANSFER_SYNTAX_ERROR = 0,
    E_CAUSEPROTOCOL_ABSTRACT_SYNTAX_ERROR_REJECT,
    E_CAUSEPROTOCOL_ABSTRACT_SYNTAX_ERROR_IGNORE_AND_NOTIFY,
    E_CAUSEPROTOCOL_MESSAGE_NOT_COMPATIBLE_WITH_RECEIVER_STATE,
    E_CAUSEPROTOCOL_SEMANTIC_ERROR,
    E_CAUSEPROTOCOL_ABSTRACT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MESSAGE,
    E_CAUSEPROTOCOL_UNSPECIFIED,

} e_causeProtocol_t;

typedef enum causeNas {
    E_CAUSENAS_NORMAL_RELEASE = 0,
    E_CAUSENAS_AUTHENTICATION_FAILURE,
    E_CAUSENAS_DEREGISTER,
    E_CAUSENAS_UNSPECIFIED,

} e_causeNas_t;

typedef enum ngApCauseGroup {
    E_GP_RADIONETWORK,
    E_GP_TRANSPORT,
    E_GP_NAS,
    E_GP_PROTOCOL,
    E_GP_MISC,
    E_GP_NOTHING = -1 
} e_ngApCauseGroup_t;

typedef struct ngApCause {
        int group;
        int value;
}ngApCause_t;

static map<CauseRadioNetwork_t,e_causeRadioNetwork_t> radioNetworkMap = {
    {CauseRadioNetwork_unspecified,
    E_CAUSERADIONETWORK_UNSPECIFIED},
    {CauseRadioNetwork_txnrelocoverall_expiry,
    E_CAUSERADIONETWORK_TXNRELOCOVERALL_EXPIRY},
    {CauseRadioNetwork_txnrelocoverall_expiry,
    E_CAUSERADIONETWORK_TXNRELOCOVERALL_EXPIRY},
    {CauseRadioNetwork_successful_handover,
    E_CAUSERADIONETWORK_SUCCESSFUL_HANDOVER},
    {CauseRadioNetwork_release_due_to_ngran_generated_reason,
    E_CAUSERADIONETWORK_RELEASE_DUE_TO_NGRAN_GENERATED_REASON},
    {CauseRadioNetwork_release_due_to_5gc_generated_reason,
    E_CAUSERADIONETWORK_RELEASE_DUE_TO_5GC_GENERATED_REASON},
    {CauseRadioNetwork_handover_cancelled,
    E_CAUSERADIONETWORK_HANDOVER_CANCELLED},
    {CauseRadioNetwork_partial_handover,
    E_CAUSERADIONETWORK_PARTIAL_HANDOVER},
    {CauseRadioNetwork_ho_failure_in_target_5GC_ngran_node_or_target_system,
    E_CAUSERADIONETWORK_HO_FAILURE_IN_TARGET_5GC_NGRAN_NODE_OR_TARGET_SYSTEM},
    {CauseRadioNetwork_ho_target_not_allowed,
    E_CAUSERADIONETWORK_HO_TARGET_NOT_ALLOWED},
    {CauseRadioNetwork_tngrelocoverall_expiry,
    E_CAUSERADIONETWORK_TNGRELOCOVERALL_EXPIRY},
    {CauseRadioNetwork_tngrelocprep_expiry,
    E_CAUSERADIONETWORK_TNGRELOCPREP_EXPIRY},
    {CauseRadioNetwork_cell_not_available,
    E_CAUSERADIONETWORK_CELL_NOT_AVAILABLE},
    {CauseRadioNetwork_unknown_targetID,
    E_CAUSERADIONETWORK_UNKNOWN_TARGETID},
    {CauseRadioNetwork_no_radio_resources_available_in_target_cell,
    E_CAUSERADIONETWORK_NO_RADIO_RESOURCES_AVAILABLE_IN_TARGET_CELL},
    {CauseRadioNetwork_unknown_local_UE_NGAP_ID,
    E_CAUSERADIONETWORK_UNKNOWN_LOCAL_UE_NGAP_ID},
    {CauseRadioNetwork_inconsistent_remote_UE_NGAP_ID,
    E_CAUSERADIONETWORK_INCONSISTENT_REMOTE_UE_NGAP_ID},
    {CauseRadioNetwork_handover_desirable_for_radio_reason,
    E_CAUSERADIONETWORK_HANDOVER_DESIRABLE_FOR_RADIO_REASON},
    {CauseRadioNetwork_time_critical_handover,
    E_CAUSERADIONETWORK_TIME_CRITICAL_HANDOVER},
    {CauseRadioNetwork_resource_optimisation_handover,
    E_CAUSERADIONETWORK_RESOURCE_OPTIMISATION_HANDOVER},
    {CauseRadioNetwork_reduce_load_in_serving_cell,
    E_CAUSERADIONETWORK_REDUCE_LOAD_IN_SERVING_CELL},
    {CauseRadioNetwork_user_inactivity,
    E_CAUSERADIONETWORK_USER_INACTIVITY},
    {CauseRadioNetwork_radio_connection_with_ue_lost,
    E_CAUSERADIONETWORK_RADIO_CONNECTION_WITH_UE_LOST},
    {CauseRadioNetwork_radio_resources_not_available,
    E_CAUSERADIONETWORK_RADIO_RESOURCES_NOT_AVAILABLE},
    {CauseRadioNetwork_invalid_qos_combination,
    E_CAUSERADIONETWORK_INVALID_QOS_COMBINATION},
    {CauseRadioNetwork_failure_in_radio_interface_procedure,
    E_CAUSERADIONETWORK_FAILURE_IN_RADIO_INTERFACE_PROCEDURE},
    {CauseRadioNetwork_interaction_with_other_procedure,
    E_CAUSERADIONETWORK_INTERACTION_WITH_OTHER_PROCEDURE},
    {CauseRadioNetwork_unknown_PDU_session_ID,
    E_CAUSERADIONETWORK_UNKNOWN_PDU_SESSION_ID},
    {CauseRadioNetwork_unkown_qos_flow_ID,
    E_CAUSERADIONETWORK_UNKOWN_QOS_FLOW_ID},
    {CauseRadioNetwork_multiple_PDU_session_ID_instances,
    E_CAUSERADIONETWORK_MULTIPLE_PDU_SESSION_ID_INSTANCES},
    {CauseRadioNetwork_multiple_qos_flow_ID_instances,
    E_CAUSERADIONETWORK_MULTIPLE_QOS_FLOW_ID_INSTANCES},
    {CauseRadioNetwork_encryption_and_or_integrity_protection_algorithms_not_supported,
    E_CAUSERADIONETWORK_ENCRYPTION_AND_OR_INTEGRITY_PROTECTION_ALGORITHMS_NOT_SUPPORTED},
    {CauseRadioNetwork_ng_intra_system_handover_triggered,
    E_CAUSERADIONETWORK_NG_INTRA_SYSTEM_HANDOVER_TRIGGERED},
    {CauseRadioNetwork_ng_inter_system_handover_triggered,
    E_CAUSERADIONETWORK_NG_INTER_SYSTEM_HANDOVER_TRIGGERED},
    {CauseRadioNetwork_xn_handover_triggered,
    E_CAUSERADIONETWORK_XN_HANDOVER_TRIGGERED},
    {CauseRadioNetwork_not_supported_5QI_value,
    E_CAUSERADIONETWORK_NOT_SUPPORTED_5QI_VALUE},
    {CauseRadioNetwork_ue_context_transfer,
    E_CAUSERADIONETWORK_UE_CONTEXT_TRANSFER},
    {CauseRadioNetwork_ims_voice_eps_fallback_or_rat_fallback_triggered,
    E_CAUSERADIONETWORK_IMS_VOICE_EPS_FALLBACK_OR_RAT_FALLBACK_TRIGGERED},
    {CauseRadioNetwork_up_integrity_protection_not_possible,
    E_CAUSERADIONETWORK_UP_INTEGRITY_PROTECTION_NOT_POSSIBLE},
    {CauseRadioNetwork_up_confidentiality_protection_not_possible,
    E_CAUSERADIONETWORK_UP_CONFIDENTIALITY_PROTECTION_NOT_POSSIBLE},
    {CauseRadioNetwork_slice_not_supported,
    E_CAUSERADIONETWORK_SLICE_NOT_SUPPORTED},
    {CauseRadioNetwork_ue_in_rrc_inactive_state_not_reachable,
    E_CAUSERADIONETWORK_UE_IN_RRC_INACTIVE_STATE_NOT_REACHABLE},
    {CauseRadioNetwork_redirection,
    E_CAUSERADIONETWORK_REDIRECTION},
    {CauseRadioNetwork_resources_not_available_for_the_slice,
    E_CAUSERADIONETWORK_RESOURCES_NOT_AVAILABLE_FOR_THE_SLICE},
    {CauseRadioNetwork_ue_max_integrity_protected_data_rate_reason,
    E_CAUSERADIONETWORK_UE_MAX_INTEGRITY_PROTECTED_DATA_RATE_REASON},
    {CauseRadioNetwork_release_due_to_cn_detected_mobility,
    E_CAUSERADIONETWORK_RELEASE_DUE_TO_CN_DETECTED_MOBILITY},
};

static map<CauseMisc_t, e_causeMisc_t> causeMiscMap = {
    {CauseMisc_control_processing_overload,
    E_CAUSEMISC_CONTROL_PROCESSING_OVERLOAD},
    {CauseMisc_not_enough_user_plane_processing_resources,
    E_CAUSEMISC_NOT_ENOUGH_USER_PLANE_PROCESSING_RESOURCES},
    {CauseMisc_hardware_failure,
    E_CAUSEMISC_HARDWARE_FAILURE},
    {CauseMisc_om_intervention,
    E_CAUSEMISC_OM_INTERVENTION},
    {CauseMisc_unknown_PLMN,
    E_CAUSEMISC_UNKNOWN_PLMN},
    {CauseMisc_unspecified,
    E_CAUSEMISC_UNSPECIFIED},
};
static map<CauseNas_t, e_causeNas_t> causeNasMap = {
    {CauseNas_normal_release,
    E_CAUSENAS_NORMAL_RELEASE},
    {CauseNas_authentication_failure,
    E_CAUSENAS_AUTHENTICATION_FAILURE},
    {CauseNas_deregister,
    E_CAUSENAS_DEREGISTER},
    {CauseNas_unspecified,
    E_CAUSENAS_UNSPECIFIED},
};

static map<CauseTransport_t, e_causeTransport_t> causeTransportMap = {
    {CauseTransport_transport_resource_unavailable,
    E_CAUSETRANSPORT_TRANSPORT_RESOURCE_UNAVAILABLE},
    {CauseTransport_unspecified,
    E_CAUSETRANSPORT_UNSPECIFIED},
};

static map<CauseProtocol_t, e_causeProtocol_t> causeProtocolMap = {
    {CauseProtocol_transfer_syntax_error,
    E_CAUSEPROTOCOL_TRANSFER_SYNTAX_ERROR},
    {CauseProtocol_abstract_syntax_error_reject,
    E_CAUSEPROTOCOL_ABSTRACT_SYNTAX_ERROR_REJECT},
    {CauseProtocol_abstract_syntax_error_ignore_and_notify,
    E_CAUSEPROTOCOL_ABSTRACT_SYNTAX_ERROR_IGNORE_AND_NOTIFY},
    {CauseProtocol_message_not_compatible_with_receiver_state,
    E_CAUSEPROTOCOL_MESSAGE_NOT_COMPATIBLE_WITH_RECEIVER_STATE},
    {CauseProtocol_semantic_error,
    E_CAUSEPROTOCOL_SEMANTIC_ERROR},
    {CauseProtocol_abstract_syntax_error_falsely_constructed_message,
    E_CAUSEPROTOCOL_ABSTRACT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MESSAGE},
    {CauseProtocol_unspecified,
    E_CAUSEPROTOCOL_UNSPECIFIED},
};


enum {
    E_NGSETUP_ERROR_UNKNOWN             =  E_NGAP_UNKOWN_CAUSE,    

    E_NGSETUP_ERROR_SLICE_MISMATCH      =  E_MISCELLANEOUS_UNSPECIFIED,
    E_NGSETUP_ERROR_UNKNOWN_PLMN        =  E_MISCELLANEOUS_UNKNOWN_PLMN
};

int decodeNgapPdu(NGAP_PDU_t **ngapPdu_p, void * blob, int blobLen);
int decodePduResSetupResTrnsfr(uint8_t* n2SmMessage);
int ngapGetProcedureCode(NGAP_PDU_t* ngapPdu);
void ngapFree(NGAP_PDU_t *ngapPdu);
void asnFree(PDUSessionResourceModifyResponseTransfer_t **ptr);
#define asnLog(td, struct_ptr)
#if (LOGGING_BITMASK & 0b001) == 1
#undef asnLog
#define asnLog(td, struct_ptr)          \
    __asnLog(td, struct_ptr, __FILENAME__, __LINE__, __func__)
#endif

/* all the below functions are in the following format:
 *        something* ngapGetSomething(NGAP_PDU_t*)
 * will return pointer to value if possible  OR  NULL on failure
 */
RAN_UE_NGAP_ID_t* ngapGetRanUeNgapId(NGAP_PDU_t *ngapPdu);
RRCEstablishmentCause_t* ngapGetRRCEstbCause(NGAP_PDU_t *ngapPdu);
FiveG_S_TMSI_t* ngapGetFiveGSTMSI(NGAP_PDU_t *ngapPdu);
AMF_UE_NGAP_ID_t* ngapGetAmfUeNgapId(NGAP_PDU_t *ngapPdu);
NAS_PDU_t* ngapGetNasPdu(NGAP_PDU_t *ngapPdu);
GlobalRANNodeID_t* ngapGetGlobalRANNodeId(NGAP_PDU_t *ngapPdu);
SupportedTAList_t* ngapGetSupportedTAList(NGAP_PDU_t* ngapPdu);
RANNodeName_t* ngapGetRANNodeName(NGAP_PDU_t* ngapPdu);
PDUSessionResourceSetupListSURes_t*
        ngapGetpduSessionResourceSetupListSURes(NGAP_PDU_t *ngapPdu);
Cause_t* ngapGetCause(NGAP_PDU_t *ngapPdu);
GUAMI_t* ngapGetGuami(NGAP_PDU_t* ngapPdu);
UESecurityCapabilities_t* ngapGetUeSecurityCapabilities(NGAP_PDU_t* ngapPdu);
UE_NGAP_IDs_t * ngapGetUeNgapIDs(NGAP_PDU_t *ngapPdu);
PDUSessionResourceToReleaseListRelCmd_t*
        PDUSessResToRelListRelCmd(NGAP_PDU_t *ngapPdu);
PDUSessionResourceSetupListSUReq_t*
        ngapGetPDUSessionResourceSetupListSUReq(NGAP_PDU_t *ngapPdu);
PDUSessionResourceSetupListSURes_t*
        ngapGetPDUSessionResourceSetupListSURes(NGAP_PDU_t *ngapPdu);
PDUSessionResourceReleasedListRelRes_t *
        ngapGetPDUSessionResourceReleasedListRelRes(NGAP_PDU_t *ngapPdu);
PDUSessionResourceListCxtRelReq_t*
ngapGetPDUSessionResourceListCxtRelReq(NGAP_PDU_t *ngapPdu);
PDUSessionResourceSetupListCxtReq_t*
        ngapGetPDUSessionResourceSetupListCxtReq(NGAP_PDU_t *ngapPdu);
PDUSessionResourceSetupListCxtRes_t*
        ngapGetPDUSessionResourceSetupListCxtRes(NGAP_PDU_t *ngapPdu);
UEPagingIdentity_t* ngapGetUePagingIdentity(NGAP_PDU_t *ngapPdu);
TAIListForPaging_t* ngapGetTAIListForPaging(NGAP_PDU_t *ngapPdu);
UserLocationInformation_t* ngapGetUserLocationInfo(NGAP_PDU_t *ngapPdu);
PDUSessionResourceModifyListModReq_t*
        ngapGetPDUSessionResourceModifyListModReq(NGAP_PDU_t *ngapPdu);
PDUSessionResourceModifyListModRes_t*                                 
        ngapGetPDUSessionResourceModifyListModRes(NGAP_PDU_t *ngapPdu);

Cause_PR getCausePR(ngap_error_cause_t errNum);
int setFailureCause(Cause_t &Cause, int errNum);
int getNgApCause(Cause_t *cause, ngApCause_t *ngApCause);
int getCause(ngApCause_t ngapCause, Cause_t* cause);



void __asnLog(const asn_TYPE_descriptor_t *td, const void *struct_ptr,
              const char* file, int line, const char* func);


#endif

