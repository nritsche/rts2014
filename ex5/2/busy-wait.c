#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "io.h"

#define MAX_THREADS 3
#define BUF 255

struct data {
int value;
char msg[BUF];
};


static void *threadA (void *arg) {
	while (1) {
		if (!io_read(1)) {
			io_write(1,1);
			usleep(5);
			io_write(1,0);	
		}
	}
	return arg;
}
static void *threadB (void *arg) {
	while (1) {
		if (!io_read(2)) {
			io_write(2,1);
			usleep(5);
			io_write(2,0);	
		}
	}
	return arg;
}

static void *threadC (void *arg) {
	while (1) {
		if (!io_read(3)) {
			io_write(3,1);
			usleep(5);
			io_write(3,0);		
		}
	}
	return arg;
}

int main(void)
{
	pthread_t th[MAX_THREADS];
	int i;
	struct data *ret[MAX_THREADS];

	io_init();

	for (i = 0; i < MAX_THREADS; i++){
		ret[i] = (struct data *)malloc(sizeof(struct data));
		if(ret[i] == NULL) {
			perror("malloc: ");
			exit(EXIT_FAILURE);
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

	return EXIT_SUCCESS;
}
