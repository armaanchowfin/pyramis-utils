/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_ConfidentialityProtectionResult_H_
#define	_ConfidentialityProtectionResult_H_


#include <asn_application.h>



#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum ConfidentialityProtectionResult {
	ConfidentialityProtectionResult_performed	= 0,
	ConfidentialityProtectionResult_not_performed	= 1
	/*
	 * Enumeration is extensible
	 */
} e_ConfidentialityProtectionResult;



typedef long	 ConfidentialityProtectionResult_t;



extern asn_per_constraints_t asn_PER_type_ConfidentialityProtectionResult_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ConfidentialityProtectionResult;
extern const asn_INTEGER_specifics_t asn_SPC_ConfidentialityProtectionResult_specs_1;
asn_struct_free_f ConfidentialityProtectionResult_free;
asn_struct_print_f ConfidentialityProtectionResult_print;
asn_constr_check_f ConfidentialityProtectionResult_constraint;
ber_type_decoder_f ConfidentialityProtectionResult_decode_ber;
der_type_encoder_f ConfidentialityProtectionResult_encode_der;
xer_type_decoder_f ConfidentialityProtectionResult_decode_xer;
xer_type_encoder_f ConfidentialityProtectionResult_encode_xer;
oer_type_decoder_f ConfidentialityProtectionResult_decode_oer;
oer_type_encoder_f ConfidentialityProtectionResult_encode_oer;
per_type_decoder_f ConfidentialityProtectionResult_decode_uper;
per_type_encoder_f ConfidentialityProtectionResult_encode_uper;
per_type_decoder_f ConfidentialityProtectionResult_decode_aper;
per_type_encoder_f ConfidentialityProtectionResult_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
