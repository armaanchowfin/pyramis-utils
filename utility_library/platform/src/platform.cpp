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

#include "platform/include/platform.h"
#include "platform/include/logging.h"

#ifdef THREADPOOL
#include "platform/include/threadpool.h"
#endif
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <netinet/sctp.h>

#ifdef THREADPOOL
std::vector<transmittal_t > jobQueue = {};
pthread_cond_t              jobAvailable = PTHREAD_COND_INITIALIZER;
pthread_mutex_t             jobMutex     = PTHREAD_MUTEX_INITIALIZER;
#endif

using namespace std;
map<int,string> compId_to_nfvName;
struct nfvInstanceData *gNfvInst = NULL;	/* a global var, to access FDs*/

int timer::epollFd;
map <int, class timer*>timer::fdToObjectMap;

int timerfd=-1;
unsigned long timeoutTime = ULONG_MAX;
static map<struct timespec,struct timerInfo> expiryToTimerMap;
/*Maintained for ease of access while stopping the timer*/
static map<unsigned long, struct timespec> idExpiryMap;
std::mutex timerMutex;
#define TIMER_GRANULARITY       10
#define NSEC_PER_MSEC 1.0e6
#define MS_PER_SEC 1000
#define ITIMESPEC_TO_MSEC(x) (((x.tv_sec) * (MS_PER_SEC)) + (round(((x.tv_nsec)/(NSEC_PER_MSEC)))))
#define MSEC_TO_SEC(x) ((x)/MS_PER_SEC)
#define MSEC_TO_NSEC(x) ((x) * (NSEC_PER_MSEC))
#define SEC_TO_MSEC(x) ((x) * (MS_PER_SEC))
#define NSEC_TO_MSEC(x) ((x)/NSEC_PER_MSEC)
/* Payload Protocol ID */
#define SCTP_PPID_NGAP          1006632960L      /* htonl(60) */
#define SCTP_PPID_S1AP          301989888L       /* htons(18) */
uint8_t ppid_warn_rate = 0;   /* will print a warning every 20th time */
/* Key of expiryMap is timespec, since it is a struct needs a function
 * so that while adding to timer , it is sorted */
bool operator<(const timespec& t1, const timespec& t2)
{
    if(t1.tv_sec < t2.tv_sec)
        return true;
    else if(t1.tv_sec == t2.tv_sec)
        if(t1.tv_nsec<t2.tv_nsec)
            return true;
    return false;
}
/* port map provides port number, peer, socket type.
 * The first index denotes source-server nfv.
 * The second index in conjunction with first index
 * makes a channel. The resulting structure defines peer nfv.
 *
 * Not exposing this array to nfv processes. Hence kept
 * part of cpp file instead of platform.h. We can move
 * to platform_internal.h
 *
*/
/* The SCTP Destination Port number value assigned by IANA to be used for
 * NGAP is 38412 . TODO move this struct to config file */

std::vector<std::vector<fdData_t> > gPortMap = {
    //RAN
    {
        /* RAN is SCTP client. Accordingly it 'connects' to AMF and should
         * have capability to 'connect' multiple AMFs */
        /* Accordingly there should be each row representing one AMF and
         * ipAddr of that AMF should be populated in place of INADDR_NONE */
        {SCTP_CLIENT_SOCKET, E_AMF_INST, 38412, 0, 0, INADDR_NONE}
    },

    //AMF
    {
        /* AMF is SCTP server. Accordingly there is no need for AMF to call
         * 'connect'. Hence ipAddr placeholder (INADDR_NONE) is not used */
        {SCTP_SERVER_ACCEPT_SOCKET, E_LOAD_GENERATOR_INST, 38412, 0, 0, INADDR_NONE}
    },

    //SMF
    {
        /* PFCP is over UDP. Hence there is no notion of 'connect' call.
         * Accordingly ipAddr placeholder (INADDR_NONE) is not used */
        /* Multiple UPFs can talk to 'this' SMF over UDP without having
         * to set up connection. Thus there is no need to runtime update
         * gPortMap */
        {PFCP_SERVER_UDP_SOCKET, E_UPF_INST, 8805, 0, 0, INADDR_NONE},
        {PFCP_CLIENT_UDP_SOCKET, E_UPF_INST, 0, 0, 0, INADDR_NONE}
    },

    //UPF
    {
        {PFCP_SERVER_UDP_SOCKET, E_SMF_INST, 8805, 0, 0, INADDR_NONE},
        {PFCP_CLIENT_UDP_SOCKET, E_SMF_INST, 0, 0, 0, INADDR_NONE}
    },
    //DPDK_UPF

    {
        {PFCP_SERVER_UDP_SOCKET, E_SMF_INST, 8805, 0, 0, INADDR_NONE},
        {PFCP_CLIENT_UDP_SOCKET, E_SMF_INST, 0, 0, 0, INADDR_NONE}
    }

};

/* This function should be called before initPlatform() */
int updateGPortMap(_e_nfv_component compId, uint32_t ipAddr)
{
    if(!gPortMap[compId].size()) {
        higLog("No matching fdd for compId %d", compId);
        return FAILURE;
    }

    fdData_t fdd = gPortMap[compId][0];

    switch(compId) {

    case E_LOAD_GENERATOR_INST: {
        if((fdd.type != SCTP_CLIENT_SOCKET)
            || (fdd.channel != E_AMF_INST)) {
            higLog("Incorrect parameters for RAN");
            return FAILURE;
        }

        if(fdd.ipAddr == INADDR_NONE) {
            gPortMap[compId][0].ipAddr = ipAddr;
        } else {
            fdd.ipAddr = ipAddr;
            gPortMap[compId].push_back(fdd);
        }
    }
    break;

    case E_AMF_INST: {
        if((fdd.type != SCTP_SERVER_ACCEPT_SOCKET)
            || (fdd.channel != E_LOAD_GENERATOR_INST)) {
            higLog("Incorrect parameters for AMF");
            return FAILURE;
        }
        if(gPortMap[compId].size() > 1) {
            higLog("AMF single process accepts connections on one socket only");
            return FAILURE;
        }
        gPortMap[compId][0].ipAddr = ipAddr;
    }
    break;

    default: {
        higLog("Use of platform for multiple component instances "
            "applicable only for TEST_RAN");
        return FAILURE;
    }

    }
    return SUCCESS;
}

