#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/mman.h> /* for mlockall */
#include <time.h> /* clock_nanosleep */
#include <sched.h> /* sched_setscheduler */
#include <string.h> /* memset */
#include <errno.h>
#include <unistd.h> /* for usleep */
#include <pthread.h>

#define NSEC_PER_SEC        1000000000

static inline void tsnorm(struct timespec *ts)
{
    while (ts->tv_nsec >= NSEC_PER_SEC) {
        ts->tv_nsec -= NSEC_PER_SEC;
        ts->tv_sec++;
    }
}

static inline int64_t calcdiff_ns(struct timespec t1, struct timespec t2)
{
    int64_t diff;
    diff = NSEC_PER_SEC * (int64_t)((int) t1.tv_sec - (int) t2.tv_sec);
    diff += ((int) t1.tv_nsec - (int) t2.tv_nsec);
    return diff;
}

struct thread_params {
    int prio;
    struct timespec period;
};

void *timerthread(void *param)
{
    int ret;
    struct sched_param schedp;
    struct timespec now, next;
    int64_t diff;
    cpu_set_t mask;
    struct thread_params *p = (struct thread_params *)param;

    /* set affinity to cpu 0 for multicore */
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    ret = sched_setaffinity(0, sizeof(mask), &mask);
    assert(ret == 0);

    /* rt-priority */
    memset(&schedp, 0, sizeof(schedp));
    schedp.sched_priority = p->prio;
    ret = sched_setscheduler(0, SCHED_FIFO, &schedp);
    assert(ret == 0);

    /* init wake time */
    clock_gettime(CLOCK_MONOTONIC, &now);
    next = now;
    next.tv_sec++;

    while (1) {
        /* Wait for next period */
        ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
        if (ret == EINTR)goto out;
        clock_gettime(CLOCK_MONOTONIC, &now);

        diff = calcdiff_ns(now, next);
        printf("latency: %lld ns\n", diff);

        next.tv_sec  += p->period.tv_sec;
        next.tv_nsec += p->period.tv_nsec;
        tsnorm(&next);
    }

out:
    /* unlock memory */
    ret = munlockall();
    assert(ret == 0);

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_attr_t attr;
    pthread_t thread;
    int status;
    struct thread_params params = {99, {1, 0}}; // default parameters

    mlockall(MCL_CURRENT|MCL_FUTURE);

    if (argc == 4) {
        params.prio = atoi(argv[1]);
        params.period.tv_sec = atoi(argv[2]);
        params.period.tv_nsec = atoi(argv[3]);
        printf("prio:%d, period:%ld.%ld\n", params.prio, params.period.tv_sec, params.period.tv_nsec);
    }

    pthread_attr_init(&attr);
    status = pthread_create(&thread, &attr, timerthread, (void *)&params);
    if (status)
        printf("failed");

    while (1) {
        usleep(10000);
    }
    return 0;
}
