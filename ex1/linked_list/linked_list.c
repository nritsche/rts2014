#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{

}

void list_delete(list_t list)
{

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
