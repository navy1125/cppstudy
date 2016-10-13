#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <thread>    // std::thread

void *thread_routine(void *arg)
{
    int my_policy;
    struct sched_param my_param;

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && !defined(sun)
    pthread_getschedparam(pthread_self(), &my_policy, &my_param);
    printf("thread_routine running at %s/%d\n",
        (my_policy == SCHED_FIFO ? "FIFO"
            : (my_policy == SCHED_RR ? "RR"
            : (my_policy == SCHED_OTHER ? "OTHER"
            : "unknown"))),
        my_param.sched_priority);
#else
    printf("thread_routine running\n");
#endif
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread_id;
    pthread_attr_t thread_attr;
    int thread_policy;
    struct sched_param thread_param;
    int status, rr_min_priority, rr_max_priority;

    pthread_attr_init(&thread_attr);

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && !defined(sun)
    pthread_attr_getschedpolicy(&thread_attr, &thread_policy);
    pthread_attr_getschedparam(&thread_attr, &thread_param);
    printf("Default policy is %s, priority is %d\n",
        (thread_policy == SCHED_FIFO ? "FIFO"
         : (thread_policy == SCHED_RR ? "RR"
            : (thread_policy == SCHED_OTHER ? "OTHER"
               : "unknown"))),
        thread_param.sched_priority);

    status = pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);
    if(status != 0)
        printf("111111111111111111Unable to set SCHED_RR policy.\n");
    else{
        rr_min_priority = sched_get_priority_min(SCHED_FIFO);
        if(rr_min_priority == -1)
            printf("Get SCHED_RR min priority");
        rr_max_priority = sched_get_priority_max(SCHED_FIFO);
        if(rr_max_priority == -1)
            printf("Get SCHED_RR max priority");
        thread_param.sched_priority = (rr_min_priority + rr_max_priority)/2;
        printf("SCHED_RR priority range is %d to %d: using %d\n",
            rr_min_priority, rr_max_priority, thread_param.sched_priority);
        pthread_attr_setschedparam(&thread_attr, &thread_param);
        printf("Creating thread at RR/%d\n", thread_param.sched_priority);
        pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
    }
#else
    printf("Priority scheduling not supported\n");
#endif
    pthread_create(&thread_id, &thread_attr, thread_routine, NULL);
    pthread_join(thread_id, NULL);
    printf("Main exiting\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return 0;
}
