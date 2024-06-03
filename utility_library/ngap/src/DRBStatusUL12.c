/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#include "DRBStatusUL12.h"

#include "ProtocolExtensionContainer.h"
static int
memb_receiveStatusOfUL_PDCP_SDUs_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size >= 1 && size <= 2048)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_receiveStatusOfUL_PDCP_SDUs_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..2048)) */};
static asn_per_constraints_t asn_PER_memb_receiveStatusOfUL_PDCP_SDUs_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 11,  11,  1,  2048 }	/* (SIZE(1..2048)) */,
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_DRBStatusUL12_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DRBStatusUL12, uL_COUNTValue),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_COUNTValueForPDCP_SN12,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"uL-COUNTValue"
		},
	{ ATF_POINTER, 2, offsetof(struct DRBStatusUL12, receiveStatusOfUL_PDCP_SDUs),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_receiveStatusOfUL_PDCP_SDUs_constr_3, &asn_PER_memb_receiveStatusOfUL_PDCP_SDUs_constr_3,  memb_receiveStatusOfUL_PDCP_SDUs_constraint_1 },
		0, 0, /* No default value */
		"receiveStatusOfUL-PDCP-SDUs"
		},
	{ ATF_POINTER, 1, offsetof(struct DRBStatusUL12, iE_Extension),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_175P37,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extension"
		},
};
static const int asn_MAP_DRBStatusUL12_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_DRBStatusUL12_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DRBStatusUL12_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* uL-COUNTValue */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* receiveStatusOfUL-PDCP-SDUs */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* iE-Extension */
};
asn_SEQUENCE_specifics_t asn_SPC_DRBStatusUL12_specs_1 = {
	sizeof(struct DRBStatusUL12),
	offsetof(struct DRBStatusUL12, _asn_ctx),
	asn_MAP_DRBStatusUL12_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_DRBStatusUL12_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_DRBStatusUL12 = {
	"DRBStatusUL12",
	"DRBStatusUL12",
	&asn_OP_SEQUENCE,
	asn_DEF_DRBStatusUL12_tags_1,
	sizeof(asn_DEF_DRBStatusUL12_tags_1)
		/sizeof(asn_DEF_DRBStatusUL12_tags_1[0]), /* 1 */
	asn_DEF_DRBStatusUL12_tags_1,	/* Same as above */
	sizeof(asn_DEF_DRBStatusUL12_tags_1)
		/sizeof(asn_DEF_DRBStatusUL12_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_DRBStatusUL12_1,
	3,	/* Elements count */
	&asn_SPC_DRBStatusUL12_specs_1	/* Additional specs */
};

