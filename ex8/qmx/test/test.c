#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

#define BUFSIZE 20

struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
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

int main(int argc, char *argv[]) {
	int i = 0;
	printf("Welcome to 1\n");

	set_priority(30);

	char *snd_buf;
	char *rcv_buf;
	pthread_mutexattr_t myattr;

	snd_buf = malloc(BUFSIZE);
	if (snd_buf == NULL){
		perror("malloc");
		return EXIT_FAILURE;
	}
	rcv_buf = malloc(BUFSIZE);
	if (rcv_buf == NULL){
		perror("malloc");
		return EXIT_FAILURE;
	}
	//create shared mem object
	int f = shm_open("dev/shmem", O_RDWR | O_CREAT, S_IRWXU);
	ftruncate(f, sizeof(struct pid_data));
	struct pid_data *share = mmap(0, sizeof(struct pid_data), PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);

	share->pid = getpid();

	pthread_mutexattr_init(&myattr);
	pthread_mutexattr_setpshared(&myattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&share->pid_mutex, &myattr);

	printf("1 wrote pid as %d\n", share->pid); 	//454693
												//454693

	int id = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);

	while(1) {
		printf("before rcv, priority is %d\n", get_priority());
		int recv_id = MsgReceive(id, rcv_buf, BUFSIZE, NULL);
		printf("after rcv, priority is %d\n", get_priority());

		printf("1 received message %d: %s\n", i++, rcv_buf);

		snd_buf = "reply from 1";
		MsgReply(recv_id, EOK, snd_buf, 12);
	}

	return EXIT_SUCCESS;
}
