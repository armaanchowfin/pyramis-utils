/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "InformationElementDefinitions.asn"
 * 	`asn1c -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -gen-PER`
 */

#include "PWSFailedCellIDList.h"

#include "EUTRA-CGIList.h"
#include "NR-CGIList.h"
#include "ProtocolIE-SingleContainer.h"
static asn_oer_constraints_t asn_OER_type_PWSFailedCellIDList_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_PWSFailedCellIDList_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_PWSFailedCellIDList_1[] = {
	{ ATF_POINTER, 0, offsetof(struct PWSFailedCellIDList, choice.eUTRA_CGI_PWSFailedList),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EUTRA_CGIList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eUTRA-CGI-PWSFailedList"
		},
	{ ATF_POINTER, 0, offsetof(struct PWSFailedCellIDList, choice.nR_CGI_PWSFailedList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NR_CGIList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nR-CGI-PWSFailedList"
		},
	{ ATF_POINTER, 0, offsetof(struct PWSFailedCellIDList, choice.choice_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolIE_SingleContainer_127P15,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"choice-Extensions"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_PWSFailedCellIDList_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* eUTRA-CGI-PWSFailedList */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* nR-CGI-PWSFailedList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* choice-Extensions */
};
asn_CHOICE_specifics_t asn_SPC_PWSFailedCellIDList_specs_1 = {
	sizeof(struct PWSFailedCellIDList),
	offsetof(struct PWSFailedCellIDList, _asn_ctx),
	offsetof(struct PWSFailedCellIDList, present),
	sizeof(((struct PWSFailedCellIDList *)0)->present),
	asn_MAP_PWSFailedCellIDList_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_PWSFailedCellIDList = {
	"PWSFailedCellIDList",
	"PWSFailedCellIDList",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_PWSFailedCellIDList_constr_1, &asn_PER_type_PWSFailedCellIDList_constr_1, CHOICE_constraint },
	asn_MBR_PWSFailedCellIDList_1,
	3,	/* Elements count */
	&asn_SPC_PWSFailedCellIDList_specs_1	/* Additional specs */
};

