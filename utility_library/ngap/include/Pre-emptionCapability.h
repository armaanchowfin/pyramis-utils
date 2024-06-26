/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_Pre_emptionCapability_H_
#define	_Pre_emptionCapability_H_


#include <asn_application.h>



#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum Pre_emptionCapability {
	Pre_emptionCapability_shall_not_trigger_pre_emption	= 0,
	Pre_emptionCapability_may_trigger_pre_emption	= 1
	/*
	 * Enumeration is extensible
	 */
} e_Pre_emptionCapability;



typedef long	 Pre_emptionCapability_t;



extern asn_per_constraints_t asn_PER_type_Pre_emptionCapability_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_Pre_emptionCapability;
extern const asn_INTEGER_specifics_t asn_SPC_Pre_emptionCapability_specs_1;
asn_struct_free_f Pre_emptionCapability_free;
asn_struct_print_f Pre_emptionCapability_print;
asn_constr_check_f Pre_emptionCapability_constraint;
ber_type_decoder_f Pre_emptionCapability_decode_ber;
der_type_encoder_f Pre_emptionCapability_encode_der;
xer_type_decoder_f Pre_emptionCapability_decode_xer;
xer_type_encoder_f Pre_emptionCapability_encode_xer;
oer_type_decoder_f Pre_emptionCapability_decode_oer;
oer_type_encoder_f Pre_emptionCapability_encode_oer;
per_type_decoder_f Pre_emptionCapability_decode_uper;
per_type_encoder_f Pre_emptionCapability_encode_uper;
per_type_decoder_f Pre_emptionCapability_decode_aper;
per_type_encoder_f Pre_emptionCapability_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
