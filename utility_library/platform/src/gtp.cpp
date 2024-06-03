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

#include "platform/include/gtp.h"

int encodeGtpHeader(uint8_t *buffer, gtpHeader_t *msgStruct, uint32_t bufLen,
                    uint32_t *encodedLen)
{
    LOG_ENTRY;
    unsigned int len = 0;

    if(!buffer || !msgStruct || !encodedLen)
    {
        higLog("Invalid input parameters");
        LOG_EXIT;
        return FAILURE;
    }

    if(len+ sizeof(msgStruct->flags) > bufLen)
    {
        higLog("Incomplete buffer length for flags");
        LOG_EXIT;
        return FAILURE;
    }
    memcpy(buffer+len, &msgStruct->flags, sizeof(msgStruct->flags));
    len+=sizeof(msgStruct->flags);

    if(len+ sizeof(msgStruct->msgType) > bufLen)
    {
        higLog("Incomplete buffer length for msgType");
        LOG_EXIT;
        return FAILURE;
    }
    memcpy(buffer+len, &msgStruct->msgType, sizeof(msgStruct->msgType));
    len+=sizeof(msgStruct->msgType);

    if(len+ sizeof(msgStruct->length) > bufLen)
    {
        higLog("Insufficient buffer length for length");
        LOG_EXIT;
        return FAILURE;
    }

    unsigned int lenOffset = len;
    len+=sizeof(msgStruct->length);

    if(len+ sizeof(msgStruct->teid) > bufLen)
    {
        higLog("Incomplete buffer length for teid");
        LOG_EXIT;
        return FAILURE;
    }
    msgStruct->teid = htonl(msgStruct->teid);
    memcpy(buffer+len, &msgStruct->teid, sizeof(msgStruct->teid));
    len+=sizeof(msgStruct->teid);

    /*NOTE 4: of page 17 of 29060
    *Following bytes will be present in the encoding if
    *(GTP_S_MASK|GTP_PN_MASK|GTP_E_MASK) is true
    * As per spec, even if only 1 optional field is present, we have to add
    * all optioanl fields (remaining optional fields will be set to 0), and
    * header length will increase by 4 Bytes.
    */

    if(msgStruct->flags & (GTP_S_MASK|GTP_PN_MASK|GTP_E_MASK))
    {
        if (msgStruct->flags & GTP_S_MASK) // seq num present
        {
            if(len+ sizeof(msgStruct->seqNo) > bufLen)
            {
                higLog("Incomplete buffer length for seq num");
                LOG_EXIT;
                return FAILURE;
            }
            msgStruct->seqNo = htons(msgStruct->seqNo);
        }

        memcpy(buffer+len, &msgStruct->seqNo, sizeof(msgStruct->seqNo));
        len+=sizeof(msgStruct->seqNo);

        if (msgStruct->flags & GTP_PN_MASK) // npdu num present
        {
            if(len+ sizeof(msgStruct->npduNum) > bufLen)
            {
                higLog("Incomplete buffer length for npdu num");
                LOG_EXIT;
                return FAILURE;
            }
        }

        memcpy(buffer+len, &msgStruct->npduNum, sizeof(msgStruct->npduNum));
        len+=sizeof(msgStruct->npduNum);

        if (msgStruct->flags & GTP_E_MASK) // next extn hdr present
        {
            extHdrContent_t extHdrContnt = {};
            for(uint8_t i = 0; i < MAX_NUM_OF_EXT_HDR_TYPES; i++){

                if(!(msgStruct->extHdrs[i].len)) {
                    lowLog("%s not present", extHdrName[extHdrList[i]].c_str());
                    continue;
                }

                buffer[len] = extHdrList[i];
                len++;
#if 1
                uint8_t _len = msgStruct->extHdrs[i].len;
                if((_len % 4) != EXT_HDR_OFFSET) {
                    higLog("ERROR: Likely Wrong Padding: _len %u", _len);
                    LOG_EXIT;
                    return FAILURE;
                }
#endif
                buffer[len] = ((msgStruct->extHdrs[i].len) + EXT_HDR_OFFSET)/4;
                len++;
                memcpy(buffer+len, msgStruct->extHdrs[i].content, msgStruct->extHdrs[i].len);
                len += msgStruct->extHdrs[i].len;

                printArray(extHdrName[extHdrList[i]].c_str(),
                        msgStruct->extHdrs[i].content, msgStruct->extHdrs[i].len);
            }
        }
        buffer[len] = E_GTP_EXT_HDR_NO_MORE_EXTENSION_HEADERS;
        len++;
    }
    *encodedLen = len;

    /* Spec ETSI TS 129 281 V15.6.0 (2019-10)

    Length field in GTP header indicates the length in octets of the payload,
    i.e. the rest of the packet following the mandatory part of the GTP header
    (that is the first 8 octets). The Sequence Number,
    the N-PDU Number or any Extension headers shall be considered to be part
    of the payload, i.e. included in the length count.

    */

    int32_t length = ((*encodedLen)- GTP_HDR_MANDATORY_FIELD_LENGTH);
    if (length < 0) {
        higLog("Encode error: encodedLen should be at least %d",
            GTP_HDR_MANDATORY_FIELD_LENGTH);
        LOG_EXIT;
        return FAILURE;
    }

    if (length > USHRT_MAX ) {
        higLog("Encode error: encodedLen out of bounds");
        LOG_EXIT;
        return FAILURE;
    }

    msgStruct->length += (uint16_t) (length);
    msgStruct->length = htons(msgStruct->length);
    memcpy(buffer+lenOffset, &msgStruct->length, sizeof(msgStruct->length));

    lowLog("encodeGtpHeader : encoded length = %d, SUCCESS", len);
    printArray("encoded GTP Header", buffer, *encodedLen);

    LOG_EXIT;
    return SUCCESS;
}

