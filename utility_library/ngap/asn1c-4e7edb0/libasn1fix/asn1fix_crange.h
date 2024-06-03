#ifndef	ASN1FIX_CRANGE_H
#define	ASN1FIX_CRANGE_H

typedef struct asn1cnst_edge_s {
	enum asn1cnst_range_edge {
		ARE_MIN,
		ARE_MAX,
		ARE_VALUE,
	} type;
	int lineno;	

	asn1c_integer_t value;	

} asn1cnst_edge_t;

typedef struct asn1cnst_range_s {
	asn1cnst_edge_t left;	

	asn1cnst_edge_t right;	


    enum asn1cnst_range_narrowing {
        

        NOT_NARROW,
        NARROW_DOUBLE64,
        NARROW_FLOAT32,
    } narrowing; 


    

	struct asn1cnst_range_s **elements;
	int el_count;
	int el_size;

	int empty_constraint;	

	int extensible;		


	int incompatible;	

	int not_OER_visible;	

	int not_PER_visible;	

} asn1cnst_range_t;

/*
 * Compute the constraint range with variable visibility restrictions.
 * 
 * (expr_type) must have the type of the top-level parent ASN.1 type.
 * (required_type) must be one of ACT_EL_RANGE, ACT_CT_SIZE or ACT_CT_FROM.
 * (minmax) and (expectation_met) should be 0.
 * ERRORS:
 * 	EINVAL:	Mandatory arguments missing.
 * 	ENOMEM:	Memory allocation failure.
 * 	EPERM:	Invalid constraint reference.
 */
enum cpr_flags {
	CPR_noflags			= 0x00,
	CPR_strict_OER_visibility	= 0x01,
	CPR_strict_PER_visibility	= 0x02,
	CPR_simulate_fbless_SIZE	= 0x04,
};
asn1cnst_range_t *asn1constraint_compute_OER_range(const char *dbg_name,
	asn1p_expr_type_e expr_type,
	const asn1p_constraint_t *ct,
	enum asn1p_constraint_type_e required_type,
	const asn1cnst_range_t *minmax,
	int *expectation_met,
	enum cpr_flags);
asn1cnst_range_t *asn1constraint_compute_PER_range(const char *dbg_name,
	asn1p_expr_type_e expr_type,
	const asn1p_constraint_t *ct,
	enum asn1p_constraint_type_e required_type,
	const asn1cnst_range_t *minmax,
	int *expectation_met,
	enum cpr_flags);


asn1cnst_range_t *asn1constraint_compute_constraint_range(const char *dbg_name,
	asn1p_expr_type_e expr_type,
	const asn1p_constraint_t *ct,
	enum asn1p_constraint_type_e required_type,
	const asn1cnst_range_t *minmax,
	int *expectation_met,
	enum cpr_flags);

void asn1constraint_range_free(asn1cnst_range_t *);

/*
 * Check that a specific constraint is compatible
 * with the given expression type.
 */
int asn1constraint_compatible(asn1p_expr_type_e expr_type,
	enum asn1p_constraint_type_e constr_type, int fbless_SIZE);

/*
 * Fetch a default alphabet for this type.
 */
asn1cnst_range_t *asn1constraint_default_alphabet(asn1p_expr_type_e expr_type);

#endif	

