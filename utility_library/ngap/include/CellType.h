/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_CellType_H_
#define	_CellType_H_


#include <asn_application.h>



#include "CellSize.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct CellType {
	CellSize_t	 cellSize;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} CellType_t;



extern asn_TYPE_descriptor_t asn_DEF_CellType;
extern asn_SEQUENCE_specifics_t asn_SPC_CellType_specs_1;
extern asn_TYPE_member_t asn_MBR_CellType_1[2];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
