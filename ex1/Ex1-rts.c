/*
 ============================================================================
 Name        : Ex1-rts.c
 Author      : Yulong Bai
 Version     :
 Copyright   : All copy rights reserved by the author
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
void allocate(int value){
	int *ptr = NULL;
	ptr = malloc(10000 * sizeof(int));
	*ptr = value;
	printf("test of allocated memory: %i\n",value);
}
int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	allocate(1000);
	return EXIT_SUCCESS;
}