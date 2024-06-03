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

#include "pfcp/pfcpIEs/pfcpDLDataNotificationDelay.h"
#include "pfcp/include/pfcpUtil.h"

int pfcpDLDataNotificationDelayEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    pfcpDLDataNotificationDelay_t *delay = (pfcpDLDataNotificationDelay_t *)ptr;

    if(buffer == NULL || bufLen < 0 || delay == NULL || encodedLen == NULL) 
    {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_DL_DATA_NOTIFICATION_DELAY_LEN + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */
    if ((delay->value != 0) ||
            ((delay->value % PFCP_DL_DATA_NOTIFICATION_DELAY_UNIT) != 0)) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input:Delay Value should be in integer multiples of"
                " 50 millisecs, or zero");
        pfcpLogEXIT;
        return FAILURE;
    }
    
    len += addIEI(buffer + len, delay->IEI);
    len += addLEN(buffer + len, PFCP_DL_DATA_NOTIFICATION_DELAY_LEN);
    pfcpLog("IEI %d", delay->IEI);
    pfcpLog("IEI and LEN encoded, len %d", len);

    buffer[len] = delay->value;
    len++;

    pfcpLog("DL Data Notification Delay value %llu encoded, len %d",
                                                           delay->value, len);

    pfcpLog("Encoded %d bytes", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;

}

int pfcpDLDataNotificationDelayDecode(uint8_t *buffer, uint16_t bufLen,
                                            void* ptr, uint16_t *decodedLen) {

    pfcpLogENTRY;
    uint16_t len = 0;
    pfcpDLDataNotificationDelay_t *delay = (pfcpDLDataNotificationDelay_t *)ptr;

    if(buffer == NULL || bufLen < 0 || delay == NULL || decodedLen == NULL) 
    {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_DL_DATA_NOTIFICATION_DELAY_LEN + PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }
    /* Fixed len IE, this is the only length check necessary */

    len+= getIEI(buffer + len, &delay->IEI);
    pfcpLog("IEI %d decoded, len %d", delay->IEI, len);

    uint16_t msgLen = 0;
    len+= getLEN(buffer + len, &msgLen);
    pfcpLog("IE Len %d decoded, len %d", delay->IEI, len);

    if( msgLen != PFCP_DL_DATA_NOTIFICATION_DELAY_LEN) {
        higLog("Length Value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }
    
    delay->value = buffer[len];
    len++;

    if ((delay->value != 0) ||
            ((delay->value % PFCP_DL_DATA_NOTIFICATION_DELAY_UNIT) != 0)) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input:Delay Value should be in integer multiples of"
                " 50 millisecs, or zero");
        pfcpLogEXIT;
        return FAILURE;
    }
    
    pfcpLog("DL Data Notification Delay Value %llu decoded, len %d",
                                                           delay->value, len);

    pfcpLog("Decode Complete %d bytes", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;

}

