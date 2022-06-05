#include <stdio.h>
#include <stdlib.h>
// #include <math.h>
#include "rbtree.h"
#include "queue.h"

static void insert_fix(struct rbroot *root, struct rbnode *node);
static void delete_fix(struct rbroot *root, struct rbnode *node);

int power(int di, int exp)
{
	int sum = 1;
	for(int i = 0; i < exp; i++)
		sum *= di;
	return sum;
}

int insert(struct rbroot *root, int value)
{
	struct rbnode *p = malloc(sizeof(*p));
	if(!p)
		return -1;

	p->value = value;
	p->color = RED;
	p->parent = NULL;
	p->left = NULL;
	p->right = NULL;

	if(!root->node){
		root->node = p;
		p->color = BLACK;
		return 0;
	}

	struct rbnode *i = root->node, *parent;
	int flag;
	while(i){
		parent = i;
		if(value < i->value){
			i = i->left;
			flag = 1;
		} else if(value > i->value){
			i = i->right;
			flag = 0;
		} else {
			free(p);
			return -1;
		}
	}

	p->parent = parent;
	flag ? (parent->left = p) : (parent->right = p);
	insert_fix(root, p);
	return 0;

}

int delete(struct rbroot *root, int value)
{
	if(!root->node)
		return -1;

	struct rbnode *i = root->node, *j;
	while(i){
		if(value < i->value){
			i = i->left;
		} else if(value > i->value){
			i = i->right;
		} else {
			if(!i->left || !i->right){
				delete_fix(root, i);
				return 0;
			}
			j = i->right;
			while(j->left)
				j = j->left;
			i->value = j->value;
			delete_fix(root, j);
			return 0;
		}
	}
	return -1;
}

static void rotate_left(struct rbroot *root, struct rbnode *y);
static void rotate_right(struct rbroot *root, struct rbnode *y);

void insert_fix(struct rbroot *root, struct rbnode *node)
{
	struct rbnode *x = node, *y, *z, *tmp;
	while(x->parent){
		y = x->parent;
		if(x == y->right){
			if(y->left && y->left->color == RED){
				y->left->color = BLACK;
				x->color = BLACK;
				y->color = RED;
				x = y;
				continue;
			}
			rotate_left(root, y);
			tmp = x, x = y, y = tmp;
		}
		if(y->color == BLACK)
			return;
		z = y->parent;
		rotate_right(root, z);
		x->color = BLACK;
		z->color = BLACK;
		y->color = RED;
		x = y;
	}
	x->color = BLACK;
}

void delete_fix(struct rbroot *root, struct rbnode *node)
{
	if(node->left && node->left->color == RED)
		rotate_right(root, node);
	if(node->color == RED)
		goto delete;

	struct rbnode *x = node, *y, *z, *w;
	int borrow_red = 0;
	while(x->parent){
		y = x->parent;
		if(x == y->left){
			z = y->right;
			w = z->left;
			if(w && w->color == RED){
				w->color = BLACK;
				borrow_red ? (borrow_red = 0) : (x->color = RED);
				rotate_right(root, z);
				rotate_left(root, y);
			} else {
				borrow_red ? (borrow_red = 0) : (x->color = RED);
				z->color = RED;
				y->color == RED ? (y->color = BLACK) : (borrow_red = 1);
				rotate_left(root, y);
				x = z;
			}
		} else {
			int rotated = 0;
			if(y->left && y->left->color == RED)
				rotate_right(root, y), rotated = 1;
			z = y->left;
			w = z->left;
			if(w && w->color == RED){
				w->color = BLACK;
				borrow_red ? (borrow_red = 0) : (x->color = RED);
				rotate_right(root, y);
				if(rotated)
					rotate_left(root, z->parent);
			} else {
				borrow_red ? (borrow_red = 0) : (x->color = RED);
				z->color = RED;
				y->color == RED ? (y->color = BLACK) : (borrow_red = 1);
				x = y;
			}
		}
		if(!borrow_red)
			break;
	}

	struct rbnode *child;
	delete:
	child = node->left ? node->left : node->right;
	if(node->parent){
		int flag = node == node->parent->left;
		flag ? (node->parent->left = child) : (node->parent->right = child);
	} else {
		root->node = child;
	}

	if(child)
		child->parent = node->parent;

	free(node);
	return;
	
}

