/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_TAICancelledEUTRA_Item_H_
#define	_TAICancelledEUTRA_Item_H_


#include <asn_application.h>



#include "TAI.h"
#include "CancelledCellsInTAI-EUTRA.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct TAICancelledEUTRA_Item {
	TAI_t	 tAI;
	CancelledCellsInTAI_EUTRA_t	 cancelledCellsInTAI_EUTRA;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} TAICancelledEUTRA_Item_t;



extern asn_TYPE_descriptor_t asn_DEF_TAICancelledEUTRA_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_TAICancelledEUTRA_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_TAICancelledEUTRA_Item_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>