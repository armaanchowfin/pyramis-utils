/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_DL_NGU_TNLInformationReused_H_
#define	_DL_NGU_TNLInformationReused_H_


#include <asn_application.h>



#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum DL_NGU_TNLInformationReused {
	DL_NGU_TNLInformationReused_true	= 0
	/*
	 * Enumeration is extensible
	 */
} e_DL_NGU_TNLInformationReused;



typedef long	 DL_NGU_TNLInformationReused_t;



extern asn_per_constraints_t asn_PER_type_DL_NGU_TNLInformationReused_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_DL_NGU_TNLInformationReused;
extern const asn_INTEGER_specifics_t asn_SPC_DL_NGU_TNLInformationReused_specs_1;
asn_struct_free_f DL_NGU_TNLInformationReused_free;
asn_struct_print_f DL_NGU_TNLInformationReused_print;
asn_constr_check_f DL_NGU_TNLInformationReused_constraint;
ber_type_decoder_f DL_NGU_TNLInformationReused_decode_ber;
der_type_encoder_f DL_NGU_TNLInformationReused_encode_der;
xer_type_decoder_f DL_NGU_TNLInformationReused_decode_xer;
xer_type_encoder_f DL_NGU_TNLInformationReused_encode_xer;
oer_type_decoder_f DL_NGU_TNLInformationReused_decode_oer;
oer_type_encoder_f DL_NGU_TNLInformationReused_encode_oer;
per_type_decoder_f DL_NGU_TNLInformationReused_decode_uper;
per_type_encoder_f DL_NGU_TNLInformationReused_encode_uper;
per_type_decoder_f DL_NGU_TNLInformationReused_decode_aper;
per_type_encoder_f DL_NGU_TNLInformationReused_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>