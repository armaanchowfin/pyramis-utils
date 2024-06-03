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

#ifndef _UTILS_IITB_H
#define _UTILS_IITB_H
#include <string>
#include "platform/include/logging.h"
#include "regex/include/regexLib.h"
using namespace std;

#define NODE_MASK_IP_MASK		(1<<0)
#define NODE_MASK_PORT 			(1<<1)
#define NODE_MASK_IP 	        (1<<2)
#define NODE_MASK_IP_ANY 	    (1<<3)
#define NODE_MASK_IP_ASSIGNED 	(1<<4)
#define NODE_MASK_IP_INVERT 	(1<<5)
#define NODE_MASK_PORT_RANGE 	(1<<6)

typedef enum IPFilterRuleAction{
	PERMIT, 
	DENY,	
	MAX_NO_ACTION
}e_IPFilterRuleAction;

typedef enum IPFilterRuleDirection{
	IN, 
	OUT,	
	MAX_NO_DIRECTION
}e_IPFilterRuleDirection;

typedef enum IPFilterRuleProtocol{
	ICMP = 1,
	IGMP = 2,
	TCP  = 6,
	UDP  = 17,
	IPV6 = 41,
	IPV6_ICMP = 58,
	IP = 144
}e_IPFilterRuleProtocol;

typedef struct IPFilterRulePortRange{
	uint16_t min;
	uint16_t max;
	IPFilterRulePortRange(){
		min = 0;
		max = 0;
	}
}IPFilterRulePortRange_t;

typedef struct IPFilterRuleAddrRange{
	uint32_t min;
	uint32_t max;
    uint32_t ip;
    uint32_t ipMask;
    uint8_t maskNo;
	IPFilterRuleAddrRange(){
		min = 0;
		max = 0;
        ip = 0;
        ipMask = 0;
        maskNo = 0;
	}
}IPFilterRuleAddrRange_t;

typedef struct IPFilterRuleNode{
	uint8_t nodeMask;

  
    union addrType{
        uint32_t ip;
        IPFilterRuleAddrRange_t addrRange;
        addrType(){
            ip = 0;
        }
    }addrTypeChoice;
	union PortType{
		uint16_t port;
		IPFilterRulePortRange_t port_range;
        PortType() {
            port = 0;
        }
	}PortType_Choice;

    IPFilterRuleNode() {
        nodeMask = 0;
        addrTypeChoice = {};
        PortType_Choice = {};
    }
}IPFilterRuleNode_t;

