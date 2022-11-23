#include <stdio.h>
#include "linkedlist.h"

struct fraction {
	int num;
	int denom;
};

void print(void *data)
{
	printf("Node Data: %i\n", ((struct fraction *)data)->num);
}

int compare(const void *key1, const void *key2)
{
	printf("weeeeee");
	return ((struct fraction *)key1)->num - ((struct fraction *)key2)->num;
}

int main(int argc, char **argv)
{
	linkedlist *list;
	list_init(&list);

	struct fraction f1 = {1, 2};
	struct fraction f2 = {3, 4};

	list_insert(list, &f1);
	list_insert(list, &f2);

	printf("list size: %i\n", list_size(list));
	printf("linkedlist contains f1: %s\n", list_contains(list, &f1, &compare) == 0 ? "YES" : "NO");
	list_print(list, &print);
}