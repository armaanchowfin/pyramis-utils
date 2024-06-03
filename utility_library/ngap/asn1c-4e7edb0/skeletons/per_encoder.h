/*-
 * Copyright (c) 2006-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_PER_ENCODER_H_
#define	_PER_ENCODER_H_

#include <asn_application.h>
#include <per_support.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;	


/*
 * Unaligned PER encoder of any ASN.1 type. May be invoked by the application.
 * WARNING: This function returns the number of encoded bits in the .encoded
 * field of the return value. Use the following formula to convert to bytes:
 * 	bytes = ((.encoded + 7) / 8)
 */
asn_enc_rval_t uper_encode(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints,
    const void *struct_ptr,                    

    asn_app_consume_bytes_f *consume_bytes_cb, 

    void *app_key                              

);

asn_enc_rval_t aper_encode(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints,
    const void *struct_ptr,                     

    asn_app_consume_bytes_f *consume_bytes_cb,  

    void *app_key                               

);

/*
 * A variant of uper_encode() which encodes data into the existing buffer
 * WARNING: This function returns the number of encoded bits in the .encoded
 * field of the return value.
 */
asn_enc_rval_t uper_encode_to_buffer(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints,
    const void *struct_ptr, 

    void *buffer,           

    size_t buffer_size      

);

asn_enc_rval_t aper_encode_to_buffer(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints,
    const void *struct_ptr,  

    void *buffer,            

    size_t buffer_size       

);
/*
 * A variant of uper_encode_to_buffer() which allocates buffer itself.
 * Returns the number of bytes in the buffer or -1 in case of failure.
 * WARNING: This function produces a "Production of the complete encoding",
 * with length of at least one octet. Contrast this to precise bit-packing
 * encoding of uper_encode() and uper_encode_to_buffer().
 */
ssize_t uper_encode_to_new_buffer(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints,
    const void *struct_ptr, 

    void **buffer_r         

);

ssize_t
aper_encode_to_new_buffer(
    const struct asn_TYPE_descriptor_s *td,
    const asn_per_constraints_t *constraints,
    const void *sptr,
    void **buffer_r
);

/*
 * Type of the generic PER encoder function.
 */
typedef asn_enc_rval_t(per_type_encoder_f)(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints, const void *struct_ptr,
    asn_per_outp_t *per_output);

#ifdef __cplusplus
}
#endif

#endif	

