/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UL_NGU_UP_TNLModifyItem_H_
#define	_UL_NGU_UP_TNLModifyItem_H_


#include <asn_application.h>



#include "UPTransportLayerInformation.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct UL_NGU_UP_TNLModifyItem {
	UPTransportLayerInformation_t	 uL_NGU_UP_TNLInformation;
	UPTransportLayerInformation_t	 dL_NGU_UP_TNLInformation;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} UL_NGU_UP_TNLModifyItem_t;



extern asn_TYPE_descriptor_t asn_DEF_UL_NGU_UP_TNLModifyItem;
extern asn_SEQUENCE_specifics_t asn_SPC_UL_NGU_UP_TNLModifyItem_specs_1;
extern asn_TYPE_member_t asn_MBR_UL_NGU_UP_TNLModifyItem_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
