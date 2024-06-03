/*
 * Generated by asn1c-0.9.29 (http:
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#ifndef	_SourceNGRANNode_ToTargetNGRANNode_TransparentContainer_H_
#define	_SourceNGRANNode_ToTargetNGRANNode_TransparentContainer_H_


#include <asn_application.h>



#include "RRCContainer.h"
#include "NGRAN-CGI.h"
#include "IndexToRFSP.h"
#include "UEHistoryInformation.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif



struct PDUSessionResourceInformationList;
struct E_RABInformationList;
struct ProtocolExtensionContainer;



typedef struct SourceNGRANNode_ToTargetNGRANNode_TransparentContainer {
	RRCContainer_t	 rRCContainer;
	struct PDUSessionResourceInformationList	*pDUSessionResourceInformationList;	

	struct E_RABInformationList	*e_RABInformationList;	

	NGRAN_CGI_t	 targetCell_ID;
	IndexToRFSP_t	*indexToRFSP;	

	UEHistoryInformation_t	 uEHistoryInformation;
	struct ProtocolExtensionContainer	*iE_Extensions;	

	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	

	asn_struct_ctx_t _asn_ctx;
} SourceNGRANNode_ToTargetNGRANNode_TransparentContainer_t;



extern asn_TYPE_descriptor_t asn_DEF_SourceNGRANNode_ToTargetNGRANNode_TransparentContainer;

#ifdef __cplusplus
}
#endif

#endif	

#include <asn_internal.h>
