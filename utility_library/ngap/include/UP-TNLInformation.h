/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UP_TNLInformation_H_
#define	_UP_TNLInformation_H_


#include <asn_application.h>



#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum UP_TNLInformation_PR {
	UP_TNLInformation_PR_NOTHING,	

	UP_TNLInformation_PR_singleTNLInformation,
	UP_TNLInformation_PR_multipleTNLInformation,
	UP_TNLInformation_PR_choice_Extensions
} UP_TNLInformation_PR;



struct SingleTNLInformation;
struct MultipleTNLInformation;
struct ProtocolIE_SingleContainer;



typedef struct UP_TNLInformation {
	UP_TNLInformation_PR present;
	union UP_TNLInformation_u {
		struct SingleTNLInformation	*singleTNLInformation;
		struct MultipleTNLInformation	*multipleTNLInformation;
		struct ProtocolIE_SingleContainer	*choice_Extensions;
	} choice;
	
	

	asn_struct_ctx_t _asn_ctx;
} UP_TNLInformation_t;



extern asn_TYPE_descriptor_t asn_DEF_UP_TNLInformation;
extern asn_CHOICE_specifics_t asn_SPC_UP_TNLInformation_specs_1;
extern asn_TYPE_member_t asn_MBR_UP_TNLInformation_1[3];
extern asn_per_constraints_t asn_PER_type_UP_TNLInformation_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>