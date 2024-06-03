/*
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
/*
 * This file contains the declaration structure called "ASN.1 Type Definition",
 * which holds all information necessary for encoding and decoding routines.
 * This structure even contains pointer to these encoding and decoding routines
 * for each defined ASN.1 type.
 */
#ifndef	_CONSTR_TYPE_H_
#define	_CONSTR_TYPE_H_

#include <ber_tlv_length.h>
#include <ber_tlv_tag.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;	

struct asn_TYPE_member_s;	


/*
 * This type provides the context information for various ASN.1 routines,
 * primarily ones doing decoding. A member _asn_ctx of this type must be
 * included into certain target language's structures, such as compound types.
 */
typedef struct asn_struct_ctx_s {
	short phase;		

	short step;		

	int context;		

	void *ptr;		

	ber_tlv_len_t left;	

} asn_struct_ctx_t;

#include <ber_decoder.h>	

#include <der_encoder.h>	

#include <xer_decoder.h>	

#include <xer_encoder.h>	

#include <per_decoder.h>	

#include <per_encoder.h>	

#include <constraints.h>	

#include <asn_random_fill.h>	


#ifdef  ASN_DISABLE_OER_SUPPORT
typedef void (oer_type_decoder_f)(void);
typedef void (oer_type_encoder_f)(void);
typedef void asn_oer_constraints_t;
#else
#include <oer_decoder.h>	

#include <oer_encoder.h>	

#endif

/*
 * Free the structure according to its specification.
 * Use one of ASN_STRUCT_{FREE,RESET,CONTENTS_ONLY} macros instead.
 * Do not use directly.
 */
enum asn_struct_free_method {
    ASFM_FREE_EVERYTHING,   

    ASFM_FREE_UNDERLYING,   

    ASFM_FREE_UNDERLYING_AND_RESET   

};
typedef void (asn_struct_free_f)(
		const struct asn_TYPE_descriptor_s *type_descriptor,
		void *struct_ptr, enum asn_struct_free_method);

/*
 * Free the structure including freeing the memory pointed to by ptr itself.
 */
#define ASN_STRUCT_FREE(asn_DEF, ptr) \
    (asn_DEF).op->free_struct(&(asn_DEF), (ptr), ASFM_FREE_EVERYTHING)

/*
 * Free the memory used by the members of the structure without freeing the
 * the structure pointer itself.
 * ZERO-OUT the structure to the safe clean state.
 * (Retaining the pointer may be useful in case the structure is allocated
 *  statically or arranged on the stack, yet its elements are dynamic.)
 */
#define ASN_STRUCT_RESET(asn_DEF, ptr) \
    (asn_DEF).op->free_struct(&(asn_DEF), (ptr), ASFM_FREE_UNDERLYING_AND_RESET)

/*
 * Free memory used by the members of the structure without freeing
 * the structure pointer itself.
 * (Retaining the pointer may be useful in case the structure is allocated
 *  statically or arranged on the stack, yet its elements are dynamic.)
 * AVOID using it in the application code;
 * Use a safer ASN_STRUCT_RESET() instead.
 */
#define ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF, ptr) \
    (asn_DEF).op->free_struct(&(asn_DEF), (ptr), ASFM_FREE_UNDERLYING)

/*
 * Print the structure according to its specification.
 */
typedef int(asn_struct_print_f)(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const void *struct_ptr,
    int level, 

    asn_app_consume_bytes_f *callback, void *app_key);

/*
 * Compare two structs between each other.
 * Returns <0 if struct_A is "smaller" than struct_B, >0 if "greater",
 * and =0 if "equal to", for some type-specific, stable definition of
 * "smaller", "greater" and "equal to".
 */
typedef int (asn_struct_compare_f)(
		const struct asn_TYPE_descriptor_s *type_descriptor,
		const void *struct_A,
		const void *struct_B);

/*
 * Return the outmost tag of the type.
 * If the type is untagged CHOICE, the dynamic operation is performed.
 * NOTE: This function pointer type is only useful internally.
 * Do not use it in your application.
 */
typedef ber_tlv_tag_t (asn_outmost_tag_f)(
		const struct asn_TYPE_descriptor_s *type_descriptor,
		const void *struct_ptr, int tag_mode, ber_tlv_tag_t tag);


asn_outmost_tag_f asn_TYPE_outmost_tag;

