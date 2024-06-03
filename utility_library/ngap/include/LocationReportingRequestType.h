/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_LocationReportingRequestType_H_
#define	_LocationReportingRequestType_H_


#include <asn_application.h>



#include "EventType.h"
#include "ReportArea.h"
#include "LocationReportingReferenceID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct AreaOfInterestList;
struct ProtocolExtensionContainer;



typedef struct LocationReportingRequestType {
	EventType_t	 eventType;
	ReportArea_t	 reportArea;
	struct AreaOfInterestList	*areaOfInterestList;	

	LocationReportingReferenceID_t	*locationReportingReferenceIDToBeCancelled;	

	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} LocationReportingRequestType_t;



extern asn_TYPE_descriptor_t asn_DEF_LocationReportingRequestType;
extern asn_SEQUENCE_specifics_t asn_SPC_LocationReportingRequestType_specs_1;
extern asn_TYPE_member_t asn_MBR_LocationReportingRequestType_1[5];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>