void rotate_left(struct rbroot *root, struct rbnode *y)
{
	struct rbnode* x = y->right;
	if(!x)
		return;

	x->parent = y->parent;
	if(y->parent){
		int flag = y == y->parent->left;
		flag ? (y->parent->left = x) : (y->parent->right = x);
	} else {
		root->node = x;
	}

	y->right = x->left;
	if(x->left)
		x->left->parent = y;

	x->left = y;
	y->parent = x;

	enum color c = x->color;
	x->color = y->color;
	y->color = c;

}

void rotate_right(struct rbroot *root, struct rbnode *y)
{
	struct rbnode *x = y->left;
	if(!x)
		return;

	x->parent = y->parent;
	if(y->parent){
		int flag = y->parent->left == y;
		flag ? (y->parent->left = x) : (y->parent->right = x);
	} else {
		root->node = x;
	}

	y->left = x->right;
	if(x->right)
		x->right->parent = y;

	x->right = y;
	y->parent = x;
	enum color color = x->color;
	x->color = y->color;
	y->color = color;
}

struct rbnode * find(struct rbnode *root, int value)
{
	struct rbnode *p = root;
	while(p){
		if (value == p->value)
			return p;
		if (value < p->value)
			p = p->left;
		else
			p = p->right;
	}
	return NULL;
}

int depth(struct rbnode *root)
{
	if (root == NULL)
		return 0;
	int left = depth(root->left);
	int right = depth(root->right);
	return 1 + (left > right ? left : right);
}

int nodes_count(struct rbnode *root)
{
	if (root == NULL)
		return 0;
	return 1 + nodes_count(root->left) + nodes_count(root->right);
}

static void print_spaces(int length)
{
	for (int i = 0; i < length; ++i)
		printf(" ");
}

void print_rbtree(struct rbnode *root)
{
	if (root == NULL){
		printf("[ empty tree ]\n");
		return;
	}
	int count = power(2, depth(root));
	struct queue *queue = new_queue(count);
	struct rbnode *crlf = (struct rbnode *)-1;
	int spaces = count/2;
	int newline = 1;
	enqueue(queue, root);
	enqueue(queue, crlf);
	int i = count + depth(root);
	while(i-- > 0){
		root = dequeue(queue);
		if (root == crlf) {
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
		if (root == NULL) {
			printf("  ");
			enqueue(queue, NULL);
			enqueue(queue, NULL);
		} else {
			printf("%s%d", root->color ? "*" : " ", root->value);
			enqueue(queue, root->left);
			enqueue(queue, root->right);
		}
	}
	free_queue(queue);
}

int  is_rbtree(struct rbnode *root)
{
	if (root == NULL)
		return 1;
	if (root->color == RED)
		return 0;
	struct queue *queue = new_queue(power(2, depth(root)));
	struct queue *leafs = new_queue(power(2, depth(root)-1));
	enqueue(queue, root);
	while(!empty(queue)){
		root = dequeue(queue);
		if (root->left){
			if (root->color == RED && root->left->color == RED){
				free_queue(queue);
				free_queue(leafs);
				return 0;
			}
			enqueue(queue, root->left);
		}
		if (root->right){
			if (root->right->color == RED){
				free_queue(queue);
				free_queue(leafs);
				return 0;
			}
			enqueue(queue, root->right);
		}
		if (root->left == NULL && root->right == NULL)
			enqueue(leafs, root);
	}
	root = dequeue(leafs);
	int path0 = 0;
	while(root){
		if (root->color == BLACK)
			++path0;
		root = root->parent;
	}
	while((root = dequeue(leafs)) != NULL){
		int path = 0;
		while(root){
			if (root->color == BLACK)
				++path;
			root = root->parent;
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

