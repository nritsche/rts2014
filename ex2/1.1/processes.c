#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define NUMBER 3
#define VFORK 0

int global = 0;

int main(void)
{
	pid_t id;
	int i;
	int local = 0;

#if VFORK
	id = vfork();
#else
	id = fork();
#endif

	if (id == -1) {
		perror ("fork failed");
		return EXIT_FAILURE;
	}

	if (id == 0) { //child
		for (i = 0; i < NUMBER; i++) {
			global++;
			local++;
			printf("child: global=%d, local=%d\n", global, local);
		}
		_exit(0);
	}
	else {			//parent
		for (i = 0; i < NUMBER; i++) {
			global++;
			local++;
			printf("parent: global=%d, local=%d\n", global, local);
		}
	}
	return EXIT_SUCCESS;
}