/* Each time a PFCP request comes, we share a token which can
be used to retrieve the sockAddr from the map.
Once we sent out the response, we release the token
and delete the map entry */
map <uint16_t, struct sockaddr_in> pfcpPeerMap;
std::mutex pfcpPeerMapMutex;
queue <uint16_t> pfcpPeerTknFreeQ;

#define MAX_MESSAGE_SIZE	3000
#define MAX_PEER_ID_SUPPORTED 100

/*Clear the peer token assigned for PFCP requests in case of any failure*/
int platformClrPeerTkn(uint16_t peerToken){

    LOG_ENTRY;
    std::lock_guard<std::mutex> guard(pfcpPeerMapMutex);
    pfcpPeerTknFreeQ.push(peerToken);
    std::map<uint16_t, struct sockaddr_in>::iterator peerIter =
                                    pfcpPeerMap.find(peerToken);
    if(peerIter == pfcpPeerMap.end()) {
        higLog("ERROR: Peer Id %u missing from the map", peerToken);
        return FAILURE;
    }
    pfcpPeerMap.erase(peerIter);
    LOG_EXIT;
    return SUCCESS;
}


/* function to set NONBLOCK flag on socket
 * check it's output if it is a must have flag.
 * 0 for success, -1 for error
 */
int setNonBlock(int fd)
{
	int flags;
	if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
		higLog("fcntl get flags failed on fd %d, Error %s", fd,
				strerror(errno));
		return -1;
	}

	if(fcntl(fd, F_SETFL, flags|O_NONBLOCK) == -1) {
		higLog("fcntl set flags failed on fd %d, Error %s", fd,
				strerror(errno));
		return -1;
	}
	return 0;

}

void eventFdRing()
{
    uint64_t data = 1;
    int ret = write(gNfvInst->eventFd, &data, sizeof(data));
    if(ret == -1)
        higLog("write to Event FD failed, %s", strerror(errno));
    else
        midLog("Event FD: tring tring");
}

void closePlatform() {
    gNfvInst->state = E_NFV_STOPPED;
    eventFdRing();
}


/* Function to open and bind sockets.
 * only Non-Blocking sockets
 * returns -1 on failure, fd on success */
int openSocketAndInit(struct fdData &fdd, uint32_t bindAddr)
{
	int fd = 0;
	struct sockaddr_in myaddr = {};
    uint port = fdd.port;
	/*no connection necessary to itself*/
	if (fdd.type == SELF_CONNECT) {
		return 0;

	/*A UDP connection*/
	} else if (fdd.type == PFCP_SERVER_UDP_SOCKET ||
               fdd.type == PFCP_CLIENT_UDP_SOCKET){
		fd = socket(AF_INET, SOCK_DGRAM|SOCK_NONBLOCK, 0);
		if (fd == -1) {
			higLog("UDP socket create failed, port %d, Error %s",
					port, strerror(errno));
			return -1;
		}

        if(port != 0){
    		/*now bind to the port*/
    		myaddr.sin_family = AF_INET;
    		myaddr.sin_port = htons(port);
            myaddr.sin_addr.s_addr = bindAddr;
    		if (bind(fd, (struct sockaddr*) &myaddr, sizeof(myaddr))== -1){
    			higLog("UDP bind failed, port %d, Error %s",
    					port, strerror(errno));
    			return -1;
    		}
        }

        if(gNfvInst->compId == E_DPDK_UPF_INST)
        {
            fdd.fd = NF_UPF_DUMMY_FD;
            fdd.id = fdd.fd ^ gNfvInst->hash;
        }
        else
        {
		    fdd.fd = fd;
            fdd.id = fd ^ gNfvInst->hash;
        }
        gNfvInst->fdmap[fdd.id] = fdd;
        lowLog("UDP port %d, creation & bind successful to fd %d id %d",
                port, fdd.fd, fdd.id);

        if(fdd.type == PFCP_SERVER_UDP_SOCKET){
            for (int i = 1; i <= MAX_PEER_ID_SUPPORTED ; i++) {
                                pfcpPeerTknFreeQ.push(i);
            }
        }

	/*A SCTP client socket*/
	} else if (fdd.type == SCTP_CLIENT_SOCKET) {
		fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
		if (fd == -1) {
			higLog("SCTP CLIENT create failed, port %d, Error %s",
					port, strerror(errno));
			return -1;
		}

        /* set 2 instreams & 2 outstreams *//* TODO: read this from a config */
        struct sctp_initmsg initmsg = {};
        initmsg.sinit_num_ostreams = 2;
        initmsg.sinit_max_instreams = 100; /* ATMOST 100 are allowed */
        lowLog("Setting %d instreams and atmost %d outstreams",
                initmsg.sinit_num_ostreams, initmsg.sinit_max_instreams);
        if (setsockopt(fd, SOL_SCTP, SCTP_INITMSG,
                        &initmsg, sizeof(initmsg)) == -1) {
            higLog("setsockopt failed, Error: %s", strerror(errno));
            return -1;
        }

		/*now connect to the port*/
		myaddr.sin_family = AF_INET;
		myaddr.sin_port = htons(port);
		myaddr.sin_addr.s_addr = fdd.ipAddr;

		/*allow connect some time, add SOCK_NONBLOCK after connect*/
		if (connect(fd, (struct sockaddr*)&myaddr,sizeof(myaddr))== -1){
			higLog("SCTP CLIENT connect failed, port %d, Error %s",
					port, strerror(errno));
			return -1;
		}
		//  setNonBlock(fd);

        /* set it so we get stream id on rcving a message */
        struct sctp_event_subscribe events = {};
        events.sctp_data_io_event = 1;
        if(setsockopt(fd, SOL_SCTP, SCTP_EVENTS, (const void *)&events,
                         sizeof(events)) == -1) {
            higLog("setsockopt failed, Error: %s", strerror(errno));
            return -1;
        }

		fdd.fd = fd;
        fdd.id = fd ^ gNfvInst->hash;
        gNfvInst->fdmap[fdd.id] = fdd;
		lowLog("SCTP CLIENT port %d, creation & connect successful fd %d id %d",
				port, fd, fdd.id);

	/* A SCTP server socket*/
	} else if (fdd.type == SCTP_SERVER_ACCEPT_SOCKET) {
		fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
		if (fd == -1) {
			higLog("SCTP SERVER create failed, port %d, Error %s",
					port, strerror(errno));
			return -1;
		}
		myaddr.sin_family = AF_INET;
		myaddr.sin_port = htons(port);
		myaddr.sin_addr.s_addr = bindAddr;

        /* set 2 instreams & 2 outstreams *//* TODO: read this from a config */
        struct sctp_initmsg initmsg = {};
        initmsg.sinit_num_ostreams = 2;
        initmsg.sinit_max_instreams = 100; /* ATMOST 100 are allowed */
        lowLog("Setting %d instreams and atmost %d outstreams",
                initmsg.sinit_num_ostreams, initmsg.sinit_max_instreams);
        if (setsockopt(fd, SOL_SCTP, SCTP_INITMSG,
                        &initmsg, sizeof(initmsg)) == -1) {
            higLog("setsockopt failed, Error: %s", strerror(errno));
            return -1;
        }

        /* set it so we get stream id on rcving a message */
        struct sctp_event_subscribe events = {};
        events.sctp_data_io_event = 1;
        if(setsockopt(fd, SOL_SCTP, SCTP_EVENTS, (const void *)&events,
                         sizeof(events)) == -1) {
            higLog("setsockopt failed, Error: %s", strerror(errno));
            return -1;
        }

		int tr = 1;
		if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
			higLog("Port reuse failed, port %d, Error %s",
					port, strerror(errno));
    		return -1;
		}

		/*now bind to the port*/
		if (bind(fd, (struct sockaddr*) &myaddr, sizeof(myaddr))== -1){
			higLog("SCTP SERVER bind failed, port %d, Error %s",
					port, strerror(errno));
			return -1;
		}

		if(listen(fd, LISTEN_QUEUE_BACKLOG) == -1) {
			higLog("SCTP SERVER listen failed, port %d, Error %s",
					port, strerror(errno));
			close(fd);
			return -1;
		}
		fdd.fd = fd;
        fdd.id = fd ^ gNfvInst->hash;
        gNfvInst->fdmap[fdd.id] = fdd;
		lowLog("SCTP SERVER port %d, creation successful fd %d id %d",
				port, fd, fdd.id);/* LNTC */
	} else {
		higLog("Unknown socket type %d", fdd.type);
		return -1;
	}
	return fd;
}

