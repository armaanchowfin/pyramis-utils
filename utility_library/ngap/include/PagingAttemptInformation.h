/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_PagingAttemptInformation_H_
#define	_PagingAttemptInformation_H_


#include <asn_application.h>



#include "PagingAttemptCount.h"
#include "IntendedNumberOfPagingAttempts.h"
#include "NextPagingAreaScope.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct PagingAttemptInformation {
	PagingAttemptCount_t	 pagingAttemptCount;
	IntendedNumberOfPagingAttempts_t	 intendedNumberOfPagingAttempts;
	NextPagingAreaScope_t	*nextPagingAreaScope;	

	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} PagingAttemptInformation_t;



extern asn_TYPE_descriptor_t asn_DEF_PagingAttemptInformation;
extern asn_SEQUENCE_specifics_t asn_SPC_PagingAttemptInformation_specs_1;
extern asn_TYPE_member_t asn_MBR_PagingAttemptInformation_1[4];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
