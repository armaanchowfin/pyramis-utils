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

#include "nas/mobility/SecurityModeComplete.h"
#include "common/include/setup.h"
#include "platform/include/platform.h"
#include "platform/include/logging.h"

int decodeSecuModeComplete(uint8_t *buffer, SecurityModeComplete_t *msgStruct,
		  uint32_t decodLen)
{
	nasLogENTRY;

	uint32_t len = 0;
	if(buffer == NULL || msgStruct == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	// IMEISV(TLV) request field OPTIONAL, not added yet
	// NAS Message Container(TLV-3) OPTIONAL
#if 0
	/* decode NAS security algorithms */
	if (len + 1 > decodLen) {
		higLog("%s", "FAILURE, Incomplete buffer");
		// be allocated outside of this function
		nasLogEXIT;
		return FAILURE;
	}
	memcpy(&(msgStruct->nasSecurityAlgorithm),buffer + len,sizeof(msgStruct->nasSecurityAlgorithm));
	len += 1;
	
	// decode ngKsi
	if (len + 1 > decodLen) {
		/* The last elements, equality mostly.*/
		higLog("%s", "FAILURE, Incomplete buffer");
		nasLogEXIT;
		return FAILURE;
	}	
	uint8_t mask = 0;
	memcpy(&mask,buffer + len,1);
	msgStruct->ngKsi.tsc = mask >> 7;
	msgStruct->ngKsi.ngksi = mask >> 4;
	len += 1;
	
	// decode replayed user security capability
	if (len + 8 > decodLen) {
		/* The last elements, equality mostly.*/
		higLog("%s", "FAILURE, Incomplete buffer");
		nasLogEXIT;
		return FAILURE;
	}
	memcpy(&(msgStruct->ueSecuCapability.NrEncryptionAlgo),buffer + len,
						sizeof(msgStruct->ueSecuCapability.NrEncryptionAlgo));
	len += sizeof(msgStruct->ueSecuCapability.NrEncryptionAlgo);
	memcpy(&(msgStruct->ueSecuCapability.NrIntegrityAlgo),buffer + len,
						sizeof(msgStruct->ueSecuCapability.NrIntegrityAlgo));
	len += sizeof(msgStruct->ueSecuCapability.NrIntegrityAlgo);
	memcpy(&(msgStruct->ueSecuCapability.UtraEncryptionAlgo),buffer + len,
						sizeof(msgStruct->ueSecuCapability.UtraEncryptionAlgo));
	len += sizeof(msgStruct->ueSecuCapability.UtraEncryptionAlgo);
	memcpy(&(msgStruct->ueSecuCapability.UtraIntglgo),buffer + len,
						sizeof(msgStruct->ueSecuCapability.UtraIntglgo));
	len += sizeof(msgStruct->ueSecuCapability.UtraIntglgo);


	// IMEISV request field OPTIONAL, not added yet
#endif

	/*TODO: Must Equality (len == decodLen) be enforced ?*/
	nasLog("Successfully decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;
}


/* An empty buffer and a filled AuthReq should be passed.
 * returns SUCCESS/FAILURE AND Number of bytes encoded is set in encodedLen.
 */

int encodeSecuModeComplete(uint8_t *buffer, uint32_t bufLen,
		  SecurityModeComplete_t *msgStruct, uint32_t *encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if( buffer == NULL || bufLen == 0 || msgStruct == NULL ||
			encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	/* 	We dont need to encode 	
		1.  ExtendedProtocolDiscriminator_t
		2. 	SecurityHeaderType_t
		3.  NasMessageType_t 
	*/

	// IMEISV request field OPTIONAL, so not adding it as of now

#if 0
	// start from nasSecurityAlgorithm IE
	if (len + 1 > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
		nasLogEXIT;
		return FAILURE;
	}
	memcpy(buffer+len,&(msgStruct->nasSecurityAlgorithm),
						sizeof(msgStruct->nasSecurityAlgorithm));
	len += sizeof(msgStruct->nasSecurityAlgorithm);	
	/*4. NaskeysetId_t, a NaskeysetId struct, 4 bits in size
		*   ecoded as a byte, extracted using a mask */
	if (len + 1 > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
		nasLogEXIT;
		return FAILURE;
	}
	uint8_t mask = 0;
	mask = (msgStruct->ngKsi.tsc) ? (1<<7) : 0;
	mask = mask | ((msgStruct->ngKsi.ngksi) << 4);
	memcpy(buffer + len,&mask,1);
	len += 1;
	/*7. copy UeSecurityCapability_t, a struct */
	if (len + 8 > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
		nasLogEXIT;
		return FAILURE;
	}
	memcpy(buffer + len,&(msgStruct->ueSecuCapability.NrEncryptionAlgo),
						sizeof(msgStruct->ueSecuCapability.NrEncryptionAlgo));
	len += sizeof(msgStruct->ueSecuCapability.NrEncryptionAlgo);
	memcpy(buffer + len,&(msgStruct->ueSecuCapability.NrIntegrityAlgo),
						sizeof(msgStruct->ueSecuCapability.NrIntegrityAlgo));
	len += sizeof(msgStruct->ueSecuCapability.NrIntegrityAlgo);
	memcpy(buffer + len,&(msgStruct->ueSecuCapability.UtraEncryptionAlgo),
						sizeof(msgStruct->ueSecuCapability.UtraEncryptionAlgo));
	len += sizeof(msgStruct->ueSecuCapability.NrEncryptionAlgo);
	memcpy(buffer + len,&(msgStruct->ueSecuCapability.UtraIntglgo),
						sizeof(msgStruct->ueSecuCapability.UtraIntglgo));
	len += sizeof(msgStruct->ueSecuCapability.UtraIntglgo);
	
	// IMEISV request field OPTIONAL, so not adding it as of now

#endif
	*encodedLen = len;
	nasLog("Encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;
}
