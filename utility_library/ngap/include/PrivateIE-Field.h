/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-Containers"
 * 	found in "ContainerDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_PrivateIE_Field_H_
#define	_PrivateIE_Field_H_


#include <asn_application.h>



#include "PrivateIE-ID.h"
#include "Criticality.h"
#include <ANY.h>
#include <asn_ioc.h>
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum PrivateMessageIEs__value_PR {
	PrivateMessageIEs__value_PR_NOTHING	

	
} PrivateMessageIEs__value_PR;



typedef struct PrivateMessageIEs {
	PrivateIE_ID_t	 id;
	Criticality_t	 criticality;
	struct PrivateMessageIEs__value {
		PrivateMessageIEs__value_PR present;
		union PrivateMessageIEs__value_u {
		} choice;
		
		

		asn_struct_ctx_t _asn_ctx;
	} value;
	
	

	asn_struct_ctx_t _asn_ctx;
} PrivateMessageIEs_t;



extern asn_TYPE_descriptor_t asn_DEF_PrivateMessageIEs;
extern asn_SEQUENCE_specifics_t asn_SPC_PrivateMessageIEs_specs_1;
extern asn_TYPE_member_t asn_MBR_PrivateMessageIEs_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
