/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_EmergencyAreaIDList_H_
#define	_EmergencyAreaIDList_H_


#include <asn_application.h>



#include "EmergencyAreaID.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef struct EmergencyAreaIDList {
	A_SEQUENCE_OF(EmergencyAreaID_t) list;
	
	

	asn_struct_ctx_t _asn_ctx;
} EmergencyAreaIDList_t;



extern asn_TYPE_descriptor_t asn_DEF_EmergencyAreaIDList;
extern asn_SET_OF_specifics_t asn_SPC_EmergencyAreaIDList_specs_1;
extern asn_TYPE_member_t asn_MBR_EmergencyAreaIDList_1[1];
extern asn_per_constraints_t asn_PER_type_EmergencyAreaIDList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
