/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_SupportedTAItem_H_
#define	_SupportedTAItem_H_


#include <asn_application.h>



#include "TAC.h"
#include "BroadcastPLMNList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct SupportedTAItem {
	TAC_t	 tAC;
	BroadcastPLMNList_t	 broadcastPLMNList;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} SupportedTAItem_t;



extern asn_TYPE_descriptor_t asn_DEF_SupportedTAItem;
extern asn_SEQUENCE_specifics_t asn_SPC_SupportedTAItem_specs_1;
extern asn_TYPE_member_t asn_MBR_SupportedTAItem_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
