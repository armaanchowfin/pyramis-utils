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

#include "pfcp/pfcpIEs/pfcpApplyAct.h"
#include "pfcp/include/pfcpUtil.h"

int pfcpApplyActionEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    applyAction_t *apac = (applyAction_t *)ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || encodedLen == NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(bufLen < PFCP_APPLY_ACTION_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    /* Fixed len IE, this is the only length check necessary */

    len += addIEI(buffer + len, apac->IEI);
    pfcpLog("IEI %d encoded, len %d", apac->IEI, len);
    len += addLEN(buffer + len, PFCP_APPLY_ACTION_LEN);
    pfcpLog("IE Len %d encoded, len %d", PFCP_APPLY_ACTION_LEN, len);

    uint8_t mask = 0;
    mask |= apac->DROP;
    mask |= apac->FORW << 1;
    mask |= apac->BUFF << 2;
    mask |= apac->NOCP << 3;
    mask |= apac->DUPL << 4;  

    buffer[len] = mask;
    len ++;
    pfcpLog("Action encoded DROP %d, FORW %d, BUFF %d, NOCP %d, DUPL %d,"
        "len %d",  apac->DROP, apac->FORW, apac->BUFF, apac->NOCP, 
                                                            apac->DUPL, len);

    pfcpLog("Apply Action Encoded, len %d", len);
    *encodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;
}
int pfcpApplyActionDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    applyAction_t *apac = (applyAction_t *)ptr;

    if(buffer == NULL || bufLen < 0 || ptr == NULL || decodedLen== NULL) {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }
    /*check was done twice, hence removed */

    if(bufLen < PFCP_APPLY_ACTION_LEN + PFCP_IE_HDR_LEN) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }

    /* Fixed len IE, this is the only length check necessary */

    len+= getIEI(buffer + len, &apac->IEI);
    uint16_t ieLen = 0;
    len+= getLEN(buffer + len, &ieLen);
    if( ieLen != PFCP_APPLY_ACTION_LEN) {
        higLog("Length Value incorrect");
        errno = E_PFCP_ERROR_INVALID_INPUT;
        pfcpLogEXIT;
        return FAILURE;
    }

    uint8_t mask = 0;
    mask = buffer[len];
    len++;
    apac->DROP = mask;
    apac->FORW = mask >> 1;
    apac->BUFF = mask >> 2;
    apac->NOCP = mask >> 3;
    apac->DUPL = mask >> 4;

    if(apac->DROP){
        if(apac->FORW || apac->BUFF){
            higLog("Only one among DROP FORW BUFF can be set");
            pfcpLogEXIT;
            return FAILURE;
        }
    }
    if(apac->FORW){
        if(apac->DROP || apac->BUFF){
            higLog("Only one among DROP FORW BUFF can be set");
            pfcpLogEXIT;
            return FAILURE;
        }
    }
    if(apac->BUFF){
        if(apac->DROP || apac->FORW){
            higLog("Only one among DROP FORW BUFF can be set");
            pfcpLogEXIT;
            return FAILURE;
        }
    }
    if((apac->NOCP) && (!apac->BUFF)){ 
        higLog("NOCP can be set only if BUFF is set");
        pfcpLogEXIT;
        return FAILURE;


    }
    if(apac->DUPL){
        if(!(apac->DROP || apac->FORW || apac->BUFF || apac->NOCP)){
            higLog("DUPL can be set only if any one among DROP FORW BUFF NOCP is set");
            pfcpLogEXIT;
            return FAILURE;

        }
    }

    pfcpLog("Action decoded DROP %d, FORW %d, BUFF %d, NOCP %d, DUPL %d,"
        "len %d",  apac->DROP, apac->FORW, apac->BUFF, apac->NOCP, 
                                                            apac->DUPL, len);

    pfcpLog("Apply Action Decoded, len %d", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;

}
