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

#include "nas/NasDatatypes/TAI.h"

int decodeTAI(uint8_t *buffer, uint32_t bufLen,
                       tAI_t *tAI, uint32_t* decodedLen) {
    nasLogENTRY;

    uint32_t len = 0;

    if(buffer == NULL || bufLen == 0 ||
        tAI == NULL || decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + TAI_SIZE > bufLen) {
        higLog("Buffer incomplete");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    nasLogEXIT;
    return SUCCESS;

}


int encodeTAI(uint8_t *buffer, uint32_t bufLen,
                       tAI_t *tAI, uint32_t *encodedLen) {

    nasLogENTRY;

    uint32_t len = 0;

    if(buffer == NULL || bufLen == 0 || tAI == NULL || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if (len + TAI_SIZE > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    nasLogEXIT;
    return SUCCESS;

}
