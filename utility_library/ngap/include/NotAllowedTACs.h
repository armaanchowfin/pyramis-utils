/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_NotAllowedTACs_H_
#define	_NotAllowedTACs_H_


#include <asn_application.h>



#include "TAC.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct NotAllowedTACs {
	A_SEQUENCE_OF(TAC_t) list;
	
	

	asn_struct_ctx_t _asn_ctx;
} NotAllowedTACs_t;



extern asn_TYPE_descriptor_t asn_DEF_NotAllowedTACs;
extern asn_SET_OF_specifics_t asn_SPC_NotAllowedTACs_specs_1;
extern asn_TYPE_member_t asn_MBR_NotAllowedTACs_1[1];
extern asn_per_constraints_t asn_PER_type_NotAllowedTACs_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
