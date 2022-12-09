#ifndef linkedlist_H
#define linkedlist_H

struct list_node {
	void *data;
	struct list_node *next;
};

struct list {
	struct list_node *head;
	struct list_node *tail;
};


typedef struct list linkedlist;

/*
 * Create an initial empty linkedlist.
 * 
 * @list:	The list to initialize
 */
void list_init(linkedlist **list);

/*
 * Destroy The linkedlist and the data that is contained therein.
 *
 * @list: 	The list to destroy.
 * @free_data: 	The function responsible for destroying the data held within the list nodes
 */
void list_destroy(linkedlist *list, void (*free_data)(void *));

/*
 * Insert a new element into the head of the list
 *
 * @list: 	The list to insert the element into.
 * @data: 	The data that is inserted into the list
 */
void list_insert(linkedlist *list, void *data);

/*
 * Remove the data from the list. If data is NULL then the data at list head is
 * removed.
 *
 * @list: 	The list to search
 * @data:	The data element to remove from the list. 
 * @element: 	element will point to the list element that was removed
 * @compare: 	The compare function that knows how to compare the data
 *  	     	contained in the list
 */
void list_remove(linkedlist *list, const void *data, void **element, 
				int (*compare)(void *key1, void *key2));

/*
 * Search the list for the element specified by data
 *
 * @list:	The list to search.
 * @data: 	The data to search for.
 * @compare: 	The function that knows how to compare the data.
 * 
 * return: 0 if the list contains the data and -1 otherwise.
 */
int list_contains(linkedlist *list, const void *data, 
		int (*compare)(const void *key1, const void *key2));

int list_consistent_item_get(void **item, linkedlist *list, const void *data, 
			int (*consistent)(const void *key1, const void *key2));
/*
 * Return the size of the list
 *
 * @list: 	The list whose size (the number of elements) that will be returned.
 */
int list_size(linkedlist *list);

void list_print(linkedlist *list, void (*print)(void *data));

#endif /* linkedlist.h */ 