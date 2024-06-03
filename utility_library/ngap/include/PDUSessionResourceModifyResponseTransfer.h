/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_PDUSessionResourceModifyResponseTransfer_H_
#define	_PDUSessionResourceModifyResponseTransfer_H_


#include <asn_application.h>
#include <QosFlowAddOrModifyResponseList.h>



#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct UPTransportLayerInformation;
struct QosFlowAddOrModifyResponseList;
struct QosFlowPerTNLInformation;
struct QosFlowList;
struct ProtocolExtensionContainer;



typedef struct PDUSessionResourceModifyResponseTransfer {
	struct UPTransportLayerInformation	*dL_NGU_UP_TNLInformation;	

	struct UPTransportLayerInformation	*uL_NGU_UP_TNLInformation;	

	struct QosFlowAddOrModifyResponseList	*qosFlowAddOrModifyResponseList;	

	struct QosFlowPerTNLInformation	*additionalQosFlowPerTNLInformation;	

	struct QosFlowList	*qosFlowFailedToAddOrModifyList;	

	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} PDUSessionResourceModifyResponseTransfer_t;



extern asn_TYPE_descriptor_t asn_DEF_PDUSessionResourceModifyResponseTransfer;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
