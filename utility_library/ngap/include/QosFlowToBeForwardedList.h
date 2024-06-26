/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_QosFlowToBeForwardedList_H_
#define	_QosFlowToBeForwardedList_H_


#include <asn_application.h>



#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif



struct QosFlowToBeForwardedItem;



typedef struct QosFlowToBeForwardedList {
	A_SEQUENCE_OF(struct QosFlowToBeForwardedItem) list;
	
	

	asn_struct_ctx_t _asn_ctx;
} QosFlowToBeForwardedList_t;



extern asn_TYPE_descriptor_t asn_DEF_QosFlowToBeForwardedList;
extern asn_SET_OF_specifics_t asn_SPC_QosFlowToBeForwardedList_specs_1;
extern asn_TYPE_member_t asn_MBR_QosFlowToBeForwardedList_1[1];
extern asn_per_constraints_t asn_PER_type_QosFlowToBeForwardedList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
