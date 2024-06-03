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

#include "common/include/setup.h"
#include "platform/include/platform.h"
#include "platform/include/logging.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "nas/mobility/SecurityModeCommand.h"

/* TODO LATER: Use a fixed mem pool into which decoded data can be written.
 */

/* elements are filled into user allocated nasMessage in the function.
 * returns SUCCESS/FAILURE
 */
int decodeSecuModeCommand(uint8_t *buffer, SecurityModeCommand_t *msgStruct,
						  uint32_t decodLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint32_t consumedLen = 0;

	if(buffer == NULL || msgStruct == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}
	/* decode NAS security algorithms */
	if (len + sizeof(msgStruct->nasSecurityAlgorithm) > decodLen) {
		higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}
	memcpy(&(msgStruct->nasSecurityAlgorithm), buffer + len,
			sizeof(msgStruct->nasSecurityAlgorithm));
	len += sizeof(msgStruct->nasSecurityAlgorithm);
	lowLog("Decode time msgStruct->nasSecurityAlgorithm.encryptionAlgo:%2X",
		msgStruct->nasSecurityAlgorithm.encryptionAlgo);
	lowLog("Decode time msgStruct->nasSecurityAlgorithm.integrityAlgo:%2X",
		msgStruct->nasSecurityAlgorithm.integrityAlgo);
	// decode ngKsi
	if (len + sizeof(msgStruct->ngKsi) > decodLen) {
		/* The last elements, equality mostly.*/
		higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}
    if(decodeNasKeySetId(buffer + len, &msgStruct->ngKsi, decodLen - len,
                      &consumedLen) == FAILURE) {
        higLog("decoding Nas Key Set Id failed");
        return FAILURE;
    }
    len += consumedLen;
    nasLog("ngKsi tsc: %d | ngksi: %d | currentLen %d | sizeof : %d",
            msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi, len, consumedLen);
	// decode replayed user security capability
	if (len + 4 > decodLen) {
		/* The last elements, equality mostly.*/
		higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
		nasLogEXIT;
		return FAILURE;
	}

    consumedLen = 0;
    if(decodeUESecCap(buffer +len, decodLen - len,
                &(msgStruct->ueSecuCapability), &consumedLen) == FAILURE) 
    {
        higLog("Decoding UE Security Capability Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += consumedLen;

//	nasLog("Encode time msgStruct->ueSecuCapability._5gNASIntAlgo:%2X", 
//		msgStruct->ueSecuCapability._5gNASIntAlgo);
//	nasLog("Encode time msgStruct->ueSecuCapability._5gNASEncAlgo:%2X", 
//		msgStruct->ueSecuCapability._5gNASEncAlgo);
//
//	memcpy(&(msgStruct->ueSecuCapability._5gNASEncAlgo),buffer + len,
//						sizeof(msgStruct->ueSecuCapability._5gNASEncAlgo));
//	len += sizeof(msgStruct->ueSecuCapability._5gNASEncAlgo);
//	memcpy(&(msgStruct->ueSecuCapability._5gNASIntAlgo),buffer + len,
//						sizeof(msgStruct->ueSecuCapability._5gNASIntAlgo));
//	len += sizeof(msgStruct->ueSecuCapability._5gNASIntAlgo);
//	memcpy(&(msgStruct->ueSecuCapability._5gASEncAlgo),buffer + len,
//						sizeof(msgStruct->ueSecuCapability._5gASEncAlgo));
//	len += sizeof(msgStruct->ueSecuCapability._5gASEncAlgo);
//	memcpy(&(msgStruct->ueSecuCapability._5gASIntAlgo),buffer + len,
//						sizeof(msgStruct->ueSecuCapability._5gASIntAlgo));
//	len += sizeof(msgStruct->ueSecuCapability._5gASIntAlgo);
//
//	nasLog("Decode time msgStruct->ueSecuCapability._5gNASIntAlgo:%2X",
//		msgStruct->ueSecuCapability._5gNASIntAlgo);
//	nasLog("Decode time msgStruct->ueSecuCapability._5gNASEncAlgo:%2X",
//		msgStruct->ueSecuCapability._5gNASEncAlgo);
//	// IMEISV request field OPTIONAL, not added yet

#if 0
	// OPTIONAL++
	//decode the hashAmf
	if(len + 9 > decodLen) {
		higLog("%s", "FAILURE, Incomplete buffer");
		nasLogEXIT;
		return FAILURE;
	}
	/*
		hashAmf IE needs to be filled by AMF if security mode command is trigged before 
	*/
	memcpy(&(msgStruct->hashAmf.iei),buffer + len,sizeof(msgStruct->hashAmf.iei));
	len += sizeof(msgStruct->hashAmf.iei);
	memcpy(&(msgStruct->hashAmf._hash),buffer + len,sizeof(msgStruct->hashAmf._hash));
	len += sizeof(msgStruct->hashAmf._hash);
#endif

	/*TODO: Must Equality (len == decodLen) be enforced ?*/
	nasLog("Successfully decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;
}


/* returns SUCCESS/FAILURE AND Number of bytes encoded is set in encodedLen.
 */
int encodeSecuModeCommand(uint8_t *buffer, uint32_t bufLen,
						  SecurityModeCommand_t *msgStruct,
						  uint32_t *encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint32_t encLen = 0;

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
	// start from nasSecurityAlgorithm IE
	if (len + sizeof(msgStruct->nasSecurityAlgorithm) > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}
	memcpy(buffer+len,&(msgStruct->nasSecurityAlgorithm),
						sizeof(msgStruct->nasSecurityAlgorithm));
	len += sizeof(msgStruct->nasSecurityAlgorithm);	

	/*4. NaskeysetId_t */
    if( encodeNasKeySetId(buffer + len, bufLen - len, &msgStruct->ngKsi,
                &encLen) == FAILURE) {
        higLog("encoding NAS Key Set Id failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += encLen;
    nasLog("ngKsi encoded | tsc %d | ngksi %d | currentLen %d | sizeof %d",
            msgStruct->ngKsi.tsc, msgStruct->ngKsi.ngksi, len, encLen);

	/*7. copy UeSecurityCapability_t, a struct */
	if (len + 4 > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
		nasLogEXIT;
		return FAILURE;
	}

    encLen = 0;
    if(encodeUESecCap(buffer +len, bufLen - len,
                &(msgStruct->ueSecuCapability), &encLen) == FAILURE) 
    {
        higLog("Encoding UE Security Capability Failed");
        nasLogEXIT;
        return FAILURE;
    }
    len += encLen;
//	nasLog("Encode time msgStruct->ueSecuCapability._5gNASIntAlgo:%2X",
//								msgStruct->ueSecuCapability._5gNASIntAlgo);
//	nasLog("Encode time msgStruct->ueSecuCapability._5gNASEncAlgo:%2X",
//								msgStruct->ueSecuCapability._5gNASEncAlgo);
//	memcpy(buffer + len,&(msgStruct->ueSecuCapability._5gNASEncAlgo),
//						sizeof(msgStruct->ueSecuCapability._5gNASEncAlgo));
//	len += sizeof(msgStruct->ueSecuCapability._5gNASEncAlgo);
//	memcpy(buffer + len,&(msgStruct->ueSecuCapability._5gNASIntAlgo),
//						sizeof(msgStruct->ueSecuCapability._5gNASIntAlgo));
//	len += sizeof(msgStruct->ueSecuCapability._5gNASIntAlgo);
//	memcpy(buffer + len,&(msgStruct->ueSecuCapability._5gASEncAlgo),
//						sizeof(msgStruct->ueSecuCapability._5gASEncAlgo));
//	len += sizeof(msgStruct->ueSecuCapability._5gASEncAlgo);
//	memcpy(buffer + len,&(msgStruct->ueSecuCapability._5gASIntAlgo),
//						sizeof(msgStruct->ueSecuCapability._5gASIntAlgo));
//	len += sizeof(msgStruct->ueSecuCapability._5gASIntAlgo);
	
	// IMEISV request field OPTIONAL, so not adding it as of now
#if 0
	// encode the hashAmf IE
	// TODO : calculate hashAmf from the received registrationRequest NAS PDU
	if (len + 9 > bufLen) {
		higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
		nasLogEXIT;
		return FAILURE;
	}
	memcpy(buffer + len,&(msgStruct->hashAmf.iei),
							sizeof(msgStruct->hashAmf.iei));
	len += sizeof(msgStruct->hashAmf.iei);
	memcpy(buffer + len,&(msgStruct->hashAmf._hash),
							sizeof(msgStruct->hashAmf._hash));
	len += sizeof(msgStruct->hashAmf._hash);
#endif

	*encodedLen = len;
	nasLog("Encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;
}