/*
 * This function opens socket on nfv-server for each channel.
 * The socket descriptor is then stored in fdList.
 *
 * Return: SUCCESS/FAILURE
*/
int openChannels(struct nfvInstanceData* nfvInst)
{
	compId_to_nfvName[0] = "AMF";
	compId_to_nfvName[1] = "RAN";

    for(uint nfv=0; nfv<(gPortMap[nfvInst->compId].size()); nfv++) {

        struct fdData fdd;
        fdd = gPortMap[nfvInst->compId][nfv];
        if(openSocketAndInit(fdd, nfvInst->bindAddr) < 0)
            higLog("ERROR in open and init socket, channel %d, type %d",
                    fdd.channel, fdd.type);
    }

	return SUCCESS;
}

/*
 * This function initializes platform objects. It also makes
 * function calls which open listening socket for each channel.
 * The number of channels depend on setup sepcified.
 *
 * Arg IN: selfNfvCompId stands for comp id of process which
 *	   is making use of Platform services. The process
 *	   which is running as nfv.
 *	   ipAddr: IPv4 Address in network byte order. For now UDP sockets will
 *	   be bound to this address. Set it to INADDR_ANY to accept packets to all
 *	   src addresses.
 *
 * Return: nfvInstanceData pointer.
*/
struct nfvInstanceData* initPlatform(_e_nfv_component selfNfvCompId,
                                     uint32_t ipAddr)
{
	struct nfvInstanceData *nfvInst;

	nfvInst = new struct nfvInstanceData;
	nfvInst->compId = selfNfvCompId;
	nfvInst->state = E_NFV_INVALID_STATE;
    srandom((int)getpid());
    nfvInst->hash = random() % UINT32_MAX;
	nfvInst->fdmap = {};
    nfvInst->epollFd = -1;
    nfvInst->acceptCB = NULL;
    nfvInst->eventCB =  NULL;
    nfvInst->dpdkEventCB = NULL;
    nfvInst->bindAddr = ipAddr;
    memset(nfvInst->CB, 0, sizeof(nfvInst->CB));

	gNfvInst = nfvInst;

	openChannels(nfvInst);

    int evfd = eventfd(0, EFD_NONBLOCK);
    if(evfd == -1) {
        higLog("eventfd creation failed, %s", strerror(errno));
    }
    nfvInst->eventFd = evfd;

	return nfvInst;
};

/* Retrieve PFCP Client Id from Fd map */
int getPfcpClientId(struct nfvInstanceData *nfvInst, uint *pfcpClientId){

    for(std::map<uint, fdData_t>::iterator fddIter = nfvInst->fdmap.begin();
                                fddIter != nfvInst->fdmap.end(); fddIter++) {
        fdData_t fdd = fddIter->second;
        /* Find the first PFCP Client Id and return */
        if(fdd.type == PFCP_CLIENT_UDP_SOCKET){
            *pfcpClientId = fdd.id;
            return SUCCESS;
        }
    }
    return FAILURE;
}

