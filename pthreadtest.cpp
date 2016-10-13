/*
 * sched_thread.c
 * Demonstrate dynamic scheduling policy use.
 */
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>

#define THREADS 5

typedef struct thread_tag {
    int index;
    pthread_t id;
} thread_t;

thread_t threads[THREADS];
int rr_min_priority;

void *thread_routine(void *arg)
{
    thread_t *self = (thread_t *)arg;
    struct sched_param my_param;
    int my_policy;
    int status;

    my_param.sched_priority = rr_min_priority + self->index;
    if(pthread_setschedparam(self->id, SCHED_FIFO, &my_param) != 0)
        printf("pthread_setschedparam failed\n");
    pthread_getschedparam(self->id, &my_policy, &my_param);
    printf("thread_routine %d running at %s/%d\n",
        self->index,
        (my_policy == SCHED_FIFO ? "FIFO"
            : (my_policy == SCHED_RR ? "RR"
            : (my_policy == SCHED_OTHER ? "OTHER"
            : "unknown"))),
        my_param.sched_priority);

    return NULL;
}

int main(int argc, char *argv[])
{
    int count;

    rr_min_priority = sched_get_priority_min(SCHED_RR);
    if(rr_min_priority == -1){
        printf("Get SCHED_RR min priority");
    }
    for(count = 0; count < THREADS; count++){
        threads[count].index = count;
        pthread_create(&threads[count].id, NULL,
                 thread_routine, (void *)&threads[count]);
    }
    for(count = 0; count < THREADS; count++){
        pthread_join(threads[count].id, NULL);
    }
    printf("Main exiting\n");

    return 0;
}
