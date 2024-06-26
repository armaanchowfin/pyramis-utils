/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_QosCharacteristics_H_
#define	_QosCharacteristics_H_


#include <asn_application.h>



#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum QosCharacteristics_PR {
	QosCharacteristics_PR_NOTHING,	

	QosCharacteristics_PR_nonDynamic5QI,
	QosCharacteristics_PR_dynamic5QI,
	QosCharacteristics_PR_choice_Extensions
} QosCharacteristics_PR;



struct NonDynamic5QIDescriptor;
struct Dynamic5QIDescriptor;
struct ProtocolIE_SingleContainer;



typedef struct QosCharacteristics {
	QosCharacteristics_PR present;
	union QosCharacteristics_u {
		struct NonDynamic5QIDescriptor	*nonDynamic5QI;
		struct Dynamic5QIDescriptor	*dynamic5QI;
		struct ProtocolIE_SingleContainer	*choice_Extensions;
	} choice;
	
	

	asn_struct_ctx_t _asn_ctx;
} QosCharacteristics_t;



extern asn_TYPE_descriptor_t asn_DEF_QosCharacteristics;
extern asn_CHOICE_specifics_t asn_SPC_QosCharacteristics_specs_1;
extern asn_TYPE_member_t asn_MBR_QosCharacteristics_1[3];
extern asn_per_constraints_t asn_PER_type_QosCharacteristics_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
