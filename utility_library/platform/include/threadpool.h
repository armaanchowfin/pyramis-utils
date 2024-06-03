#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include <pthread.h>
#include <cstdint>                                                              
#include <cstdlib>                                                              
#include <bits/stdc++.h>  
#include <vector>

using namespace std;

typedef void (*cbType)(void * msg, int len, uint Id, uint16_t streamId);

typedef struct transmittal {
    cbType  fp;
    void    *msg;
    int     len;
    uint    Id;
    uint16_t    streamId;
} transmittal_t;

extern std::vector<transmittal_t > jobQueue;

extern pthread_cond_t  jobAvailable;
extern pthread_mutex_t jobMutex;    

void *consumer(void*);

#endif 
