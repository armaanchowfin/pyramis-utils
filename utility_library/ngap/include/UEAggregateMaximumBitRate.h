/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UEAggregateMaximumBitRate_H_
#define	_UEAggregateMaximumBitRate_H_


#include <asn_application.h>



#include "BitRate.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct UEAggregateMaximumBitRate {
	BitRate_t	 uEAggregateMaximumBitRateDL;
	BitRate_t	 uEAggregateMaximumBitRateUL;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} UEAggregateMaximumBitRate_t;



extern asn_TYPE_descriptor_t asn_DEF_UEAggregateMaximumBitRate;
extern asn_SEQUENCE_specifics_t asn_SPC_UEAggregateMaximumBitRate_specs_1;
extern asn_TYPE_member_t asn_MBR_UEAggregateMaximumBitRate_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
