/*
 * Copyright (c) 2005-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	ASN_BIT_DATA
#define	ASN_BIT_DATA

#include <asn_system.h>		


#ifdef __cplusplus
extern "C" {
#endif

/*
 * This structure describes a position inside an incoming PER bit stream.
 */
typedef struct asn_bit_data_s {
  const uint8_t *buffer;  

         size_t  nboff;   

         size_t  nbits;   

         size_t  moved;   

  int (*refill)(struct asn_bit_data_s *);
  void *refill_key;
} asn_bit_data_t;

/*
 * Create a contiguous non-refillable bit data structure.
 * Can be freed by FREEMEM().
 */
asn_bit_data_t *asn_bit_data_new_contiguous(const void *data, size_t size_bits);

/*
 * Extract a small number of bits (<= 31) from the specified PER data pointer.
 * This function returns -1 if the specified number of bits could not be
 * extracted due to EOD or other conditions.
 */
int32_t asn_get_few_bits(asn_bit_data_t *, int get_nbits);



void asn_get_undo(asn_bit_data_t *, int get_nbits);

/*
 * Extract a large number of bits from the specified PER data pointer.
 * This function returns -1 if the specified number of bits could not be
 * extracted due to EOD or other conditions.
 */
int asn_get_many_bits(asn_bit_data_t *, uint8_t *dst, int right_align,
			int get_nbits);



char *asn_bit_data_string(asn_bit_data_t *);

/*
 * This structure supports forming bit output.
 */
typedef struct asn_bit_outp_s {
	uint8_t *buffer;	

	size_t nboff;		

	size_t nbits;		

	uint8_t tmpspace[32];	

	int (*output)(const void *data, size_t size, void *op_key);
	void *op_key;		

	size_t flushed_bytes;	

} asn_bit_outp_t;



int asn_put_few_bits(asn_bit_outp_t *, uint32_t bits, int obits);



int asn_put_many_bits(asn_bit_outp_t *, const uint8_t *src, int put_nbits);

/*
 * Flush whole bytes (0 or more) through (outper) member.
 * The least significant bits which are not used are guaranteed to be set to 0.
 * Returns -1 if callback returns -1. Otherwise, 0.
 */
int asn_put_aligned_flush(asn_bit_outp_t *);

#ifdef __cplusplus
}
#endif

#endif	

