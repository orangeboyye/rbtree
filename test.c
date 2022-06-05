#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rbtree.h"


int main(int argc, char const *argv[])
{
	printf("\n");
	printf("----------------------------------\n");
	printf("\n");

	int count = 10;
	if (argc > 1)
		count = atoi(argv[1]);
	if (count < 1)
		count = 1;

	srand(time(NULL));
	int arr[count], key, again;
	for (int i = 0; i < count; ++i) {
		key = rand()%count + 1, again = 0;
		for (int j = 0; j < i; ++j) 
			if (arr[j] == key && (again = 1)) 
				break;
		again ? --i : (arr[i] = key);
	}
	printf("print the array\n");
	for (int i = 0; i < count; ++i)
		printf("%2d ", arr[i]);
	printf("\n");

	struct rbroot root = { NULL };
	for (int i = 0; i < count; ++i){
		insert(&root, arr[i]);
		printf("\ninsert %d into the tree\n", arr[i]);
		print_rbtree(root.node);
		if (is_rbtree(root.node)) {
			printf("the tree is red blcak tree\n");
		} else {
			printf("the tree is NOT red blcak tree!!!\n");
			exit(-1);
		}
	}

	for (int i = 0; i < count; ++i){
		delete(&root, arr[i]);
		printf("\n");
		printf("delete %d in the tree\n", arr[i]);
		print_rbtree(root.node);
		if (is_rbtree(root.node)) {
			printf("the tree is red blcak tree\n");
		} else {
			printf("the tree is NOT red blcak tree!!!\n");
			exit(-1);
		}
	}
	return 0;
}


