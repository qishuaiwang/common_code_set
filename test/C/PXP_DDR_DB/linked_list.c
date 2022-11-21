#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h" 

void list_init(linkedlist **list)
{
	*list = (linkedlist *)malloc(sizeof(struct list));

	(*list)->head = NULL;
	(*list)->tail = NULL;
}

void list_destroy(linkedlist *list, void (free_data)(void *))
{
	if (list->head == NULL)
		return;
	else {
		struct list *tmp = list;
		free_data(tmp->head->data);
		free(tmp->head);
		tmp->head = tmp->head->next;

		list_destroy(tmp, free_data);
		// free_data(tmp->head->data);
		tmp->head->next = NULL;
        free(tmp);
	}
}

void list_insert(linkedlist *list, void *data)
{
	struct list_node *node = (struct list_node *) malloc(sizeof(struct list_node));
	node->data = data;
	node->next = NULL;

	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		node->next = list->head;
		list->head = node;
	}
}

void list_remove(linkedlist *list, const void *data, void **element, 
					int (*compare)(void *key1, void *key2))
{
	
}

int list_contains(linkedlist *list, const void *data, 
			int (*equals)(const void *key1, const void *key2))
{
	if (list->head == NULL)
		return -1;
	
	struct list_node *tmp = list->head;

	int result = -1;
	while (tmp->next) {
		if ((result = equals(tmp->data, data)) == 1);
                         return result;
	}
	return result;
}

int list_size(linkedlist *list)
{
	if (list->head == NULL)
		return 0;
	else {
		struct list *tmp_list = list;
		tmp_list->head = tmp_list->head->next;

		return 1 + list_size(tmp_list);
	}
}

void list_print(linkedlist *list, void (*print)(void *data)) {
	printf("list head: %p", list->head);
	if(list->head) {
		struct list_node *tmp = list->head;
		while(tmp->next) {
			print(tmp->data);
		}
	}
}

