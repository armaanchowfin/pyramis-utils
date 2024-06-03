#include "nas/NasDatatypes/PacketFilterContents.h"

int decodePacketFilterContents(uint8_t *buffer, uint32_t bufLen,
	PacketFilterContents_t *packetFilterContents, uint32_t* decodedLen,
	uint8_t lenghtOfPFC)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || packetFilterContents == NULL || decodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	while(len<lenghtOfPFC){
		uint8_t componentId = buffer[len];
		len++;
		switch(componentId){
			case QOS_RULES_MATCH_ALL_TYPE:{
				nasLog("MATCH ALL TYPE component, ignoring any other components if any");
				packetFilterContents->pfcMask |= PFC_MASK_MATCH_ALL_TYPE;
				// *decodedLen = lenghtOfPFC;
				// nasLog("Packet_filter_list_create decoded | decoded %d bytes", *decodedLen);
				// nasLogEXIT;
				// return SUCCESS;
			}
			break;

			case QOS_RULES_IPV4_REMOTE_ADDRESS_TYPE:{
				if((lenghtOfPFC - len)<sizeof(Ipv4AddressType_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->
					remoteIpV4AddressType),buffer + len, sizeof(Ipv4AddressType_t));
				len += sizeof(Ipv4AddressType_t);
				packetFilterContents->pfcMask |= PFC_MASK_IPV4_REMOTE_ADDRESS_TYPE;
				nasLog("Remote IP Address: %s",inet_ntoa(*(struct in_addr *)
					&(packetFilterContents->
					remoteIpV4AddressType.ipv4Address)));
				nasLog("Remote IP Address Mask: %s",inet_ntoa(*(struct in_addr *)
					&(packetFilterContents->
					remoteIpV4AddressType.ipv4Mask)));
			}
			break;

			case QOS_RULES_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE:{
				if((lenghtOfPFC - len)<sizeof(Ipv6AddressType_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->
					remoteIpV6AddressType),buffer + len, sizeof(Ipv6AddressType_t));
				len += sizeof(Ipv6AddressType_t);
				packetFilterContents->pfcMask |= PFC_MASK_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE;
			}
			break;

			case QOS_RULES_IPV4_LOCAL_ADDRESS_TYPE:{
				if((lenghtOfPFC - len)<sizeof(Ipv4AddressType_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->
					localIpV4AddressType),buffer + len, sizeof(Ipv4AddressType_t));
				len += sizeof(Ipv4AddressType_t);
				packetFilterContents->pfcMask |= PFC_MASK_IPV4_LOCAL_ADDRESS_TYPE;
				nasLog("Local IP Address: %s",inet_ntoa(*(struct in_addr *)
					&(packetFilterContents->
					localIpV4AddressType.ipv4Address)));
				nasLog("Local IP Address Mask: %s",inet_ntoa(*(struct in_addr *)
					&(packetFilterContents->
					localIpV4AddressType.ipv4Mask)));
			}
			break;

			case QOS_RULES_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE:{
				if((lenghtOfPFC - len)<sizeof(Ipv6AddressType_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->
					localIpV6AddressType),buffer + len, sizeof(Ipv6AddressType_t));
				len += sizeof(Ipv6AddressType_t);
				packetFilterContents->pfcMask |= PFC_MASK_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE;
			}
			break;

			case QOS_RULES_PROTOCOL_IDENTIFIER_OR_NEXT_HEADER_TYPE:{
				if((lenghtOfPFC - len)<sizeof(uint8_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->protocolIdentifier),buffer + len, sizeof(uint8_t));
				len += sizeof(uint8_t);
				packetFilterContents->pfcMask |= PFC_MASK_PROTOCOL_IDENTIFIER_OR_NEXT_HEADER_TYPE;
			}
			break;

			case QOS_RULES_SINGLE_LOCAL_PORT_TYPE:{
				if((lenghtOfPFC - len)<sizeof(uint16_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->
					singleLocalPort),buffer + len, sizeof(uint16_t));
				len += sizeof(uint16_t);
				nasLog("Local Port: %d",packetFilterContents->
					singleLocalPort);
				packetFilterContents->pfcMask |= PFC_MASK_SINGLE_LOCAL_PORT_TYPE;
			}
			break;

			case QOS_RULES_LOCAL_PORT_RANGE_TYPE:{
				if((lenghtOfPFC - len)<sizeof(PortRange_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
                packetFilterContents->localPortRange.min = *(buffer + len); 
                packetFilterContents->localPortRange.min = (((packetFilterContents->localPortRange.min) << 8) | *(buffer + len + 1)); 
                packetFilterContents->localPortRange.max = *(buffer + len + 2); 
                packetFilterContents->localPortRange.max = (((packetFilterContents->localPortRange.max) << 8) | *(buffer + len + 3)); 
				len += sizeof(PortRange_t);
				packetFilterContents->pfcMask |= PFC_MASK_LOCAL_PORT_RANGE_TYPE;
			}
			break;

			case QOS_RULES_SINGLE_REMOTE_PORT_TYPE:{
				if((lenghtOfPFC - len)<sizeof(uint16_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->
					singleRemotePort),buffer + len, sizeof(uint16_t));
				len += sizeof(uint16_t);
				nasLog("Remote Port: %d",packetFilterContents->
					singleRemotePort);
				packetFilterContents->pfcMask |= PFC_MASK_SINGLE_REMOTE_PORT_TYPE;
			}
			break;

			case QOS_RULES_REMOTE_PORT_RANGE_TYPE:{
				if((lenghtOfPFC - len)<sizeof(PortRange_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
                packetFilterContents->remotePortRange.min = *(buffer + len); 
                packetFilterContents->remotePortRange.min = (((packetFilterContents->remotePortRange.min) << 8) | *(buffer + len + 1)); 
                packetFilterContents->remotePortRange.max = *(buffer + len + 2); 
                packetFilterContents->remotePortRange.max = (((packetFilterContents->remotePortRange.max) << 8) | *(buffer + len + 3)); 
				len += sizeof(PortRange_t);
				packetFilterContents->pfcMask |= PFC_MASK_REMOTE_PORT_RANGE_TYPE;
			}
			break;

			case QOS_RULES_SECURITY_PARAMETER_INDEX_TYPE:{
				if((lenghtOfPFC - len)<sizeof(uint32_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->securityParameterIndex),
					buffer + len, sizeof(uint32_t));
				len += sizeof(uint32_t);
				packetFilterContents->pfcMask |= PFC_MASK_SECURITY_PARAMETER_INDEX_TYPE;
			}
			break;

			case QOS_RULES_TYPE_OF_SERVICE_OR_TRAFFIC_CLASS_TYPE:{
				if((lenghtOfPFC - len)<sizeof(TOS_TC_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->typeOfService_TrafficClass),
					buffer + len, sizeof(TOS_TC_t));
				len += sizeof(TOS_TC_t);
				packetFilterContents->pfcMask |= PFC_MASK_TYPE_OF_SERVICE_OR_TRAFFIC_CLASS_TYPE;
			}
			break;

			case QOS_RULES_FLOW_LABEL_TYPE:{
				if((lenghtOfPFC - len)<sizeof(FlowLabel_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->flowLabelType),
					buffer + len, sizeof(FlowLabel_t));
				len += sizeof(FlowLabel_t);
				packetFilterContents->pfcMask |= PFC_MASK_FLOW_LABEL_TYPE;
			}
			break;

			case QOS_RULES_DESTINATION_MAC_ADDRESS_TYPE:{
				if((lenghtOfPFC - len)<(sizeof(uint8_t)*MAC_ADDRESS_LENGTH)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->destinationMac),
					buffer + len, (sizeof(uint8_t)*MAC_ADDRESS_LENGTH));
				len += (sizeof(uint8_t)*MAC_ADDRESS_LENGTH);
				packetFilterContents->pfcMask |= PFC_MASK_DESTINATION_MAC_ADDRESS_TYPE;
			}
			break;

			case QOS_RULES_SOURCE_MAC_ADDRESS_TYPE:{
				if((lenghtOfPFC - len)<(sizeof(uint8_t)*MAC_ADDRESS_LENGTH)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->sourceMac),
					buffer + len, (sizeof(uint8_t)*MAC_ADDRESS_LENGTH));
				len += (sizeof(uint8_t)*MAC_ADDRESS_LENGTH);
				packetFilterContents->pfcMask |= PFC_MASK_SOURCE_MAC_ADDRESS_TYPE;
			}
			break;

			case QOS_RULES__802_1Q_C_TAG_VID_TYPE:{
				if((lenghtOfPFC - len)<sizeof(VID_TAG_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->_802_1q_ctag_vid),
					buffer + len, sizeof(VID_TAG_t));
				len += sizeof(VID_TAG_t);
				packetFilterContents->pfcMask |= PFC_MASK__802_1Q_C_TAG_VID_TYPE;
			}
			break;

			case QOS_RULES__802_1Q_S_TAG_VID_TYPE:{
				if((lenghtOfPFC - len)<sizeof(VID_TAG_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->_802_1q_stag_vid),
					buffer + len, sizeof(VID_TAG_t));
				len += sizeof(VID_TAG_t);
				packetFilterContents->pfcMask |= PFC_MASK__802_1Q_S_TAG_VID_TYPE;
			}
			break;

			case QOS_RULES__802_1Q_C_TAG_PCP_OR_DEI_TYPE:{
				if((lenghtOfPFC - len)<sizeof(PCP_DEI_TAG_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->_802_1q_ctag_pcp_dei),
					buffer + len, sizeof(PCP_DEI_TAG_t));
				len += sizeof(PCP_DEI_TAG_t);
				packetFilterContents->pfcMask |= PFC_MASK__802_1Q_C_TAG_PCP_OR_DEI_TYPE;
			}
			break;

			case QOS_RULES__802_1Q_S_TAG_PCP_OR_DEI_TYPE:{
				if((lenghtOfPFC - len)<sizeof(PCP_DEI_TAG_t)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->_802_1q_stag_pcp_dei),
					buffer + len, sizeof(PCP_DEI_TAG_t));
				len += sizeof(PCP_DEI_TAG_t);
				packetFilterContents->pfcMask |= PFC_MASK__802_1Q_S_TAG_PCP_OR_DEI_TYPE;
			}
			break;

			case QOS_RULES_ETHERTYPE:{
				if((lenghtOfPFC - len)<(sizeof(uint8_t)*ETHER_TYPE_LENGTH)){
					*decodedLen = len;
					higLog("Insufficient buffer Size");
					nasLogEXIT;
					return FAILURE;
				}
				memcpy(&(packetFilterContents->ethertype),
					buffer + len,(sizeof(uint8_t)*ETHER_TYPE_LENGTH));
				len += (sizeof(uint8_t)*ETHER_TYPE_LENGTH);
				packetFilterContents->pfcMask |= PFC_MASK_ETHERTYPE;
			}
			break;

			default:{
				*decodedLen = len;
				higLog("Unexpected Component Id %d",componentId);
				errno = E_NAS_ERROR_INVALID_INPUT;
				nasLogEXIT;
				return FAILURE;
			}		
		}	
	}
	*decodedLen = len;
	nasLog("Packet_filter_list_create decoded | decoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;
}


int encodePacketFilterContents(uint8_t *buffer, uint32_t bufLen,
	 PacketFilterContents_t *packetFilterContents, uint32_t* encodedLen)
{
	nasLogENTRY;
	uint32_t len = 0;

	if(buffer == NULL || packetFilterContents == NULL || encodedLen == NULL){
		higLog("Invalid Input, Failure decoding");
		errno = E_NAS_ERROR_INVALID_INPUT;
		nasLogEXIT;
		return FAILURE;
	}

	if(packetFilterContents->pfcMask & PFC_MASK_MATCH_ALL_TYPE){
		//To-Do: Add check to see if other fields are set
		//Return failue if you find so
		buffer[len] = QOS_RULES_MATCH_ALL_TYPE;
		len++;
		*encodedLen = len;
		nasLog("Packet_filter_contents encoded | encoded %d bytes", len);
		nasLogEXIT;
		return SUCCESS;
	}

	//To-Do: Using memcpy everywhere, remove from places where 
	//assignment can be used
	if((packetFilterContents->pfcMask & PFC_MASK_IPV4_REMOTE_ADDRESS_TYPE) ==
		PFC_MASK_IPV4_REMOTE_ADDRESS_TYPE){
		buffer[len] = QOS_RULES_IPV4_REMOTE_ADDRESS_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->
			remoteIpV4AddressType), sizeof(Ipv4AddressType_t));
		len += sizeof(Ipv4AddressType_t);
	}

	if((packetFilterContents->pfcMask & PFC_MASK_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE) ==
		PFC_MASK_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE){
		buffer[len] = QOS_RULES_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->
			remoteIpV6AddressType), sizeof(Ipv6AddressType_t));
		len += sizeof(Ipv6AddressType_t);
	}

	if((packetFilterContents->pfcMask & PFC_MASK_IPV4_LOCAL_ADDRESS_TYPE) ==
		PFC_MASK_IPV4_LOCAL_ADDRESS_TYPE){
		buffer[len] = QOS_RULES_IPV4_LOCAL_ADDRESS_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->
			localIpV4AddressType), sizeof(Ipv4AddressType_t));
		len += sizeof(Ipv4AddressType_t);
	}

	if((packetFilterContents->pfcMask & PFC_MASK_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE) ==
		PFC_MASK_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE){
		buffer[len] = QOS_RULES_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->
			localIpV6AddressType), sizeof(Ipv6AddressType_t));
		len += sizeof(Ipv6AddressType_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK_PROTOCOL_IDENTIFIER_OR_NEXT_HEADER_TYPE){
		buffer[len] = QOS_RULES_PROTOCOL_IDENTIFIER_OR_NEXT_HEADER_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->protocolIdentifier),
		 sizeof(uint8_t));
		len += sizeof(uint8_t);
	}

	if((packetFilterContents->pfcMask & PFC_MASK_LOCAL_PORT_RANGE_TYPE) == 
		PFC_MASK_LOCAL_PORT_RANGE_TYPE){
		buffer[len] = QOS_RULES_LOCAL_PORT_RANGE_TYPE;
		len++;
		*(buffer + len) = (packetFilterContents->localPortRange.min >> 8);
		*(buffer + len + 1) = (packetFilterContents->localPortRange.min);
		*(buffer + len + 2) = (packetFilterContents->localPortRange.max >> 8);
		*(buffer + len + 3) = (packetFilterContents->localPortRange.max);
		len += sizeof(PortRange_t);
	} else if((packetFilterContents->pfcMask & PFC_MASK_SINGLE_LOCAL_PORT_TYPE) ==
		PFC_MASK_SINGLE_LOCAL_PORT_TYPE){
		buffer[len] = QOS_RULES_SINGLE_LOCAL_PORT_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->
			singleLocalPort),sizeof(uint16_t));
		len += sizeof(uint16_t);
	}



	if((packetFilterContents->pfcMask & PFC_MASK_REMOTE_PORT_RANGE_TYPE) == 
		PFC_MASK_REMOTE_PORT_RANGE_TYPE){
		buffer[len] = QOS_RULES_REMOTE_PORT_RANGE_TYPE;
		len++;
		*(buffer + len) = (packetFilterContents->remotePortRange.min >> 8);
		*(buffer + len + 1) = (packetFilterContents->remotePortRange.min);
		*(buffer + len + 2) = (packetFilterContents->remotePortRange.max >> 8);
		*(buffer + len + 3) = (packetFilterContents->remotePortRange.max);
		len += sizeof(PortRange_t);
	} else if((packetFilterContents->pfcMask & PFC_MASK_SINGLE_REMOTE_PORT_TYPE)==
		PFC_MASK_SINGLE_REMOTE_PORT_TYPE){
		buffer[len] = QOS_RULES_SINGLE_REMOTE_PORT_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->
			singleRemotePort),sizeof(uint16_t));
		len += sizeof(uint16_t);
	}


	if(packetFilterContents->pfcMask & PFC_MASK_SECURITY_PARAMETER_INDEX_TYPE){
		buffer[len] = QOS_RULES_SECURITY_PARAMETER_INDEX_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->securityParameterIndex),
			sizeof(uint32_t));
		len += sizeof(uint32_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK_TYPE_OF_SERVICE_OR_TRAFFIC_CLASS_TYPE){
		buffer[len] = QOS_RULES_TYPE_OF_SERVICE_OR_TRAFFIC_CLASS_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->typeOfService_TrafficClass)
			,sizeof(TOS_TC_t));
		len += sizeof(TOS_TC_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK_FLOW_LABEL_TYPE){
		buffer[len] = QOS_RULES_FLOW_LABEL_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->flowLabelType)
			,sizeof(FlowLabel_t));
		len += sizeof(FlowLabel_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK_DESTINATION_MAC_ADDRESS_TYPE){
		buffer[len] = QOS_RULES_DESTINATION_MAC_ADDRESS_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->destinationMac),
			MAC_ADDRESS_LENGTH * sizeof(uint8_t));
		len += MAC_ADDRESS_LENGTH * sizeof(uint8_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK_SOURCE_MAC_ADDRESS_TYPE){
		buffer[len] = QOS_RULES_SOURCE_MAC_ADDRESS_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->sourceMac),
			MAC_ADDRESS_LENGTH * sizeof(uint8_t));
		len += MAC_ADDRESS_LENGTH * sizeof(uint8_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK__802_1Q_C_TAG_VID_TYPE){
		buffer[len] = QOS_RULES__802_1Q_C_TAG_VID_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->_802_1q_ctag_vid)
			,sizeof(VID_TAG_t));
		len += sizeof(VID_TAG_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK__802_1Q_S_TAG_VID_TYPE){
		buffer[len] = QOS_RULES__802_1Q_S_TAG_VID_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->_802_1q_stag_vid)
			,sizeof(VID_TAG_t));
		len += sizeof(VID_TAG_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK__802_1Q_C_TAG_PCP_OR_DEI_TYPE){
		buffer[len] = QOS_RULES__802_1Q_C_TAG_PCP_OR_DEI_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->_802_1q_ctag_pcp_dei)
			,sizeof(PCP_DEI_TAG_t));
		len += sizeof(PCP_DEI_TAG_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK__802_1Q_S_TAG_PCP_OR_DEI_TYPE){
		buffer[len] = QOS_RULES__802_1Q_S_TAG_PCP_OR_DEI_TYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->_802_1q_stag_pcp_dei)
			,sizeof(PCP_DEI_TAG_t));
		len += sizeof(PCP_DEI_TAG_t);
	}

	if(packetFilterContents->pfcMask & PFC_MASK_ETHERTYPE){
		buffer[len] = QOS_RULES_ETHERTYPE;
		len++;
		memcpy(buffer + len, &(packetFilterContents->ethertype)
			,ETHER_TYPE_LENGTH * sizeof(uint8_t));
		len += ETHER_TYPE_LENGTH * sizeof(uint8_t);
	}

	*encodedLen = len;
	nasLog("Packet_filter_contents encoded | encoded %d bytes", len);
	nasLogEXIT;
	return SUCCESS;
}
