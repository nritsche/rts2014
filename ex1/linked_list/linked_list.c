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
	if(!list){
			perror("the argument invalid");
			return;
		}
	struct node * current = list->head;
	for(int i = 0; i<= index; i++){
		current = current->next;
	}
	current->data = data;
	current =  NULL;/* tangle pointer */
}

void list_append(list_t list, int data)
{
	if (list == NULL) {
		perror ("list_append: argument list invalid");
		return;
	}

	struct node * new_node = malloc (sizeof(struct node));

	if (new_node == NULL) {
		perror ("list_append: malloc: out of memory");
		return;
	}

	new_node->data = data;
	list->tail->next = new_node;
	list->tail = new_node;
}

void list_print(list_t list)
{
	struct node * cur = list->head;

	while (cur->next != NULL) {
		printf ("%d ", cur->data);
		cur = cur->next;
	}
	printf ("\n");
}

long list_sum(list_t list)
{
	int sum = 0;
	struct node * cur = list->head;
	while (cur->next != NULL) {
		sum += cur->data;
		cur = cur->next;
	}
	return sum;
}

int list_get(list_t list, int index)
{
	struct node * cur = list->head;
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
	struct node * cur = list->head;
	int i = 0;
	while (i != index) {
		if (cur->next == NULL) {
			perror("list_extract: index out of bounds");
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
