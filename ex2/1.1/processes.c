#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define NUMBER 3

int global = 0;

int main(void)
{
	pid_t id;
	int i;
	int local = 0;

	id = vfork();

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
