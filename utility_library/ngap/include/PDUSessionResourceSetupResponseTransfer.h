/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_PDUSessionResourceSetupResponseTransfer_H_
#define	_PDUSessionResourceSetupResponseTransfer_H_


#include <asn_application.h>



#include "QosFlowPerTNLInformation.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct QosFlowPerTNLInformation;
struct SecurityResult;
struct QosFlowList;
struct ProtocolExtensionContainer;



typedef struct PDUSessionResourceSetupResponseTransfer {
	QosFlowPerTNLInformation_t	 qosFlowPerTNLInformation;
	struct QosFlowPerTNLInformation	*additionalQosFlowPerTNLInformation;	

	struct SecurityResult	*securityResult;	

	struct QosFlowList	*qosFlowFailedToSetupList;	

	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} PDUSessionResourceSetupResponseTransfer_t;



extern asn_TYPE_descriptor_t asn_DEF_PDUSessionResourceSetupResponseTransfer;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
