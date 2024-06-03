/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_HandoverType_H_
#define	_HandoverType_H_


#include <asn_application.h>



#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum HandoverType {
	HandoverType_intra5gs	= 0,
	HandoverType_fivegs_to_eps	= 1,
	HandoverType_eps_to_5gs	= 2
	/*
	 * Enumeration is extensible
	 */
} e_HandoverType;



typedef long	 HandoverType_t;



extern asn_per_constraints_t asn_PER_type_HandoverType_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_HandoverType;
extern const asn_INTEGER_specifics_t asn_SPC_HandoverType_specs_1;
asn_struct_free_f HandoverType_free;
asn_struct_print_f HandoverType_print;
asn_constr_check_f HandoverType_constraint;
ber_type_decoder_f HandoverType_decode_ber;
der_type_encoder_f HandoverType_encode_der;
xer_type_decoder_f HandoverType_decode_xer;
xer_type_encoder_f HandoverType_encode_xer;
oer_type_decoder_f HandoverType_decode_oer;
oer_type_encoder_f HandoverType_encode_oer;
per_type_decoder_f HandoverType_decode_uper;
per_type_encoder_f HandoverType_encode_uper;
per_type_decoder_f HandoverType_decode_aper;
per_type_encoder_f HandoverType_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>