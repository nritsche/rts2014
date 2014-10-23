#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

#define BUFSIZE 20
#define THREADS 4

struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
};

struct data {
int value;
char msg[BUFSIZE];
};

int set_priority(int priority)
{
	int policy;
	struct sched_param param;
	// check priority in range
	if (priority < 1 || priority > 63) return -1;
	// set priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	param.sched_priority = priority;
	return pthread_setschedparam(pthread_self(), policy, &param);
}

int get_priority()
{
	int policy;
	struct sched_param param;
	// get priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	return param.sched_curpriority;
}

static void *thread (void *arg) {
	struct data *d = (struct data *)arg;

	if(d->value % 2)
		set_priority(40);
	else
		set_priority(10);

	char *snd_buf = malloc(BUFSIZE);
	if (snd_buf == NULL){
		perror("malloc");
		return EXIT_FAILURE;
	}
	char *rcv_buf = malloc(BUFSIZE);
		if (rcv_buf == NULL){
			perror("malloc");
			return EXIT_FAILURE;
		}
	sprintf(snd_buf, "message from %d", d->value);

	//create shared mem object
	int f = shm_open("dev/shmem", O_RDWR, S_IRWXU);
	struct pid_data *share = mmap(0, sizeof(struct pid_data), PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);

	printf("2 reads pid as %d\n", share->pid);

	//connect to message channel
	int id = ConnectAttach(0, share->pid, 1, 0, 0);
	int status = MsgSend(id, snd_buf, 20, rcv_buf, BUFSIZE);

	printf("2 received reply: %s\n", rcv_buf);

	ConnectDetach(id);
	return arg;
}

int main(int argc, char *argv[]) {
	int i;
	printf("Welcome to 2\n");

	set_priority(63);

	// start 4 threads
	pthread_t th[THREADS];
	struct data *ret[THREADS];
	for (i = 0; i < THREADS; i++){
		ret[i] = (struct data *)malloc(sizeof(struct data));
		if(ret[i] == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		ret[i]->value = i;
		if(pthread_create(&th[i],NULL,&thread,ret[i]) !=0) {
			perror("couldn't create thread");
			exit (EXIT_FAILURE);
		}
	}
	//wait for threads
	for( i=0; i < THREADS; i++)
		pthread_join(th[i], NULL);

	return EXIT_SUCCESS;
}