int platformRecvFromPeer(uint id, char *msg)
{
	int rc = 0;
    std::map<uint, fdData_t>::iterator fddIter = gNfvInst->fdmap.find(id);
    if(fddIter == gNfvInst->fdmap.end()) {
        higLog("ERROR: id %u missing from the map", id);
        return -1;
    }
	switch(fddIter->second.type) {
    case SCTP_CLIENT_SOCKET:
        rc = platformRecvData(fddIter->second.fd, msg);
		break;
	default:
		higLog("Unsupported socket type or incorrect id or corrupted fdData"
                    " id %u, socket type %d",
					id, fddIter->second.type);
	}

	if(rc == -1) {
		lowLog("platformRecvData failed, Error: %s", strerror(errno));
		return rc;
	} else
		return rc;

}

/* peer, msg and len are compulsory.
 * sin and streamId are optional.
 * set sin to NULL if sender addr is not important
 * streamId will be ignored for all NON-SCTP connections
 */
int platformSendToPeer(uint id, char *msg, int len,
						struct sockaddr *sin, uint16_t id2)
{
	int rc = 0;
    std::map<uint, fdData_t>::iterator fddIter = gNfvInst->fdmap.find(id);
    if(fddIter == gNfvInst->fdmap.end()) {
        higLog("ERROR: id %u missing from the map", id);
        return -1;
    }
    uint16_t streamId = id2;
	switch(fddIter->second.type) {
    case PFCP_CLIENT_UDP_SOCKET:
	case PFCP_SERVER_UDP_SOCKET:{
        struct sockaddr_in sin2;
        uint16_t peerToken = id2;
        if(sin == NULL){
            std::lock_guard<std::mutex> guard(pfcpPeerMapMutex);
            std::map<uint16_t, struct sockaddr_in>::iterator peerIter =
                                            pfcpPeerMap.find(peerToken);
            if(peerIter == pfcpPeerMap.end()) {
                higLog("ERROR: Peer Id %u missing from the map", peerToken);
                return -1;
            }
            sin2 = peerIter->second;
            sin = (sockaddr*)&sin2;
            pfcpPeerMap.erase(peerIter);
            pfcpPeerTknFreeQ.push(peerToken);

        }
        if(gNfvInst->compId == E_DPDK_UPF_INST) // [DIP] separating functionality for DPDK UPF
        {
            midLog("Time to send PFCP message");
            gNfvInst->dpdkEventCB((void *)msg, len, (sockaddr_in *)sin); // TODO: check correctness [DIP]
        }
        else
        {
		    rc = sendto(fddIter->second.fd, msg, len, 0, sin,
					    sizeof(struct sockaddr_in));
            if(rc == -1) {
                higLog("Error in sendto");
            }
            else {
                midLog("Sent %d bytes", rc);
            }
        }
    }
	break;
    case SCTP_CLIENT_SOCKET:
        rc = sctp_sendmsg(fddIter->second.fd, msg, len, sin,
                          sin ? sizeof(struct sockaddr_in) : 0,
                          SCTP_PPID_NGAP, 0, streamId, 0, 0);
		break;
    case SCTP_SERVER_DATA_SOCKET:
        rc = sctp_sendmsg(fddIter->second.fd, msg, len, sin,
                          sin ? sizeof(struct sockaddr_in) : 0,
                          SCTP_PPID_NGAP, 0, streamId, 0, 0);
        midLog("Sent %d bytes on stream %d", rc, streamId);
		break;
	default:
		higLog("Incorrect id or corrupted fdData struct. id %u, socket type %d",
					id, fddIter->second.type);
	}

	if(rc == -1) {
		lowLog("sendto failed, Error: %s", strerror(errno));
		return rc;
	} else
		return rc;

}


/* send the message, return rc of sendto
 */
int platformSendData(int fd, char *msg, int len, struct sockaddr *sin)
{
	int rc;

	rc = sendto(fd, msg, len, 0, sin,
			sin ? sizeof(struct sockaddr_in) : 0);
	if(rc == -1) {
		lowLog("sendto failed, Error: %s", strerror(errno));
		return rc;
	} else {
        midLog("Sent %d bytes", rc);
		return rc;
    }

}

int platformSCTPSendData(int fd, char *msg, int len, struct sockaddr *sin,
                         uint16_t streamId)
{
	int rc;

	rc = sctp_sendmsg(fd, msg, len, sin, sin ? sizeof(struct sockaddr_in) : 0,
                        SCTP_PPID_NGAP, 0, streamId, 0, 0);
	if(rc == -1) {
		higLog("sendto failed, Error: %s", strerror(errno));
		return rc;
	} else {
        midLog("Sent %d bytes on stream %d", rc, streamId);
		return rc;
    }

}

/* recv the message, return recvfrom's output */
int platformRecvData(int activeFD, char *msg)
{
	int rc;

	rc = recvfrom(activeFD, msg, MAX_MESSAGE_SIZE,
			0, NULL, NULL);
	if(rc < 0) {
		lowLog("recvfrom failed, Error: %s", strerror(errno));
		return rc;
	} else if(rc == 0) {
		lowLog("%s", "recvfrom returned 0 len msg");
		return rc;
	} else
		return rc;
}

/* recv the message, return recvfrom's output */
int platformUDPRecvData(int activeFD, char *msg, struct sockaddr_in *sin)
{
    int rc;
    socklen_t sockAddLen = sizeof(*sin);
    rc = recvfrom(activeFD, msg, MAX_MESSAGE_SIZE,
            0, (sockaddr*)sin, &sockAddLen);
    if(rc == -1) {
        lowLog("recvfrom failed, Error: %s", strerror(errno));
        return rc;
    } else if(rc == 0) {
        lowLog("%s", "recvfrom returned 0 len msg");
        return rc;
    } else
        return rc;
}

