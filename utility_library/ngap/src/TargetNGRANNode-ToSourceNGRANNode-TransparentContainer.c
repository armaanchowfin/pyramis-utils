/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#include "TargetNGRANNode-ToSourceNGRANNode-TransparentContainer.h"

#include "ProtocolExtensionContainer.h"
static asn_TYPE_member_t asn_MBR_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TargetNGRANNode_ToSourceNGRANNode_TransparentContainer, rRCContainer),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRCContainer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rRCContainer"
		},
	{ ATF_POINTER, 1, offsetof(struct TargetNGRANNode_ToSourceNGRANNode_TransparentContainer, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_175P165,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* rRCContainer */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* iE-Extensions */
};
static asn_SEQUENCE_specifics_t asn_SPC_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_specs_1 = {
	sizeof(struct TargetNGRANNode_ToSourceNGRANNode_TransparentContainer),
	offsetof(struct TargetNGRANNode_ToSourceNGRANNode_TransparentContainer, _asn_ctx),
	asn_MAP_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer = {
	"TargetNGRANNode-ToSourceNGRANNode-TransparentContainer",
	"TargetNGRANNode-ToSourceNGRANNode-TransparentContainer",
	&asn_OP_SEQUENCE,
	asn_DEF_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tags_1,
	sizeof(asn_DEF_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tags_1)
		/sizeof(asn_DEF_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tags_1[0]), /* 1 */
	asn_DEF_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tags_1,	/* Same as above */
	sizeof(asn_DEF_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tags_1)
		/sizeof(asn_DEF_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_1,
	2,	/* Elements count */
	&asn_SPC_TargetNGRANNode_ToSourceNGRANNode_TransparentContainer_specs_1	/* Additional specs */
};

