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

#include <iostream>
#include <bitset>
#include <string>
#include "common/include/utils-iitb.h"

int __highestPowerOf4(uint64_t n) {
    int res = 0;
    while(n) {
        res++;
        n = n/4;
    }
    return res;
}

uint64_t __get4RaiseTo(int i) {
    uint64_t res = 1;
    while(i>0) {
        res = 4*res;
        i--;
    }
    return res;
}

/* should not cross uint8_t (as per use case) */
uint8_t __getUnit(uint64_t n /*in Kbps*/) {
    lowLog("n %llu Kbps : Unit %d", n, __highestPowerOf4(n));
    return __highestPowerOf4(n);
}

/* should not cross uint16_t (as per use case) */
uint16_t __getValue(uint64_t n /*in Kbps*/) {
    uint8_t unit = __highestPowerOf4(n);
    uint16_t val = unit ? n / __get4RaiseTo(unit-1) : 0;
    lowLog("n %llu Kbps : Value %d", n, val);
    return val;
}

int convertToKbps(string value, uint64_t &rateKbps){
    LOG_ENTRY;

    // std::smatch match;
    // std::regex bitRateRegex("((^\\d+)(\\.\\d+)?) (bps|Kbps|Mbps|Gbps|Tbps)$");
    // if (!(std::regex_match(value, match, bitRateRegex))){
        
    //    higLog("Not a valid Value %s", value.c_str());
    //    LOG_EXIT;
    //    return FAILURE;
    // }

    size_t nmatch = MAX_NUM_MATCHES;
    regmatch_t pmatch[MAX_NUM_MATCHES+1];
    int eflags = 0;
    int bitRateValueLen = value.length();
    char bitRateValueStr[MAX_PATTERN_STRING_LENGTH];
    strcpy(bitRateValueStr, value.c_str());

    std::unordered_map<REGULAR_EXPRESSION, regex_t >::iterator itr = 
                                    regexToDfaMap.find(BITRATE_REGEXP);
    if(itr == regexToDfaMap.end())
    {
        higLog("Regex not present");
        LOG_EXIT;
        return FAILURE;
    }
    regex_t bitratePReg = itr->second;
    if (regexMatchFull(&bitratePReg, bitRateValueStr, bitRateValueLen, 
                            nmatch, pmatch, eflags) != SUCCESS)
    {
        higLog("Not a valid Value %s", value.c_str());
        LOG_EXIT;
        return FAILURE;
    }

    std::string bitRate = value.substr(pmatch[1].rm_so, pmatch[1].rm_eo-pmatch[1].rm_so); //match[1].str();
    std::string unit = value.substr(pmatch[4].rm_so, pmatch[4].rm_eo-pmatch[4].rm_so); //match[4].str();

	double rate = stod(bitRate);
    if (unit == "Mbps")
        rate = rate * 1000;
    if (unit == "Gbps")
        rate = rate * 1000 * 1000;
    if (unit == "Tbps")
        rate = rate * 1000 * 1000 * 1000;

    rateKbps = ceil(rate);
    /* As per spec 36.413 section 9.2.1.19 */
    if (rateKbps > MAX_BITRATE_KBPS) {
       higLog("Bit rate send %d Kbps exceeds the maximum range %d Kbps", rateKbps,
               MAX_BITRATE_KBPS);
       LOG_EXIT;
       return FAILURE;

    }
    LOG_EXIT;
    return SUCCESS;
}

