/*
 * A generic value of different syntaxes.
 */
#ifndef	ASN1_PARSER_VALUE_H
#define	ASN1_PARSER_VALUE_H

struct asn1p_constraint_s;	

struct asn1p_module_s;
struct asn1p_expr_s;

/*
 * A wrapper around various kinds of values.
 */
typedef struct asn1p_value_s {
	/*
	 * The value of the element.
	 */
	enum {
		ATV_NOVALUE,
		ATV_TYPE,	

		ATV_NULL,	

		ATV_REAL,	

		ATV_INTEGER,	

		ATV_MAX,
		ATV_MIN,
		ATV_TRUE,
		ATV_FALSE,
		ATV_TUPLE,	

		ATV_QUADRUPLE,	

		ATV_STRING,	

		ATV_UNPARSED,
		ATV_BITVECTOR,
		ATV_VALUESET,	

		ATV_REFERENCED,	

		ATV_CHOICE_IDENTIFIER,	

	} type;	


	union {
		struct asn1p_constraint_s *constraint;	

		struct asn1p_expr_s	*v_type;	

		asn1p_ref_t	*reference;
		asn1c_integer_t	 v_integer;
		double		 v_double;
		/*
		 * Binary bits vector.
		 */
		struct {
			uint8_t *buf;
			int size;
		} string;
		struct {
			uint8_t *bits;
			int size_in_bits;
		} binary_vector;
		struct {
			char *identifier;
			struct asn1p_value_s *value;
		} choice_identifier;
	} value;
} asn1p_value_t;

/*
 * Destructor and constructors for value.
 * If ref, bits or buffer are omitted, the corresponding function returns
 * (asn1p_value_t *)0 with errno = EINVAL.
 * Allocated value (where applicable) is guaranteed to be NUL-terminated.
 */
void asn1p_value_free(asn1p_value_t *);
asn1p_value_t *asn1p_value_fromref(asn1p_ref_t *ref, int do_copy);
asn1p_value_t *asn1p_value_fromconstr(struct asn1p_constraint_s *ct, int dc);
asn1p_value_t *asn1p_value_frombits(uint8_t *bits, int size_in_bits, int dc);
asn1p_value_t *asn1p_value_frombuf(char *buffer, int size, int do_copy);
asn1p_value_t *asn1p_value_fromdouble(double);
asn1p_value_t *asn1p_value_fromint(asn1c_integer_t);
asn1p_value_t *asn1p_value_fromtype(struct asn1p_expr_s *);
asn1p_value_t *asn1p_value_clone(asn1p_value_t *);
asn1p_value_t *asn1p_value_clone_with_resolver(asn1p_value_t *,
		asn1p_value_t *(*resolver)(asn1p_value_t *, void *rarg),
		void *rarg);
int asn1p_value_compare(const asn1p_value_t *, const asn1p_value_t *);
void asn1p_value_set_source(asn1p_value_t *, struct asn1p_module_s *, int line);

#endif	

