#ifndef __PACKET_FILTER_CONTENTS_H
#define __PACKET_FILTER_CONTENTS_H
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"



#define PACKET_FILTER_CONTENTS_MAX_NO	 	64
#define IPV6_ADDRESS_LENGTH 		 		16
#define MAC_ADDRESS_LENGTH 			 		6
#define ETHER_TYPE_LENGTH			 		2



#define QOS_RULES_MATCH_ALL_TYPE							0b00000001
#define QOS_RULES_IPV4_REMOTE_ADDRESS_TYPE					0b00010000
#define QOS_RULES_IPV4_LOCAL_ADDRESS_TYPE					0b00010001
#define QOS_RULES_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE	0b00100001
#define QOS_RULES_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE	0b00100011
#define QOS_RULES_PROTOCOL_IDENTIFIER_OR_NEXT_HEADER_TYPE	0b00110000
#define QOS_RULES_SINGLE_LOCAL_PORT_TYPE					0b01000000
#define QOS_RULES_LOCAL_PORT_RANGE_TYPE						0b01000001
#define QOS_RULES_SINGLE_REMOTE_PORT_TYPE					0b01010000
#define QOS_RULES_REMOTE_PORT_RANGE_TYPE					0b01010001
#define QOS_RULES_SECURITY_PARAMETER_INDEX_TYPE				0b01100000
#define QOS_RULES_TYPE_OF_SERVICE_OR_TRAFFIC_CLASS_TYPE		0b01110000
#define QOS_RULES_FLOW_LABEL_TYPE							0b10000000
#define QOS_RULES_DESTINATION_MAC_ADDRESS_TYPE				0b10000001
#define QOS_RULES_SOURCE_MAC_ADDRESS_TYPE					0b10000010
#define QOS_RULES__802_1Q_C_TAG_VID_TYPE					0b10000011
#define QOS_RULES__802_1Q_S_TAG_VID_TYPE					0b10000100
#define QOS_RULES__802_1Q_C_TAG_PCP_OR_DEI_TYPE				0b10000101
#define QOS_RULES__802_1Q_S_TAG_PCP_OR_DEI_TYPE				0b10000110
#define QOS_RULES_ETHERTYPE 								0b10000111



/*1st byte: conditional bits for non-coexisting fields
 2nd & 3rd byte: normal masking per field*/

#define PFC_MASK_MATCH_ALL_TYPE								0x000001
#define PFC_MASK_IPV4_REMOTE_ADDRESS_TYPE					0x000002
#define PFC_MASK_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE	0x010002
#define PFC_MASK_IPV4_LOCAL_ADDRESS_TYPE					0x000004
#define PFC_MASK_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE	0x020004
#define PFC_MASK_PROTOCOL_IDENTIFIER_OR_NEXT_HEADER_TYPE	0x000008
#define PFC_MASK_SINGLE_LOCAL_PORT_TYPE						0x000010
#define PFC_MASK_LOCAL_PORT_RANGE_TYPE						0x040010
#define PFC_MASK_SINGLE_REMOTE_PORT_TYPE					0x000020
#define PFC_MASK_REMOTE_PORT_RANGE_TYPE						0x080020
#define PFC_MASK_SECURITY_PARAMETER_INDEX_TYPE				0x000040
#define PFC_MASK_TYPE_OF_SERVICE_OR_TRAFFIC_CLASS_TYPE		0x000080
#define PFC_MASK_FLOW_LABEL_TYPE							0x000100
#define PFC_MASK_DESTINATION_MAC_ADDRESS_TYPE				0x000200
#define PFC_MASK_SOURCE_MAC_ADDRESS_TYPE					0x000400
#define PFC_MASK__802_1Q_C_TAG_VID_TYPE						0x000800
#define PFC_MASK__802_1Q_S_TAG_VID_TYPE						0x001000
#define PFC_MASK__802_1Q_C_TAG_PCP_OR_DEI_TYPE				0x002000
#define PFC_MASK__802_1Q_S_TAG_PCP_OR_DEI_TYPE				0x004000
#define PFC_MASK_ETHERTYPE									0x008000

