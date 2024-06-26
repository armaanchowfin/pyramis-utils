/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_CellIDListForRestart_H_
#define	_CellIDListForRestart_H_


#include <asn_application.h>



#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum CellIDListForRestart_PR {
	CellIDListForRestart_PR_NOTHING,	

	CellIDListForRestart_PR_eUTRA_CGIListforRestart,
	CellIDListForRestart_PR_nR_CGIListforRestart,
	CellIDListForRestart_PR_choice_Extensions
} CellIDListForRestart_PR;



struct EUTRA_CGIList;
struct NR_CGIList;
struct ProtocolIE_SingleContainer;



typedef struct CellIDListForRestart {
	CellIDListForRestart_PR present;
	union CellIDListForRestart_u {
		struct EUTRA_CGIList	*eUTRA_CGIListforRestart;
		struct NR_CGIList	*nR_CGIListforRestart;
		struct ProtocolIE_SingleContainer	*choice_Extensions;
	} choice;
	
	

	asn_struct_ctx_t _asn_ctx;
} CellIDListForRestart_t;



extern asn_TYPE_descriptor_t asn_DEF_CellIDListForRestart;
extern asn_CHOICE_specifics_t asn_SPC_CellIDListForRestart_specs_1;
extern asn_TYPE_member_t asn_MBR_CellIDListForRestart_1[3];
extern asn_per_constraints_t asn_PER_type_CellIDListForRestart_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
