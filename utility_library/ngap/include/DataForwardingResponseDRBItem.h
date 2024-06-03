/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_DataForwardingResponseDRBItem_H_
#define	_DataForwardingResponseDRBItem_H_


#include <asn_application.h>



#include "DRB-ID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct UPTransportLayerInformation;
struct ProtocolExtensionContainer;



typedef struct DataForwardingResponseDRBItem {
	DRB_ID_t	 dRB_ID;
	struct UPTransportLayerInformation	*dLForwardingUP_TNLInformation;	

	struct UPTransportLayerInformation	*uLForwardingUP_TNLInformation;	

	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} DataForwardingResponseDRBItem_t;



extern asn_TYPE_descriptor_t asn_DEF_DataForwardingResponseDRBItem;
extern asn_SEQUENCE_specifics_t asn_SPC_DataForwardingResponseDRBItem_specs_1;
extern asn_TYPE_member_t asn_MBR_DataForwardingResponseDRBItem_1[4];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
