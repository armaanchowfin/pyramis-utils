/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_CompletedCellsInTAI_NR_Item_H_
#define	_CompletedCellsInTAI_NR_Item_H_


#include <asn_application.h>



#include "NR-CGI.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct CompletedCellsInTAI_NR_Item {
	NR_CGI_t	 nR_CGI;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} CompletedCellsInTAI_NR_Item_t;



extern asn_TYPE_descriptor_t asn_DEF_CompletedCellsInTAI_NR_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_CompletedCellsInTAI_NR_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_CompletedCellsInTAI_NR_Item_1[2];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