int encodeIPFilterRule(IPFilterRule_t ipFilterRule,std::string &flowDesc){
    LOG_ENTRY;
    flowDesc = "";

    switch(ipFilterRule.action){
        case PERMIT: flowDesc+="permit";break;
        case DENY: flowDesc+="deny";break;
        default:{
            higLog("Unsupported Action");
            LOG_EXIT;
            return FAILURE;
        }
    }

    switch(ipFilterRule.dir){
        case IN: flowDesc+=" in";break;
        case OUT: flowDesc+=" out";break;
        default:{
            higLog("Unsupported Direction");
            LOG_EXIT;
            return FAILURE;
        }
    }

    switch(ipFilterRule.proto){
        case IP: flowDesc +=" ip";break;
        case UDP: flowDesc += " 17"; break;
        case TCP: flowDesc += " 6"; break;
        default:{
            higLog("UnSupported PROTO");
            LOG_EXIT;
            return FAILURE;
        }
    }

    /*Source Node*/
    flowDesc +=" from ";
    if ((ipFilterRule.srcNode.nodeMask & NODE_MASK_IP_INVERT) == NODE_MASK_IP_INVERT) {
        flowDesc += "!";
    }
    if ((ipFilterRule.srcNode.nodeMask & NODE_MASK_IP_ANY) == NODE_MASK_IP_ANY) {
       flowDesc+="any";
    }
    else if ((ipFilterRule.srcNode.nodeMask & NODE_MASK_IP_ASSIGNED) == NODE_MASK_IP_ASSIGNED) {
       flowDesc+="assigned";
    }
    else if ((ipFilterRule.srcNode.nodeMask & NODE_MASK_IP) == NODE_MASK_IP) {

        string ipAddress = string(inet_ntoa(*(struct in_addr *)&ipFilterRule.srcNode.addrTypeChoice.ip));
        flowDesc += ipAddress;
    }
    else if ((ipFilterRule.srcNode.nodeMask & NODE_MASK_IP_MASK) == NODE_MASK_IP_MASK) {
        string ipAddress = string(inet_ntoa(*(struct in_addr *)&ipFilterRule.srcNode.addrTypeChoice.addrRange.ip));
        flowDesc += ipAddress;
        flowDesc+="/"+to_string(ipFilterRule.srcNode.addrTypeChoice.addrRange.maskNo);
    }
    else {
        higLog("src Address is mandatory");
        return FAILURE;
    }
    if((ipFilterRule.srcNode.nodeMask & NODE_MASK_PORT) == NODE_MASK_PORT){
        flowDesc += " "+to_string(ipFilterRule.srcNode.PortType_Choice.port);
    }
    else if((ipFilterRule.srcNode.nodeMask & NODE_MASK_PORT_RANGE) == NODE_MASK_PORT_RANGE){
            string minPort = to_string(ipFilterRule.srcNode.PortType_Choice.port_range.min);
            string maxPort = to_string(ipFilterRule.srcNode.PortType_Choice.port_range.max);
            flowDesc += " "+minPort+"-"+maxPort;
    }

    /*Destination Node*/
    flowDesc +=" to ";
    if ((ipFilterRule.destNode.nodeMask & NODE_MASK_IP_INVERT) == NODE_MASK_IP_INVERT) {
        flowDesc += "!";
    }
    if ((ipFilterRule.destNode.nodeMask & NODE_MASK_IP_ANY) == NODE_MASK_IP_ANY) {
       flowDesc+="any";
    }
    else if ((ipFilterRule.destNode.nodeMask & NODE_MASK_IP_ASSIGNED) == NODE_MASK_IP_ASSIGNED) {
       flowDesc+="assigned";
    }
    else if ((ipFilterRule.destNode.nodeMask & NODE_MASK_IP) == NODE_MASK_IP) {

        string ipAddress = string(inet_ntoa(*(struct in_addr *)&ipFilterRule.destNode.addrTypeChoice.ip));
        flowDesc += ipAddress;
    }
    else if ((ipFilterRule.destNode.nodeMask & NODE_MASK_IP_MASK) == NODE_MASK_IP_MASK) {
        string ipAddress = string(inet_ntoa(*(struct in_addr *)&ipFilterRule.destNode.addrTypeChoice.addrRange.ip));
        flowDesc += ipAddress;
        flowDesc+="/"+to_string(ipFilterRule.destNode.addrTypeChoice.addrRange.maskNo);
    }
    else {
        higLog("src Address is mandatory");
        LOG_EXIT;
        return FAILURE;
    }
    if((ipFilterRule.destNode.nodeMask & NODE_MASK_PORT) == NODE_MASK_PORT){
        flowDesc += " "+to_string(ipFilterRule.destNode.PortType_Choice.port);
    }
    else if((ipFilterRule.destNode.nodeMask & NODE_MASK_PORT_RANGE) == NODE_MASK_PORT_RANGE){
            string minPort = to_string(ipFilterRule.destNode.PortType_Choice.port_range.min);
            string maxPort = to_string(ipFilterRule.destNode.PortType_Choice.port_range.max);
            flowDesc += " "+minPort+"-"+maxPort;
    }
    LOG_EXIT;
    return SUCCESS;
}

