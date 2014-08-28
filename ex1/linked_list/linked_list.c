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
		perror("list_delete: the argument invalid");
	}
	struct node * cur = list->head;

	if (cur == NULL)
		return;

	while (cur->next != NULL) {
		cur = cur->next;
		free(cur->prev);
	}

	free (cur);
	free (list);

	return;
}

void list_insert(list_t list, int index, int data)
{
	int i;

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
	if(current == NULL && index != 0) {
		perror ("list_insert: index out of bounds");
		return;
	}
	// get the position to insert
	for(i = 0; i<= index; i++){
		if (current->next == NULL) {
			perror ("list_insert: index out of bounds");
			return;
		}
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
	if (list == NULL) {
		perror ("list_append: argument list invalid");
		return;
	}

	struct node * new_node = malloc (sizeof(struct node));

	if (new_node == NULL) {
		perror ("list_append: malloc: out of memory");
		return;
	}

	if (list->head == NULL) {
		list->head = new_node;
		list->tail = list->head;
	}
	else {
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
	}

	new_node->next = NULL;
	new_node->data = data;
	list->length++;

	new_node = NULL;
}

void list_print(list_t list)
{
	struct node * cur = list->head;

	if (cur == NULL) {
		printf("list empty\n");
		return;
	}

	while (cur->next != NULL) {
		printf ("%d ", cur->data);
		cur = cur->next;
	}
	printf ("\n");

	cur = NULL;
}

long list_sum(list_t list)
{
	int sum = 0;
	struct node * cur = list->head;

	if (cur == NULL)
		return 0;

	while (cur->next != NULL) {
		sum += cur->data;
		cur = cur->next;
	}

	cur = NULL;
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
			return 0;
		}
		cur = cur->next;
		i++;
	}
	ret = cur->data;

	//remove node
	if (cur->prev == NULL)
		list->head = cur->next;
	else
		cur->prev->next = cur->next;
	if (cur->next == NULL)
		list->tail = cur->prev;
	else
		cur->next->prev = cur->prev;

	free(cur);
	list->length--;

	cur = NULL;
	return ret;
}
