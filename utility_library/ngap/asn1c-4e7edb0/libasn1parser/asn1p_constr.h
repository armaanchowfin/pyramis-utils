/*
 * Type constraints.
 */
#ifndef	ASN1_PARSER_CONSTRAINT_H
#define	ASN1_PARSER_CONSTRAINT_H

typedef struct asn1p_constraint_s {

	enum asn1p_constraint_type_e {
		ACT_INVALID,		

		/*
		 * Constraint elements.
		 */
		ACT_EL_TYPE,		

		ACT_EL_VALUE,		

		ACT_EL_RANGE,		

		ACT_EL_LLRANGE,		

		ACT_EL_RLRANGE,		

		ACT_EL_ULRANGE,		

		ACT_EL_EXT,		

		/*
		 * Constraint types.
		 */
		ACT_CT_SIZE,		

		ACT_CT_FROM,		

		ACT_CT_WCOMP,		

		ACT_CT_WCOMPS,		

		ACT_CT_CTDBY,		

		ACT_CT_CTNG,		

		ACT_CT_PATTERN,		

		/*
		 * Arrays of constraints.
		 */
		ACT_CA_SET,		

		ACT_CA_CRC,		

		ACT_CA_CSV,		

		ACT_CA_UNI,		

		ACT_CA_INT,		

		ACT_CA_EXC,		

		ACT_CA_AEX,		

	} type;

	enum asn1p_constr_pres_e {
		ACPRES_DEFAULT,
		ACPRES_PRESENT,
		ACPRES_ABSENT,
		ACPRES_OPTIONAL,
	} presence;

	struct asn1p_constraint_s *parent_ct;	


	/*
	 * Separate types and values.
	 */
	asn1p_value_t *containedSubtype;
	asn1p_value_t *value;
	asn1p_value_t *range_start;
	asn1p_value_t *range_stop;

	/*
	 * A collection of constraint elements.
	 */
	struct asn1p_constraint_s **elements;
	unsigned int el_count;	

	unsigned int el_size;	


	struct asn1p_module_s *module;	

	int _lineno;	

} asn1p_constraint_t;



const char *asn1p_constraint_type2str(enum asn1p_constraint_type_e);

const asn1p_constraint_t *asn1p_get_component_relation_constraint(
    asn1p_constraint_t *);

int asn1p_constraint_compare(const asn1p_constraint_t *,
                             const asn1p_constraint_t *);
void asn1p_constraint_set_source(asn1p_constraint_t *,
                                 struct asn1p_module_s *module, int lineno);

/*
 * Constructors and a recursive destructor.
 */
asn1p_constraint_t *asn1p_constraint_new(int _lineno, asn1p_module_t *mod);
void asn1p_constraint_free(asn1p_constraint_t *);

/*
 * Clone the constraint and all its children.
 */
asn1p_constraint_t *asn1p_constraint_clone(asn1p_constraint_t *source_to_clone);
asn1p_constraint_t *asn1p_constraint_clone_with_resolver(
	asn1p_constraint_t *source_to_clone,
	asn1p_value_t *(*resolver)(asn1p_value_t *, void *), void *);

/*
 * Insert additional element into the element array of a (to) constraint.
 */
int asn1p_constraint_insert(asn1p_constraint_t *into, asn1p_constraint_t *what);
int asn1p_constraint_prepend(asn1p_constraint_t *before, asn1p_constraint_t *what);

#endif	