typedef struct IPFilterRule{
	e_IPFilterRuleAction action;
	e_IPFilterRuleDirection dir;
	e_IPFilterRuleProtocol proto;
	IPFilterRuleNode_t srcNode;
	IPFilterRuleNode_t destNode;
	

	
    
    
    
    IPFilterRule() {
        action = MAX_NO_ACTION;
        dir = MAX_NO_DIRECTION;
        proto = IP;
        srcNode = {};
        destNode = {};
    }
    int setSource(string ip,string mask,uint16_t port){
    	if (inet_pton(AF_INET, ip.c_str(), &(this->srcNode.addrTypeChoice.addrRange.ip))
                  < 0){
		    higLog("inet_pton: Failed to convert Ip address");
		    return FAILURE;
		}
    	if (inet_pton(AF_INET, mask.c_str(), &(this->srcNode.addrTypeChoice.addrRange.ipMask))
                  < 0){
		    higLog("inet_pton: Failed to convert Ip address");
		    return FAILURE;
		}
        uint32_t addr = this->srcNode.addrTypeChoice.addrRange.ip;
        uint32_t ipMask = this->srcNode.addrTypeChoice.addrRange.ipMask;
        uint8_t maskNo = 0;
        while ( ipMask )
        {
            maskNo += ( ipMask & 0x01 );
            ipMask >>= 1;
        }
        ipMask = this->srcNode.addrTypeChoice.addrRange.ipMask;
        this->srcNode.addrTypeChoice.addrRange.maskNo = maskNo;
        this->srcNode.addrTypeChoice.addrRange.min = (addr & ipMask);
        this->srcNode.addrTypeChoice.addrRange.max = (addr | (~ipMask));
		this->srcNode.PortType_Choice.port = port;
		this->srcNode.nodeMask |= NODE_MASK_IP_MASK | NODE_MASK_PORT;
		return SUCCESS;
    }

    int setDestination(string ip,string mask,uint16_t port){
    	if (inet_pton(AF_INET, ip.c_str(), &(this->destNode.addrTypeChoice.addrRange.ip))
                  < 0){
		    higLog("inet_pton: Failed to convert Ip address");
		    return FAILURE;
		}
    	if (inet_pton(AF_INET, mask.c_str(), &(this->destNode.addrTypeChoice.addrRange.ipMask))
                  < 0){
		    higLog("inet_pton: Failed to convert Ip address");
		    return FAILURE;
		}
        uint32_t addr = this->destNode.addrTypeChoice.addrRange.ip;
        uint32_t ipMask = this->destNode.addrTypeChoice.addrRange.ipMask;
        uint8_t maskNo = 0;
        while ( ipMask )
        {
            maskNo += ( ipMask & 0x01 );
            ipMask >>= 1;
        }
        ipMask = this->destNode.addrTypeChoice.addrRange.ipMask;
        this->destNode.addrTypeChoice.addrRange.maskNo = maskNo;
        this->destNode.addrTypeChoice.addrRange.min = (addr & ipMask);
        this->destNode.addrTypeChoice.addrRange.max = (addr | (~ipMask));

        this->destNode.PortType_Choice.port = port;
		this->destNode.nodeMask |= NODE_MASK_IP_MASK | NODE_MASK_PORT;
		return SUCCESS;
    }

    int setSource(uint32_t ip, uint32_t ipMask){
    	this->srcNode.addrTypeChoice.addrRange.ip = ip;
    	this->srcNode.addrTypeChoice.addrRange.ipMask = ipMask;
        uint8_t maskNo = 0;
        while ( ipMask )
        {
            maskNo += ( ipMask & 0x01 );
            ipMask >>= 1;
        }
        this->srcNode.addrTypeChoice.addrRange.maskNo = maskNo;
        /*uint32_t lowAddr = ntohl(ip);
        uint32_t upAddr = ntohl(ip);
        for (uint32_t i = 0; i < (32 - maskNo); i++) {
            lowAddr &=  ~(1<<i);
            upAddr |= (1<<i);
        }
        this->srcNode.addrTypeChoice.addrRange.min = htonl(lowAddr);
        this->srcNode.addrTypeChoice.addrRange.max = htonl(upAddr);
        */
        

        ipMask = this->srcNode.addrTypeChoice.addrRange.ipMask;
        this->srcNode.addrTypeChoice.addrRange.min = (ip & ipMask);
        this->srcNode.addrTypeChoice.addrRange.max = (ip | (~ipMask));

		this->srcNode.nodeMask |= NODE_MASK_IP_MASK;
		return SUCCESS;
    }

    int setDestination(uint32_t ip, uint32_t ipMask){
    	this->destNode.addrTypeChoice.addrRange.ip = ip;
    	this->destNode.addrTypeChoice.addrRange.ipMask = ipMask;
        uint8_t maskNo = 0;
        while ( ipMask )
        {
            maskNo += ( ipMask & 0x01 );
            ipMask >>= 1;
        }
        this->destNode.addrTypeChoice.addrRange.maskNo = maskNo;
        /*uint32_t lowAddr = ntohl(ip);
        uint32_t upAddr = ntohl(ip);
        for (uint32_t i = 0; i < (32 - maskNo); i++) {
            lowAddr &=  ~(1<<i);
            upAddr |= (1<<i);
        }
        this->destNode.addrTypeChoice.addrRange.min = htonl(lowAddr);
        this->destNode.addrTypeChoice.addrRange.max = htonl(upAddr);*/
        ipMask = this->destNode.addrTypeChoice.addrRange.ipMask;
        this->destNode.addrTypeChoice.addrRange.min = (ip & ipMask);
        this->destNode.addrTypeChoice.addrRange.max = (ip | (~ipMask));

		this->destNode.nodeMask |= NODE_MASK_IP_MASK;
		return SUCCESS;
    }
}IPFilterRule_t;

int encodeIPFilterRule(IPFilterRule_t ipFilterRule,std::string &rule);
int decodeIPFilterRule(IPFilterRule_t &ipFilterRule,std::string rule);



#define SECONDS_TO_MINUTES(val)   (val/60)
#define SECONDS_TO_HOURS(val)     (val/3600)
#define MINUTES_TO_SECONDS(val)   (val*60)

#define DOWNCONVERT_BY_KILO(val)      (val*1024)

int __highestPowerOf4(uint64_t n);
uint64_t __get4RaiseTo(int i);
uint16_t __getValue(uint64_t n);
uint8_t __getUnit(uint64_t n);
int convertToKbps(string value, uint64_t &rateKbps);

#define HIGHEST_POWER_VALUE(n) __getValue(n)

#define HIGHEST_POWER_UNIT(n) __getUnit(n)

#define MAX_BITRATE_KBPS 10000000

#endif