int decodeGtpHeader(uint8_t *buffer, gtpHeader_t *msgStruct, uint32_t bufLen,
    uint32_t *decodedLen)
{
    LOG_ENTRY;
    unsigned int len = 0;

    if(!buffer || !msgStruct)
    {
        higLog("Invalid input parameters");
        LOG_EXIT;
        return FAILURE;
    }

    printArray("GTP header received", buffer, bufLen);

    if(len+sizeof(msgStruct->flags) > bufLen)
    {
        higLog("Incomplete buffer length for flags");
        LOG_EXIT;
        return FAILURE;
    }
    memcpy(&msgStruct->flags, buffer+len, sizeof(msgStruct->flags));
    len += sizeof(msgStruct->flags);

    if(len+sizeof(msgStruct->msgType) > bufLen)
    {
        higLog("Incomplete buffer length for msgType");
        LOG_EXIT;
        return FAILURE;
    }
    memcpy(&msgStruct->msgType, buffer+len, sizeof(msgStruct->msgType));
    len += sizeof(msgStruct->msgType);

    if(len+sizeof(msgStruct->length) > bufLen)
    {
        higLog("Incomplete buffer length for length");
        LOG_EXIT;
        return FAILURE;
    }
    memcpy(&msgStruct->length, buffer+len, sizeof(msgStruct->length));
    msgStruct->length = ntohs(msgStruct->length);
    len += sizeof(msgStruct->length);

    if(len+sizeof(msgStruct->teid) > bufLen)
    {
        higLog("Incomplete buffer length for teid");
        LOG_EXIT;
        return FAILURE;
    }
    memcpy(&msgStruct->teid, buffer+len, sizeof(msgStruct->teid));
    msgStruct->teid = ntohl(msgStruct->teid);
    len += sizeof(msgStruct->teid);

    if(msgStruct->flags & (GTP_S_MASK|GTP_PN_MASK|GTP_E_MASK))
    {
        if(msgStruct->flags & GTP_S_MASK)
        {
            if(len+sizeof(msgStruct->seqNo) > bufLen)
            {
                higLog("Incomplete buffer length for seqNo");
                LOG_EXIT;
                return FAILURE;
            }
        }
        memcpy(&msgStruct->seqNo, buffer+len, sizeof(msgStruct->seqNo));
        msgStruct->seqNo = ntohs(msgStruct->seqNo);
        len += sizeof(msgStruct->seqNo);

        if(msgStruct->flags & GTP_PN_MASK)
        {
            if(len+sizeof(msgStruct->npduNum) > bufLen)
            {
                higLog("Incomplete buffer length for npduNum");
                LOG_EXIT;
                return FAILURE;
            }
        }
        memcpy(&msgStruct->npduNum, buffer+len, sizeof(msgStruct->npduNum));
        len += sizeof(msgStruct->npduNum);

        if(msgStruct->flags & GTP_E_MASK)
        {
            if(len+sizeof(msgStruct->nxtExtnHdrType) > bufLen)
            {
                higLog("Incomplete buffer length for nxtExtnHdrType");
                LOG_EXIT;
                return FAILURE;
            }
            uint8_t nxtExtnHdrType = buffer[len];
            len++;
            while(nxtExtnHdrType !=
                                E_GTP_EXT_HDR_NO_MORE_EXTENSION_HEADERS){

                extHdrContent_t extHdrContent = {};
                /*  Spec - draft-hmm-dmm-5g-uplane-analysis-02

                If n is given as Ext-Hdr Length in buffer then total
                buffer content (Ext-Hdr Length + Extension Header Content +
                Ext-Hdr Length)will be 4n bytes. So two bytes should be
                subtracted from 4n bytes to get only the Extension Header
                Content length.

                */
                uint8_t extHdrLen = 0;
                if(len+sizeof(extHdrLen) > bufLen)
                {
                    higLog("Incomplete buffer length for extHdrLen");
                    LOG_EXIT;
                    return FAILURE;
                }
                extHdrContent.len = (buffer[len]*4)-EXT_HDR_OFFSET;
                len++;

                if(len+extHdrContent.len > bufLen)
                {
                    higLog("Incomplete buffer length for extHdrContent");
                    LOG_EXIT;
                    return FAILURE;
                }
                memcpy(extHdrContent.content, buffer+len, extHdrContent.len);

                switch(nxtExtnHdrType){

                    case E_GTP_EXT_HDR_PDU_SESSION_CONTAINER:
                    {
                        msgStruct->extHdrs[0].len = extHdrContent.len;
                        memcpy(msgStruct->extHdrs[0].content,
                             extHdrContent.content, extHdrContent.len);
                        len += extHdrContent.len;
                        lowLog("PDU Sess Container added to map %d",
                             msgStruct->extHdrs[0].len);
                    }
                    break;

                    default:
                    {
                        higLog("Unknown/Unsupported Extension Header Type");
                        LOG_EXIT;
                        return FAILURE;
                    }
                }
                if(len+sizeof(nxtExtnHdrType) > bufLen)
                {
                    higLog("Incomplete buffer length for nxtExtnHdrType");
                    LOG_EXIT;
                    return FAILURE;
                }
                nxtExtnHdrType = buffer[len];
                len++;
            }
        }
    }
    *decodedLen = len;

    msgStruct->gtpHdrLen = GTP_HDR_MANDATORY_FIELD_LENGTH;
    if(msgStruct->flags & (GTP_S_MASK|GTP_PN_MASK|GTP_E_MASK)){
        msgStruct->gtpHdrLen += GTP_HDR_OPTIONAL_FIELD_LENGTH;
    }
    /* Decode each extension header */
    for(uint8_t i = 0; i < 1; i++){

        if(nxtExtHdrOperations[extHdrList[i]].decode(msgStruct) == FAILURE){
            higLog("Decode of extension header failed");
            LOG_EXIT;
            return FAILURE;
        }
        if(msgStruct->extHdrs[i].len) {
            msgStruct->gtpHdrLen += (msgStruct->extHdrs[i].len + 2);
        }
    }

    LOG_EXIT;
    return SUCCESS;
}

