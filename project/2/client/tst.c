#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "miniproject.h"

struct udp_conn server;
char buffer[100];

void set_u(double u){

	strcpy(buffer, "SET:");
	sprintf(buffer + 4, "%f", u);

	//printf("u = %f\n", u);

	udp_send(&server, buffer, strlen(buffer) + 1);

}

double get_y(){

	udp_send(&server, "GET", 4);

	//for first task: ignore receiving signal messages
	do{
		udp_receive(&server, buffer, 100);
	}while(buffer[0] != 'G');

	double y = atof(buffer + 8);
	//printf("y = %f\n", y);

	return y;
}

int main(){


	if(udp_init_client(&server, 9999, "192.168.0.1")){
		perror("udp_init_client");
		return -1;
	}

	udp_send(&server, "START", 6);

	int t;
	double i = 0;
	double kp = 10;
	double ki = 800;

	double period = 2000;

	struct timespec next;

        clock_gettime(CLOCK_REALTIME, &next);
	for (t = 0; t < 500000 / period; t++){
                timespec_add_us(&next, period);

		double p = 1 - get_y();
		i += p * period / 1000 / 1000 ;

		set_u(kp * p + ki * i);
                clock_nanosleep(&next);
	}

	udp_send(&server, "STOP", 5);
	udp_close(&server);

	return 0;
}
