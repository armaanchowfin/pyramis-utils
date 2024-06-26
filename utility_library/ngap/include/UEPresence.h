/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UEPresence_H_
#define	_UEPresence_H_


#include <asn_application.h>



#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum UEPresence {
	UEPresence_in	= 0,
	UEPresence_out	= 1,
	UEPresence_unknown	= 2
	/*
	 * Enumeration is extensible
	 */
} e_UEPresence;



typedef long	 UEPresence_t;



extern asn_per_constraints_t asn_PER_type_UEPresence_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_UEPresence;
extern const asn_INTEGER_specifics_t asn_SPC_UEPresence_specs_1;
asn_struct_free_f UEPresence_free;
asn_struct_print_f UEPresence_print;
asn_constr_check_f UEPresence_constraint;
ber_type_decoder_f UEPresence_decode_ber;
der_type_encoder_f UEPresence_encode_der;
xer_type_decoder_f UEPresence_decode_xer;
xer_type_encoder_f UEPresence_encode_xer;
oer_type_decoder_f UEPresence_decode_oer;
oer_type_encoder_f UEPresence_encode_oer;
per_type_decoder_f UEPresence_decode_uper;
per_type_encoder_f UEPresence_encode_uper;
per_type_decoder_f UEPresence_decode_aper;
per_type_encoder_f UEPresence_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
