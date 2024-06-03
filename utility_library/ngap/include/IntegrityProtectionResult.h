/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_IntegrityProtectionResult_H_
#define	_IntegrityProtectionResult_H_


#include <asn_application.h>



#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum IntegrityProtectionResult {
	IntegrityProtectionResult_performed	= 0,
	IntegrityProtectionResult_not_performed	= 1
	/*
	 * Enumeration is extensible
	 */
} e_IntegrityProtectionResult;



typedef long	 IntegrityProtectionResult_t;



extern asn_per_constraints_t asn_PER_type_IntegrityProtectionResult_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_IntegrityProtectionResult;
extern const asn_INTEGER_specifics_t asn_SPC_IntegrityProtectionResult_specs_1;
asn_struct_free_f IntegrityProtectionResult_free;
asn_struct_print_f IntegrityProtectionResult_print;
asn_constr_check_f IntegrityProtectionResult_constraint;
ber_type_decoder_f IntegrityProtectionResult_decode_ber;
der_type_encoder_f IntegrityProtectionResult_encode_der;
xer_type_decoder_f IntegrityProtectionResult_decode_xer;
xer_type_encoder_f IntegrityProtectionResult_encode_xer;
oer_type_decoder_f IntegrityProtectionResult_decode_oer;
oer_type_encoder_f IntegrityProtectionResult_encode_oer;
per_type_decoder_f IntegrityProtectionResult_decode_uper;
per_type_encoder_f IntegrityProtectionResult_encode_uper;
per_type_decoder_f IntegrityProtectionResult_decode_aper;
per_type_encoder_f IntegrityProtectionResult_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