/* recv the message, return recvfrom's output */
int platformSCTPRecvData(int activeFD, char *msg, int *streamId)
{
	int rc;

    struct sctp_sndrcvinfo info = {};
    int flags = 0;
	rc = sctp_recvmsg(activeFD, msg, MAX_MESSAGE_SIZE, NULL, NULL, &info,
                      &flags);
	if(rc < 0) {
		lowLog("recvfrom failed, Error: %s", strerror(errno));
        if(errno == ECONNRESET) /* Non Graceful shutdown */
            return 0;
		return rc;
	} else if(rc == 0) {    /* Graceful Shutdown */
		lowLog("%s", "recvfrom returned 0 len msg");
		return rc;
    } else if(flags & MSG_NOTIFICATION) {
        /*received a notification, dont need it, NOT handling it.
         * typecast msg into sctp_notification if info is needed */
        return -1;
    /* flags & MSG_EOR will be set if msg was of sufficient length.
     * TODO: start checking MSG_EOR flag too. For now msglen 3000 is suffi */
	} else {
        *streamId = info.sinfo_stream;
        if(info.sinfo_ppid != SCTP_PPID_NGAP) {
            if(ppid_warn_rate++%20 == 0) {
                midLog("RATELIMITED_LOG: Warning: PPID is not 60, recv.d %d",
                        ntohl(info.sinfo_ppid));
                /* We dont need strict rate limited logging, which is why
                 * we are not using locks to protect ppid_warn_rate */
            }
        }
		return rc;
    }
}

/* accept connections and add it to epoll */
void acceptConnections(fdData_t *acceptFdd, int epollFd)
{
    int newfd;
	struct epoll_event event;

    newfd = accept4(acceptFdd->fd, NULL, NULL, SOCK_NONBLOCK);
    if(newfd == -1) {
        higLog("accept4 failed, Error: %s", strerror(errno));
        return;
    }

    if(gNfvInst->fdmap.size() == MAX_CONNECTIONS) {
        higLog("%s","fd map full");
        close(newfd);   /*refuse connection */
        return;
    }

    /* init emptyFD does not need protection, as it happens only
     * in the main thread. */
    fdData_t fdd;
    fdd.type = SCTP_SERVER_DATA_SOCKET;
    fdd.channel = acceptFdd->channel;
    fdd.fd = newfd;
    fdd.id = newfd ^ gNfvInst->hash;
    if(gNfvInst->fdmap.find(fdd.id) != gNfvInst->fdmap.end()) {
        higLog("Error while adding fd into the map");
        close(newfd);
        __attribute__((unused)) int erasedFds = gNfvInst->fdmap.erase(newfd);
        higLog("Erased %d fds", erasedFds);
        return;
    } else if(gNfvInst->acceptCB && gNfvInst->acceptCB(fdd.id) == FAILURE) {
        higLog("Error: fd accept was rejected");
        close(newfd);
        return;
    }
    gNfvInst->fdmap[fdd.id] = fdd;

    memset(&event,0, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = newfd;
    if(epoll_ctl(epollFd, EPOLL_CTL_ADD, newfd,&event)==-1) {
        higLog("epoll_ctl failed, fd id %d, Error %s", fdd.id,
                strerror(errno));
        return;
    }
    midLog("Accepted a new connection, id %d, fd %d", fdd.id, fdd.fd);/* LNTC */

    return;
}

void inline removeFdFromEpoll(int fd)
{
    int epollFd = gNfvInst->epollFd;

	if(epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL)==-1) {
		higLog("epoll_ctl DEL failed, Error %s", strerror(errno));
	}

}

void recvDpdkUPFCallbacks(struct nfvInstanceData *nfvInst, uint8_t *msg,
                            int msgLen, struct sockaddr_in *sin)
{
    LOG_ENTRY;
    uint16_t peerToken = 0;
    uint id = NF_UPF_DUMMY_FD ^ nfvInst->hash;
    fdData_t fdd = {PFCP_SERVER_UDP_SOCKET, E_SMF_INST, 8805, id, 0, INADDR_NONE}; // TODO: change this??
    int channel = fdd.channel;
    if(fdd.type == PFCP_SERVER_UDP_SOCKET){
        pfcpPeerMapMutex.lock();
        if(pfcpPeerTknFreeQ.size()==0){
            higLog("Empty pfcpPeerTknFreeQ");
            free(msg);
            LOG_EXIT;
            pfcpPeerMapMutex.unlock();
            return;
        }
        peerToken = pfcpPeerTknFreeQ.front();
        pfcpPeerTknFreeQ.pop();
        pfcpPeerMap[peerToken]= *(sin);
        pfcpPeerMapMutex.unlock();
    }
    lowLog("Call cb on nfv %d", channel);
    nfvInst->CB[channel]((void*) msg, msgLen, fdd.id, peerToken); // CHECK ID CORRECTNESS [DIP]
    LOG_EXIT;
}

