/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_TNLAssociationUsage_H_
#define	_TNLAssociationUsage_H_


#include <asn_application.h>



#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum TNLAssociationUsage {
	TNLAssociationUsage_ue	= 0,
	TNLAssociationUsage_non_ue	= 1,
	TNLAssociationUsage_both	= 2
	/*
	 * Enumeration is extensible
	 */
} e_TNLAssociationUsage;



typedef long	 TNLAssociationUsage_t;



extern asn_per_constraints_t asn_PER_type_TNLAssociationUsage_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_TNLAssociationUsage;
extern const asn_INTEGER_specifics_t asn_SPC_TNLAssociationUsage_specs_1;
asn_struct_free_f TNLAssociationUsage_free;
asn_struct_print_f TNLAssociationUsage_print;
asn_constr_check_f TNLAssociationUsage_constraint;
ber_type_decoder_f TNLAssociationUsage_decode_ber;
der_type_encoder_f TNLAssociationUsage_encode_der;
xer_type_decoder_f TNLAssociationUsage_decode_xer;
xer_type_encoder_f TNLAssociationUsage_encode_xer;
oer_type_decoder_f TNLAssociationUsage_decode_oer;
oer_type_encoder_f TNLAssociationUsage_encode_oer;
per_type_decoder_f TNLAssociationUsage_decode_uper;
per_type_encoder_f TNLAssociationUsage_encode_uper;
per_type_decoder_f TNLAssociationUsage_decode_aper;
per_type_encoder_f TNLAssociationUsage_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>