int encodeGtpMessage(uint8_t *buffer, uint32_t bufLen, gtpMessage_t *msgStruct,         uint32_t *encodedLen)
{
    LOG_ENTRY;
    unsigned int len = 0;
    uint32_t encLen = 0;

    if(!buffer || !msgStruct || !encodedLen)
    {
        higLog("Invalid input parameters");
        LOG_EXIT;
        return FAILURE;
    }

    if(len+ msgStruct->gtp_header.gtpHdrLen > bufLen)
    {
        higLog("Incomplete buffer length for GTP header");
        LOG_EXIT;
        return FAILURE;
    }

    if(encodeGtpHeader(buffer+len,&msgStruct->gtp_header,bufLen-len, &encLen)
                                                            == FAILURE){
        higLog("Header encoding failed");
        LOG_EXIT;
        return FAILURE;
    }
    len+=encLen;
    lowLog("len = %d, encLen = %d", len, encLen);
    if(len+msgStruct->payloadLength > bufLen)
    {
        higLog("Incomplete buffer length for GTP payload");
        LOG_EXIT;
        return FAILURE;
    }
    memcpy(buffer+len, msgStruct->payload, msgStruct->payloadLength);
    len+=msgStruct->payloadLength;

    *encodedLen = len;
    lowLog("encodeGtpMessage : encoded length = %d, SUCCESS", len);
    printArray("encoded GTP Message", buffer, *encodedLen);
    LOG_EXIT;
    return SUCCESS;
}


