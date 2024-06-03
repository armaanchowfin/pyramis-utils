/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#include "TAIListForPagingItem.h"

#include "ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_TAIListForPagingItem_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TAIListForPagingItem, tAI),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TAI,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"tAI"
		},
	{ ATF_POINTER, 1, offsetof(struct TAIListForPagingItem, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_175P163,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_TAIListForPagingItem_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_TAIListForPagingItem_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TAIListForPagingItem_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* tAI */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_TAIListForPagingItem_specs_1 = {
	sizeof(struct TAIListForPagingItem),
	offsetof(struct TAIListForPagingItem, _asn_ctx),
	asn_MAP_TAIListForPagingItem_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_TAIListForPagingItem_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_TAIListForPagingItem = {
	"TAIListForPagingItem",
	"TAIListForPagingItem",
	&asn_OP_SEQUENCE,
	asn_DEF_TAIListForPagingItem_tags_1,
	sizeof(asn_DEF_TAIListForPagingItem_tags_1)
		/sizeof(asn_DEF_TAIListForPagingItem_tags_1[0]), /* 1 */
	asn_DEF_TAIListForPagingItem_tags_1,	/* Same as above */
	sizeof(asn_DEF_TAIListForPagingItem_tags_1)
		/sizeof(asn_DEF_TAIListForPagingItem_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_TAIListForPagingItem_1,
	2,	/* Elements count */
	&asn_SPC_TAIListForPagingItem_specs_1	/* Additional specs */
};

