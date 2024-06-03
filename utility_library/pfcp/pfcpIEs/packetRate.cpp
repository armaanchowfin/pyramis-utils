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

#include "pfcp/pfcpIEs/packetRate.h"
#include "pfcp/include/pfcpUtil.h"

int packetRateEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen)
{
    pfcpLogENTRY;
    uint16_t len = 0;
    packetRate_t *packetRate = (packetRate_t *)ptr;

    if(buffer == NULL || bufLen < 0 || packetRate == NULL || encodedLen == NULL)
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

    len += addIEI(buffer + len, packetRate->IEI);
    uint16_t lenOffset = len;
    len += PFCP_IE_LEN_SIZE;
    pfcpLog("IEI %d", packetRate->IEI);
    pfcpLog("IEI and LEN encoded, len %d", len);

    if(len + PFCP_PACKET_RATE_MASK_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to encode");
        pfcpLogEXIT;
        return FAILURE;
    }

    uint8_t mask = 0;
    mask |= packetRate->ULPR;
    mask |= packetRate->DLPR << 1;

    buffer[len] = mask;
    len++;
    pfcpLog("Packet Rate encoded ULPR %d, DLPR %d,"
        "len %d",  packetRate->ULPR, packetRate->DLPR, len);

    if(packetRate->ULPR == 1 && packetRate->DLPR == 1){
        if(len + PFCP_PACKET_RATE_DLPR_LEN + PFCP_PACKET_RATE_ULPR_LEN > bufLen){
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        mask = 0;
        mask |= packetRate->uplinkTimeUnit;
        buffer[len] = mask;
        len++;
        pfcpLog("Packet Rate encoded Uplink Time Unit %d, "
        "len %d",  packetRate->uplinkTimeUnit, len);

        len += addU16(buffer + len, packetRate->maximumUplinkPacketRate);   //TODO: Check
        pfcpLog("Max Uplink Packet Rate encoded %d, "
        "len %d", packetRate->maximumUplinkPacketRate, len);

        mask = 0;
        mask |= packetRate->downlinkTimeUnit;
        buffer[len] = mask;
        len++;
        pfcpLog("Packet Rate encoded Downlink Time Unit %d, "
        "len %d",  packetRate->downlinkTimeUnit, len);

        len += addU16(buffer + len, packetRate->maximumDownlinkPacketRate);    //TODO: Check
        pfcpLog("Max Downlink Packet Rate encoded %d, "
        "len %d", packetRate->maximumDownlinkPacketRate, len);
    }
    else if(packetRate->ULPR == 1){
        if(len + PFCP_PACKET_RATE_ULPR_LEN > bufLen){
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        mask = 0;
        mask |= packetRate->uplinkTimeUnit;
        buffer[len] = mask;
        len++;
        pfcpLog("Packet Rate encoded Uplink Time Unit %d, "
        "len %d",  packetRate->uplinkTimeUnit, len);

        len += addU16(buffer + len, packetRate->maximumUplinkPacketRate);   //TODO: Check
        pfcpLog("Max Uplink Packet Rate encoded %d, "
        "len %d", packetRate->maximumUplinkPacketRate, len);
    }
    else{
        if(len + PFCP_PACKET_RATE_DLPR_LEN > bufLen){
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to encode");
            pfcpLogEXIT;
            return FAILURE;
        }
        mask = 0;
        mask |= packetRate->downlinkTimeUnit;
        buffer[len] = mask;
        len++;
        pfcpLog("Packet Rate encoded Downlink Time Unit %d, "
        "len %d",  packetRate->downlinkTimeUnit, len);

        len += addU16(buffer + len, packetRate->maximumDownlinkPacketRate);     //TODO: Check
        pfcpLog("Max Downlink Packet Rate encoded %d, "
        "len %d", packetRate->maximumDownlinkPacketRate, len);
    }

    addLEN(buffer + lenOffset, len - PFCP_IE_HDR_LEN);
    *encodedLen = len;
    pfcpLog("Encoded Packet Rate, len %d", len);
    pfcpLogEXIT;
    return SUCCESS;
}

int packetRateDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen)
{

    pfcpLogENTRY;
    uint16_t len = 0;
    packetRate_t *packetRate = (packetRate_t *)ptr;

    if(buffer == NULL || bufLen < 0 || packetRate == NULL || decodedLen == NULL)
    {
        errno = E_PFCP_ERROR_INVALID_INPUT;
        higLog("Invalid Input");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len +  PFCP_IE_HDR_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }
    len+= getIEI(buffer + len, &packetRate->IEI);
    uint16_t ieLen = 0;
    len+= getLEN(buffer + len, &ieLen);
    pfcpLog("IE %d decoded, len %d", packetRate->IEI, len);

    if(ieLen < PFCP_PACKET_RATE_MIN_LEN || ieLen > PFCP_PACKET_RATE_MAX_LEN) {
        errno = E_PFCP_ERROR_CONSTR_CHECK_FAIL;
        higLog("Length incorrect");
        pfcpLogEXIT;
        return FAILURE;
    }

    if(len + PFCP_PACKET_RATE_MASK_LEN > bufLen) {
        errno = E_PFCP_ERROR_BUFF_INSUFF;
        higLog("Buffer Insufficient to decode");
        pfcpLogEXIT;
        return FAILURE;
    }

    uint8_t mask = 0;
    mask = buffer[len];
    len++;
    packetRate->ULPR = mask;    //TODO: Check validity
    packetRate->DLPR = mask >> 1;   //TODO: Check validity
    pfcpLog("Packet Rate decoded ULPR %d, DLPR %d,"
        "len %d", packetRate->ULPR, packetRate->DLPR, len);

    if(packetRate->ULPR==1 && packetRate->DLPR==1){ //TODO: check condition works
        if(len + PFCP_PACKET_RATE_ULPR_LEN + PFCP_PACKET_RATE_DLPR_LEN > bufLen){
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        mask = 0;
        mask = buffer[len];
        len++;
        packetRate->uplinkTimeUnit = mask; //TODO: Check validity
        pfcpLog("Packet Rate decoded uplink Time Unit %d"
        "len %d", packetRate->uplinkTimeUnit, len);

        len += getU16(buffer + len, &packetRate->maximumUplinkPacketRate);
        pfcpLog("Packet Rate decoded uplink Packet Rate %d"
        "len %d", packetRate->maximumUplinkPacketRate, len);

        mask = 0;
        mask = buffer[len];
        len++;
        packetRate->downlinkTimeUnit = mask; //TODO: Check validity
        pfcpLog("Packet Rate decoded downlink Time Unit %d"
        "len %d", packetRate->downlinkTimeUnit, len);

        len += getU16(buffer + len, &packetRate->maximumDownlinkPacketRate);
        pfcpLog("Packet Rate decoded downlink Packet Rate %d"
        "len %d", packetRate->maximumDownlinkPacketRate, len);

    }else if(packetRate->ULPR==1){ //TODO: check condition works
        if(len + PFCP_PACKET_RATE_ULPR_LEN > bufLen){
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        mask = 0;
        mask = buffer[len];
        len++;
        packetRate->uplinkTimeUnit = mask; //TODO: Check validity
        pfcpLog("Packet Rate decoded uplink Time Unit %d"
        "len %d", packetRate->uplinkTimeUnit, len);

        len += getU16(buffer + len, &packetRate->maximumUplinkPacketRate);  //TODO: Check
        pfcpLog("Packet Rate decoded uplink Packet Rate %d"
        "len %d", packetRate->maximumUplinkPacketRate, len);

    }else{
        if(len + PFCP_PACKET_RATE_ULPR_LEN + PFCP_PACKET_RATE_DLPR_LEN > bufLen){
            errno = E_PFCP_ERROR_BUFF_INSUFF;
            higLog("Buffer Insufficient to decode");
            pfcpLogEXIT;
            return FAILURE;
        }
        mask = 0;
        mask = buffer[len];
        len++;
        packetRate->downlinkTimeUnit = mask; //TODO: Check validity
        pfcpLog("Packet Rate decoded downlink Time Unit %d"
        "len %d", packetRate->downlinkTimeUnit, len);

        len += getU16(buffer + len, &packetRate->maximumDownlinkPacketRate);    //TODO: Check
        pfcpLog("Packet Rate decoded downlink Packet Rate %d"
        "len %d", packetRate->maximumDownlinkPacketRate, len);
    }

    pfcpLog("Decoded Packet Rate, len %d", len);
    *decodedLen = len;
    pfcpLogEXIT;
    return SUCCESS;

}
