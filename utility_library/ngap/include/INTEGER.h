/*-
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_INTEGER_H_
#define	_INTEGER_H_

#include <asn_application.h>
#include <asn_codecs_prim.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef ASN__PRIMITIVE_TYPE_t INTEGER_t;

extern asn_TYPE_descriptor_t asn_DEF_INTEGER;
extern asn_TYPE_operation_t asn_OP_INTEGER;



typedef struct asn_INTEGER_enum_map_s {
	long		 nat_value;	

	size_t		 enum_len;	

	const char	*enum_name;	

} asn_INTEGER_enum_map_t;



typedef struct asn_INTEGER_specifics_s {
	const asn_INTEGER_enum_map_t *value2enum;	

	const unsigned int *enum2value;		

	int map_count;				

	int extension;				

	int strict_enumeration;			

	int field_width;			

	int field_unsigned;			

} asn_INTEGER_specifics_t;

#define INTEGER_free    ASN__PRIMITIVE_TYPE_free
#define INTEGER_decode_ber	ber_decode_primitive
#define INTEGER_constraint	asn_generic_no_constraint
asn_struct_print_f INTEGER_print;
asn_struct_compare_f INTEGER_compare;
der_type_encoder_f INTEGER_encode_der;
xer_type_decoder_f INTEGER_decode_xer;
xer_type_encoder_f INTEGER_encode_xer;
oer_type_decoder_f INTEGER_decode_oer;
oer_type_encoder_f INTEGER_encode_oer;
per_type_decoder_f INTEGER_decode_uper;
per_type_encoder_f INTEGER_encode_uper;
per_type_decoder_f INTEGER_decode_aper;
per_type_encoder_f INTEGER_encode_aper;
asn_random_fill_f  INTEGER_random_fill;

/***********************************
 * Some handy conversion routines. *
 ***********************************/

/*
 * Natiwe size-independent conversion of native integers to/from INTEGER.
 * (l_size) is in bytes.
 * Returns 0 if it was possible to convert, -1 otherwise.
 * -1/EINVAL: Mandatory argument missing
 * -1/ERANGE: Value encoded is out of range for long representation
 * -1/ENOMEM: Memory allocation failed (in asn_*2INTEGER()).
 */
int asn_INTEGER2imax(const INTEGER_t *i, intmax_t *l);
int asn_INTEGER2umax(const INTEGER_t *i, uintmax_t *l);
int asn_imax2INTEGER(INTEGER_t *i, intmax_t l);
int asn_umax2INTEGER(INTEGER_t *i, uintmax_t l);

/*
 * Size-specific conversion helpers.
 */
int asn_INTEGER2long(const INTEGER_t *i, long *l);
int asn_INTEGER2ulong(const INTEGER_t *i, unsigned long *l);
int asn_long2INTEGER(INTEGER_t *i, long l);
int asn_ulong2INTEGER(INTEGER_t *i, unsigned long l);
int asn_int642INTEGER(INTEGER_t *i, int64_t l);
int asn_uint642INTEGER(INTEGER_t *i, uint64_t l);



enum asn_strtox_result_e {
    ASN_STRTOX_ERROR_RANGE = -3,  

    ASN_STRTOX_ERROR_INVAL = -2,  

    ASN_STRTOX_EXPECT_MORE = -1,  

    ASN_STRTOX_OK          =  0,  

    ASN_STRTOX_EXTRA_DATA  =  1   

};
enum asn_strtox_result_e asn_strtol_lim(const char *str, const char **end,
                                        long *l);
enum asn_strtox_result_e asn_strtoul_lim(const char *str, const char **end,
                                         unsigned long *l);
enum asn_strtox_result_e asn_strtoimax_lim(const char *str, const char **end,
                                           intmax_t *l);
enum asn_strtox_result_e asn_strtoumax_lim(const char *str, const char **end,
                                           uintmax_t *l);

/*
 * Convert the integer value into the corresponding enumeration map entry.
 */
const asn_INTEGER_enum_map_t *INTEGER_map_value2enum(
    const asn_INTEGER_specifics_t *specs, long value);

#ifdef __cplusplus
}
#endif

#endif	

