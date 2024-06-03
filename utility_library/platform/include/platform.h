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

/*
 * This file contains datastructures pertaining to platform
 *
 * Platform acts a software interface which calls socket calls
*/

#ifndef __PLATFORM_H
#define __PLATFORM_H

#include "common/include/datatypes.h"
#include "common/include/setup.h"
#include <map>
#include "common/include/setup.h"
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <queue>
#include <sys/eventfd.h>

/* Default duration & retries are set to sane values, change them in the
 * derived class's constructor */
#define TIMER_DEFAULT_DURATION (6)
#define TIMER_DEFAULT_RETRIES  (4)


#define NF_UPF_DUMMY_FD 64246


using namespace std;

class timer{
private:
	int fd;
	timer(const class timer& t) = delete;
	class timer & operator=(const timer &t) = delete;
public:
	int duration;
	int retries;
	static int epollFd;
	static map <int, class timer*> fdToObjectMap;

	timer();
	~timer();
	virtual void timeOutFunction();
	void startTimer(int duration,int retries);
	void startTimer();
	void stopTimer();
	int getFd();
};



/*
 * Better not to keep this global. Encapsulate in platform.
 * Thay way only platform will have write access to fdList.
 *
 * Each process will form channels with all nfv comp instances.
 * Likewise gFdList has E_MAX_NFV_COMPONENTS channel fds.
*/
/*
 * Similarly ipAddr,compId,CB[] should be part of object.
 * Thiss object should be encapsulated at platform layer.
*/
typedef void (*cbs)(void * msg, int len, uint Id, uint16_t streamId);
#define MAX_CONNECTIONS     10
#define MAX_EPOLL_EVENTS	E_MAX_NFV_COMPONENTS*MAX_CONNECTIONS
#define MAX_MESSAGE_SIZE	3000
#define LISTEN_QUEUE_BACKLOG	10



#define NON_UE_MESSAGE_STREAM   0
#define UE_MESSAGE_STREAM       1

typedef enum executionStates {
	E_NFV_INVALID_STATE = -1,
	E_NFV_STARTED = 0,
	E_NFV_STOPPED
} _e_nfv_exec_state;

typedef enum socketTypes {
	SELF_CONNECT = -1,
	PFCP_SERVER_UDP_SOCKET = 0,
    PFCP_CLIENT_UDP_SOCKET,
    SCTP_SERVER_ACCEPT_SOCKET,
    SCTP_SERVER_DATA_SOCKET,
	SCTP_CLIENT_SOCKET,
    EVENTFD_SOCKET,
	NUM_SOCKET_TYPES	
} _e_socketType;

typedef struct fdData {
    _e_socketType type;
    _e_nfv_component channel;
    uint port;
    uint id;
    int fd;
    uint32_t ipAddr; 

    // timer callback
    void (*timerCB) (/*...*/);
} fdData_t;

struct nfvInstanceData {
	_e_nfv_component	compId;
    uint32_t            bindAddr; 

	_e_nfv_exec_state	state;
	cbs	CB[E_MAX_NFV_COMPONENTS];
    int (*acceptCB) (uint id);
    void (*eventCB) ();
    void (*dpdkEventCB) (void *, int, struct sockaddr_in *);
    unsigned int hash;    

    std::map<uint, fdData_t> fdmap; /* a map of all fds thru which data
                                 can be sent or received. Not for listen FDs */

    /* TODO: locking so that multiple ppl read the map, a few edit it.
     * std::shared_mutex fdmap_mutex;
     */
    int epollFd;

    int eventFd;
};

/*
 * This function initializes platform objects. It also makes
 * function calls which open listening socket for each channel.
 * The number of channels depend on setup sepcified.
 *
 * Arg IN: selfNfvCompId stands for comp id of process which
 *	   is making use of Platform services. The process
 *	   which is running as nfv.
 *
 * Return: nfvInstanceData pointer.
*/
struct nfvInstanceData* initPlatform(_e_nfv_component selfNfvCompId,
                                     uint32_t ipAddr);

int updateGPortMap(_e_nfv_component selfNfvCompId, uint32_t ipAddr);

/* a blocked call. Will poll on sockets and run corresponding callbacks
 */
void pollOnEvents(struct nfvInstanceData *nfvInst);
int platformSendData(int fd, char *msg, int len, struct sockaddr *sin);
int platformSCTPSendData(int fd, char *msg, int len, struct sockaddr *sin,
                         uint16_t streamId);
int platformRecvFromPeer(uint id, char *msg);
int platformRecvData(int activeFD, char *msg);
int platformRead(int activeFD, char *msg);
int platformSendToPeer(uint ranId, char *msg, int len,
						struct sockaddr *sin, uint16_t streamId);



void eventFdRing();
void eventFdRing(void*);
void closePlatform();


int getPfcpClientId(struct nfvInstanceData *nfvInst, uint *pfcpClientId);

int platformClrPeerTkn(uint16_t peerToken);



void recvDpdkUPFCallbacks(struct nfvInstanceData *nfvInst, uint8_t *msg,
                                int msgLen, struct sockaddr_in *sin);



int connectToIpfromGPortMap(_e_nfv_component compId,uint32_t ipAddr);
int reportCBrkToGPortMap(_e_nfv_component compId,uint32_t ipAddr);
int getFDfromGPortMap(_e_nfv_component compId,uint32_t ipAddr);



struct timerInfo
{
    unsigned long Id;
    int duration;
    int retries;
    void (*timeoutFunction)(struct timerInfo);
    timerInfo(){
        Id=0;
        retries=0;
    }
};
unsigned long timeSpecTomSec(struct timespec now);
struct itimerspec mSecToitimeSpec(unsigned long mSec);
int setupTimerFd();
int startTimer(struct timerInfo tInfo);
int stopTimer(unsigned long  Id);
int updateTimer();
void timeOutFunction();
int checkTimer(unsigned long  Id);
#endif 
