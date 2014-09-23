#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include "io.h"

#define MAX_THREADS 3
#define DIST_THREADS 10
#define BUF 255

struct data {
int value;
char msg[BUF];
};

int set_cpu(int cpu_number)
{
// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof	(cpu_set_t),
&cpu);
}


void timespec_add_us(struct timespec *t, long us)
{
// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;
	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000)
	{
		t->tv_nsec = t->tv_nsec - 1000000000;
		t->tv_sec += 1;
	}
}

static void *threadA (void *arg) {
	struct timespec next;
	if (set_cpu(1)) {
		perror("threadA couldn't set CPU");
		return arg;
	}
	clock_gettime(CLOCK_REALTIME, &next);
	while (1) {
		timespec_add_us(&next, 100);
		if (!io_read(1)) {
			io_write(1,1);
			usleep(5);
			io_write(1,0);	
		}
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
	return arg;
}
static void *threadB (void *arg) {
struct timespec next;
	if (set_cpu(1)) {
		perror("threadB couldn't set CPU");
		return arg;
	}
clock_gettime(CLOCK_REALTIME, &next);
	while (1) {
		timespec_add_us(&next, 100);
		if (!io_read(2)) {
			io_write(2,1);
			usleep(5);
			io_write(2,0);	
		}
clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
	return arg;
}

static void *threadC (void *arg) {
	struct timespec next;
	if (set_cpu(1)) {
		perror("threadC couldn't set CPU");
		return arg;
	}
	clock_gettime(CLOCK_REALTIME, &next);
	while (1) {
		timespec_add_us(&next, 100);
		if (!io_read(3)) {
			io_write(3,1);
			usleep(5);
			io_write(3,0);		
		}
clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
	return arg;
}

static void *threadD (void *arg) {
	if (set_cpu(1)) {
		perror("threadD couldn't set CPU");
		return arg;
	}
	int i;
	while (1) {
		i = i * i;
	}
	return arg;
}

int main(void)
{
	pthread_t th[MAX_THREADS];
	pthread_t d_th[DIST_THREADS];
	int i;
	struct data *ret[MAX_THREADS];
	struct data *d_ret[DIST_THREADS];

	io_init();

	for (i = 0; i < MAX_THREADS; i++){
		ret[i] = (struct data *)malloc(sizeof(struct data));
		if(ret[i] == NULL) {
			perror("malloc: ");
			exit(EXIT_FAILURE);
		}
	}
for (i = 0; i < DIST_THREADS; i++){
		d_ret[i] = (struct data *)malloc(sizeof(struct data));
		if(d_ret[i] == NULL) {
			perror("malloc: ");
			exit(EXIT_FAILURE);
		}

		if(pthread_create(&d_th[i],NULL,&threadD,d_ret[i]) !=0) {
			perror("couldn't create thread");
			exit (EXIT_FAILURE);
		}
	}
	if(pthread_create(&th[0],NULL,&threadA,ret[0]) !=0) {
		perror("couldn't create thread");
		exit (EXIT_FAILURE);
	}
	if(pthread_create(&th[1],NULL,&threadB,ret[1]) !=0) {
		perror("couldn't create thread");
		exit (EXIT_FAILURE);
	}
	if(pthread_create(&th[1],NULL,&threadC,ret[1]) !=0) {
		perror("couldn't create thread");
		exit (EXIT_FAILURE);
	}
	//wait for threads
	for( i=0; i < MAX_THREADS; i++)
		pthread_join(th[i], NULL);
	for( i=0; i < DIST_THREADS; i++)
		pthread_join(d_th[i], NULL);

	return EXIT_SUCCESS;
}
