/*-
 * Copyright (c) 2004-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	ASN_CODECS_PRIM_H
#define	ASN_CODECS_PRIM_H

#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ASN__PRIMITIVE_TYPE_s {
    uint8_t *buf;   

    size_t size;    

} ASN__PRIMITIVE_TYPE_t;	


asn_struct_free_f ASN__PRIMITIVE_TYPE_free;
ber_type_decoder_f ber_decode_primitive;
der_type_encoder_f der_encode_primitive;

/*
 * A callback specification for the xer_decode_primitive() function below.
 */
enum xer_pbd_rval {
    XPBD_SYSTEM_FAILURE,  

    XPBD_DECODER_LIMIT,   

    XPBD_BROKEN_ENCODING, 

    XPBD_NOT_BODY_IGNORE, 

    XPBD_BODY_CONSUMED    

};
typedef enum xer_pbd_rval(xer_primitive_body_decoder_f)(
    const asn_TYPE_descriptor_t *td, void *struct_ptr, const void *chunk_buf,
    size_t chunk_size);

/*
 * Specific function to decode simple primitive types.
 * Also see xer_decode_general() in xer_decoder.h
 */
asn_dec_rval_t xer_decode_primitive(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *type_descriptor, void **struct_ptr,
    size_t struct_size, const char *opt_mname, const void *buf_ptr, size_t size,
    xer_primitive_body_decoder_f *prim_body_decoder);

#ifdef __cplusplus
}
#endif

#endif	

