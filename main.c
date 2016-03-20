#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <signal.h>
#include <pthread.h>
//#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <execinfo.h>
#include <sys/mman.h>
//#include <sys/syscall.h>

#define NUMST 7
#define TRUE 1
#define FALSE 0

struct task_data
{
    pthread_t id;
    long tid;
    int tasknumber;
    int response_sig;
    struct itimerspec period_dl;
    timer_t timer_id;
};

static struct task_data tasks[NUMST];
static int stopflag = FALSE;
static struct timespec startdelay = {.tv_sec = 1, .tv_nsec = 0};
static struct timespec starttime;
static int next_sig = 0;

static inline void timespec_add(struct timespec *res, const struct timespec *a, const struct timespec *b)
{
   res->tv_sec = a->tv_sec + b->tv_sec;
   if ((a->tv_nsec + b->tv_nsec) >= 1e9)
   {
       res->tv_sec++;
       res->tv_nsec = a->tv_nsec + b->tv_nsec - 1e9;
   }
   else
   {
       res->tv_nsec = a->tv_nsec + b->tv_nsec;
   }
}

static inline void timespec_sub(struct timespec *res, const struct timespec *a, const struct timespec *b)
{
   res->tv_sec = a->tv_sec - b->tv_sec;
   if ((a->tv_nsec - b->tv_nsec) < 0)
   {
       res->tv_sec--;
       res->tv_nsec = 1e9 + a->tv_nsec - b->tv_nsec;
   }
   else
   {
       res->tv_nsec = a->tv_nsec - b->tv_nsec;
   }
}

static void signal_catcher(int signal)
{
    switch(signal)
    {
        case SIGINT:
            printf("SIGINT CAUGHT!\n");
            stopflag = TRUE;
            break;
        case SIGHUP:
            printf("SIGHUP CAUGHT!\n");
            stopflag = TRUE;
            break;
        case SIGXCPU:
            printf("SIGXCPU CAUGHT!\n");
            break;
        case SIGTERM:
            printf("SIGTERM CAUGHT!\n");
            stopflag = TRUE;
            break;
        default:
            printf("UNKNWON SIGNAL CAUGHT!\n");
            break;
    }
}

static void fail (char *reason)
{
    perror(reason);
    exit(EXIT_FAILURE);
}

void *worker(void *arg)
{
    int myid = *((int *)arg);
    struct task_data *taskptr;
    taskptr = &tasks[myid];
    siginfo_t si;
    sigset_t mask;
    sigevent_t sigev;

    taskptr->tid = myid; //syscall(SYS_gettid);
    printf("Starting Thread: %d\n", myid);

    if(next_sig == 0)
    {
        next_sig = SIGRTMIN;
    }
    if(next_sig > SIGRTMAX)
    {
        fail("NO MORE SIGRTS!!");
    }
    taskptr->response_sig = next_sig;
    next_sig++;

    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = taskptr->response_sig;
    sigev.sigev_value.sival_ptr = (void *)taskptr->timer_id;

    if (sigemptyset(&mask) != 0) fail("Sig Empty Set\n");
    if (sigaddset(&mask, taskptr->response_sig) != 0) fail("Sig Add Set\n");
    //if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) fail("pthread add mask\n");

    if(timer_create(CLOCK_MONOTONIC, &sigev, &taskptr->timer_id) != 0) fail("timer create\n");

    taskptr->period_dl.it_interval.tv_sec = 0;
    taskptr->period_dl.it_interval.tv_nsec = myid * 1e8;
    //clock_gettime(CLOCK_MONOTONIC, &taskptr->period_dl.it_value);
    memcpy(&taskptr->period_dl.it_value, &starttime, sizeof(struct timespec));

    if (timer_settime(taskptr->timer_id, TIMER_ABSTIME, &taskptr->period_dl, NULL) != 0) fail("timer settime\n");

    mlockall(MCL_CURRENT|MCL_FUTURE);

    while(stopflag != TRUE)
    {
        sigwaitinfo(&mask, &si);
        int i;
        for (i = 0; i < myid; i++) printf("              ");
        printf("Th: %d OR: %d\n", myid, si.si_overrun);
    }

    if(timer_delete(taskptr->timer_id) != 0) exit(EXIT_FAILURE);

    return arg;
}

int main(int argc, char** argv)
{
    int taskindex;
    char names[16];
    struct sched_param params;
    pthread_attr_t attrs;
    memset(names, '\0' ,sizeof(char)*16);
    long maintid = 0; //syscall(SYS_gettid);
    sigset_t alarm_sig;

    //for(taskindex = 0; taskindex < argc; taskindex++)
    //{
    //    printf("ARG%d: %s\n", taskindex, argv[taskindex]);
    //}

    signal(SIGINT, signal_catcher);
    signal(SIGHUP, signal_catcher);
    signal(SIGXCPU, signal_catcher);
    signal(SIGTERM, signal_catcher);

    sigemptyset (&alarm_sig);
    int i;
    for (i = SIGRTMIN; i <= SIGRTMAX; i++) sigaddset (&alarm_sig, i);
    sigprocmask (SIG_BLOCK, &alarm_sig, NULL);

    clock_gettime(CLOCK_MONOTONIC, &starttime);
    printf("Monotonic time now     : %ld.%09ld\n", starttime.tv_sec, starttime.tv_nsec);
    timespec_add(&starttime, &starttime, &startdelay);
    printf("Monotonic start time   : %ld.%09ld\n", starttime.tv_sec, starttime.tv_nsec);

    for (taskindex = 0; taskindex < NUMST; taskindex++)
    {
        printf("Creating Thread: %d\n", taskindex);

        struct task_data *taskptr = &tasks[taskindex];
        taskptr->tasknumber = taskindex;

        params.sched_priority = 99 - (taskindex*2);
        if(pthread_attr_init(&attrs) != 0) fail ("attributes init\n");
        if(pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE) != 0) fail ("could not create joinable\n");
        if(pthread_attr_setinheritsched(&attrs, PTHREAD_EXPLICIT_SCHED) != 0) fail ("no explicit scheduler\n");
        if(pthread_attr_setschedpolicy(&attrs, SCHED_FIFO) !=0 ) fail ("scheduler policy\n");
        if(pthread_attr_setschedparam(&attrs, &params) != 0) fail ("schedparams\n");

        if(pthread_create(&taskptr->id, &attrs, worker, &taskptr->tasknumber) == -1) fail("Thread Creation\n");

        snprintf(names, 16, "t%02d_task", taskptr->tasknumber);
        if(pthread_setname_np(taskptr->id, names) != 0) fail ("setting name\n");

        if(pthread_attr_destroy(&attrs) != 0) fail("attributes destroy\n");
    }

    while(stopflag != TRUE)
    {
        printf("TID: %ld : MAIN Thread\n", maintid);
        sleep(NUMST + 1);
    }

    for (taskindex = 0; taskindex < NUMST; taskindex++)
    {
        struct task_data *taskptr;
        taskptr = &tasks[taskindex];
        pthread_join(taskptr->id, NULL);
    }

    printf("\n\n\n******************************************\n\n\n");

    return 0;
}
