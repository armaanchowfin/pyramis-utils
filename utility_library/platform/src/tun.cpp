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

#include "platform/include/tun.h"

int tun_alloc(std::string dev, int flags) {
    LOG_ENTRY;
    int fd, err=FAILURE;

    if (dev.length()==0) {
        higLog("Tun Name Not Provided");
        LOG_EXIT;
        return err;
    }

    struct ifreq ifr;
    const char *clonedev = "/dev/net/tun";
    if( (fd = open(clonedev, O_RDWR)) < 0 ) {
        higLog("Error open() failed. %s", strerror(errno));
        LOG_EXIT;
        return fd;
    }
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name,dev.c_str());
    ifr.ifr_flags = flags;
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
        higLog("ioctl failed : %s",strerror(errno));
        close(fd);
        LOG_EXIT;
        return err;
    }
    LOG_EXIT;
    return fd;
}

int tunRead(int tunFd, char *buf, int bufLen) {
    LOG_ENTRY;
    int rlen;
    rlen = read(tunFd, buf, bufLen);
    if (rlen < 0) {
        higLog("tunRead has failed. %s ", strerror(errno));
        LOG_EXIT;
        return FAILURE;
    }
    lowLog("read %d bytes from tun", rlen);
    LOG_EXIT;
    return rlen;
}

int tunWrite(int tunFd, char *buf, int bufLen) {
    LOG_ENTRY;
    int wlen;
    wlen = write(tunFd, buf, bufLen);
    if (wlen < 0) {
        higLog("tunWrite has failed. %s ", strerror(errno));
        LOG_EXIT;
        return FAILURE;
    }
    lowLog("wrote %d bytes to tun", wlen);
    LOG_EXIT;
    return wlen;
}

int setRoute(bool add_rule,routing_structure_t rule)
{
    LOG_ENTRY;
    struct rtentry route;
    memset(&route,0,sizeof(rtentry));
    struct sockaddr_in *temp_addr;
    
    /*Mandatory Fields*/
    //Check if destination ip is set 
    if(rule.dst_ip.compare("0.0.0.0")==0)
    {
        higLog("Destination ip is not set");
        errno = EINVAL;
        LOG_EXIT;
        return FAILURE;
    }
    if(rule.interface.length()==0)
    {
        higLog("Interface not set");
        errno = EINVAL;
        LOG_EXIT;
        return FAILURE; 
    }
    temp_addr = (struct sockaddr_in*) &route.rt_dst;
    temp_addr->sin_family = AF_INET;
    temp_addr->sin_addr.s_addr = inet_addr(rule.dst_ip.c_str());

    route.rt_dev=(char*)rule.interface.c_str();

    /*Optional Fields*/
    //If genmask not provided, default is 255.255.255.255
    temp_addr = (struct sockaddr_in*) &route.rt_genmask;
    temp_addr->sin_family = AF_INET;
    temp_addr->sin_addr.s_addr = inet_addr(rule.genmask.c_str());
    
    //If genmask not provided, default is 0.0.0.0
    temp_addr = (struct sockaddr_in*) &route.rt_gateway;
    temp_addr->sin_family = AF_INET;
    temp_addr->sin_addr.s_addr = inet_addr(rule.gateway.c_str());
    
    //Flags
    route.rt_flags = RTF_UP;
    if(rule.genmask.compare("255.255.255.255")==0){
        route.rt_flags |=RTF_HOST;
    }
    //Currently relying on User to give proper destination ip and genmask
    if (rule.gateway.compare("0.0.0.0")!=0){
        route.rt_flags |=RTF_GATEWAY;   
    }
  
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    //Add Route
    if(add_rule)
    {
        if (ioctl(sockfd,SIOCADDRT,&route)){
            higLog("Could not add new IP route to routing table : %s ",strerror(errno));
            if(errno != EEXIST)
            {
                close(sockfd);
                LOG_EXIT;
                return FAILURE;
            }
        }
    }
    //Remove Route
    else
    {
        if (ioctl(sockfd,SIOCDELRT,&route)){
            higLog("Could not remove requested IP route from routing table : %s ",strerror(errno));
            if(errno != ESRCH)
            {
                close(sockfd);
                LOG_EXIT;
                return FAILURE;
            }
        }
    }
    close(sockfd);
    LOG_EXIT;
    return SUCCESS;
}