typedef struct Ipv4AddressType{
	uint32_t ipv4Address;
	uint32_t ipv4Mask;
	Ipv4AddressType(){
		ipv4Address = INADDR_NONE;
		ipv4Mask = INADDR_NONE;
	}
}Ipv4AddressType_t;

typedef struct Ipv6AddressType{
	uint8_t ipv6Address[IPV6_ADDRESS_LENGTH];
	uint8_t prefixLength;
	Ipv6AddressType(){
		prefixLength = 0;
	}
}Ipv6AddressType_t;

typedef struct PortRange{
	uint16_t min;
	uint16_t max;
	PortRange(){
		min = 0;
		max = 0;
	}
}PortRange_t;

typedef struct TOS_TC{
	uint8_t field;
	uint8_t mask;
	TOS_TC(){
		field = 0;
		mask = 0;
	}
}TOS_TC_t;

typedef struct FlowLabel{
	uint8_t spare:4;
	uint32_t flowLabel:20;
	FlowLabel(){
		spare = 0;
		flowLabel = 0;
	}
}FlowLabel_t;

typedef struct VID_TAG{
	uint8_t spare:4;
	uint16_t vid:12;
	VID_TAG(){
		spare = 0;
		vid = 0;
	}
}VID_TAG_t;

typedef struct PCP_DEI_TAG{
	uint8_t spare:4;
	uint8_t pcp:3;
	uint8_t dei:1;
	PCP_DEI_TAG(){
		spare = 0;
		pcp = 0;
		dei = 0;
	}
}PCP_DEI_TAG_t;

