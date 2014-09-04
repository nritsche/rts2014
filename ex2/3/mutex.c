#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_THREADS 2
#define BUF 255

struct data {
   int value;
   char msg[BUF];
};
pthread_mutex_t mutex;
int running = 1;
int var1 = 0;
int var2 = 0;

static void *thread1 (void *arg) {
	while (running) {
		pthread_mutex_lock(&mutex);
		var1 = var1 + 1;
		var2 = var1;
		pthread_mutex_unlock(&mutex);
	}
	return arg;
}

static void *thread2 (void *arg) {
	int i;
	for (i = 0; i < 20; i++) {
		pthread_mutex_lock(&mutex);
		printf("Number 1 is %d, number 2 is %d\n", var1, var2);
		pthread_mutex_unlock(&mutex);
		usleep(100000);
	}
	running = 0;

   return arg;
}

int main(void)
{
   pthread_t th[MAX_THREADS];
   int i;
   struct data *ret[MAX_THREADS];

	if (pthread_mutex_init(&mutex, NULL)) {
		perror("mutex init");
		return EXIT_FAILURE;
	}

   for (i = 0; i < MAX_THREADS; i++){
      ret[i] = (struct data *)malloc(sizeof(struct data));
      if(ret[i] == NULL) {
         perror("malloc: ");
         exit(EXIT_FAILURE);
      }
   }

   if(pthread_create(&th[0],NULL,&thread1,ret[0]) !=0) {
      perror("couldn't create thread");
		exit (EXIT_FAILURE);
   }
 
   if(pthread_create(&th[1],NULL,&thread2,ret[1]) !=0) {
      perror("couldn't create thread");
		exit (EXIT_FAILURE);
   }

	//wait for threads
   for( i=0; i < MAX_THREADS; i++)
      pthread_join(th[i], NULL);

	pthread_mutex_destroy(&mutex);

	return EXIT_SUCCESS;
}
