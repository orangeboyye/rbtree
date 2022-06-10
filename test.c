#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rbtree.h"
#include "rbutils.h"


int main(int argc, char const *argv[])
{
	printf("\nUsage: %s [count] \n", argv[0]);
	printf("----------------------------------\n\n");

	int count = 10;
	if (argc > 1)
		count = atoi(argv[1]);
	if (count < 1)
		count = 1;

	printf("create a random uniqe array\n");
	srand(time(NULL));
	int random_ints[count], key, again;
	for (int i = 0; i < count; i++) {
		key = rand()%count + 1, again = 0;
		for (int j = 0; j < i; ++j) 
			if (random_ints[j] == key && (again = 1)) 
				break;
		again ? --i : (random_ints[i] = key);
	}
	// for (int i = 0; i < count; i++)
	// 	random_ints[i] = i;
	for (int i = 0; i < count; i++)
		printf("%2d ", random_ints[i]);
	printf("\n");

	struct rbtree tree = { NULL, 0, 0 };
	print_rbtree(&tree);
	for (int i = 0; i < count; i++){
		insert(&tree, random_ints[i]);
		printf("\ninsert %d into the tree\n", random_ints[i]);
		print_rbtree(&tree);
		if (check_rbtree(&tree)) {
			printf("rbtree valid: √√√\n");
		} else {
			printf("rbtree valid: ✗✗✗\n");
			exit(-1);
		}
	}

	printf("-----------------------------------------------------------\n");
	printf("***********************************************************\n");
	printf("-----------------------------------------------------------\n");

	for (int i = count-1; i >= 0; i--){
		delete(&tree, random_ints[i]);
		printf("\n");
		printf("delete %d in the tree\n", random_ints[i]);
		print_rbtree(&tree);
		if (check_rbtree(&tree)) {
			printf("rbtree valid: √√√\n");
		} else {
			printf("rbtree valid: ✗✗✗\n");
			exit(-1);
		}
	}

		// delete(&tree, 6);
		// printf("\n");
		// printf("delete %d in the tree\n", 6);
		// print_rbtree(&tree);
		// if (check_rbtree(&tree)) {
		// 	printf("rbtree valid: √√√\n");
		// } else {
		// 	printf("rbtree valid: ✗✗✗\n");
		// 	exit(-1);
		// }

		// delete(&tree, 4);
		// printf("\n");
		// printf("delete %d in the tree\n", 4);
		// print_rbtree(&tree);
		// if (check_rbtree(&tree)) {
		// 	printf("rbtree valid: √√√\n");
		// } else {
		// 	printf("rbtree valid: ✗✗✗\n");
		// 	exit(-1);
		// }

		// delete(&tree, 5);
		// printf("\n");
		// printf("delete %d in the tree\n", 5);
		// print_rbtree(&tree);
		// if (check_rbtree(&tree)) {
		// 	printf("rbtree valid: √√√\n");
		// } else {
		// 	printf("rbtree valid: ✗✗✗\n");
		// 	exit(-1);
		// }


	return 0;
}


