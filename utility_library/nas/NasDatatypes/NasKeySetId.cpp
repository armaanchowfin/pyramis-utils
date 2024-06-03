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

#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/NasKeySetId.h"

int decodeNasKeySetId(uint8_t *buffer, NaskeysetId_t *ngksi, uint32_t bufLen,
                      uint32_t* consumedLen, bool msbAligned /* false */)
{
    nasLogENTRY;
    if(bufLen < sizeof(NaskeysetId_t) || buffer == NULL || ngksi == NULL ||
       consumedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(!msbAligned) {
        ngksi->tsc = (buffer[0] & 0x08) >> 3;
        ngksi->ngksi = (buffer[0] & 0x07);
    } else {
        ngksi->tsc = (buffer[0] & 0x80) >> 7;
        ngksi->ngksi = (buffer[0] & 0x70) >> 4;
    }
    *consumedLen = sizeof(NaskeysetId_t);
    nasLogEXIT;
    return SUCCESS;
}

int encodeNasKeySetId(uint8_t *buffer, uint32_t bufLen, NaskeysetId_t *ngksi,
                      uint32_t *encodedLen, bool msbAligned /* false */)
{
    nasLogENTRY;
    if(bufLen < sizeof(NaskeysetId_t) || buffer == NULL || ngksi == NULL ||
       encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(!msbAligned) {
        buffer[0] |= (ngksi->tsc << 3) | (ngksi->ngksi);
    } else {
        buffer[0] |= ((ngksi->tsc << 3) | (ngksi->ngksi)) << 4;
    }
    *encodedLen = sizeof(NaskeysetId_t);
    nasLogEXIT;
    return SUCCESS;
}
