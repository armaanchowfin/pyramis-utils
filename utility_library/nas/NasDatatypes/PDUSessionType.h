/*********************************************************************
*
* IITB CONFIDENTIAL
* __________________
*
* All Rights Reserved © 2018-19 IITB, India
* All Rights Reserved © 2019-20 IITB, India
*
* NOTICE: All information contained in this document/file is, and remains the 
* property of Indian Institute of Technology, Bombay (IITB).The intellectual
* and technical concepts contained herein may be proprietary to IITB and are
* protected by trade secret or copyright laws. Unauthorized use, dissemination, 
* reproduction, duplication, reverse engineering, any form of redistribution, 
* or use in part or in whole, by any means is strictly forbidden unless prior, 
* express, written and signed consent is obtained from IITB.
*
* Institute   : Indian Institute of Technology Bombay (IITB)
*/

#ifndef __PDUSESSIONTYPE_H
#define __PDUSESSIONTYPE_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define SIZE_OF_PDU_SESSION_TYPE	1

#define PDU_SESSION_TYPE_INVALID    0b000
#define PDU_SESSION_TYPE_IPV4  0b001
#define	PDU_SESSION_TYPE_IPV6  0b010
#define PDU_SESSION_TYPE_IPV4V6  0b011
#define PDU_SESSION_TYPE_UNSTRUCTURED 0b100
#define PDU_SESSION_TYPE_ETHERNET 0b101
#define PDU_SESSION_TYPE_RESERVED 0b111



typedef struct pduSessionType{
	uint8_t:5;      

	uint8_t pdu_session_type_value:3;
}pduSessionType_t;

int decodePDUSessionType(uint8_t *buffer, uint32_t bufLen,
			           pduSessionType_t *pduSType, uint32_t* decodedLen,bool isOptional);
int encodePDUSessionType(uint8_t *buffer, uint32_t bufLen,
                       pduSessionType_t *pduSType, uint32_t *encodedLen,bool isOptional);

#endif