int setIpFrwdRule(std::string interface)
{
    LOG_ENTRY;
    std::string rp_filter_path="/proc/sys/net/ipv4/conf/"+interface+"/rp_filter";
    if(std::ifstream(rp_filter_path)){
        if(!static_cast<bool>(std::ofstream(rp_filter_path).put('0'))){
            higLog("Failed to Write in rp_filter of provided Interface: run with sudo");
            LOG_EXIT;
            return FAILURE;
        }
    }
    else{
        higLog("No rp_filter file available for provided interface");     
        LOG_EXIT;
        return FAILURE;
    }
    if(!static_cast<bool>(std::ofstream("/proc/sys/net/ipv4/conf/all/rp_filter").put('0'))){
        higLog("Failed to Write in rp_filter for all: run with sudo");
        LOG_EXIT;
        return FAILURE;
    }
    if(!static_cast<bool>(std::ofstream("/proc/sys/net/ipv4/ip_forward").put('1'))){
        higLog("Failed to enable ip forwarding: run with sudo");
        LOG_EXIT;
        return FAILURE;
    }
    LOG_EXIT;
    return SUCCESS; 
}

int ifConfig(bool state,std::string interface)
{
    LOG_ENTRY;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        higLog("Failed to create dummy socket fd : %s ",strerror(errno));
        LOG_EXIT;
        return FAILURE;
    }
    struct ifreq ifr;
    memset(&ifr,0,sizeof(ifr));

    strcpy(ifr.ifr_name,interface.c_str());
    /*Add these flags in case of failure in TUN Communication:
    IFF_UP | IFF_POINTOPOINT | IFF_RUNNING | IFF_NOARP | IFF_MULTICAST;*/
    if(state)
        ifr.ifr_flags |= IFF_UP;
    else
        ifr.ifr_flags |=  ~IFF_UP;
    if(ioctl(sockfd, SIOCSIFFLAGS, &ifr)){
        higLog("Failed to change state of provided Interface : %s ",strerror(errno));
        close(sockfd);
        LOG_EXIT;
        return FAILURE; 
    }
    close(sockfd); 
    LOG_EXIT; 
    return SUCCESS;
}


int get_ip_from_interface(std::string interface,std::string &ip)
{
    LOG_ENTRY;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        higLog("Failed to create dummy socket fd : %s ",strerror(errno));
        LOG_EXIT;
        return FAILURE;
    }
    struct ifreq ifr;
    memset(&ifr,0,sizeof(ifr));

    strcpy(ifr.ifr_name,interface.c_str());
    if(ioctl(sockfd,SIOCGIFADDR,&ifr)==FAILURE){
        higLog("IOCTL Call Failed: %s ",strerror(errno));
        close(sockfd);
        LOG_EXIT;
        return FAILURE;
    }
    ip=std::string(inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
    close(sockfd);
    LOG_EXIT;
    return SUCCESS;
}

int assignIp(std::string interface,std::string ip)
{
    LOG_ENTRY;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        higLog("Failed to create dummy socket fd : %s ",strerror(errno));
        LOG_EXIT;
        return FAILURE;
    }
    struct sockaddr_in *ip_addr; 
    struct ifreq ifr;
    memset(&ifr,0,sizeof(ifr));
    strcpy(ifr.ifr_name,interface.c_str());
    ifr.ifr_addr.sa_family = AF_INET;
    ip_addr=(struct sockaddr_in *)&ifr.ifr_addr;
    inet_pton(AF_INET,ip.c_str(),&ip_addr->sin_addr);
    if(ioctl(sockfd,SIOCSIFADDR,&ifr)==FAILURE){
        higLog("IOCTL Call Failed: %s ",strerror(errno));
        close(sockfd);
        LOG_EXIT;
        return FAILURE;
    }
    close(sockfd);
    LOG_EXIT;
    return SUCCESS;
}

void printRoutingStruct(routing_structure_t rs)
{
    LOG_ENTRY;

    lowLog("dst_ip %s", rs.dst_ip.c_str());
    lowLog("genmask %s", rs.genmask.c_str());
    lowLog("gateway %s", rs.gateway.c_str());
    lowLog("interface %s", rs.interface.c_str());

    LOG_EXIT;
    return;
}

#if 0 
/* uncomment if we need to use it */
int system_command(std::string command,std::string &outBuffer)
{
    //To-Do:Figure out way to handle error in system command here only
    //Tried with feof(fp) and ferror(fp) : No Difference
    LOG_ENTRY;
    FILE* fp;
    char buffer[MAX_MESSAGE_SIZE]={};
    fp = popen(command.c_str(),"r");
    if(fp == NULL) {
        higLog("popen Call Failed: %s ",strerror(errno));
    }
    int len=fread(buffer,1,sizeof(buffer),fp);  /* TODO add error check */
    outBuffer=std::string(buffer);
    fclose (fp);
    LOG_EXIT;
    return len;
    
}
#endif
