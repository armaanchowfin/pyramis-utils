/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_UserLocationInformationNR_H_
#define	_UserLocationInformationNR_H_


#include <asn_application.h>



#include "NR-CGI.h"
#include "TAI.h"
#include "TimeStamp.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct UserLocationInformationNR {
	NR_CGI_t	 nR_CGI;
	TAI_t	 tAI;
	TimeStamp_t	*timeStamp;	

	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} UserLocationInformationNR_t;



extern asn_TYPE_descriptor_t asn_DEF_UserLocationInformationNR;
extern asn_SEQUENCE_specifics_t asn_SPC_UserLocationInformationNR_specs_1;
extern asn_TYPE_member_t asn_MBR_UserLocationInformationNR_1[4];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
