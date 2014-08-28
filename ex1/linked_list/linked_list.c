#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{
	list_t ptr = NULL;
	ptr = malloc(sizeof(struct list));
	if(!ptr){
		perror("list creation error");
		return NULL;
	}
	ptr->head = NULL;
	ptr->tail = NULL;
	ptr->length = 0;
	return ptr;
}

void list_delete(list_t list)
{
/*entry test*/
	if(!list){
		perror("the argument invalid");
	}
	struct node * current = list->head;
	while(current->next){
		// next is not null, or current not tail
		current=
		free()
	}
}

void list_insert(list_t list, int index, int data)
{
	if(!list){
			perror("the argument invalid");
			return;
		}
	struct node * current = list->head;
	struct node * new = malloc(sizeof(struct node));
	if(!new){
		perror(" node creating errror1");
		return;
	}
	new->data = data;
	// get the position to insert
	for(int i = 0; i < index; i++){
		current = current->next;
	}
	current->next->prev = new;
	new->next = current->next;
	current->next = new;
	new->prev = current;
	list->length++;
	current =  NULL;/* tangle pointer */
}

void list_append(list_t list, int data)
{

}

void list_print(list_t list)
{
	struct node cur = list->head;

	while (cur->next != NULL) {
		printf ("%d ", cur->data);
		cur = cur->next;
	}
	printf ("\n");
}

long list_sum(list_t list)
{

}

int list_get(list_t list, int index)
{
	struct node cur = list->head;
	int i = 0;
	while (i != index) {
		cur = cur->next;
		i++;
	}
	return cur->data;
}

int list_extract(list_t list, int index)
{
	int ret;
	struct node cur = list->head;
	int i = 0;
	while (i != index) {
		cur = cur->next;
		i++;
	}
	ret = cur->data;

	//remove node
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	free(cur);

	return ret;
}
