/*-
 * Copyright (c) 2005-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_PER_DECODER_H_
#define	_PER_DECODER_H_

#include <asn_application.h>
#include <per_support.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;	


/*
 * Unaligned PER decoder of a "complete encoding" as per X.691 (08/2015) #11.1.
 * On success, this call always returns (.consumed >= 1), as per #11.1.3.
 */
asn_dec_rval_t uper_decode_complete(
    const struct asn_codec_ctx_s *opt_codec_ctx,
    const struct asn_TYPE_descriptor_s *type_descriptor, 

    void **struct_ptr,  

    const void *buffer, 

    size_t size         

);

/*
 * Unaligned PER decoder of any ASN.1 type. May be invoked by the application.
 * WARNING: This call returns the number of BITS read from the stream. Beware.
 */
asn_dec_rval_t uper_decode(
    const struct asn_codec_ctx_s *opt_codec_ctx,
    const struct asn_TYPE_descriptor_s *type_descriptor, 

    void **struct_ptr,  

    const void *buffer, 

    size_t size,        

    int skip_bits,      

    int unused_bits     

);

/*
 * Aligned PER decoder of a "complete encoding" as per X.691#10.1.
 * On success, this call always returns (.consumed >= 1), in BITS, as per X.691#10.1.3.
 */
asn_dec_rval_t aper_decode_complete(
       const struct asn_codec_ctx_s *opt_codec_ctx,
       const struct asn_TYPE_descriptor_s *type_descriptor,	

       void **struct_ptr,	

       const void *buffer,	

       size_t size		

									);

/*
 * Aligned PER decoder of any ASN.1 type. May be invoked by the application.
 * WARNING: This call returns the number of BITS read from the stream. Beware.
 */
asn_dec_rval_t aper_decode(
      const struct asn_codec_ctx_s *opt_codec_ctx,
      const struct asn_TYPE_descriptor_s *type_descriptor,	

      void **struct_ptr,	

      const void *buffer,	

      size_t size,		

      int skip_bits,		

      int unused_bits		

      );

/*
 * Type of the type-specific PER decoder function.
 */
typedef asn_dec_rval_t(per_type_decoder_f)(
    const asn_codec_ctx_t *opt_codec_ctx,
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const asn_per_constraints_t *constraints, void **struct_ptr,
    asn_per_data_t *per_data);

#ifdef __cplusplus
}
#endif

#endif	

