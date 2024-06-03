/*-
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_DER_ENCODER_H_
#define	_DER_ENCODER_H_

#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;	


/*
 * The DER encoder of any type. May be invoked by the application.
 * Produces DER- and BER-compliant encoding. (DER is a subset of BER).
 *
 * NOTE: Use the ber_decode() function (ber_decoder.h) to decode data
 * produced by der_encode().
 */
asn_enc_rval_t der_encode(const struct asn_TYPE_descriptor_s *type_descriptor,
                          const void *struct_ptr, 

                          asn_app_consume_bytes_f *consume_bytes_cb,
                          void *app_key 

);



asn_enc_rval_t der_encode_to_buffer(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const void *struct_ptr, 

    void *buffer,           

    size_t buffer_size      

);

/*
 * Type of the generic DER encoder.
 */
typedef asn_enc_rval_t(der_type_encoder_f)(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const void *struct_ptr, 

    int tag_mode,           

    ber_tlv_tag_t tag, asn_app_consume_bytes_f *consume_bytes_cb, 

    void *app_key 

);


/*******************************
 * INTERNALLY USEFUL FUNCTIONS *
 *******************************/

/*
 * Write out leading TL[v] sequence according to the type definition.
 */
ssize_t der_write_tags(const struct asn_TYPE_descriptor_s *type_descriptor,
                       size_t struct_length,
                       int tag_mode,      

                       int last_tag_form, 

                       ber_tlv_tag_t tag,
                       asn_app_consume_bytes_f *consume_bytes_cb,
                       void *app_key);

#ifdef __cplusplus
}
#endif

#endif	