typedef struct PacketFilterContents {
	uint32_t pfcMask:20;
	

		Ipv4AddressType_t remoteIpV4AddressType;
		Ipv6AddressType_t remoteIpV6AddressType;

		Ipv4AddressType_t localIpV4AddressType;
		Ipv6AddressType_t localIpV6AddressType;

	
	uint8_t protocolIdentifier;

		uint16_t singleLocalPort;
		PortRange_t localPortRange;

		uint16_t singleRemotePort;
		PortRange_t remotePortRange;

	uint32_t securityParameterIndex;
	TOS_TC_t typeOfService_TrafficClass;
	FlowLabel_t flowLabelType;
	uint8_t destinationMac[MAC_ADDRESS_LENGTH];
	uint8_t sourceMac[MAC_ADDRESS_LENGTH];
	VID_TAG_t _802_1q_ctag_vid;
	VID_TAG_t _802_1q_stag_vid;
	PCP_DEI_TAG_t _802_1q_ctag_pcp_dei;
	PCP_DEI_TAG_t _802_1q_stag_pcp_dei;
	uint8_t ethertype[ETHER_TYPE_LENGTH];

	int setLocalIPV4Address(std::string ip, std::string mask){
		if((this->pfcMask & PFC_MASK_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE) ==
			PFC_MASK_IPV6_LOCAL_ADDRESS_OR_PREFIX_LENGTH_TYPE){
			higLog("IPV6 FLAG Already Set");
			return FAILURE;
		}
		int rc = 0;
		rc = inet_pton(AF_INET, ip.c_str(),
			&(this->localIpV4AddressType.ipv4Address));

		if(!rc) {
		    higLog("Not a valid input ipAddr");
		    return FAILURE;
		}
		if(rc<0) {
			
		    higLog("Not a valid input AF value");
		    return FAILURE;
		}

		rc = inet_pton(AF_INET, mask.c_str(),
			&(this->localIpV4AddressType.ipv4Mask));

		if(!rc) {
		    higLog("Not a valid input ipAddr");
		    return FAILURE;
		}
		if(rc<0) {
			
		    higLog("Not a valid input AF value");
		    return FAILURE;
		}
		this->pfcMask |= PFC_MASK_IPV4_LOCAL_ADDRESS_TYPE;
		return SUCCESS;
	}

	int setRemoteIPV4Address(std::string ip, std::string mask){
		if((this->pfcMask & PFC_MASK_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE) ==
			PFC_MASK_IPV6_REMOTE_ADDRESS_OR_PREFIX_LENGTH_TYPE){
			higLog("IPV6 FLAG Already Set");
			return FAILURE;
		}
		int rc = 0;
		rc = inet_pton(AF_INET, ip.c_str(),
			&(this->remoteIpV4AddressType.ipv4Address));

		if(!rc) {
		    higLog("Not a valid input ipAddr");
		    return FAILURE;
		}
		if(rc<0) {
			
		    higLog("Not a valid input AF value");
		    return FAILURE;
		}

		rc = inet_pton(AF_INET, mask.c_str(),
			&(this->remoteIpV4AddressType.ipv4Mask));

		if(!rc) {
		    higLog("Not a valid input ipAddr");
		    return FAILURE;
		}
		if(rc<0) {
			
		    higLog("Not a valid input AF value");
		    return FAILURE;
		}
		this->pfcMask |= PFC_MASK_IPV4_REMOTE_ADDRESS_TYPE;
		return SUCCESS;
	}

	int setLocalSinglePort(uint16_t port){
		if((this->pfcMask & PFC_MASK_LOCAL_PORT_RANGE_TYPE) ==
			PFC_MASK_LOCAL_PORT_RANGE_TYPE){
			higLog("PORT Range FLAG Already Set");
			return FAILURE;
		}

		this->singleLocalPort = port;
		this->pfcMask |= PFC_MASK_SINGLE_LOCAL_PORT_TYPE;

		return SUCCESS;
	}

	int setRemoteSinglePort(uint16_t port){
		if((this->pfcMask & PFC_MASK_REMOTE_PORT_RANGE_TYPE) ==
			PFC_MASK_REMOTE_PORT_RANGE_TYPE){
			higLog("PORT Range FLAG Already Set");
			return FAILURE;
		}

		this->singleRemotePort = port;
		this->pfcMask |= PFC_MASK_SINGLE_REMOTE_PORT_TYPE;

		return SUCCESS;
	}

    int getLen() {
        int len = 0;

        if(!(this->pfcMask)) {
            return len;
        }

        if ((this->pfcMask & PFC_MASK_MATCH_ALL_TYPE)
                == PFC_MASK_MATCH_ALL_TYPE) {
            len++;
        }

        if ((this->pfcMask & PFC_MASK_PROTOCOL_IDENTIFIER_OR_NEXT_HEADER_TYPE)
                == PFC_MASK_PROTOCOL_IDENTIFIER_OR_NEXT_HEADER_TYPE) {
            len++;
            len += 1;
        }

        if ((this->pfcMask & PFC_MASK_IPV4_LOCAL_ADDRESS_TYPE)
                == PFC_MASK_IPV4_LOCAL_ADDRESS_TYPE) {
            len++;
            len += 8;
        }

        if ((this->pfcMask & PFC_MASK_IPV4_REMOTE_ADDRESS_TYPE)
                == PFC_MASK_IPV4_REMOTE_ADDRESS_TYPE) {
            len++;
            len += 8;
        }

        if ((this->pfcMask & PFC_MASK_LOCAL_PORT_RANGE_TYPE)
                == PFC_MASK_LOCAL_PORT_RANGE_TYPE) {
            len++;
            len += 4;
        } else if ((this->pfcMask & PFC_MASK_SINGLE_LOCAL_PORT_TYPE)
                == PFC_MASK_SINGLE_LOCAL_PORT_TYPE) {
            len++;
            len += 2;
        }

        if ((this->pfcMask & PFC_MASK_REMOTE_PORT_RANGE_TYPE)
                == PFC_MASK_REMOTE_PORT_RANGE_TYPE) {
            len++;
            len += 4;
        } else if ((this->pfcMask & PFC_MASK_SINGLE_REMOTE_PORT_TYPE)
                == PFC_MASK_SINGLE_REMOTE_PORT_TYPE) {
            len++;
            len += 2;
        }

        return len;

    }

}PacketFilterContents_t;

int decodePacketFilterContents(uint8_t *buffer, uint32_t bufLen, PacketFilterContents_t *packetFilterContents, uint32_t* decodedLen,
	uint8_t lenghtOfPFC);

int encodePacketFilterContents(uint8_t *buffer, uint32_t bufLen, PacketFilterContents_t *packetFilterContents, uint32_t* encodedLen);

#endif 

