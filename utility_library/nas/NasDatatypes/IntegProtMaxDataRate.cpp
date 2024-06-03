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

#include "nas/NasDatatypes/IntegProtMaxDataRate.h"

int decodeIntegProtMaxDataRate(uint8_t *buffer, uint32_t bufLen,
                    integProtMaxDataRate_t * ipmdr, uint32_t* decodedLen)
{
    nasLogENTRY;

    int len = 0;
    if(buffer == NULL || ipmdr == NULL || bufLen == 0 || decodedLen == NULL) {
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(bufLen < INTEG_PROT_MAX_DATA_RATE_LEN) {
        higLog("Buffer incomplete");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    ipmdr->downlinkRate = buffer[len];
    len++;
    if( ipmdr->downlinkRate < INTEG_PROT_MAX_DATA_RATE_64_KBPS ||
        ipmdr->downlinkRate > INTEG_PROT_MAX_DATA_RATE_FULL) {
        higLog("Invalid downlink rate %d", ipmdr->downlinkRate);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return  FAILURE;
    }
    if( ipmdr->downlinkRate != INTEG_PROT_MAX_DATA_RATE_64_KBPS &&
        ipmdr->downlinkRate != INTEG_PROT_MAX_DATA_RATE_FULL) {
        nasLog("Received downlink rate %d, Interpreted as 64 kbps",
                                                    ipmdr->downlinkRate);
        ipmdr->downlinkRate = INTEG_PROT_MAX_DATA_RATE_64_KBPS;
    }

    ipmdr->uplinkRate = buffer[len];
    len++;
    if(ipmdr->uplinkRate < INTEG_PROT_MAX_DATA_RATE_64_KBPS ||
        ipmdr->uplinkRate > INTEG_PROT_MAX_DATA_RATE_FULL) {
        higLog("Invalid uplink rate %d", ipmdr->uplinkRate);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return  FAILURE;
    }
    if( ipmdr->uplinkRate != INTEG_PROT_MAX_DATA_RATE_64_KBPS &&
        ipmdr->uplinkRate != INTEG_PROT_MAX_DATA_RATE_FULL) {
        nasLog("Received uplink rate %d, Interpreted as 64 kbps",
                                                    ipmdr->uplinkRate);
        ipmdr->uplinkRate = INTEG_PROT_MAX_DATA_RATE_64_KBPS;
    }

    nasLog("Downlink rate %d, uplink rate %d", ipmdr->downlinkRate,
            ipmdr->uplinkRate);
    nasLog("decoded %d bytes", len);
    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
int encodeIntegProtMaxDataRate(uint8_t *buffer, uint32_t bufLen,
                    integProtMaxDataRate_t* ipmdr, uint32_t *encodedLen)
{
    nasLogENTRY;

    int len = 0;
    if(buffer == NULL || ipmdr == NULL || bufLen == 0 || encodedLen == NULL) {
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(bufLen < INTEG_PROT_MAX_DATA_RATE_LEN) {
        higLog("Buffer insufficient");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[len] = ipmdr->downlinkRate;
    len++;
    buffer[len] = ipmdr->uplinkRate;
    len++;
    nasLog("Downlink rate %d, uplink rate %d", ipmdr->downlinkRate,
            ipmdr->uplinkRate);
    nasLog("encoded %d bytes", len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
