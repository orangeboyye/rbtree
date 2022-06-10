#include <stdio.h>
#include "rbtree.h"
#include "queue.h"


static int power(int d, int exp)
{
	int sum = 1;
	for(int i = 0; i < exp; i++)
		sum *= d;
	return sum;
}

static void print_spaces(int length)
{
	for (int i = 0; i < length; ++i)
		printf(" ");
}

static int rb_depth(struct rbnode *root)
{
	if (root == NULL)
		return 0;
	int left  = rb_depth(root->left);
	int right = rb_depth(root->right);
	return 1 + (left > right ? left : right);
}

void print_rbtree(struct rbtree *tree)
{
	if (tree == NULL || tree->root == NULL){
		printf("[ empty tree ]\n");
		return;
	}
	struct rbnode *node = tree->root;
	int depth = rb_depth(node);
	int count = power(2, depth);
	struct queue *queue = new_queue(count);
	struct rbnode *crlf = (struct rbnode *)-1;
	int spaces = count/2;
	int newline = 1;

	enqueue(queue, node);
	enqueue(queue, crlf);
	int i = count + depth;
	while(--i > 0){
		node = dequeue(queue);
		if (node == crlf) {
			printf("\n");
			newline = 1;
			spaces /= 2;
			enqueue(queue, crlf);
			continue;
		}
		print_spaces(spaces-1);
		if (newline)
			newline = 0;
		else
			print_spaces(spaces-1);
		if (node == NULL) {
			printf("  ");
			enqueue(queue, NULL);
			enqueue(queue, NULL);
		} else {
			printf("%s%d", node->color ? "*" : " ", node->value);
			enqueue(queue, node->left);
			enqueue(queue, node->right);
		}
	}
	free_queue(queue);
}

int  check_rbtree(struct rbtree *tree)
{
	if (tree == NULL || tree->root == NULL)
		return 1;
	if (tree->root->color == RED)
		return 0;

	struct rbnode *node = tree->root;
	int depth = rb_depth(node);
	struct queue *queue = new_queue(power(2, depth));
	struct queue *leafs = new_queue(power(2, depth-1));
	enqueue(queue, node);
	while( (node = dequeue(queue)) ){
		if (node->left){
			if (node->color == RED && node->left->color == RED){
				free_queue(queue);
				free_queue(leafs);
				return 0;
			}
			enqueue(queue, node->left);
		}
		if (node->right){
			if (node->right->color == RED){
				free_queue(queue);
				free_queue(leafs);
				return 0;
			}
			enqueue(queue, node->right);
		}
		if (node->left == NULL && node->right == NULL)
			enqueue(leafs, node);
	}
	node = dequeue(leafs);
	int path0 = 0;
	while(node){
		if (node->color == BLACK)
			++path0;
		node = node->parent;
	}
	while( (node = dequeue(leafs)) ){
		int path = 0;
		while(node){
			if (node->color == BLACK)
				++path;
			node = node->parent;
		}
		if (path != path0){
			free_queue(queue);
			free_queue(leafs);
			return 0;
		}
	}
	free_queue(queue);
	free_queue(leafs);
	return 1;
}