int decodeIPFilterRuleNode(IPFilterRuleNode_t &node, std::string nodeString) {
    LOG_ENTRY;

    // std::smatch match;
    // std::regex addRegex("(!?)(any|assigned|(25[0-6]|2[0-4][0-9]|1[0-9]{1,2}|"
    //         "[0-9]{1,2}).(25[0-6]|2[0-4][0-9]|1[0-9]{1,2}|[0-9]{1,2}).(25[0-6]"
    //         "|2[0-4][0-9]|1[0-9]{1,2}|[0-9]{1,2}).(25[0-6]|2[0-4][0-9]|1[0-9]"
    //         "{1,2}|[0-9]{1,2}))(/[0-9]{1,3})?( [0-9,-]*)?");
    // if (!(std::regex_match(nodeString, match, addRegex))){
    //     higLog("Not a valid Node set %s", nodeString.c_str());
    //     LOG_EXIT;
    //     return FAILURE;
    // }

    size_t nmatch = MAX_NUM_MATCHES;
    regmatch_t pmatch[MAX_NUM_MATCHES+1];
    int eflags = 0;
    int nodeStringLen = nodeString.length();
    char nodeStringStr[MAX_PATTERN_STRING_LENGTH];
    strcpy(nodeStringStr, nodeString.c_str());

    std::unordered_map<REGULAR_EXPRESSION, regex_t >::iterator itr = 
                                    regexToDfaMap.find(IP_FILTER_RULE_NODE_REGEXP);
    if(itr == regexToDfaMap.end())
    {
        higLog("Regex not present");
        LOG_EXIT;
        return FAILURE;
    }
    regex_t ipFilterRuleNodePReg = itr->second;
    if (regexMatchFull(&ipFilterRuleNodePReg, nodeStringStr, nodeStringLen, 
                            nmatch, pmatch, eflags) != SUCCESS)
    {
        higLog("Not a valid Node set %s", nodeString.c_str());
        LOG_EXIT;
        return FAILURE;
    }

    // if (match[1].str() == "!") node.nodeMask |= NODE_MASK_IP_INVERT;
    if (nodeString.substr(pmatch[1].rm_so, pmatch[1].rm_eo-pmatch[1].rm_so) == "!")
        node.nodeMask |= NODE_MASK_IP_INVERT;
    std::string addr = nodeString.substr(pmatch[2].rm_so, pmatch[2].rm_eo-pmatch[2].rm_so); //match[2].str();
    if (addr.empty()) {
        higLog("IP Address mandatory field missing");
        LOG_EXIT;
        return FAILURE;
    }
    if (addr == "any") node.nodeMask |= NODE_MASK_IP_ANY;
    else if (addr == "assigned") node.nodeMask |= NODE_MASK_IP_ASSIGNED;
    else {
        std::string subNetMask = nodeString.substr(pmatch[7].rm_so, pmatch[7].rm_eo-pmatch[7].rm_so); //match[7].str();
        if (subNetMask.empty()) {
            if (inet_pton(AF_INET, addr.c_str(), &node.addrTypeChoice.ip)
                  == FAILURE){
                higLog("inet_pton: Failed to convert Ip address");
                LOG_EXIT;
                return FAILURE;
            }
            node.nodeMask |= NODE_MASK_IP;
        }
        else {
            uint32_t ip;
            if (inet_pton(AF_INET, addr.c_str(), &ip)
                  == FAILURE){
                higLog("inet_pton: Failed to convert Ip address");
                LOG_EXIT;
                return FAILURE;
            }
            node.addrTypeChoice.addrRange.ip = ip;
            subNetMask = subNetMask.substr(1); // to remove the /
            uint8_t maskNo = (uint8_t)std::stoi(subNetMask);
            node.addrTypeChoice.addrRange.maskNo = maskNo;
            uint32_t ipMask = ntohl(~((1 << (32 - maskNo)) - 1));
            node.addrTypeChoice.addrRange.ipMask = ipMask;

            node.addrTypeChoice.addrRange.min = (ip & ipMask);
            node.addrTypeChoice.addrRange.max = (ip | (~ipMask));

            lowLog("%s ipmask:",inet_ntoa(*(struct in_addr *)&ipMask));
            lowLog("%s lowAddress:",inet_ntoa(*(struct in_addr *)&node.addrTypeChoice.addrRange.min));
            lowLog("%s upAddress:",inet_ntoa(*(struct in_addr *)&node.addrTypeChoice.addrRange.max));

            node.nodeMask |= NODE_MASK_IP_MASK;
        }
    }
    /* port */
    std::string port;
    if(pmatch[8].rm_so > 0 && pmatch[8].rm_eo>pmatch[8].rm_so)
    {
        port = nodeString.substr(pmatch[8].rm_so, pmatch[8].rm_eo-pmatch[8].rm_so); //match[8].str();
        if (port.empty()) {
            lowLog("Port is not present");
            LOG_EXIT;
            return SUCCESS;
        }
    }
    else
    {
        lowLog("Port is not present");
        LOG_EXIT;
        return SUCCESS;
    }
    port = port.substr(1); // to remove a space
    size_t pos = 0;
    pos = port.find('-');
    if (pos != std::string::npos) {
        int intPort(std::stoi(port.substr(0,pos)));
        node.PortType_Choice.port_range.min = (uint16_t) intPort;
        intPort = (std::stoi(port.substr(pos+1)));
        node.PortType_Choice.port_range.max = (uint16_t) intPort;
        node.nodeMask |= NODE_MASK_PORT_RANGE;
    } else {
        int intPort(std::stoi(port.substr(0,pos)));
        node.PortType_Choice.port = (uint16_t) intPort;
        node.nodeMask |= NODE_MASK_PORT;
    }
    return SUCCESS;
}


