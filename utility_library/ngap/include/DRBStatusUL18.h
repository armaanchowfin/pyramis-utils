/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_DRBStatusUL18_H_
#define	_DRBStatusUL18_H_


#include <asn_application.h>



#include "COUNTValueForPDCP-SN18.h"
#include <BIT_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct DRBStatusUL18 {
	COUNTValueForPDCP_SN18_t	 uL_COUNTValue;
	BIT_STRING_t	*receiveStatusOfUL_PDCP_SDUs;	

	struct ProtocolExtensionContainer	*iE_Extension;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} DRBStatusUL18_t;



extern asn_TYPE_descriptor_t asn_DEF_DRBStatusUL18;
extern asn_SEQUENCE_specifics_t asn_SPC_DRBStatusUL18_specs_1;
extern asn_TYPE_member_t asn_MBR_DRBStatusUL18_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>