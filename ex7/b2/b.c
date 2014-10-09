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
RT_TASK demo_taskB;
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
RT_MUTEX res;

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

	if (rt_mutex_acquire(&res, TM_INFINITE)) {
		perror("rt_sem_p: ");
		exit(EXIT_FAILURE);
	}
	rt_printf("L locked ressource\n");
	
	rt_timer_spin(3000000l);

	rt_printf("L released ressource\n");
	rt_mutex_release(&res);

}
void taskM(void *arg)
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
		
	rt_printf("M got through\n");

	rt_task_sleep(1000000l);
	rt_printf("M woke up and good night again\n");
	rt_timer_spin(5000000l);
	rt_printf("M is done\n");	
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

	rt_task_sleep(2000000l);

	rt_printf("H waits for the ressource\n");
	if (rt_mutex_acquire(&res, TM_INFINITE)) {
		perror("rt_sem_p: ");
		exit(EXIT_FAILURE);
	}
	rt_printf("H locked the ressource\n");
	rt_timer_spin(2000000l);
	
	rt_printf("H released the ressource\n");
	if (rt_mutex_release(&res)) {
		perror("rt_sem_v: ");
		exit(EXIT_FAILURE);
	}
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
  if (rt_task_create(&demo_taskA, "taskL", 0, 5, T_CPU(0)|T_JOINABLE)) {
	perror("rt_task_create: ");
	exit(EXIT_FAILURE);
  }
  if (rt_task_create(&demo_taskB, "taskM", 0, 50, T_CPU(0)|T_JOINABLE)) {
	perror("rt_task_create: ");
	exit(EXIT_FAILURE);
  }
  if (rt_task_create(&demo_taskC, "taskH", 0, 99, T_CPU(0)|T_JOINABLE)) {
	perror("rt_task_create: ");
	exit(EXIT_FAILURE);
  }
  if (rt_sem_create(&sem, "sem", 0, S_PRIO)) {
	perror("rt_sem_create: ");
	exit(EXIT_FAILURE);
  }
  if (rt_mutex_create(&res, "res")) {
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
	rt_task_start(&demo_taskB, &taskM, &b);
	rt_task_start(&demo_taskC, &taskH, &c);

	usleep(100);
	if(rt_sem_broadcast(&sem)) {
		perror("rt_sem_broadcast: ");
		exit(EXIT_FAILURE);
	}
	usleep(1000000);
	if(rt_sem_delete(&sem)) {
		perror("rt_sem_delete: ");
		exit(EXIT_FAILURE);
	}

	if(rt_mutex_delete(&res)) {
		perror("rt_sem_delete: ");
		exit(EXIT_FAILURE);
	}
}
