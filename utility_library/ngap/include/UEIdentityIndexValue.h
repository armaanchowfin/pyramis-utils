/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UEIdentityIndexValue_H_
#define	_UEIdentityIndexValue_H_


#include <asn_application.h>



#include <BIT_STRING.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum UEIdentityIndexValue_PR {
	UEIdentityIndexValue_PR_NOTHING,	

	UEIdentityIndexValue_PR_indexLength10,
	UEIdentityIndexValue_PR_choice_Extensions
} UEIdentityIndexValue_PR;



struct ProtocolIE_SingleContainer;



typedef struct UEIdentityIndexValue {
	UEIdentityIndexValue_PR present;
	union UEIdentityIndexValue_u {
		BIT_STRING_t	 indexLength10;
		struct ProtocolIE_SingleContainer	*choice_Extensions;
	} choice;
	
	

	asn_struct_ctx_t _asn_ctx;
} UEIdentityIndexValue_t;



extern asn_TYPE_descriptor_t asn_DEF_UEIdentityIndexValue;
extern asn_CHOICE_specifics_t asn_SPC_UEIdentityIndexValue_specs_1;
extern asn_TYPE_member_t asn_MBR_UEIdentityIndexValue_1[2];
extern asn_per_constraints_t asn_PER_type_UEIdentityIndexValue_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>