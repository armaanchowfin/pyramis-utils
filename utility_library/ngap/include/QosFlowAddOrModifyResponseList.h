/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_QosFlowAddOrModifyResponseList_H_
#define	_QosFlowAddOrModifyResponseList_H_


#include <asn_application.h>



#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif



struct QosFlowAddOrModifyResponseItem;



typedef struct QosFlowAddOrModifyResponseList {
	A_SEQUENCE_OF(struct QosFlowAddOrModifyResponseItem) list;
	
	

	asn_struct_ctx_t _asn_ctx;
} QosFlowAddOrModifyResponseList_t;



extern asn_TYPE_descriptor_t asn_DEF_QosFlowAddOrModifyResponseList;
extern asn_SET_OF_specifics_t asn_SPC_QosFlowAddOrModifyResponseList_specs_1;
extern asn_TYPE_member_t asn_MBR_QosFlowAddOrModifyResponseList_1[1];
extern asn_per_constraints_t asn_PER_type_QosFlowAddOrModifyResponseList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>