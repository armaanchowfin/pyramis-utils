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

#include "nas/NasDatatypes/PDUSessionId2.h"

/* TV type, no length param. */

int decodePDUSessionId2(uint8_t *buffer, uint32_t bufLen,
                        PDUSessionId2_t *pduSid, uint32_t* decodedLen)
{
    nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || pduSid == NULL || decodedLen == NULL) {
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if( bufLen < PDU_SESSION_ID_LEN) {
        higLog("Buffer insufficient to read %d bytes", PDU_SESSION_ID_LEN);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    pduSid->value = buffer[0];
    *decodedLen = PDU_SESSION_ID_LEN;
    nasLog("PDUSessionID2 decoded, %d | %d bytes read", pduSid->value,
            PDU_SESSION_ID_LEN);

    nasLogEXIT;
    return SUCCESS;
}
int encodePDUSessionId2(uint8_t *buffer, uint32_t bufLen,
                       PDUSessionId2_t * pduSid, uint32_t *encodedLen)
{
    nasLogENTRY;
    if(buffer == NULL || bufLen == 0 || pduSid == NULL || encodedLen == NULL) {
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if( bufLen < PDU_SESSION_ID_LEN) {
        higLog("Buffer insufficient to read %d bytes", PDU_SESSION_ID_LEN);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    buffer[0] = pduSid->value;
    *encodedLen = PDU_SESSION_ID_LEN;
    nasLog("PDUSessionID2 encoded, %d | %d bytes read", pduSid->value,
            PDU_SESSION_ID_LEN);

    nasLogEXIT;
    return SUCCESS;
}
