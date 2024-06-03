#ifndef	ASN1PRINT_H
#define	ASN1PRINT_H

enum asn1print_flags {
	APF_NOFLAGS,
	APF_NOINDENT		= 0x01,	

	APF_LINE_COMMENTS	= 0x02, 

	APF_PRINT_XML_DTD	= 0x04,	

	APF_PRINT_CONSTRAINTS	= 0x08,	

	APF_PRINT_CLASS_MATRIX	= 0x10,	

};

/*
 * Print the contents of the parsed ASN.1 syntax tree.
 */
int asn1print(asn1p_t *asn, enum asn1print_flags flags);

const char *asn1p_constraint_string(const asn1p_constraint_t *ct);

#endif	

