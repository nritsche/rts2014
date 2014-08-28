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
}

void list_insert(list_t list, int index, int data)
{

}

void list_append(list_t list, int data)
{

}

void list_print(list_t list)
{

}

long list_sum(list_t list)
{
	int sum = 0;
	struct node cur = list->head;

	while (cur->next != NULL) {
		sum += cur->data;
		cur = cur->next;
	}

	return sum;
}

int list_get(list_t list, int index)
{
	struct node cur = list->head;
	int i = 0;
	while (i != index) {
		cur = cur->next;
		i++;
		if (cur->next == NULL && i != index)
			return 0;
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
		if (cur->next == NULL && i != index)
			return 0;
	}
	ret = cur->data;

	//remove node
	if (cur->prev == NULL) { //remove head
		list->head = cur->next;
		cur->next->prev = NULL;
	}
	else if (cur->next == NULL) { //remove tail
		list->tail = cur->prev;
		cur->prev->next = NULL;
	}
	else {
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
	}
	free(cur);

	return ret;
}
