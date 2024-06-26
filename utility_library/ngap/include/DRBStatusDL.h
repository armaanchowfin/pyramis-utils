/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_DRBStatusDL_H_
#define	_DRBStatusDL_H_


#include <asn_application.h>



#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum DRBStatusDL_PR {
	DRBStatusDL_PR_NOTHING,	

	DRBStatusDL_PR_dRBStatusDL12,
	DRBStatusDL_PR_dRBStatusDL18,
	DRBStatusDL_PR_choice_Extensions
} DRBStatusDL_PR;



struct DRBStatusDL12;
struct DRBStatusDL18;
struct ProtocolIE_SingleContainer;



typedef struct DRBStatusDL {
	DRBStatusDL_PR present;
	union DRBStatusDL_u {
		struct DRBStatusDL12	*dRBStatusDL12;
		struct DRBStatusDL18	*dRBStatusDL18;
		struct ProtocolIE_SingleContainer	*choice_Extensions;
	} choice;
	
	

	asn_struct_ctx_t _asn_ctx;
} DRBStatusDL_t;



extern asn_TYPE_descriptor_t asn_DEF_DRBStatusDL;
extern asn_CHOICE_specifics_t asn_SPC_DRBStatusDL_specs_1;
extern asn_TYPE_member_t asn_MBR_DRBStatusDL_1[3];
extern asn_per_constraints_t asn_PER_type_DRBStatusDL_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
