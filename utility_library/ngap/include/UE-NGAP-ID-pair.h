/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UE_NGAP_ID_pair_H_
#define	_UE_NGAP_ID_pair_H_


#include <asn_application.h>



#include "AMF-UE-NGAP-ID.h"
#include "RAN-UE-NGAP-ID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct UE_NGAP_ID_pair {
	AMF_UE_NGAP_ID_t	 aMF_UE_NGAP_ID;
	RAN_UE_NGAP_ID_t	 rAN_UE_NGAP_ID;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} UE_NGAP_ID_pair_t;



extern asn_TYPE_descriptor_t asn_DEF_UE_NGAP_ID_pair;
extern asn_SEQUENCE_specifics_t asn_SPC_UE_NGAP_ID_pair_specs_1;
extern asn_TYPE_member_t asn_MBR_UE_NGAP_ID_pair_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