void recvAndProcessCallbacks(struct nfvInstanceData *nfvInst, int epollFd,
                             std::map<uint, fdData_t>::iterator &fddIter)
{
    fdData_t &fdd = fddIter->second;
	int channel = fdd.channel, rc;
	char *msg;

	msg = (char *) malloc(sizeof(char)* MAX_MESSAGE_SIZE);
	/* alloc here, either freed in CB
	 * or MUST be freed in the current func (failure or CB is not called) */

    switch(fdd.type) {
        case SCTP_SERVER_DATA_SOCKET: {
            int streamId = 0;
            rc = platformSCTPRecvData(fdd.fd, msg, &streamId);
            if(rc > 0) {
                lowLog("Call cb on nfv %d:%s", channel,
                        compId_to_nfvName[channel].c_str());
#ifndef THREADPOOL
                nfvInst->CB[channel]((void*) msg, rc, fdd.id, streamId);
#else
                transmittal_t   job;
                job.fp = nfvInst->CB[channel];
                job.msg = msg;
                job.len = rc;
                job.Id  = fdd.id;
                job.streamId = streamId;
                pthread_mutex_lock(&jobMutex);
                jobQueue.push_back(job);
                pthread_mutex_unlock(&jobMutex);
                pthread_cond_signal(&jobAvailable);
#endif
            } else if(rc == 0) {
                removeFdFromEpoll(fdd.fd);
                if(close(fdd.fd) == -1) {
                    higLog("close failed, Error %s", strerror(errno));
                }
                /* TODO: notify that the connection has closed. */
                midLog("Removed data fd %d, id%d", fdd.fd, fdd.id);
                fdd.fd = -1;
                gNfvInst->fdmap.erase(fddIter);
                free(msg);
            }
            return;
        }

        case PFCP_SERVER_UDP_SOCKET:
        case PFCP_CLIENT_UDP_SOCKET: {
            struct sockaddr_in sin;
            uint16_t peerToken = 0;
            rc = platformUDPRecvData(fdd.fd, msg, &sin);
            if(rc > 0) {
                if(fdd.type == PFCP_SERVER_UDP_SOCKET){
                    pfcpPeerMapMutex.lock();
                    if(pfcpPeerTknFreeQ.size()==0){
                        higLog("Empty pfcpPeerTknFreeQ");
                        free(msg);
                        LOG_EXIT;
                        pfcpPeerMapMutex.unlock();
                        return;
                    }
                    peerToken = pfcpPeerTknFreeQ.front();
                    pfcpPeerTknFreeQ.pop();
                    pfcpPeerMap[peerToken]= sin;
                    pfcpPeerMapMutex.unlock();
                }
                lowLog("Call cb on nfv %d", channel);
                nfvInst->CB[channel]((void*) msg, rc, fdd.id, peerToken);
            } /*UDP: ignore if 0 length message is received */
            else
                free(msg);
            return;
        }

        case SCTP_CLIENT_SOCKET: {
            rc = platformRecvData(fdd.fd, msg);
            lowLog("ENTRY\n");
            if(rc > 0) {
                lowLog("Call cb on nfv %d", channel);
                //nfvInst->CB[channel]((void*) msg);
            } else if(rc == 0) {
                higLog("deleted SCTP client socket, nfv %d", channel);
                if(close(fdd.fd) == -1) {
                    higLog("close failed, Error %s", strerror(errno));
                }
                fdd.fd = -1;
                gNfvInst->fdmap.erase(fddIter);
                midLog("Removed dataFD %d", fdd.fd);
                free(msg);
            }
            return;
        }

        default: {
            lowLog("no action needed for fdd.type %d", fdd.type);
            free(msg);
            return;
        }
    }
}


/* a blocked call. Will poll on sockets and run corresponding callbacks
 */
void pollOnEvents(struct nfvInstanceData *nfvInst)
{
	int epollFd, numFds, fd;
	struct epoll_event event, activeEvents[MAX_EPOLL_EVENTS];

	/*create a epollfd*/
	epollFd = epoll_create1(0);
	if(epollFd == -1) {
		higLog("epoll fd creation failed, Error: %s", strerror(errno));
		exit(1);
	}
	timer::epollFd=epollFd;

#ifdef THREADPOOL
#define NUM_OF_POOLTHREADS  4
    pthread_t tid[NUM_OF_POOLTHREADS];
    for (int i = 0; i < NUM_OF_POOLTHREADS; i++) {
        if(pthread_create(&tid[i], NULL, consumer, NULL)) {
            higLog("Error creating threadpool");
            exit(1);
        }
    }
    midLog("Threadpool of size %d created", NUM_OF_POOLTHREADS);
#endif

    if(gNfvInst->compId != E_DPDK_UPF_INST)
    {
    	/*Add all active FDs*/
        for(std::map<uint, fdData_t>::iterator fddIter = nfvInst->fdmap.begin();
                                    fddIter != nfvInst->fdmap.end(); fddIter++) {
            fdData_t fdd = fddIter->second;
            if(fdd.type == SELF_CONNECT || fdd.fd == -1)
                continue;	/*ignore theses FDs*/

            memset(&event,0, sizeof(event));
            event.events = EPOLLIN;
            event.data.fd = fdd.fd;

            if(epoll_ctl(epollFd, EPOLL_CTL_ADD, fdd.fd, &event)==-1){
                higLog("epoll_ctl failed, nfv %d 's listenFD, Error %s",
                        fdd.channel, strerror(errno));
                continue;
            }
            lowLog("added fd %d into epoll", fdd.fd);
            /* dataFDs will added when they get created. */
        }

        /* Add the eventFD */
        memset(&event,0, sizeof(event));
        event.events = EPOLLIN;
        event.data.fd = nfvInst->eventFd;
        if(epoll_ctl(epollFd, EPOLL_CTL_ADD, nfvInst->eventFd, &event)==-1){
            higLog("epoll_ctl failed, eventFD %d,  Error %s",
                    nfvInst->eventFd, strerror(errno));
        }
        lowLog("added eventfd %d into epoll", nfvInst->eventFd);

    }

    map <int, class timer*>::iterator timerItr = timer::fdToObjectMap.begin();
    while(timerItr != timer::fdToObjectMap.end()) {
        memset(&event,0, sizeof(event));
        event.events = EPOLLIN;
        event.data.fd = timerItr->second->getFd();
        if(epoll_ctl(epollFd, EPOLL_CTL_ADD,
                     timerItr->second->getFd(), &event)==-1){
            higLog("epoll_ctl failed, eventFD %d,  Error %s",
                    timerItr->second->getFd(), strerror(errno));
        }
        lowLog("added timer fd %d into epoll", timerItr->second->getFd());

        timerItr++;
    }

    /*For Timer with Single FD*/
    if(timerfd!=-1){
        memset(&event,0, sizeof(event));
        event.events = EPOLLIN;
        event.data.fd = timerfd;
        if(epoll_ctl(epollFd, EPOLL_CTL_ADD, timerfd, &event)==-1){
            higLog("Failed to  register timerfd to epollFd:%s",strerror(errno));
            return;
        }
        lowLog("Successfully registered timerfd to epollFd");
    }

    nfvInst->epollFd = epollFd;
	/* epoll on fds, call corresponding callback */
	lowLog("%s","polling on fds, on rcving data, call correct callback");
    nfvInst->state = E_NFV_STARTED;
	while(1) {
		lowLog("%s","..."); //TODO remove this.
        logFlush();


		/* polling... */
		numFds = epoll_wait(epollFd, activeEvents, MAX_EPOLL_EVENTS, -1);
		specialLog("Got %d events, processing...", numFds);
        for(int wokeup = 0; wokeup < numFds; wokeup++) {
            lowLog("Event on fd %d", activeEvents[wokeup].data.fd);
        }

		for(fd = 0; fd < numFds; fd++) {

            /*For Handling Timer with Single FD*/
            if(activeEvents[fd].data.fd==timerfd){
                timeOutFunction();
                continue;
            }

			/*For Handling Timers : Check if fd belongs to any timer*/
            map <int, class timer*>::iterator timerItr =
                        timer::fdToObjectMap.find(activeEvents[fd].data.fd);
			if(timerItr != timer::fdToObjectMap.end()){
				/*Timeout Function will execute and if all retries are
				exhausted then timer will be de-registered.What action to be
				after exhausting all timers is yet to discuss and implement */
				lowLog("Timeout Function for fd %d Triggered",
                        activeEvents[fd].data.fd);
				(timerItr->second)->timeOutFunction();
                continue;
			}

            if(activeEvents[fd].data.fd == gNfvInst->eventFd) {
                lowLog("Recvd data on eventFD");
                if(gNfvInst->state == E_NFV_STOPPED) {
                    midLog("NFV is in STOPPED STATE. No more polling on epoll");
                    return;
                }
                uint64_t data = 1;
                int ret = read(gNfvInst->eventFd, &data, sizeof(data));
                if(ret != sizeof(data)) {
                    higLog("read from Event FD failed, %s", strerror(errno));
                } else
                    gNfvInst->eventCB();
                continue;
            }

            /* find the fd from the map */
            uint id = activeEvents[fd].data.fd ^ nfvInst->hash;
            std::map<uint, fdData_t>::iterator fddIter
                            = nfvInst->fdmap.find(id);

            if(fddIter == nfvInst->fdmap.end()) {
                higLog("%s","Received an event on a fd not present in map");
                higLog("%s","ERROR: Issue with map clean up!!!!");
                continue;
            }

            if(fddIter->second.type == SCTP_SERVER_ACCEPT_SOCKET) {
                /*1. on all listening stream sockets accept and
                 *   add the sock to epoll*/
                acceptConnections(&fddIter->second, epollFd);
                continue;
            } else {
                /*2. recv and process callbacks on data fds */
                recvAndProcessCallbacks(nfvInst, epollFd, fddIter);
            }
		}
	}

	return;
}

