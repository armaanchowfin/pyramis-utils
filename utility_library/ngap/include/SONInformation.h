/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_SONInformation_H_
#define	_SONInformation_H_


#include <asn_application.h>



#include "SONInformationRequest.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum SONInformation_PR {
	SONInformation_PR_NOTHING,	

	SONInformation_PR_sONInformationRequest,
	SONInformation_PR_sONInformationReply,
	SONInformation_PR_choice_Extensions
} SONInformation_PR;



struct SONInformationReply;
struct ProtocolIE_SingleContainer;



typedef struct SONInformation {
	SONInformation_PR present;
	union SONInformation_u {
		SONInformationRequest_t	 sONInformationRequest;
		struct SONInformationReply	*sONInformationReply;
		struct ProtocolIE_SingleContainer	*choice_Extensions;
	} choice;
	
	

	asn_struct_ctx_t _asn_ctx;
} SONInformation_t;



extern asn_TYPE_descriptor_t asn_DEF_SONInformation;
extern asn_CHOICE_specifics_t asn_SPC_SONInformation_specs_1;
extern asn_TYPE_member_t asn_MBR_SONInformation_1[3];
extern asn_per_constraints_t asn_PER_type_SONInformation_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
