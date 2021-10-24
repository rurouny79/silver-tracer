// HOWTOUSE :
// - add -finstrument-functions in Compile Option
// TODO :
// - fwrite and fopen,close - synchronization??

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <pthread.h>

#include "silver.h"

#define LOGFILE "fdat"

void silver_init() __attribute__((no_instrument_function));
extern "C" void regist_task_name(pthread_t t, const char* name) __attribute__((no_instrument_function));
void sig_handler_USR1(int signo) __attribute__((no_instrument_function));
void sig_handler_USR2(int signo) __attribute__((no_instrument_function));
extern "C" void __cyg_profile_func_enter(void *, void *) __attribute__((no_instrument_function));
extern "C" void __cyg_profile_func_exit(void *, void *) __attribute__((no_instrument_function));

const char *my_task_name[99] = { 0, };
static FILE *logfp = NULL;

void silver_init(int start_log) {
	signal(SIGUSR1, sig_handler_USR1);
	signal(SIGUSR2, sig_handler_USR2);
}

void regist_task_name(pthread_t t, const char* name) {
	//my_task_name[syscall(__NR_gettid) % 100] = name;
	my_task_name[t % 100] = name;
}

void sig_handler_USR1(int signo) {
	char logfile[100];

	if (!logfp) {
		sprintf(logfile, "%s_%d.data", LOGFILE, getpid());
		logfp = fopen(logfile, "w");

		printf("--- (pid:%d), SILVER [ON]\n", getpid());
	} else {
		fclose(logfp);
		logfp = NULL;

		printf("--- (pid:%d), SILVER [OFF]\n", getpid());
	}
}

void sig_handler_USR2(int signo) {
	//int tid = syscall(__NR_gettid); // get LWP ID
	int tid = pthread_self();

	printf("--- (tid:%d), I'm [%s]\n", tid, my_task_name[tid % 100]);
}

static int funlevel = 0;

void __cyg_profile_func_enter(void *func, void *caller) {
    char logfile[100];
    funlevel++;
	MYDATA d;

    if(logfp == NULL){
        //sprintf(logfile, "%s_%d.dat", LOGFILE, getpid());
        sprintf(logfile, "fdat");
		logfp = fopen(logfile, "w");
    }

	d.tid = syscall(__NR_gettid);
    d.level = funlevel;
	d.arrow = 1; // enter
	d.faddr = (unsigned long long) func;
	d.caddr = (unsigned long long) caller;

	fwrite(&d, sizeof(MYDATA), 1, logfp);
    printf("-enter- %u %u %d %llx\n", d.tid, d.level, d.arrow, d.faddr);
}

void __cyg_profile_func_exit(void *func, void *caller) {
	MYDATA d;

	d.tid = syscall(__NR_gettid);
    d.level = funlevel;
	d.arrow = 0; // exit
	d.faddr = (unsigned long long) func;
	d.caddr = (unsigned long long) caller;

	fwrite(&d, sizeof(MYDATA), 1, logfp);
    printf("-exit- %u %u %d %llx\n", d.tid, d.level, d.arrow, d.faddr);

    funlevel--;
}
