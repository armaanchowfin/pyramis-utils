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

#include "nas/NasDatatypes/SessionAMBR.h"

int decodeSessionAMBR(uint8_t *buffer, uint32_t bufLen,
                        SessionAMBR_t *ambr, uint32_t* decodedLen)
{
    nasLogENTRY;
    uint8_t len = 0;
    if(buffer == NULL || bufLen == 0 || ambr == NULL || decodedLen == NULL) {
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    ambr->len = buffer[0];
    if( bufLen < ambr->len) {
        higLog("Buffer incomplete to read %d bytes", ambr->len);
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }
    len++;

    ambr->downlinkUnit = buffer[1];
    len++;
    ambr->downlinkValue = (buffer[2] << 8) | buffer[3];
    len+=sizeof(ambr->downlinkValue);

    ambr->uplinkUnit = buffer[4];
    len++;
    ambr->uplinkValue = (buffer[5] << 8) | buffer[6];
    len+=sizeof(ambr->uplinkValue);

    *decodedLen = len;

    std::string downlinkStr = "Downlink PDU SessionAMBR is ";
    downlinkStr += std::to_string(ambr->downlinkValue);
    downlinkStr += session_AMBR_log_str[ambr->downlinkUnit];

    std::string uplinkStr = "Uplink PDU SessionAMBR is ";
    uplinkStr += std::to_string(ambr->uplinkValue);
    uplinkStr += session_AMBR_log_str[ambr->uplinkUnit];
    nasLog("SessionAMBR decoded, %s | %s | %d bytes decoded", downlinkStr.c_str(),
            uplinkStr.c_str(), len);

    nasLogEXIT;
    return SUCCESS;
}

int encodeSessionAMBR(uint8_t *buffer, uint32_t bufLen,
                       SessionAMBR_t * ambr, uint32_t *encodedLen)
{
    nasLogENTRY;
    uint8_t len = 0;
    if(buffer == NULL || bufLen == 0 || ambr == NULL || encodedLen == NULL) {
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if( bufLen < ambr->len) {
        higLog("Buffer insufficient to encode %d bytes", ambr->len);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[len++] = ambr->len;
    buffer[len++] = ambr->downlinkUnit;

    buffer[len++] = (ambr->downlinkValue & 0xFF00) >> 8;
    buffer[len++] = ambr->downlinkValue & 0xFF;

    buffer[len++] = ambr->uplinkUnit;

    buffer[len++] = (ambr->uplinkValue & 0xFF00) >> 8;
    buffer[len++] = ambr->uplinkValue & 0xFF;

    std::string downlinkStr = "Downlink PDU SessionAMBR is ";
    downlinkStr += std::to_string(ambr->downlinkValue);
    downlinkStr += session_AMBR_log_str[ambr->downlinkUnit];

    std::string uplinkStr = "Uplink PDU SessionAMBR is ";
    uplinkStr += std::to_string(ambr->uplinkValue);
    uplinkStr += session_AMBR_log_str[ambr->uplinkUnit];
    *encodedLen = len;
    nasLog("SessionAMBR encoded, %s | %s | %d bytes encoded", downlinkStr.c_str(),
            uplinkStr.c_str(), len);

    nasLogEXIT;
    return SUCCESS;
}
