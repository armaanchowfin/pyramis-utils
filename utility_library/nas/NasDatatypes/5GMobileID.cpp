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

#include "nas/NasDatatypes/5GMobileID.h"

int decode5GGuti(uint8_t *buffer, uint32_t bufLen,
			           guti_5gMobileId_t* guti, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint8_t mask = 0;

    if(buffer == NULL || bufLen == 0 || guti == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}
    
    if(len + GUTI_LENGTH > bufLen){
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

	mask = *(buffer + len);
	guti->reserved = (mask & 0xF0) >> 4;    //bits 4to7
    guti->spare = (mask & (1<<3) ) >> 3;  //3rd bit
	guti->identityType = mask & 0x07;
	len++;

	nasLog("5G Guti : reserved %d | spare %d | identityType %d", 
	            guti->reserved, guti->spare, guti->identityType);
	nasLog("current len %d | decoded 1 byte", len);

	mask = *(buffer + len);
	guti->mccDigit1 = mask & 0x0F;
	guti->mccDigit2 = (mask & 0xF0)>> 4;
	len++;

	mask = *(buffer + len);
	guti->mccDigit3 = mask & 0x0F;
	guti->mncDigit3 = (mask & 0xF0)>> 4;
	len++;

	mask = *(buffer + len);
	guti->mncDigit1 = mask & 0x0F;
	guti->mncDigit2 = (mask & 0xF0)>> 4;
	len++;

	nasLog("mccDigit1 0x%x | mccDigit2 0x%x | mccDigit3 0x%x", 
	         guti->mccDigit1, guti->mccDigit2, guti->mccDigit3); 
	nasLog("mncDigit1 0x%x | mncDigit2 0x%x | mncDigit3 0x%x", 
	         guti->mncDigit1, guti->mncDigit2, guti->mncDigit3);
	nasLog("current len %d | decoded 3 bytes", len);

    if( guti->mccDigit1 > 9 || guti->mccDigit2 > 9 || guti->mccDigit3 > 9 ||
        (guti->mncDigit3 == 0xF ? false : guti->mncDigit3 > 9) ||
        guti->mncDigit2 > 9 || guti->mncDigit1 > 9 ) {
        higLog("MCC MNC Digit Constraint check failed");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

	memcpy(&(guti->amfRegionId), buffer + len, sizeof(guti->amfRegionId));
	len += sizeof(guti->amfRegionId);

    /*variable to store and set bit fields value accordingly*/
    uint16_t var;
    var = *(buffer + len);
    var = (var << 8 | *(buffer + len + 1));
    guti->amfSetId = (var >> 6);
    guti->amfPointer = (var & 0x3F);
    
    len += sizeof(uint16_t);

	nasLog("amfRegionId 0x%x | amfSetId 0x%x | amfPointer 0x%x", 
	          guti->amfRegionId, guti->amfSetId, guti->amfPointer);
	nasLog("current len %d | decoded 3 bytes", len);

    guti->_5gTMSI = *(buffer + len);
    guti->_5gTMSI = ((guti->_5gTMSI << 8) | *(buffer + len + 1));
    guti->_5gTMSI = ((guti->_5gTMSI << 8) | *(buffer + len + 2));
    guti->_5gTMSI = ((guti->_5gTMSI << 8) | *(buffer + len + 3));
	len += sizeof(guti->_5gTMSI);
	
	nasLog("5gTMSI 0x%x | current len %d | decoded %d bytes",  guti->_5gTMSI, len, 
	            					sizeof(guti->_5gTMSI));

	*decodedLen = len;
    nasLogEXIT;
	return SUCCESS;
}

int encode5GGuti(uint8_t *buffer, uint32_t bufLen,
                       guti_5gMobileId_t* guti, uint32_t *encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint8_t mask = 0;

    if(buffer == NULL || bufLen == 0 || guti == NULL || encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    if (len + GUTI_LENGTH > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

	mask = mask | ((guti->reserved) << 4); 
	mask = mask | ((guti->spare) << 3);
	mask = mask | (guti->identityType);
	*(buffer + len) = mask;
	len++;

	nasLog("5G Guti : reserved %d | spare %d | identityType %d", 
		guti->reserved, guti->spare, guti->identityType);
	nasLog("current len %d | encoded 1 byte", len);

	mask = 0;
	mask = mask | (guti->mccDigit1);
	mask = mask | ((guti->mccDigit2) << 4);
	*(buffer + len) = mask;
	len++;

	mask = 0;
	mask = mask | (guti->mccDigit3);
	mask = mask | ((guti->mncDigit3) << 4);
	*(buffer + len) = mask;
	len++;

	mask = 0;
	mask = mask | (guti->mncDigit1);
	mask = mask | ((guti->mncDigit2) << 4);
	*(buffer + len) = mask;
	len++;

	nasLog("mccDigit1 0x%x | mccDigit2 0x%x | mccDigit3 0x%x", 
		guti->mccDigit1, guti->mccDigit2, guti->mccDigit3); 
	nasLog("mncDigit1 0x%x | mncDigit2 0x%x | mncDigit3 0x%x", 
		guti->mncDigit1, guti->mncDigit2, guti->mncDigit3);
	nasLog("current len %d | encoded 3 bytes", len);
	

	memcpy(buffer + len, &(guti->amfRegionId), sizeof(guti->amfRegionId));
	len += sizeof(guti->amfRegionId);

    *(buffer + len) = (guti->amfSetId >> 2); 
    *(buffer + len + 1) = ((guti->amfSetId & 0x03) << 6) | (guti->amfPointer);
     
    len += sizeof(uint16_t);

	nasLog("amfRegionId 0x%x | amfSetId 0x%x | amfPointer 0x%x", 
		guti->amfRegionId, guti->amfSetId, guti->amfPointer);
	nasLog("current len %d | encoded 3 bytes", len);

    *(buffer + len)     = (guti->_5gTMSI >> 24);
    *(buffer + len + 1) = (guti->_5gTMSI >> 16);
    *(buffer + len + 2) = (guti->_5gTMSI >> 8);
    *(buffer + len + 3) = (guti->_5gTMSI);
	len += sizeof(guti->_5gTMSI);

	nasLog("5gTMSI 0x%x | current len %d | encoded %d bytes", 
		guti->_5gTMSI, len, sizeof(guti->_5gTMSI));

	*encodedLen = len;
    nasLogEXIT;
	return SUCCESS;
}


/**
 * typedef struct suci_5gMobileId {
    uint8_t          spare1:1;
    uint8_t          supiFormat:3;
    uint8_t          spare:1;
    uint8_t          identityType:3;
    uint8_t          mccDigit1:4;
    uint8_t          mccDigit2:4;
    uint8_t          mccDigit3:4;
    uint8_t          mncDigit3:4;
    uint8_t          mncDigit2:4;
    uint8_t          mncDigit1:4;
    uint8_t          routingInc1:4;   
    uint8_t          routingInc2:4; 
    uint8_t          routingInc3:4;
    uint8_t          routingInc4:4;
    uint8_t          spare2:4;
    uint8_t          protectionSchId:4;
    uint8_t          homeNtwrkPKI;
    uint8_t          schemeOutput[MSIN_LENGTH];

}suci_5gMobileId_t;
*/
int decodeSuci(uint8_t *buffer, uint32_t bufLen,
			           suci_5gMobileId_t* suci, uint32_t* decodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;
    uint8_t mask = 0;

    if(buffer == NULL || bufLen == 0 || suci == NULL || decodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}
    // expects the buffer to contain a full message?
    if(sizeof(*suci) > bufLen){
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    // decode first byte
	mask = *(buffer + len); // mask is the current (1st) byte
    suci->identityType = mask & 0x07; // val & 0000 0111 i.e. last 3 bits of the first byte == identity type
    suci->spare = mask & (1<<3); // val & 1000 == 00001000) i.e. 5th bit in the byte
    suci->supiFormat = mask & 0x70; // val & 0111 0000 i.e. bits 1, 2 ,3 of the byte
    suci->spare1 = mask & (1<<7); // val & 10000000 i.e. first bit of the byte.
    len++; // go to next byte.

    nasLog("identityType %d | spare %d | supiFormat %d",
            suci->identityType, suci->spare, suci->supiFormat);
                           

    nasLog("current len %d | decoded 1 byte", len);
    
    mask = *(buffer + len);
    suci->mccDigit1 = mask & 0x0F; // val & 0000 1111 i.e. last 4 bits of the byte
    suci->mccDigit2 = (mask & 0xF0)>> 4; // val & 1111 0000 >> 4 i.e. first 4 bits of the byte
    len++;

    mask = *(buffer + len);
    suci->mccDigit3 = mask & 0x0F;
    suci->mncDigit3 = (mask & 0xF0)>> 4;
    len++;

    mask = *(buffer + len);
    suci->mncDigit1 = mask & 0x0F;
    suci->mncDigit2 = (mask & 0xF0)>> 4;
    len++;

    nasLog("mccDigit1 %d | mccDigit2 %d | mccDigit3 %d", 
                            suci->mccDigit1, 
                            suci->mccDigit2, 
                            suci->mccDigit3); 
    nasLog("mncDigit1 %d | mncDigit2 %d | mncDigit3 %d", 
            suci->mncDigit1, suci->mncDigit2, suci->mncDigit3);
    nasLog("current len %d | decoded 3 bytes", len);

    if( suci->mccDigit1 > 9 || suci->mccDigit2 > 9 || suci->mccDigit3 > 9 ||
        (suci->mncDigit3 == 0xF ? false : suci->mncDigit3 > 9) ||
        suci->mncDigit2 > 9 || suci->mncDigit1 > 9 ) {
        higLog("MCC MNC Digit Constraint check failed");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    mask = *(buffer + len);
    suci->routingInc1 = mask & 0x0F;
    suci->routingInc2 = (mask & 0xF0)>> 4;
    len++;


    mask = *(buffer + len);
    suci->routingInc3 = mask & 0x0F;
    suci->routingInc4 = (mask & 0xF0)>> 4;
    len++;

    nasLog("routerDigit1 %d| routerDigit2 %d| routerDigit3 %d| routerDigit4 %d", 
    suci->routingInc1, suci->routingInc2, suci->routingInc3, suci->routingInc4);

    nasLog("current len %d | decoded 2 bytes", len);

    mask = *(buffer + len);
    suci->protectionSchId = mask & 0x0F;
    suci->spare2 = (mask & 0xF0)>> 4;
    len++;

    nasLog("protection scheme Id %d | current len %d | decoded 1 byte", 
                suci->protectionSchId, len); 

    memcpy(&(suci->homeNtwrkPKI), 
    buffer + len, sizeof(suci->homeNtwrkPKI));
    len += sizeof(suci->homeNtwrkPKI);

    nasLog("home network public key identifier %d |current len %d | decoded %d bytes", 
            suci->homeNtwrkPKI, len, sizeof(suci->homeNtwrkPKI)); 

    /* Scheme Output length. Currently taken as 5 octet */
    memcpy(&(suci->schemeOutput[0]),
            buffer + len, sizeof(suci->schemeOutput));
    len += sizeof(suci->schemeOutput);

    nasLog("scheme output 0x%02x%02x%02x%02x%02x",
            suci->schemeOutput[0], 
            suci->schemeOutput[1], 
            suci->schemeOutput[2], 
            suci->schemeOutput[3], 
            suci->schemeOutput[4]);
    /*
        MSIN last digit := (suci->schemeOutput[4] & 0xF0)>>4

        When mncDigit3 == 0xF expect MSIN of precise 10 digits
        When mncDigit3 <= 9 expect MSIN of precise 9 digits

        When MSIN is precise 9 digits; MSIN last digit should be 0xF

        Thus if ((mncDigit3 == 0xF) && (MSIN is not of 10 digits))
        it is failure
        OR
        ((mncDigit3 != 0xF) && (MSIN is not of 9 digits))
        it is failure

     */

    /* Scheme Output length. Currently taken as 5 octet */
    uint8_t msbDigit = (suci->schemeOutput[MSIN_LENGTH-1] & 0xF0 )>>4;
    uint8_t lsbDigit = suci->schemeOutput[MSIN_LENGTH-1] & 0x0F;

    if ((suci->mncDigit3 == 0xF) && (msbDigit == 0xF)) {
        higLog("Expect MSIN of precise 10 digits");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
    }
    if ((suci->mncDigit3 != 0xF) && (msbDigit != 0xF)) {
        higLog("Expect MSIN of precise 9 digits");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
    }
    /* Only below to cases will reach this execution step
     * mncDigit3 == 0xF && msbDigit != 0xF
     * mncDigit3 != 0xF && msbDigit == 0xF
     * */

    if  ((msbDigit == 0xF ? false : msbDigit  > 9) || (lsbDigit > 9)) {
        higLog("MSIN BCD encoding is incorrect");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    for (uint8_t i =0; i < MSIN_LENGTH-1; i++) {
        msbDigit = (suci->schemeOutput[i] & 0xF0 )>>4;
        lsbDigit = suci->schemeOutput[i] & 0x0F;
        if  ((msbDigit  > 9) || (lsbDigit > 9)) {
            higLog("Recvd incorrectly BCD-encoded MSIN");
            errno = E_NAS_ERROR_INVALID_INPUT;
            nasLogEXIT;
            return FAILURE;
        }
    }

    *decodedLen = len;
    nasLogEXIT;
	return SUCCESS;

}

int encodeSuci(uint8_t *buffer, uint32_t bufLen,
                       suci_5gMobileId_t* suci, uint32_t *encodedLen)
{

	nasLogENTRY;
	uint32_t len = 0;
    uint8_t mask = 0;

    if(buffer == NULL || bufLen == 0 || suci == NULL || encodedLen == NULL) {
		higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
		return FAILURE;
	}

    if(bufLen < sizeof(*suci)){
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

	mask = mask | ((suci->spare1) << 7);
    mask = mask | ((suci->supiFormat) << 4);
    mask = mask | ((suci->spare) << 3);
    mask = mask | (suci->identityType);
    *(buffer + len) = mask;
    len++;

    nasLog("identityType %d | spare %d | supiFormat %d",
            suci->identityType, suci->spare, suci->supiFormat);

    nasLog("current len %d | encoded 1 byte", len);

    mask = 0;
    mask = mask | (suci->mccDigit1);
    mask = mask | ((suci->mccDigit2) << 4);
    *(buffer + len) = mask;
    len++;

    mask = 0;
    mask = mask | (suci->mccDigit3);
    mask = mask | ((suci->mncDigit3) << 4);
    *(buffer + len) = mask;
    len++;

    mask = 0;
    mask = mask | (suci->mncDigit1);
    mask = mask | ((suci->mncDigit2) << 4);
    *(buffer + len) = mask;
    len++;

    nasLog("mccDigit1 %d | mccDigit2 %d | mccDigit3 %d", 
           suci->mccDigit1, suci->mccDigit2, suci->mccDigit3); 
    nasLog("mncDigit1 %d | mncDigit2 %d | mncDigit3 %d", 
           suci->mncDigit1, suci->mncDigit2, suci->mncDigit3);
    nasLog("current len %d | encoded 3 bytes", len);

    if( suci->mccDigit1 > 9 || suci->mccDigit2 > 9 || suci->mccDigit3 > 9 ||
        (suci->mncDigit3 == 0xF ? false : suci->mncDigit3 > 9) ||
        suci->mncDigit2 > 9 || suci->mncDigit1 > 9 ) {
        higLog("MCC MNC Digit Constraint check failed");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    mask = 0;
    mask = mask | (suci->routingInc1);
    mask = mask | ((suci->routingInc2) << 4);
    *(buffer + len) = mask;
    len++;

    mask = 0;
    mask = mask | (suci->routingInc3);
    mask = mask | ((suci->routingInc4) << 4);
    *(buffer + len) = mask;
    len++;

    nasLog("routerDigit1 %d| routerDigit2 %d| routerDigit3 %d| routerDigit4 %d", 
    suci->routingInc1, suci->routingInc2, suci->routingInc3, suci->routingInc4);

    nasLog("current len %d | encoded 2 bytes", len);

    mask =0;
    mask = mask | (suci->protectionSchId);
    mask = mask | ((suci->spare2) << 4);
    *(buffer + len) = mask;
    len++;

    nasLog("protection scheme Id %d | current len %d | encoded 1 byte", 
                suci->protectionSchId, len); 

    memcpy(buffer + len, &(suci->homeNtwrkPKI), sizeof(suci->homeNtwrkPKI));
    len += sizeof(suci->homeNtwrkPKI);

    nasLog("home network public key identifier %d |current len %d | encoded %d bytes", 
            suci->homeNtwrkPKI, len, sizeof(suci->homeNtwrkPKI));  


    /* Scheme Output length. Currently taken as 5 octet */
    uint8_t msbDigit = (suci->schemeOutput[MSIN_LENGTH-1] & 0xF0 )>>4;
    uint8_t lsbDigit = suci->schemeOutput[MSIN_LENGTH-1] & 0x0F;

    if ((suci->mncDigit3 == 0xF) && (msbDigit == 0xF)) {
        higLog("MSIN should be precise 10 digits");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
		return FAILURE;
    }
    if ((suci->mncDigit3 != 0xF) && (msbDigit != 0xF)) {
        higLog("MSIN should be precise 9 digits");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
		return FAILURE;
    }
    /* Only below to cases will reach this execution step
     * mncDigit3 == 0xF && msbDigit != 0xF
     * mncDigit3 != 0xF && msbDigit == 0xF
     * */

    if  ((msbDigit == 0xF ? false : msbDigit  > 9) || (lsbDigit > 9)) {
        higLog("MSIN BCD encoding is incorrect");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    for (uint8_t i =0; i < MSIN_LENGTH-1; i++) {
        msbDigit = (suci->schemeOutput[i] & 0xF0 )>>4;
        lsbDigit = suci->schemeOutput[i] & 0x0F;
        if  ((msbDigit  > 9) || (lsbDigit > 9)) {
            higLog("MSIN BCD encoding is incorrect");
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
            nasLogEXIT;
            return FAILURE;
        }
    }

    uint8_t *tempPtr = buffer + len;
    memcpy(buffer+len,
            &(suci->schemeOutput[0]),
            sizeof(suci->schemeOutput));
    len += sizeof(suci->schemeOutput);

    nasLog("scheme output 0x%02x%02x%02x%02x%02x",
            tempPtr[0], tempPtr[1], tempPtr[2], tempPtr[3], tempPtr[4]);

    *encodedLen = len;
    nasLogEXIT;
	return SUCCESS;
}
