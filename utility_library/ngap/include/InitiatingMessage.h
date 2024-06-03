/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-PDU-Descriptions"
 * 	found in "ElementaryProcedureDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_InitiatingMessage_H_
#define	_InitiatingMessage_H_


#include <asn_application.h>



#include "ProcedureCode.h"
#include "Criticality.h"
#include <ANY.h>
#include <asn_ioc.h>
#include "AMFConfigurationUpdate.h"
#include "AMFConfigurationUpdateAcknowledge.h"
#include "AMFConfigurationUpdateFailure.h"
#include "HandoverCancel.h"
#include "HandoverCancelAcknowledge.h"
#include "HandoverRequired.h"
#include "HandoverCommand.h"
#include "HandoverPreparationFailure.h"
#include "HandoverRequest.h"
#include "HandoverRequestAcknowledge.h"
#include "HandoverFailure.h"
#include "InitialContextSetupRequest.h"
#include "InitialContextSetupResponse.h"
#include "InitialContextSetupFailure.h"
#include "NGReset.h"
#include "NGResetAcknowledge.h"
#include "NGSetupRequest.h"
#include "NGSetupResponse.h"
#include "NGSetupFailure.h"
#include "PathSwitchRequest.h"
#include "PathSwitchRequestAcknowledge.h"
#include "PathSwitchRequestFailure.h"
#include "PDUSessionResourceModifyRequest.h"
#include "PDUSessionResourceModifyResponse.h"
#include "PDUSessionResourceModifyIndication.h"
#include "PDUSessionResourceModifyConfirm.h"
#include "PDUSessionResourceReleaseCommand.h"
#include "PDUSessionResourceReleaseResponse.h"
#include "PDUSessionResourceSetupRequest.h"
#include "PDUSessionResourceSetupResponse.h"
#include "PWSCancelRequest.h"
#include "PWSCancelResponse.h"
#include "RANConfigurationUpdate.h"
#include "RANConfigurationUpdateAcknowledge.h"
#include "RANConfigurationUpdateFailure.h"
#include "UEContextModificationRequest.h"
#include "UEContextModificationResponse.h"
#include "UEContextModificationFailure.h"
#include "UEContextReleaseCommand.h"
#include "UEContextReleaseComplete.h"
#include "UERadioCapabilityCheckRequest.h"
#include "UERadioCapabilityCheckResponse.h"
#include "WriteReplaceWarningRequest.h"
#include "WriteReplaceWarningResponse.h"
#include "AMFStatusIndication.h"
#include "CellTrafficTrace.h"
#include "DeactivateTrace.h"
#include "DownlinkNASTransport.h"
#include "DownlinkNonUEAssociatedNRPPaTransport.h"
#include "DownlinkRANConfigurationTransfer.h"
#include "DownlinkRANStatusTransfer.h"
#include "DownlinkUEAssociatedNRPPaTransport.h"
#include "ErrorIndication.h"
#include "HandoverNotify.h"
#include "InitialUEMessage.h"
#include "LocationReport.h"
#include "LocationReportingControl.h"
#include "LocationReportingFailureIndication.h"
#include "NASNonDeliveryIndication.h"
#include "OverloadStart.h"
#include "OverloadStop.h"
#include "Paging.h"
#include "PDUSessionResourceNotify.h"
#include "PrivateMessage.h"
#include "PWSFailureIndication.h"
#include "PWSRestartIndication.h"
#include "RerouteNASRequest.h"
#include "RRCInactiveTransitionReport.h"
#include "TraceFailureIndication.h"
#include "TraceStart.h"
#include "UEContextReleaseRequest.h"
#include "UERadioCapabilityInfoIndication.h"
#include "UETNLABindingReleaseRequest.h"
#include "UplinkNASTransport.h"
#include "UplinkNonUEAssociatedNRPPaTransport.h"
#include "UplinkRANConfigurationTransfer.h"
#include "UplinkRANStatusTransfer.h"
#include "UplinkUEAssociatedNRPPaTransport.h"
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum InitiatingMessage__value_PR {
	InitiatingMessage__value_PR_NOTHING,	

	InitiatingMessage__value_PR_AMFConfigurationUpdate,
	InitiatingMessage__value_PR_HandoverCancel,
	InitiatingMessage__value_PR_HandoverRequired,
	InitiatingMessage__value_PR_HandoverRequest,
	InitiatingMessage__value_PR_InitialContextSetupRequest,
	InitiatingMessage__value_PR_NGReset,
	InitiatingMessage__value_PR_NGSetupRequest,
	InitiatingMessage__value_PR_PathSwitchRequest,
	InitiatingMessage__value_PR_PDUSessionResourceModifyRequest,
	InitiatingMessage__value_PR_PDUSessionResourceModifyIndication,
	InitiatingMessage__value_PR_PDUSessionResourceReleaseCommand,
	InitiatingMessage__value_PR_PDUSessionResourceSetupRequest,
	InitiatingMessage__value_PR_PWSCancelRequest,
	InitiatingMessage__value_PR_RANConfigurationUpdate,
	InitiatingMessage__value_PR_UEContextModificationRequest,
	InitiatingMessage__value_PR_UEContextReleaseCommand,
	InitiatingMessage__value_PR_UERadioCapabilityCheckRequest,
	InitiatingMessage__value_PR_WriteReplaceWarningRequest,
	InitiatingMessage__value_PR_AMFStatusIndication,
	InitiatingMessage__value_PR_CellTrafficTrace,
	InitiatingMessage__value_PR_DeactivateTrace,
	InitiatingMessage__value_PR_DownlinkNASTransport,
	InitiatingMessage__value_PR_DownlinkNonUEAssociatedNRPPaTransport,
	InitiatingMessage__value_PR_DownlinkRANConfigurationTransfer,
	InitiatingMessage__value_PR_DownlinkRANStatusTransfer,
	InitiatingMessage__value_PR_DownlinkUEAssociatedNRPPaTransport,
	InitiatingMessage__value_PR_ErrorIndication,
	InitiatingMessage__value_PR_HandoverNotify,
	InitiatingMessage__value_PR_InitialUEMessage,
	InitiatingMessage__value_PR_LocationReport,
	InitiatingMessage__value_PR_LocationReportingControl,
	InitiatingMessage__value_PR_LocationReportingFailureIndication,
	InitiatingMessage__value_PR_NASNonDeliveryIndication,
	InitiatingMessage__value_PR_OverloadStart,
	InitiatingMessage__value_PR_OverloadStop,
	InitiatingMessage__value_PR_Paging,
	InitiatingMessage__value_PR_PDUSessionResourceNotify,
	InitiatingMessage__value_PR_PrivateMessage,
	InitiatingMessage__value_PR_PWSFailureIndication,
	InitiatingMessage__value_PR_PWSRestartIndication,
	InitiatingMessage__value_PR_RerouteNASRequest,
	InitiatingMessage__value_PR_RRCInactiveTransitionReport,
	InitiatingMessage__value_PR_TraceFailureIndication,
	InitiatingMessage__value_PR_TraceStart,
	InitiatingMessage__value_PR_UEContextReleaseRequest,
	InitiatingMessage__value_PR_UERadioCapabilityInfoIndication,
	InitiatingMessage__value_PR_UETNLABindingReleaseRequest,
	InitiatingMessage__value_PR_UplinkNASTransport,
	InitiatingMessage__value_PR_UplinkNonUEAssociatedNRPPaTransport,
	InitiatingMessage__value_PR_UplinkRANConfigurationTransfer,
	InitiatingMessage__value_PR_UplinkRANStatusTransfer,
	InitiatingMessage__value_PR_UplinkUEAssociatedNRPPaTransport
} InitiatingMessage__value_PR;



