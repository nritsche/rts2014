#include <stdio.h>
#include <unistd.h>
#include <sys/times.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_THREADS 2
#define BUF 255

struct data {
   int value;
   char msg[BUF];
};

int global = 0;

static void *mythread (void *arg) {
	int i, local = 0;
	for (i = 0; i < 3; i++) {
		global++;
		local++;
		printf("local = %d, global = %d\n", local, global);
	}
   return arg;
}

int main (void) {
   pthread_t th[MAX_THREADS];
   int i;
   struct data *ret[MAX_THREADS];

   for (i = 0; i < MAX_THREADS; i++){
      ret[i] = (struct data *)malloc(sizeof(struct data));
      if(ret[i] == NULL) {
         perror("malloc: ");
         exit(EXIT_FAILURE);
      }
   }

   for (i = 0; i < MAX_THREADS; i++) {
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