/*  1.buffer = input buffer
*   2. msgStruct = empty gtp message
*   3. bufLen = length to decode
*/
int decodeGtpMessage(uint8_t *buffer, gtpMessage_t *msgStruct, uint32_t bufLen)
{
    LOG_ENTRY;
    unsigned int len = 0;
    uint32_t consumedLen = 0;

    if(!buffer || !msgStruct)
    {
        higLog("Invalid input parameters");
        LOG_EXIT;
        return FAILURE;
    }

    printArray("GTP message received", buffer, bufLen);

#if 0 /* TODO: This check is not required */
    if(gtpHeaderLen(buffer + len, bufLen - len) == FAILURE) {
        higLog("Incomplete buffer length for GTP header");
        LOG_EXIT;
        return FAILURE;
    }
#endif
    if(decodeGtpHeader(buffer + len,&(msgStruct->gtp_header),bufLen - len,
            &consumedLen) == FAILURE)
    {
        higLog("decodeGtpHeader failed");
        LOG_EXIT;
        return FAILURE;
    }
    len+=consumedLen;

    if(msgStruct->gtp_header.gtpHdrLen > bufLen){
        higLog("Incomplete buffer");
        LOG_EXIT;
        return FAILURE;
    }

    uint32_t gtpPayloadLength = bufLen - consumedLen;
    if(len + gtpPayloadLength > bufLen)
    {
        higLog("Incomplete buffer length for payload");
        LOG_EXIT;
        return FAILURE;
    }
    memcpy(&msgStruct->payload, buffer+len, gtpPayloadLength);
    msgStruct->payloadLength = gtpPayloadLength;
    lowLog("GTP payload length = %d ", gtpPayloadLength);

    LOG_EXIT;
    return SUCCESS;
}

int pduSessionContainerDecode(gtpHeader_t *msgStruct)
{
    LOG_ENTRY;

    uint8_t buffer[MAX_MESSAGE_SIZE] = {};
    uint32_t bufLen = 0, extHdrLen = 0;
    gtpPduSess_t pduContainer = {};

    if(!(msgStruct->extHdrs[0].len)) {
        higLog("Extension Header type PDU Session Container not found");
        LOG_EXIT;
        return FAILURE;
    }

    printArray("extHdrPduSess: ",
             msgStruct->extHdrs[0].content, msgStruct->extHdrs[0].len);
    memcpy(buffer+bufLen,
         msgStruct->extHdrs[0].content, msgStruct->extHdrs[0].len);

    uint8_t pduType = buffer[bufLen]>>4;
    bufLen++;

    switch(pduType){

        case UL_PDU_SESSION_INFO:
        {
            pduContainer.ulContainer.pduType = pduType;
            pduContainer.ulContainer.qfi = buffer[bufLen] & 0x3F;
            bufLen++;
        }
        break;

        case DL_PDU_SESSION_INFO:
        {
            pduContainer.ulContainer.pduType = pduType;
            pduContainer.ulContainer.qfi = buffer[bufLen] & 0x3F;
            bufLen++;
        }
        break;

        default:
        {
            higLog("Unknown PDU Type");
            LOG_EXIT;
            return FAILURE;
        }
    }

    msgStruct->pduSessContainer = pduContainer;

    LOG_EXIT;
    return SUCCESS;
}

int pduSessionContainerEncode(gtpHeader_t *msgStruct)
{
    LOG_ENTRY;

    uint8_t buffer[MAX_MESSAGE_SIZE] = {};
    uint32_t bufLen = 0;
    uint8_t numPadBytes = 0;
    extHdrContent_t extHdrPduSess = {};

    gtpPduSess_t pduContainer = msgStruct->pduSessContainer;

    uint8_t pduType = pduContainer.ulContainer.pduType;

    switch(pduType){

        case UL_PDU_SESSION_INFO:
        {
            buffer[bufLen] = pduType << 4;
            bufLen++;
            buffer[bufLen] = pduContainer.ulContainer.qfi;
            bufLen++;
        }
        break;

        case DL_PDU_SESSION_INFO:
        {
            buffer[bufLen] = pduType << 4;
            bufLen++;
            buffer[bufLen] = pduContainer.ulContainer.qfi;
            bufLen++;
        }
        break;

        default:
        {
            higLog("Unknown PDU Type");
            LOG_EXIT;
            return FAILURE;
        }

    }

    /* Calculate padding */
    numPadBytes = (3*bufLen + 2)%4;
    if(numPadBytes > 0){
        memset(buffer+bufLen, 0, numPadBytes);
        bufLen += numPadBytes;
    }

    extHdrPduSess.len = bufLen;
    memcpy(extHdrPduSess.content, buffer, extHdrPduSess.len);

    if(((extHdrPduSess.len + EXT_HDR_OFFSET)%4)!=0){
        higLog("Extension header length != 4n-2");
        LOG_EXIT;
        return FAILURE;
    }
    msgStruct->extHdrs[0].len = extHdrPduSess.len;
    memcpy(msgStruct->extHdrs[0].content,
        extHdrPduSess.content, extHdrPduSess.len);
    printArray("extHdrPduSess: ", extHdrPduSess.content, extHdrPduSess.len);

    LOG_EXIT;
    return SUCCESS;
}