timer::timer(){
	/*Initially default duration set to 6 Sec*/
	this->duration=TIMER_DEFAULT_DURATION;
	this->retries=TIMER_DEFAULT_RETRIES;
	this->fd=-1;
}

int timer::getFd(){
	return this->fd;
}

timer::~timer(){
    LOG_ENTRY;
	this->stopTimer();
    LOG_EXIT;
}

void timer::startTimer(int duration,int retries)
{
    LOG_ENTRY;

	struct epoll_event ev = {};
	struct itimerspec iterationDetails = {};
	if(this->fd==-1){
		/*New fd is created for timer,required while executing for first time*/
		this->fd=timerfd_create(CLOCK_REALTIME, 0);
        if(this->fd==-1){
            higLog("Error in creating fd for Timer : %s ",strerror(errno));
            return;
        }
		/*this fd is added in the map to retrieve back entire object
		when it will get triggered from*/
		fdToObjectMap[this->fd]=this;
	}
	this->duration=(duration<0)?TIMER_DEFAULT_DURATION:duration;
	this->retries=(retries<0)?TIMER_DEFAULT_RETRIES:retries;
	struct timespec now = {};
	//Get Current time
	if (clock_gettime(CLOCK_REALTIME, &(now)) == -1)
		lowLog("Error in getting Clock");/*Has not occured yet*/
	//Configuring Event for Fd created
	ev.events=EPOLLIN | EPOLLET;/*Edge Triggered Behavior*/
	ev.data.fd=this->fd;
	//Configuring Duration of timer
	iterationDetails.it_value.tv_sec = now.tv_sec + duration;
	iterationDetails.it_value.tv_nsec = now.tv_nsec;

	/*Following values will get autofilled after timeout
	1 is subtracted because time includes 0 as the last cout
	i.e. 4:4,3,2,1,0 but our intention was 4 not 5*/
	iterationDetails.it_interval.tv_sec = duration-1;
	iterationDetails.it_interval.tv_nsec = now.tv_nsec;
	//start/update the timer
	if (timerfd_settime(this->fd, TFD_TIMER_ABSTIME, &iterationDetails,
		NULL) == -1)
	{
		higLog("Error in settingup timer : %s",strerror(errno));
	}
	//Registered fd to epollFd for monitoring
    if(timer::epollFd == 0) {
        lowLog("Epoll fd has not been created. fd %d will be added later",
                this->fd);
    }else if(epoll_ctl(timer::epollFd,EPOLL_CTL_ADD,this->fd,&ev)==-1)
	{
		higLog("Error in settingup epoll event for Timer: %s",strerror(errno));
	}
	lowLog("Successfully starting Timer with fd %d in epfd %d",
		this->fd,timer::epollFd);
    LOG_EXIT;
}

void timer::startTimer(){
	startTimer(this->duration,this->retries);
}

void timer::stopTimer(){
	lowLog("Default stopTimer for fd %d",this->fd);
    if(this->fd!=-1)
    {
        //De-Register the timer
        epoll_ctl(timer::epollFd,EPOLL_CTL_DEL,this->getFd(),NULL);
        /*Remove the entry from the map*/
        fdToObjectMap.erase(this->fd);
        /*Free the fd resource/closing timer*/
        close(this->fd);
        this->fd=-1;
    }
    /* Further Actions for stopTimer should be implemented by derived class. */
}

void timer::timeOutFunction(){
	lowLog("Default Timeout Function for fd %d, Timer Attempt Remaining : %d",
		this->fd,retries);
	this->retries-=1;
	if(this->retries<=0)
	{
		//De-register the timer
		this->stopTimer();
		return;
	}
	/*Following Code is necessity for reloading Timer,Reason Unknown....*/
	struct itimerspec temp;
	int rc=timerfd_gettime(this->fd,&temp);
	/*timerfd_gettime return 0 on success and -1 on failure*/
	if(rc==-1)
	{
		lowLog("Failed in reloading the timer and extracting the current bufferes from fd %d",this->fd);
	}
}

