/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-PDU-Contents"
 * 	found in "PDUDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UEContextModificationFailure_H_
#define	_UEContextModificationFailure_H_


#include <asn_application.h>



#include "ProtocolIE-Container.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct UEContextModificationFailure {
	ProtocolIE_Container_124P19_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} UEContextModificationFailure_t;



extern asn_TYPE_descriptor_t asn_DEF_UEContextModificationFailure;
extern asn_SEQUENCE_specifics_t asn_SPC_UEContextModificationFailure_specs_1;
extern asn_TYPE_member_t asn_MBR_UEContextModificationFailure_1[1];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>