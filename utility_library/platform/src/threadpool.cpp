#include "platform/include/threadpool.h"

void* consumer(void*) {
   for(;;) {

        transmittal_t job = {};

        pthread_mutex_lock(&jobMutex);
        while(jobQueue.size() == 0)
            pthread_cond_wait(&jobAvailable, &jobMutex);
        job = jobQueue.front();
        jobQueue.erase(jobQueue.begin());
        pthread_mutex_unlock(&jobMutex);
        job.fp(job.msg, job.len, job.Id, job.streamId);
   } 
}
