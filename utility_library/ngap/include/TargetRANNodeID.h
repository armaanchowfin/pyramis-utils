/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_TargetRANNodeID_H_
#define	_TargetRANNodeID_H_


#include <asn_application.h>



#include "GlobalRANNodeID.h"
#include "TAI.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct TargetRANNodeID {
	GlobalRANNodeID_t	 globalRANNodeID;
	TAI_t	 selectedTAI;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} TargetRANNodeID_t;



extern asn_TYPE_descriptor_t asn_DEF_TargetRANNodeID;
extern asn_SEQUENCE_specifics_t asn_SPC_TargetRANNodeID_specs_1;
extern asn_TYPE_member_t asn_MBR_TargetRANNodeID_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
