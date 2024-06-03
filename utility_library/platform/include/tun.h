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

#ifndef  __TUN_H
#define  __TUN_H

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <net/route.h>
#include "platform/include/logging.h"
#include "common/include/datatypes.h"
#include "platform/include/platform.h"

typedef struct routing_structure{
	std::string dst_ip="0.0.0.0";
	std::string genmask="255.255.255.255";
	std::string gateway="0.0.0.0";
	std::string interface="";
}routing_structure_t;



int tun_alloc(std::string dev, int flags);

int tunRead(int tunFd, char *buf, int bufLen);

int tunWrite(int tunFd, char *buf, int bufLen);

int assignIp(std::string interface,std::string ip);

int get_ip_from_interface(std::string interface,std::string &ip);

int ifConfig(bool state,std::string interface);

int setIpFrwdRule(std::string interface);

int setRoute(bool add_rule,routing_structure_t rule);
inline int addRoute(routing_structure_t rule) {
    return setRoute(true, rule);
}
inline int delRoute(routing_structure_t rule) {
    return setRoute(false, rule);
}
void printRoutingStruct(routing_structure_t rule);

int system_command(std::string command,std::string &outBuffer);
#endif  