int setupTimerFd()
{
    LOG_ENTRY;
    timerfd=timerfd_create(CLOCK_REALTIME, 0);
    if(timerfd==-1){
        higLog("Failed to get FD for Timer:%s", strerror(errno));
        LOG_EXIT;
        return FAILURE;
    }
    struct itimerspec iterationDetails = {};
    iterationDetails.it_value.tv_sec = 0;
    iterationDetails.it_value.tv_nsec = 0;
    timeoutTime=ULONG_MAX;
    if (timerfd_settime(timerfd, TFD_TIMER_ABSTIME, &iterationDetails,NULL) == -1)
    {
        higLog("Error in settingup timer %s",strerror(errno));
        LOG_EXIT;
        return FAILURE;
    }
    LOG_EXIT;
    return SUCCESS;
}

int startTimer(struct timerInfo tInfo){
    LOG_ENTRY;
    timerMutex.lock();
    if(idExpiryMap.find(tInfo.Id) != idExpiryMap.end())
    {
        //Here we can either overwrite previous timer or drop new timer.
        //Choosing to drop new Timer.
        higLog("Error in adding Timer to the map.Timer for Id %lu already exists",tInfo.Id);
        timerMutex.unlock();
        LOG_EXIT;
        return FAILURE;
    }
    /*Step 1: Adding Timer to Map*/
    struct timespec now = {};
    //Get Current Epoch Time
    if (clock_gettime(CLOCK_REALTIME, &(now)) == -1){
        higLog("Error in getting Clock for Id %lu",tInfo.Id);
        timerMutex.unlock();
        LOG_EXIT;
        return FAILURE;
    }
    //Calculate Expected Expiry Epoch Time
    now.tv_sec += tInfo.duration;
    idExpiryMap[tInfo.Id] = now;
    expiryToTimerMap[now] = tInfo;
    lowLog("Successfully Added Timer for Id %lu",tInfo.Id);

    /*Step 2:update Timer Event*/
    unsigned long currentTime = (SEC_TO_MSEC(now.tv_sec) + round(NSEC_TO_MSEC(now.tv_nsec)));
    if (currentTime < timeoutTime) {
        timeoutTime = currentTime;
        updateTimer();
    }

    timerMutex.unlock();
    LOG_EXIT;
    return SUCCESS;
}
void timeOutFunction() {

    timerMutex.lock();
    timeoutTime = ULONG_MAX;
    struct timespec now = {};
    if (clock_gettime(CLOCK_REALTIME, &(now)) == -1){
        higLog("Error in getting Clock for timerfd %lu",timerfd);
        timerMutex.unlock();
        LOG_EXIT;
        return;
    }

    unsigned long currentTime = (SEC_TO_MSEC(now.tv_sec) +
                                            round(NSEC_TO_MSEC(now.tv_nsec)));
    unsigned long expiryTime = ULONG_MAX;

    vector<struct timerInfo> expiredTimers = {};
    map<struct timespec, struct timerInfo> nonExpiredTimerMap = {};
    
    map<struct timespec, struct timerInfo>::iterator it =
                 expiryToTimerMap.begin();

    for (;it != expiryToTimerMap.end(); it++) {
        expiryTime = (SEC_TO_MSEC(it->first.tv_sec) +
                                     round(NSEC_TO_MSEC(it->first.tv_nsec)));
        if (expiryTime > currentTime) {
            nonExpiredTimerMap[it->first] = it->second;
        } else {
            struct timerInfo tInfo = it->second;
            idExpiryMap.erase(tInfo.Id);
            expiredTimers.push_back(tInfo);
        }
    }
    expiryToTimerMap = nonExpiredTimerMap;

    if (expiryToTimerMap.size() != 0) {
        struct timespec temp = expiryToTimerMap.begin()->first;
        timeoutTime = (SEC_TO_MSEC(temp.tv_sec) + round(NSEC_TO_MSEC(temp.tv_nsec)));
    
        if((timeoutTime - currentTime) < TIMER_GRANULARITY) {
            timeoutTime = currentTime + TIMER_GRANULARITY;
        }
    }
    updateTimer();
    timerMutex.unlock();
    
    /* timeOutFunction can't be called with lock held */
    for (int i =0; i< expiredTimers.size();i++)
        expiredTimers[i].timeoutFunction(expiredTimers[i]);
}
int stopTimer(unsigned long Id)
{
    LOG_ENTRY;
    timerMutex.lock();
    map<unsigned long, struct timespec>::iterator it = idExpiryMap.find(Id);
    if(it == idExpiryMap.end())
    {
        higLog("Error,No Timer associated with Id : %lu",Id);
        timerMutex.unlock();
        LOG_EXIT;
        return FAILURE;
    }
    struct timespec now = it->second;
    idExpiryMap.erase(Id);
    expiryToTimerMap.erase(it->second);
    lowLog("Successfully Stopped Timer with Id %lu",Id);

    if (expiryToTimerMap.size() == 0 ){
        timeoutTime = ULONG_MAX;
        updateTimer();
    }
    timerMutex.unlock();

    LOG_EXIT;
    return SUCCESS;
}

int updateTimer()
{
    LOG_ENTRY;
    struct itimerspec iterationDetails = {};
    if(timeoutTime == ULONG_MAX){
        iterationDetails.it_value.tv_sec = 0;
        iterationDetails.it_value.tv_nsec = 0;
    }
    else{
        iterationDetails.it_value.tv_sec = MSEC_TO_SEC(timeoutTime);
        iterationDetails.it_value.tv_nsec = MSEC_TO_NSEC(timeoutTime % MS_PER_SEC);
    }

    if (timerfd_settime(timerfd, TFD_TIMER_ABSTIME, &iterationDetails,NULL) == -1)
    {
        higLog("Error in setting up timer: %s",strerror(errno));
        LOG_EXIT;
        return FAILURE;
    }
    LOG_EXIT;
    return SUCCESS;
}

