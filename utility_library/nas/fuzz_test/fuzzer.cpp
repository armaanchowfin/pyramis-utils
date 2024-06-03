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

#include <unistd.h>
#include <sys/syscall.h>

#include "nas/NasMessage.h"
#include "platform/include/logging.h"
#include "common/include/utils.h"

#define MAX_NAS_LENGTH 200

void randfill(char* ptr, int len)
{
    for(int i=0;i<len;i++)
        ptr[i]= rand()%UCHAR_MAX;
}

void* randmalloc(int len)
{
    void* ptr = malloc(len);
    randfill((char*)ptr, len);
    return ptr;
}


int typeArray[28] = {
    REGISTRATION_REQUEST,
    REGISTRATION_ACCEPT,
    REGISTRATION_COMPLETE,
    REGISTRATION_REJECT,
    DEREGISTRATION_REQUEST_UE_ORIGINATING,
    DEREGISTRATION_ACCEPT_UE_ORIGINATING,
    DEREGISTRATION_REQUEST_UE_TERMINATED,
    DEREGISTRATION_ACCEPT_UE_TERMINATED,
    SERVICE_REQUEST,
    SERVICE_REJECT,
    SERVICE_ACCEPT,
    CONFIGURATION_UPDATE_COMMAND,
    CONFIGURATION_UPDATE_COMPLETE,
    AUTHENTICATION_REQUEST,
    AUTHENTICATION_RESPONSE,
    AUTHENTICATION_REJECT,
    AUTHENTICATION_FAILURE,
    AUTHENTICATION_RESULT,
    IDENTITY_REQUEST,
    IDENTITY_RESPONSE,
    SECURITY_MODE_COMMAND,
    SECURITY_MODE_COMPLETE,
    SECURITY_MODE_REJECT,
    _5GMM_STATUS,
    NOTIFICATION,
    NOTIFICATION_RESPONSE,
    UL_NAS_TRANSPORT,
    DL_NAS_TRANSPORT
};

int main(int argc, char *argv[])
{
    srand(syscall(SYS_gettid));

    while(1) {
        //encode fuzz
        nasMessage_t nasMsg = {};
        uint8_t nasPdu[MAX_NAS_LENGTH] = {};
        uint32_t nasPduLen = 0;
        randfill((char*)&nasMsg, sizeof(nasMsg));
        nasMsg.header.epd  = _5GS_MOBILITY_MANAGEMENT_MESSAGE;
        nasMsg.header.secuHeader = SECURITY_HEADER_TYPE_NOT_PROTECTED;
        nasMsg.plain._5gmmMsg.mmheader.msgType = typeArray[rand()%28];
        printArray("Encode nasMsg: ", (uint8_t*)&nasMsg, sizeof(nasMsg));
        nasMessageEncode(nasPdu, MAX_NAS_LENGTH, &nasMsg, &nasPduLen, NULL);


        //decode fuzz
        int len = rand() % MAX_NAS_LENGTH + 3;
        uint8_t *msg = (uint8_t*)randmalloc(len);
        msg[0] = _5GS_MOBILITY_MANAGEMENT_MESSAGE;
        msg[1] = SECURITY_HEADER_TYPE_NOT_PROTECTED;
        msg[2] = typeArray[rand()%28];
        printArray("Decode msg: ", msg, len);
        nasMessage_t nasMsg2 = {};

        nasMessageDecode(msg, &nasMsg2, len, NULL);
        free(msg);
    }

    return 0;
}
