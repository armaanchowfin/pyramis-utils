/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_NGRANTraceID_H_
#define	_NGRANTraceID_H_


#include <asn_application.h>



#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef OCTET_STRING_t	 NGRANTraceID_t;



extern asn_per_constraints_t asn_PER_type_NGRANTraceID_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_NGRANTraceID;
asn_struct_free_f NGRANTraceID_free;
asn_struct_print_f NGRANTraceID_print;
asn_constr_check_f NGRANTraceID_constraint;
ber_type_decoder_f NGRANTraceID_decode_ber;
der_type_encoder_f NGRANTraceID_encode_der;
xer_type_decoder_f NGRANTraceID_decode_xer;
xer_type_encoder_f NGRANTraceID_encode_xer;
oer_type_decoder_f NGRANTraceID_decode_oer;
oer_type_encoder_f NGRANTraceID_encode_oer;
per_type_decoder_f NGRANTraceID_decode_uper;
per_type_encoder_f NGRANTraceID_encode_uper;
per_type_decoder_f NGRANTraceID_decode_aper;
per_type_encoder_f NGRANTraceID_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
