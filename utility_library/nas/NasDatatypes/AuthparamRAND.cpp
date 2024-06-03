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
#include "common/include/datatypes.h"
#include "nas/NasDatatypes/AuthparamRAND.h"

int decodeAuthParamRAND(uint8_t *buffer, RAND_t *rand, uint32_t bufLen,
                        uint32_t* consumedLen)
{
    nasLogENTRY;
    if(bufLen < RAND_SIZE || buffer == NULL || rand == NULL ||
       consumedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(rand->RAND, buffer, RAND_SIZE);
    *consumedLen = RAND_SIZE;

    nasLogEXIT;
    return SUCCESS;
}

int encodeAuthParamRAND(uint8_t *buffer, uint32_t buflen, RAND_t *rand,
                        uint32_t *encodedLen)
{
    nasLogENTRY;
    if(buflen < RAND_SIZE || buffer == NULL || rand == NULL ||
        encodedLen  == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(buffer, rand->RAND, RAND_SIZE);
    *encodedLen = RAND_SIZE;

    nasLogEXIT;
    return SUCCESS;
}
