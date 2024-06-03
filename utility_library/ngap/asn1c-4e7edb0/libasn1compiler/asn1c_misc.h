#ifndef	ASN1_COMPILER_MISC_H
#define	ASN1_COMPILER_MISC_H

/*
 * Make the target language identifier out of one or more names.
 * The function will concatenate the names and replace unsafe characters
 * with safe ones.
 */
enum ami_flags_e {
	AMI_MASK_ONLY_SPACES	= 1,	

	AMI_CHECK_RESERVED	= 2,	

	AMI_NODELIMITER       = 4,	

	AMI_USE_PREFIX        = 8,	

};
const char *asn1c_make_identifier(enum ami_flags_e, asn1p_expr_t *expr, ...);

/*
 * Return the type name of the specified expression.
 * The returned string is a pointer to a statically allocated buffer which is
 * going to be clobbered by the subsequent invocation of this function.
 */
enum tnfmt {
	TNF_UNMODIFIED	= 0x10,	

	TNF_INCLUDE	= 0x20,	

	TNF_CTYPE	= 0x30,	

	TNF_CONSTYPE = 0x40, 

	TNF_SAFE	= 0x50, 

	TNF_RSAFE	= 0x60,	

};
const char *asn1c_type_name(arg_t *arg, asn1p_expr_t *expr, enum tnfmt _format);

/*
 * Check whether the specified INTEGER or ENUMERATED type can be represented
 * using the generic 'long' type.
 * Return values:
 * 	FL_NOTFIT:	No, it cannot be represented using long.
 * 	FL_FITS_SIGNED:	It can be represented using signed long.
 * 	FL_FITS_UNSIGN:	It can be represented using unsigned long.
 * 	FL_PRESUMED:	Probably can't, but -fwide-types is not in effect.
 */
enum asn1c_fitslong_e {
	FL_NOTFIT,
	FL_FITS_SIGNED,
	FL_FITS_UNSIGN,
	FL_PRESUMED,
};
enum asn1c_fitslong_e asn1c_type_fits_long(arg_t *arg, asn1p_expr_t *expr);

enum asn1c_fitsfloat_e {
    RL_NOTFIT,
    RL_FITS_FLOAT32,
    RL_FITS_DOUBLE64
};
enum asn1c_fitsfloat_e asn1c_REAL_fits(arg_t *arg, asn1p_expr_t *expr);
const char *asn1c_prefix(void);

#endif	