int decodeIPFilterRule(IPFilterRule_t &ipFilterRule, std::string flowDesc){
   LOG_ENTRY;

    size_t nmatch = MAX_NUM_MATCHES;
    regmatch_t pmatch[MAX_NUM_MATCHES+1];
    int eflags = 0;
    int flowDescLen = flowDesc.length();
    char flowDescStr[MAX_PATTERN_STRING_LENGTH];
    strcpy(flowDescStr, flowDesc.c_str());

    // std::smatch match;
    // std::regex flowRegex("(.+)\\s+(.+)\\s+(.+)\\s+from\\s+(.+)\\s+to\\s+(.+)");
    // if (!(std::regex_match(flowDesc, match, flowRegex))){
    //     higLog("Not a valid flow Value");
    //     LOG_EXIT;
    //     return FAILURE;
    // }

    std::unordered_map<REGULAR_EXPRESSION, regex_t >::iterator itr = 
                                    regexToDfaMap.find(IP_FILTER_RULE_REGEXP);
    if(itr == regexToDfaMap.end())
    {
        higLog("Regex not present");
        LOG_EXIT;
        return FAILURE;
    }
    regex_t ipFilterRulePReg = itr->second;
    if (regexMatchFull(&ipFilterRulePReg, flowDescStr, flowDescLen, 
                            nmatch, pmatch, eflags) != SUCCESS)
    {
        higLog("Not a valid flow Value, %d %d", pmatch[0].rm_eo-pmatch[0].rm_so, 
                        strlen(flowDescStr));
        LOG_EXIT;
        return FAILURE;
    }

    // if (match.size() < 5)
    if(pmatch[5].rm_so <= 0 && pmatch[5].rm_eo <= 0)
    {
        higLog("Mandatory parameters missing in flow value");
        LOG_EXIT;
        return FAILURE;
    }

    /*action */
    std::string action = flowDesc.substr(pmatch[1].rm_so, pmatch[1].rm_eo-pmatch[1].rm_so); //match[1].str();
    if (action == "permit") {
        ipFilterRule.action = PERMIT;
    } else if (action == "deny") {
        ipFilterRule.action = DENY;
    } else {
            higLog("Incorrect value for action in flow desc %s", action.c_str());
            LOG_EXIT;
            return FAILURE;
    }

    /* direction */
    std::string dir = flowDesc.substr(pmatch[2].rm_so, pmatch[2].rm_eo-pmatch[2].rm_so); //match[2].str();
    if (dir == "in") {
        ipFilterRule.dir = IN;
    } else if (dir == "out") {
        ipFilterRule.dir = OUT;
    } else {
            higLog("Incorrect value for direction in flow desc %s", dir.c_str());
            LOG_EXIT;
            return FAILURE;
    }

    /* protocol */

    std::string protocol = flowDesc.substr(pmatch[3].rm_so, pmatch[3].rm_eo-pmatch[3].rm_so); //match[3].str();
    if (protocol == "ip") {
            ipFilterRule.proto = IP;
    } else {
        size_t nmatchInteger = MAX_NUM_MATCHES;
        regmatch_t pmatchInteger[MAX_NUM_MATCHES+1];
        int protocolLen = protocol.length();
        char protocolStr[MAX_PATTERN_STRING_LENGTH];
        strcpy(protocolStr, protocol.c_str());
        itr = regexToDfaMap.find(INTEGER_REGEXP);
        if(itr == regexToDfaMap.end())
        {
            higLog("Regex not present");
            LOG_EXIT;
            return FAILURE;
        }
        regex_t integerPReg = itr->second;

        if (regexMatchFull(&integerPReg, protocolStr, protocolLen, 
                            nmatchInteger, pmatchInteger, eflags) != SUCCESS)
        {
            higLog("Not a valid protocol value %s", protocol.c_str());
            LOG_EXIT;
            return FAILURE;
        }
        
        // if (!(std::regex_match(protocol, integerRegex))){
        //         higLog("Not a valid protocol value %s", protocol.c_str());
        //         LOG_EXIT;
        //         return FAILURE;
        //     }
        
        /*if we use stoi without number check UPF will abort */
        ipFilterRule.proto = (e_IPFilterRuleProtocol)std::stoi(protocol);
    }
    /* src Node */
    decodeIPFilterRuleNode(ipFilterRule.srcNode,
            flowDesc.substr(pmatch[4].rm_so, pmatch[4].rm_eo-pmatch[4].rm_so)/*match[4].str()*/);
    decodeIPFilterRuleNode(ipFilterRule.destNode,
            flowDesc.substr(pmatch[5].rm_so, pmatch[5].rm_eo-pmatch[5].rm_so)/*match[5].str()*/);

    return SUCCESS;
}


