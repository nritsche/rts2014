#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

#include <native/sem.h>
#include <native/mutex.h>
#include <native/task.h>
#include <native/timer.h>

#include  <rtdk.h>

#include <stdio.h>
#include <pthread.h>
#include <sched.h>


RT_TASK demo_taskA;
RT_TASK demo_taskC;

int set_cpu(int cpu_number) {
        // setting cpu set to the selected cpu
        cpu_set_t cpu;
        CPU_ZERO(&cpu);
        CPU_SET(cpu_number, &cpu);
        // set cpu set to current thread and return
        return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),
        &cpu);
}

void* disturb(void* input){
        set_cpu(1);
        while(1){volatile int x = 3 +2;}
}

RT_SEM sem;
RT_MUTEX mutexA;
RT_MUTEX mutexB;

void taskL(void *arg)
{
	int id = *(int*) arg;
	set_cpu(0);


	//RT_TASK *curtask = rt_task_self();
	//RT_TASK_INFO curtaskinfo;
	//rt_task_inquire(curtask,&curtaskinfo);
  	rt_printf("Task name : %d \n", id);

	if (rt_sem_p(&sem, TM_INFINITE)) {
		perror("rt_sem_p: ");
		exit(EXIT_FAILURE);
	}
		
	rt_printf("L got through\n");

	if (rt_mutex_acquire(&mutexA, TM_INFINITE)) {
		perror("L couldn't acqu A");
		exit(EXIT_FAILURE);
	}
	rt_task_set_priority(&demo_taskA, 2);
	rt_printf("L locked A\n");
	

	rt_timer_spin(3000000l);
	rt_printf("L waits for B\n");
	if (rt_mutex_acquire(&mutexB, TM_INFINITE)) {
		perror("L couldn't acqu B");
		exit(EXIT_FAILURE);
	}
	rt_task_set_priority(&demo_taskA, 3);
	rt_printf("L locked B\n");

	rt_timer_spin(3000000l);

	rt_printf("L released B\n");
	rt_mutex_release(&mutexB);

	rt_timer_spin(3000000l);

	rt_printf("L released A\n");
	rt_mutex_release(&mutexA);
	rt_task_set_priority(&demo_taskA, 1);

	rt_timer_spin(1000000l);
}
void taskH(void *arg)
{
	int id = *(int*) arg;
	set_cpu(0);


	//RT_TASK *curtask = rt_task_self();
	//RT_TASK_INFO curtaskinfo;
	//rt_task_inquire(curtask,&curtaskinfo);
  	rt_printf("Task name : %d \n", id);

	if (rt_sem_p(&sem, TM_INFINITE)) {
		perror("rt_sem_p: ");
		exit(EXIT_FAILURE);
	}
		
	rt_printf("H got through\n");

	rt_task_sleep(1000000l);

	rt_printf("H waits for B\n");
	if (rt_mutex_acquire(&mutexB, TM_INFINITE)) {
		perror("H couldn't acqu B");
		exit(EXIT_FAILURE);
	}
	rt_task_set_priority(&demo_taskA, 3);
	rt_printf("H locked B\n");
	rt_timer_spin(1000000l);
	
	rt_printf("H waits for A\n");
	if (rt_mutex_acquire(&mutexA, TM_INFINITE)) {
		perror("H couldn't acqu A");
		exit(EXIT_FAILURE);
	}
	rt_printf("H locked A\n");

	rt_timer_spin(2000000l);

	rt_printf("H released A\n");
	if (rt_mutex_release(&mutexA)) {
		perror("rt_sem_v: ");
		exit(EXIT_FAILURE);
	}
	rt_printf("H released B\n");
	if (rt_mutex_release(&mutexB)) {
		perror("rt_sem_v: ");
		exit(EXIT_FAILURE);
	}
	rt_task_set_priority(&demo_taskA, 1);
	rt_timer_spin(1000000l);
}

int main(int argc, char* argv[])
{
  rt_print_auto_init(1);
//	io_init();

  mlockall(MCL_CURRENT|MCL_FUTURE);
/*  if (rt_task_shadow(NULL, "main", 99, T_CPU(0))) {
	perror("rt_task_shadow: ");
	exit(EXIT_FAILURE);
	}
*/
  rt_printf("start task\n");

  /*
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */
  if (rt_task_create(&demo_taskA, "taskL", 0, 1, T_CPU(0)|T_JOINABLE)) {
	perror("rt_task_create: ");
	exit(EXIT_FAILURE);
  }
  if (rt_task_create(&demo_taskC, "taskH", 0, 1, T_CPU(0)|T_JOINABLE)) {
	perror("rt_task_create: ");
	exit(EXIT_FAILURE);
  }
  if (rt_sem_create(&sem, "sem", 0, S_PRIO)) {
	perror("rt_sem_create: ");
	exit(EXIT_FAILURE);
  }
  if (rt_mutex_create(&mutexB, "B")) {
	perror("rt_sem_create: ");
	exit(EXIT_FAILURE);
  }
  if (rt_mutex_create(&mutexA, "A")) {
	perror("rt_sem_create: ");
	exit(EXIT_FAILURE);
  }

  /*
   * Arguments: &task,
   *            task function,
   *            function argument
   */
	int a = 3;
	int b = 1;
	int c = 2;
	rt_task_start(&demo_taskA, &taskL, &a);
	rt_task_start(&demo_taskC, &taskH, &c);

	usleep(100);
	if(rt_sem_broadcast(&sem)) {
		perror("rt_sem_broadcast: ");
		exit(EXIT_FAILURE);
	}
	usleep(5000000);
	if(rt_sem_delete(&sem)) {
		perror("rt_sem_delete: ");
		exit(EXIT_FAILURE);
	}

	if(rt_mutex_delete(&mutexB)) {
		perror("rt_sem_delete: ");
		exit(EXIT_FAILURE);
	}
	if(rt_mutex_delete(&mutexA)) {
		perror("rt_sem_delete: ");
		exit(EXIT_FAILURE);
	}

}

