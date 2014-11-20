#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "miniproject.h"

pthread_mutex_t snd_mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t pi_sem;
sem_t resp_sem;


struct udp_conn server;

char buffer_recv[100];
char buffer_send[100];

int flag_run = 1;
double y = 0;

static void *thread_udp (void *arg) {

    // send get
    
    if(pthread_mutex_lock(&snd_mutex)){
            perror("pthread lock");
            exit(-1);
    }
    udp_send(&server, "GET", 4);
    if(pthread_mutex_unlock(&snd_mutex)){
            perror("pthread unlock");
            exit(-1);
    }

	while (flag_run) {
	         	    
		udp_receive(&server, buffer_recv, 100);
		//printf("recieved: %s\n", buffer_recv);
		
		if(buffer_recv[0] == 'G'){
		    y = atof(buffer_recv + 8);
		    // wake up pi 
		    if(sem_post(&pi_sem)){
                perror("sem unlock");
                exit(-1);
            }
	    }
	    else{
	        // wake up response
	        if(sem_post(&resp_sem)){
                perror("sem unlock");
                exit(-1);
            }
	    }
	}
	if(sem_post(&resp_sem)){
        perror("sem unlock");
        exit(-1);
    }
    if(sem_post(&pi_sem)){
        perror("sem unlock");
        exit(-1);
    }
	return NULL;
}

static void *thread_pi (void *arg) {

    static double i = 0;
    
    double period = 4000;
    double kp = 10;
    double ki = 800;
    
    struct timespec next;
    
    clock_gettime(CLOCK_REALTIME, &next);
    
    while(1){
    
        if(pthread_mutex_lock(&snd_mutex)){
            perror("pthread lock");
            exit(-1);
        }
        
		udp_send(&server, "GET", 4);
		
		if(pthread_mutex_unlock(&snd_mutex)){
            perror("pthread unlock");
            exit(-1);
        }
        
        //wait
        if(sem_wait(&pi_sem)){
                perror("sem lock");
                exit(-1);
        }
        
        if(!flag_run)
            return NULL;
           
        double p = 1 - y;
        
        i += p * period / 1000 / 1000;
        
        float u = kp * p + ki * i;
        
        strcpy(buffer_send, "SET:");
        sprintf(buffer_send + 4, "%f", u);

        if(pthread_mutex_lock(&snd_mutex)){
            perror("pthread lock");
            exit(-1);
        }
   
        udp_send(&server, buffer_send, strlen(buffer_send) + 1);
        if(pthread_mutex_unlock(&snd_mutex)){
            perror("pthread unlock");
            exit(-1);
        }
        
        // time wait
        timespec_add_us(&next, period);
        clock_nanosleep(&next);
    }
}

static void *thread_respond (void *arg) {
    
    
    while(1){
    
        // wait for signal to wake up
        if(sem_wait(&resp_sem)){
                perror("sem lock");
                exit(-1);
        }
        
        if(!flag_run)
            return NULL;
        
        if(pthread_mutex_lock(&snd_mutex)){
            perror("pthread lock");
            exit(-1);
        }
        
		udp_send(&server, "SIGNAL_ACK", 11);
		
		if(pthread_mutex_unlock(&snd_mutex)){
            perror("pthread unlock");
            exit(-1);
        }
        
     }
}


pthread_t thread[3];

int main(){

    if (sem_init(&resp_sem,0,0)) {
        perror("sem_init");
        exit (-1);
    }
    if (sem_init(&pi_sem,0,0)) {
        perror("sem_init");
        exit (-1);
    }
    
    if(udp_init_client(&server, 9999, "192.168.0.1")){
		perror("udp_init_client");
		return -1;
	}

	udp_send(&server, "START", 6);
    pthread_create(&thread[0],NULL, thread_udp, NULL);
    pthread_create(&thread[1],NULL, thread_pi, NULL);
    pthread_create(&thread[2],NULL, thread_respond, NULL);
   
	
    int t;
	struct timespec next;
    float period = 5 * 1000;
    clock_gettime(CLOCK_REALTIME, &next);
	for (t = 0; t < 1000; t++){
                timespec_add_us(&next, period);
                clock_nanosleep(&next);
	}
	
    
    // signaling all the thread to exit joining?
	flag_run = 0;
    int i;
    for( i=0; i < 3; i++)
	    pthread_join(thread[i], NULL);
	    
	
	udp_send(&server, "STOP", 5);
	udp_close(&server);
	return 0;
}
