/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_PDUSessionResourceInformationItem_H_
#define	_PDUSessionResourceInformationItem_H_


#include <asn_application.h>



#include "PDUSessionID.h"
#include "QosFlowInformationList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct DRBsToQosFlowsMappingList;
struct ProtocolExtensionContainer;



typedef struct PDUSessionResourceInformationItem {
	PDUSessionID_t	 pDUSessionID;
	QosFlowInformationList_t	 qosFlowInformationList;
	struct DRBsToQosFlowsMappingList	*dRBsToQosFlowsMappingList;	

	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} PDUSessionResourceInformationItem_t;



extern asn_TYPE_descriptor_t asn_DEF_PDUSessionResourceInformationItem;
extern asn_SEQUENCE_specifics_t asn_SPC_PDUSessionResourceInformationItem_specs_1;
extern asn_TYPE_member_t asn_MBR_PDUSessionResourceInformationItem_1[4];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
