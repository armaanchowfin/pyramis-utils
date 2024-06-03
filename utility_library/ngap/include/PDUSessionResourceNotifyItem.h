/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_PDUSessionResourceNotifyItem_H_
#define	_PDUSessionResourceNotifyItem_H_


#include <asn_application.h>



#include "PDUSessionID.h"
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct ProtocolExtensionContainer;



typedef struct PDUSessionResourceNotifyItem {
	PDUSessionID_t	 pDUSessionID;
	OCTET_STRING_t	 pDUSessionResourceNotifyTransfer;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} PDUSessionResourceNotifyItem_t;



extern asn_TYPE_descriptor_t asn_DEF_PDUSessionResourceNotifyItem;
extern asn_SEQUENCE_specifics_t asn_SPC_PDUSessionResourceNotifyItem_specs_1;
extern asn_TYPE_member_t asn_MBR_PDUSessionResourceNotifyItem_1[3];

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>