/*
 * Fetch the desired type of the Open Type based on the
 * Information Object Set driven constraints.
 */
typedef struct asn_type_selector_result_s {
    const struct asn_TYPE_descriptor_s *type_descriptor; 

    unsigned presence_index; 

} asn_type_selector_result_t;
typedef asn_type_selector_result_t(asn_type_selector_f)(
    const struct asn_TYPE_descriptor_s *parent_type_descriptor,
    const void *parent_structure_ptr);

/*
 * Generalized functions for dealing with the speciic type.
 * May be directly invoked by applications.
 */
typedef struct asn_TYPE_operation_s {
    asn_struct_free_f *free_struct;     

    asn_struct_print_f *print_struct;   

    asn_struct_compare_f *compare_struct; 

    ber_type_decoder_f *ber_decoder;      

    der_type_encoder_f *der_encoder;      

    xer_type_decoder_f *xer_decoder;      

    xer_type_encoder_f *xer_encoder;      

    oer_type_decoder_f *oer_decoder;      

    oer_type_encoder_f *oer_encoder;      

    per_type_decoder_f *uper_decoder;     

    per_type_encoder_f *uper_encoder;     

    per_type_decoder_f *aper_decoder;     

    per_type_encoder_f *aper_encoder;     

    asn_random_fill_f *random_fill;       

    asn_outmost_tag_f *outmost_tag;       

} asn_TYPE_operation_t;

/*
 * A constraints tuple specifying both the OER and PER constraints.
 */
typedef struct asn_encoding_constraints_s {
    const struct asn_oer_constraints_s *oer_constraints;
    const struct asn_per_constraints_s *per_constraints;
    asn_constr_check_f *general_constraints;
} asn_encoding_constraints_t;

/*
 * The definitive description of the destination language's structure.
 */
typedef struct asn_TYPE_descriptor_s {
    const char *name;       

    const char *xml_tag;    


    /*
     * Generalized functions for dealing with the specific type.
     * May be directly invoked by applications.
     */
    asn_TYPE_operation_t *op;

    /***********************************************************************
     * Internally useful members. Not to be used by applications directly. *
     **********************************************************************/

    /*
     * Tags that are expected to occur.
     */
    const ber_tlv_tag_t *tags;      

    unsigned tags_count;            

    const ber_tlv_tag_t *all_tags;  

    unsigned all_tags_count;        


    

    asn_encoding_constraints_t encoding_constraints;

    /*
     * An ASN.1 production type members (members of SEQUENCE, SET, CHOICE).
     */
    struct asn_TYPE_member_s *elements;
    unsigned elements_count;

    /*
     * Additional information describing the type, used by appropriate
     * functions above.
     */
    const void *specifics;
} asn_TYPE_descriptor_t;

/*
 * This type describes an element of the constructed type,
 * i.e. SEQUENCE, SET, CHOICE, etc.
 */
  enum asn_TYPE_flags_e {
    ATF_NOFLAGS,
    ATF_POINTER = 0x01,   

    ATF_OPEN_TYPE = 0x02, 

    ATF_ANY_TYPE = 0x04   

  };
typedef struct asn_TYPE_member_s {
    enum asn_TYPE_flags_e flags; 

    unsigned optional;      

    unsigned memb_offset;   

    ber_tlv_tag_t tag;      

    int tag_mode;           

    asn_TYPE_descriptor_t *type;            

    asn_type_selector_f *type_selector;     

    asn_encoding_constraints_t encoding_constraints;
    int (*default_value_cmp)(const void *sptr); 

    int (*default_value_set)(void **sptr);      

    const char *name; 

} asn_TYPE_member_t;

/*
 * BER tag to element number mapping.
 */
typedef struct asn_TYPE_tag2member_s {
    ber_tlv_tag_t el_tag;   

    unsigned el_no;         

    int toff_first;         

    int toff_last;          

} asn_TYPE_tag2member_t;

/*
 * This function prints out the contents of the target language's structure
 * (struct_ptr) into the file pointer (stream) in human readable form.
 * RETURN VALUES:
 * 	 0: The structure is printed.
 * 	-1: Problem dumping the structure.
 * (See also xer_fprint() in xer_encoder.h)
 */
int asn_fprint(FILE *stream, 

               const asn_TYPE_descriptor_t *td, 

               const void *struct_ptr);         


#ifdef __cplusplus
}
#endif

#endif	

