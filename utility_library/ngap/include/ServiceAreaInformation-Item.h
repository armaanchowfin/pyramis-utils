/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_ServiceAreaInformation_Item_H_
#define	_ServiceAreaInformation_Item_H_


#include <asn_application.h>



#include "PLMNIdentity.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct AllowedTACs;
struct NotAllowedTACs;
struct ProtocolExtensionContainer;



typedef struct ServiceAreaInformation_Item {
	PLMNIdentity_t	 pLMNIdentity;
	struct AllowedTACs	*allowedTACs;	

	struct NotAllowedTACs	*notAllowedTACs;	

	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} ServiceAreaInformation_Item_t;



extern asn_TYPE_descriptor_t asn_DEF_ServiceAreaInformation_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_ServiceAreaInformation_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_ServiceAreaInformation_Item_1[4];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>