/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#include "PDUSessionResourceModifyConfirmTransfer.h"

#include "TNLMappingList.h"
#include "QosFlowList.h"
#include "ProtocolExtensionContainer.h"
static asn_TYPE_member_t asn_MBR_PDUSessionResourceModifyConfirmTransfer_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PDUSessionResourceModifyConfirmTransfer, qosFlowModifyConfirmList),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_QosFlowModifyConfirmList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"qosFlowModifyConfirmList"
		},
	{ ATF_POINTER, 3, offsetof(struct PDUSessionResourceModifyConfirmTransfer, tNLMappingList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TNLMappingList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"tNLMappingList"
		},
	{ ATF_POINTER, 2, offsetof(struct PDUSessionResourceModifyConfirmTransfer, qosFlowFailedToModifyList),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_QosFlowList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"qosFlowFailedToModifyList"
		},
	{ ATF_POINTER, 1, offsetof(struct PDUSessionResourceModifyConfirmTransfer, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_175P94,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_PDUSessionResourceModifyConfirmTransfer_oms_1[] = { 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_PDUSessionResourceModifyConfirmTransfer_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PDUSessionResourceModifyConfirmTransfer_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* qosFlowModifyConfirmList */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* tNLMappingList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* qosFlowFailedToModifyList */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* iE-Extensions */
};
static asn_SEQUENCE_specifics_t asn_SPC_PDUSessionResourceModifyConfirmTransfer_specs_1 = {
	sizeof(struct PDUSessionResourceModifyConfirmTransfer),
	offsetof(struct PDUSessionResourceModifyConfirmTransfer, _asn_ctx),
	asn_MAP_PDUSessionResourceModifyConfirmTransfer_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_PDUSessionResourceModifyConfirmTransfer_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PDUSessionResourceModifyConfirmTransfer = {
	"PDUSessionResourceModifyConfirmTransfer",
	"PDUSessionResourceModifyConfirmTransfer",
	&asn_OP_SEQUENCE,
	asn_DEF_PDUSessionResourceModifyConfirmTransfer_tags_1,
	sizeof(asn_DEF_PDUSessionResourceModifyConfirmTransfer_tags_1)
		/sizeof(asn_DEF_PDUSessionResourceModifyConfirmTransfer_tags_1[0]), /* 1 */
	asn_DEF_PDUSessionResourceModifyConfirmTransfer_tags_1,	/* Same as above */
	sizeof(asn_DEF_PDUSessionResourceModifyConfirmTransfer_tags_1)
		/sizeof(asn_DEF_PDUSessionResourceModifyConfirmTransfer_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PDUSessionResourceModifyConfirmTransfer_1,
	4,	/* Elements count */
	&asn_SPC_PDUSessionResourceModifyConfirmTransfer_specs_1	/* Additional specs */
};

