/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-PDU-Contents"
 * 	found in "PDUDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UEContextReleaseRequest_H_
#define	_UEContextReleaseRequest_H_


#include <asn_application.h>



#include "ProtocolIE-Container.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct UEContextReleaseRequest {
	ProtocolIE_Container_124P14_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} UEContextReleaseRequest_t;



extern asn_TYPE_descriptor_t asn_DEF_UEContextReleaseRequest;
extern asn_SEQUENCE_specifics_t asn_SPC_UEContextReleaseRequest_specs_1;
extern asn_TYPE_member_t asn_MBR_UEContextReleaseRequest_1[1];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
