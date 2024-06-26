/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_PDUSessionResourceReleaseCommandTransfer_H_
#define	_PDUSessionResourceReleaseCommandTransfer_H_


#include <asn_application.h>



#include "Cause.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct PDUSessionResourceReleaseCommandTransfer {
	Cause_t	 cause;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} PDUSessionResourceReleaseCommandTransfer_t;



extern asn_TYPE_descriptor_t asn_DEF_PDUSessionResourceReleaseCommandTransfer;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
