/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_DRBsSubjectToStatusTransferList_H_
#define	_DRBsSubjectToStatusTransferList_H_


#include <asn_application.h>



#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif



struct DRBsSubjectToStatusTransferItem;



typedef struct DRBsSubjectToStatusTransferList {
	A_SEQUENCE_OF(struct DRBsSubjectToStatusTransferItem) list;
	
	

	asn_struct_ctx_t _asn_ctx;
} DRBsSubjectToStatusTransferList_t;



extern asn_TYPE_descriptor_t asn_DEF_DRBsSubjectToStatusTransferList;
extern asn_SET_OF_specifics_t asn_SPC_DRBsSubjectToStatusTransferList_specs_1;
extern asn_TYPE_member_t asn_MBR_DRBsSubjectToStatusTransferList_1[1];
extern asn_per_constraints_t asn_PER_type_DRBsSubjectToStatusTransferList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
