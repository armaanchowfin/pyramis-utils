/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UEPresenceInAreaOfInterestItem_H_
#define	_UEPresenceInAreaOfInterestItem_H_


#include <asn_application.h>



#include "LocationReportingReferenceID.h"
#include "UEPresence.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct UEPresenceInAreaOfInterestItem {
	LocationReportingReferenceID_t	 locationReportingReferenceID;
	UEPresence_t	 uEPresence;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} UEPresenceInAreaOfInterestItem_t;



extern asn_TYPE_descriptor_t asn_DEF_UEPresenceInAreaOfInterestItem;
extern asn_SEQUENCE_specifics_t asn_SPC_UEPresenceInAreaOfInterestItem_specs_1;
extern asn_TYPE_member_t asn_MBR_UEPresenceInAreaOfInterestItem_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
