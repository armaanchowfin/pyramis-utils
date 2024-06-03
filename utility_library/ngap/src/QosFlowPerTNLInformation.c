/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#include "QosFlowPerTNLInformation.h"

#include "ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_QosFlowPerTNLInformation_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct QosFlowPerTNLInformation, uPTransportLayerInformation),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_UPTransportLayerInformation,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"uPTransportLayerInformation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct QosFlowPerTNLInformation, associatedQosFlowList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AssociatedQosFlowList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"associatedQosFlowList"
		},
	{ ATF_POINTER, 1, offsetof(struct QosFlowPerTNLInformation, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_175P132,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_QosFlowPerTNLInformation_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_QosFlowPerTNLInformation_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_QosFlowPerTNLInformation_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* uPTransportLayerInformation */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* associatedQosFlowList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_QosFlowPerTNLInformation_specs_1 = {
	sizeof(struct QosFlowPerTNLInformation),
	offsetof(struct QosFlowPerTNLInformation, _asn_ctx),
	asn_MAP_QosFlowPerTNLInformation_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_QosFlowPerTNLInformation_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_QosFlowPerTNLInformation = {
	"QosFlowPerTNLInformation",
	"QosFlowPerTNLInformation",
	&asn_OP_SEQUENCE,
	asn_DEF_QosFlowPerTNLInformation_tags_1,
	sizeof(asn_DEF_QosFlowPerTNLInformation_tags_1)
		/sizeof(asn_DEF_QosFlowPerTNLInformation_tags_1[0]), /* 1 */
	asn_DEF_QosFlowPerTNLInformation_tags_1,	/* Same as above */
	sizeof(asn_DEF_QosFlowPerTNLInformation_tags_1)
		/sizeof(asn_DEF_QosFlowPerTNLInformation_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_QosFlowPerTNLInformation_1,
	3,	/* Elements count */
	&asn_SPC_QosFlowPerTNLInformation_specs_1	/* Additional specs */
};