typedef struct InitiatingMessage {
	ProcedureCode_t	 procedureCode;
	Criticality_t	 criticality;
	struct InitiatingMessage__value {
		InitiatingMessage__value_PR present;
		union InitiatingMessage__value_u {
			AMFConfigurationUpdate_t	 AMFConfigurationUpdate;
			HandoverCancel_t	 HandoverCancel;
			HandoverRequired_t	 HandoverRequired;
			HandoverRequest_t	 HandoverRequest;
			InitialContextSetupRequest_t	 InitialContextSetupRequest;
			NGReset_t	 NGReset;
			NGSetupRequest_t	 NGSetupRequest;
			PathSwitchRequest_t	 PathSwitchRequest;
			PDUSessionResourceModifyRequest_t	 PDUSessionResourceModifyRequest;
			PDUSessionResourceModifyIndication_t	 PDUSessionResourceModifyIndication;
			PDUSessionResourceReleaseCommand_t	 PDUSessionResourceReleaseCommand;
			PDUSessionResourceSetupRequest_t	 PDUSessionResourceSetupRequest;
			PWSCancelRequest_t	 PWSCancelRequest;
			RANConfigurationUpdate_t	 RANConfigurationUpdate;
			UEContextModificationRequest_t	 UEContextModificationRequest;
			UEContextReleaseCommand_t	 UEContextReleaseCommand;
			UERadioCapabilityCheckRequest_t	 UERadioCapabilityCheckRequest;
			WriteReplaceWarningRequest_t	 WriteReplaceWarningRequest;
			AMFStatusIndication_t	 AMFStatusIndication;
			CellTrafficTrace_t	 CellTrafficTrace;
			DeactivateTrace_t	 DeactivateTrace;
			DownlinkNASTransport_t	 DownlinkNASTransport;
			DownlinkNonUEAssociatedNRPPaTransport_t	 DownlinkNonUEAssociatedNRPPaTransport;
			DownlinkRANConfigurationTransfer_t	 DownlinkRANConfigurationTransfer;
			DownlinkRANStatusTransfer_t	 DownlinkRANStatusTransfer;
			DownlinkUEAssociatedNRPPaTransport_t	 DownlinkUEAssociatedNRPPaTransport;
			ErrorIndication_t	 ErrorIndication;
			HandoverNotify_t	 HandoverNotify;
			InitialUEMessage_t	 InitialUEMessage;
			LocationReport_t	 LocationReport;
			LocationReportingControl_t	 LocationReportingControl;
			LocationReportingFailureIndication_t	 LocationReportingFailureIndication;
			NASNonDeliveryIndication_t	 NASNonDeliveryIndication;
			OverloadStart_t	 OverloadStart;
			OverloadStop_t	 OverloadStop;
			Paging_t	 Paging;
			PDUSessionResourceNotify_t	 PDUSessionResourceNotify;
			PrivateMessage_t	 PrivateMessage;
			PWSFailureIndication_t	 PWSFailureIndication;
			PWSRestartIndication_t	 PWSRestartIndication;
			RerouteNASRequest_t	 RerouteNASRequest;
			RRCInactiveTransitionReport_t	 RRCInactiveTransitionReport;
			TraceFailureIndication_t	 TraceFailureIndication;
			TraceStart_t	 TraceStart;
			UEContextReleaseRequest_t	 UEContextReleaseRequest;
			UERadioCapabilityInfoIndication_t	 UERadioCapabilityInfoIndication;
			UETNLABindingReleaseRequest_t	 UETNLABindingReleaseRequest;
			UplinkNASTransport_t	 UplinkNASTransport;
			UplinkNonUEAssociatedNRPPaTransport_t	 UplinkNonUEAssociatedNRPPaTransport;
			UplinkRANConfigurationTransfer_t	 UplinkRANConfigurationTransfer;
			UplinkRANStatusTransfer_t	 UplinkRANStatusTransfer;
			UplinkUEAssociatedNRPPaTransport_t	 UplinkUEAssociatedNRPPaTransport;
		} choice;
		
		

		asn_struct_ctx_t _asn_ctx;
	} value;
	
	

	asn_struct_ctx_t _asn_ctx;
} InitiatingMessage_t;



extern asn_TYPE_descriptor_t asn_DEF_InitiatingMessage;
extern asn_SEQUENCE_specifics_t asn_SPC_InitiatingMessage_specs_1;
extern asn_TYPE_member_t asn_MBR_InitiatingMessage_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>