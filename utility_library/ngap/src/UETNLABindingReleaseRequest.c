/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-PDU-Contents"
 * 	found in "PDUDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#include "UETNLABindingReleaseRequest.h"

asn_TYPE_member_t asn_MBR_UETNLABindingReleaseRequest_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UETNLABindingReleaseRequest, protocolIEs),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolIE_Container_124P75,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"protocolIEs"
		},
};
static const ber_tlv_tag_t asn_DEF_UETNLABindingReleaseRequest_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UETNLABindingReleaseRequest_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* protocolIEs */
};
asn_SEQUENCE_specifics_t asn_SPC_UETNLABindingReleaseRequest_specs_1 = {
	sizeof(struct UETNLABindingReleaseRequest),
	offsetof(struct UETNLABindingReleaseRequest, _asn_ctx),
	asn_MAP_UETNLABindingReleaseRequest_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UETNLABindingReleaseRequest = {
	"UETNLABindingReleaseRequest",
	"UETNLABindingReleaseRequest",
	&asn_OP_SEQUENCE,
	asn_DEF_UETNLABindingReleaseRequest_tags_1,
	sizeof(asn_DEF_UETNLABindingReleaseRequest_tags_1)
		/sizeof(asn_DEF_UETNLABindingReleaseRequest_tags_1[0]), /* 1 */
	asn_DEF_UETNLABindingReleaseRequest_tags_1,	/* Same as above */
	sizeof(asn_DEF_UETNLABindingReleaseRequest_tags_1)
		/sizeof(asn_DEF_UETNLABindingReleaseRequest_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UETNLABindingReleaseRequest_1,
	1,	/* Elements count */
	&asn_SPC_UETNLABindingReleaseRequest_specs_1	/* Additional specs */
};

