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

#include "nas/NasDatatypes/DNN.h"

int decodeDNN(uint8_t *buffer,  uint32_t bufLen, DNN_t *dnn,
              uint32_t *decodedLen)
{
    nasLogENTRY;
    int len = 0;
    if(buffer == NULL || dnn == NULL || bufLen < DNN_IE_MIN_LEN ||
            decodedLen == NULL) {
        higLog("Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    uint8_t ieLen = buffer[len];
    len++;
    nasLog("DNN IE Len read %d", ieLen);

    if(ieLen > DNN_IE_MAX_LEN || ieLen < DNN_IE_MIN_LEN) {
        higLog("Invalid Length %d", ieLen);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    dnn->len = ieLen;
    nasLog("dnn len %d | currentLen %d ", dnn->len, len);

    if(len + ieLen > bufLen) {
        higLog("Buffer insufficient to read %d bytes", ieLen);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(dnn->value, buffer + len, ieLen);
    len += ieLen;
    nasLog("DNN decoded, %d bytes read", len);

    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}

int encodeDNN(uint8_t *buffer, uint32_t bufLen, DNN_t *dnn,
              uint32_t *encodedLen)
{
    nasLogENTRY;
    int len = 0;
    if(buffer == NULL || dnn == NULL || bufLen < DNN_IE_MIN_LEN ||
            encodedLen == NULL) {
        higLog("Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    uint8_t ieLen = dnn->len;

    if(ieLen > DNN_IE_MAX_LEN || ieLen < DNN_IE_MIN_LEN) {
        higLog("Invalid Length %d", ieLen);
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }


    buffer[len] = ieLen;
    nasLog("Encoded DNN IE Len %d", buffer[len]);
    len++;

    if(len + dnn->len > bufLen) {
        higLog("Buffer insufficient to write %d bytes", ieLen);
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    memcpy(buffer + len, dnn->value, dnn->len);
    len += dnn->len;
    nasLog("DNN encoded | currentLen %d | %d bytes dnn", len, dnn->len);

    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
