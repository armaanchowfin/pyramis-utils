#ifndef	ASN1_COMPILER_NAMING_H
#define	ASN1_COMPILER_NAMING_H


/*
 * A member or a field name.
 */
struct c_names {
    struct {
        const char *asn_name;       

        const char *part_name;      

        const char *base_name;      

        const char *c_name;         

        const char *constrained_c_name; 

    } type;
    const char *asn_name;      

    const char *part_name;     

    const char *base_name;     

    const char *short_name;    

    const char *full_name;     

    const char *as_member;     

    const char *presence_enum; 

    const char *presence_name; 

    const char *members_enum;  

    const char *members_name;  

    const char *compound_name; 

};

struct c_names c_name(arg_t *);
struct c_names c_expr_name(arg_t *, asn1p_expr_t *);
const char *c_member_name(arg_t *, asn1p_expr_t *);     

const char *c_presence_name(arg_t *, asn1p_expr_t *);   


const char *c_names_format(struct c_names); 


/*
 * Returns 0 if no C name clashes have been encountered.
 * Returns 1 if C name clashes have been encountered.
 * Prints out the clashing items and suggests -fcompound-names.
 */
int c_name_clash(arg_t *arg);

void c_name_clash_finder_init(void);

void c_name_clash_finder_destroy(void);

#endif	

