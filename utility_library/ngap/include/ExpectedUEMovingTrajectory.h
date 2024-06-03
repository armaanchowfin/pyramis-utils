/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_ExpectedUEMovingTrajectory_H_
#define	_ExpectedUEMovingTrajectory_H_


#include <asn_application.h>



#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ExpectedUEMovingTrajectoryItem;



typedef struct ExpectedUEMovingTrajectory {
	A_SEQUENCE_OF(struct ExpectedUEMovingTrajectoryItem) list;
	
	

	asn_struct_ctx_t _asn_ctx;
} ExpectedUEMovingTrajectory_t;



extern asn_TYPE_descriptor_t asn_DEF_ExpectedUEMovingTrajectory;
extern asn_SET_OF_specifics_t asn_SPC_ExpectedUEMovingTrajectory_specs_1;
extern asn_TYPE_member_t asn_MBR_ExpectedUEMovingTrajectory_1[1];
extern asn_per_constraints_t asn_PER_type_ExpectedUEMovingTrajectory_constr_1;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
