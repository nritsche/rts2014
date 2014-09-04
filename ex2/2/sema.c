#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_THREADS 5
#define BUF 255

struct data {
   int value;
   char msg[BUF];
};
sem_t sem;

static void *mythread (void *arg) {
   struct data *my_data = (struct data *) arg;

	if(sem_wait(&sem)) {
		perror("sem_wait");
		return arg;
	}

	printf("Thread number %d\n", my_data->value);
	sleep(1);

	if (sem_post(&sem)) {
		perror("sem_post");
		return arg;
	}
   return arg;
}

int main(void)
{
   pthread_t th[MAX_THREADS];
   int i;
   struct data *ret[MAX_THREADS];

	if (sem_init(&sem, 0,3)) {
		perror("semaphore init");
		return EXIT_FAILURE;
	}

   for (i = 0; i < MAX_THREADS; i++){
      ret[i] = (struct data *)malloc(sizeof(struct data));
      if(ret[i] == NULL) {
         perror("malloc: ");
         exit(EXIT_FAILURE);
      }
   }

   for (i = 0; i < MAX_THREADS; i++) {
		ret[i]->value = i;
      if(pthread_create(&th[i],NULL,&mythread,ret[i]) !=0) {
         perror("couldn't create thread");
         exit (EXIT_FAILURE);
      }
   }

	//wait for threads
   for( i=0; i < MAX_THREADS; i++)
      pthread_join(th[i], (void **)&ret[i]);

	return EXIT_SUCCESS;
}
