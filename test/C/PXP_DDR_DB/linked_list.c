#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h" 

void list_init(linkedlist **list)
{
	*list = (linkedlist *)malloc(sizeof(struct list));

	(*list)->head = NULL;
	(*list)->tail = NULL;
}
#if 0
void list_destroy(linkedlist *list, void (free_data)(void *))
{
	if (list->head == NULL)
		return;
	else {
		struct list *tmp = list;
		tmp->head = tmp->head->next;

		list_destroy(tmp, free_data);
		free_data(tmp->head->data);
		tmp->head->next = NULL;
	}
}
#else
void list_destroy(linkedlist *list, void (free_data)(void *))
{
	if (list->head == NULL) {
        free(list);
		return;
	}
	else {
		struct list *tmp = list;
		struct list_node *head = list->head->next;
		free_data(tmp->head->data);
		free(tmp->head);
		tmp->head = head;

		list_destroy(tmp, free_data);
		// free_data(tmp->head->data);
		// tmp->head->next = NULL;
		// tmp->tail = NULL;
	}
}
#endif
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
extern int equals(const void *x, const void *y);
void list_remove_node(linkedlist *list, struct list_node *node)
{
	if (list_contains(list, node->data, equals)) {
		if(list->head) {
			struct list_node *tmp = list->head;
			while(tmp) {
				if (node == tmp->next) {
					tmp->next = node->next;
					free(node);
					break;
				}
				tmp = tmp->next;
			}
		}
	} else {
		perror("This node is not contain in the lsit.\n");
	}
}

void list_remove(linkedlist *list, const void *data, void **element, 
					int (*equals)(void *key1, void *key2))
{
	
}

int list_contains(linkedlist *list, const void *data, 
			int (*equals)(const void *key1, const void *key2))
{
	if (list->head == NULL)
		return -1;
	
	struct list_node *tmp = list->head;

	int result = -1;
	do{
		if ((result = equals(tmp->data, data)) == 1);
                         return result;
		tmp = tmp->next;
	}while (tmp);
	return result;
}

int list_consistent_item_get(void **item, linkedlist *list, const void *data, 
			int (*consistent)(const void *key1, const void *key2))
{
	if (list->head == NULL)
		return -1;
	
	struct list_node *tmp = list->head;

	int result = -1;
	do{
		if ((result = consistent(tmp->data, data)) == 1){
			*item = tmp;
            return result;
		}
		tmp = tmp->next;
	}while (tmp);
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
		while(tmp) {
			print(tmp->data);
			tmp = tmp->next;
		}
	}
}

