#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_THREADS 2
#define BUF 255

struct data {
   int value;
   char msg[BUF];
};

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
		times(&exec_time);
	}
}

static void *mythread (void *arg) {
	printf("hello, i'm a thread\n");
	busy_wait_delay(5);
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

   printf("<- Main-Thread finished\n");
   return EXIT_SUCCESS;
}
