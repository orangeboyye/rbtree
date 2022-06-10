#include <stdlib.h>
#include "rbtree.h"


static void color_flip_up(struct rbnode *node);
static void color_flip_down(struct rbnode *node);
static void rotate_left(struct rbtree *tree, struct rbnode *y);
static void rotate_right(struct rbtree *tree, struct rbnode *y);
static void insert_fix(struct rbtree *tree, struct rbnode *node);
static void delete_fix(struct rbtree *tree, struct rbnode *node);

struct rbnode * find(struct rbtree *tree, int value)
{
	struct rbnode *p = tree->root;
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

int insert(struct rbtree *tree, int value)
{
	struct rbnode *p = malloc(sizeof(*p));
	if(!p)
		return -1;

	p->value = value;
	p->color = RED;
	p->parent = NULL;
	p->left = NULL;
	p->right = NULL;

	if(!tree->root){
		tree->root = p;
		p->color = BLACK;
		tree->size++;
		return 0;
	}

	struct rbnode *i = tree->root, *parent;
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
	insert_fix(tree, p);
	tree->size++;
	return 0;

}

int delete(struct rbtree *tree, int value)
{
	if(!tree->root)
		return -1;

	struct rbnode *i = tree->root, *j;
	while(i){
		if(value < i->value){
			i = i->left;
		} else if(value > i->value){
			i = i->right;
		} else {
			if(!i->left || !i->right){
				delete_fix(tree, i);
				tree->size--;
				return 0;
			}
			j = i->right;
			while(j->left)
				j = j->left;
			i->value = j->value;
			delete_fix(tree, j);
			tree->size--;
			return 0;
		}
	}
	return -1;
}

void insert_fix(struct rbtree *tree, struct rbnode *node)
{
	struct rbnode *x = node, *y, *tmp;
	while(x->parent){
		y = x->parent; 
		if(x == y->left){
			if(y->color == BLACK)
				return;
			rotate_right(tree, y->parent);
			color_flip_up(y);
			x = y;
		} else {
			if(y->left && y->left->color == RED){
				color_flip_up(y);
				x = y;
			} else {
				rotate_left(tree, y);
				tmp = x, x = y, y = tmp;
				if(y->color == BLACK)
					return;
				rotate_right(tree, y->parent);
				color_flip_up(y);
				x = y;
			}
		}
	}
	tree->root->color = BLACK;
}

void delete_fix(struct rbtree *tree, struct rbnode *node)
{
	if(node->left && node->left->color == RED)
		rotate_right(tree, node);
	if(node->color == RED)
		goto delete;

	struct rbnode *self = node, *parent, *brother, *nephew;
	int red_borrowed = 0;
	while(self->parent){
		parent = self->parent;
		if(self == parent->left){
			brother = parent->right;
			nephew = brother->left;
			if(nephew && nephew->color == RED){
				nephew->color = BLACK;
				rotate_right(tree, brother);
				rotate_left(tree, parent);
				// red_borrowed ? (red_borrowed = 0) : (self->color = RED);
				self->color = RED;
				if(red_borrowed){
					self->color = BLACK;
					red_borrowed = 0;
				}
			} else {
				// red_borrowed ? (red_borrowed = 0) : (self->color = RED);
				self->color = RED;
				if(red_borrowed){
					self->color = BLACK;
					red_borrowed = 0;
				}
				brother->color = RED;
				parent->color == RED ? (parent->color = BLACK) : (red_borrowed = 1);
				rotate_left(tree, parent);
				self = brother;
			}
		} else {
			int rotated = 0;
			if(parent->left && parent->left->color == RED)
				rotate_right(tree, parent), rotated = 1;
			brother = parent->left;
			nephew = brother->left;
			if(nephew && nephew->color == RED){
				nephew->color = BLACK;
				rotate_right(tree, parent);
				red_borrowed ? (red_borrowed = 0) : (self->color = RED);
				if(rotated)
					rotate_left(tree, brother->parent);
			} else {
				red_borrowed ? (red_borrowed = 0) : (self->color = RED);
				brother->color = RED;
				parent->color == RED ? (parent->color = BLACK) : (red_borrowed = 1);
				self = parent;
			}
		}
		if(!red_borrowed)
			break;
	}

	struct rbnode *child;
	delete:
	child = node->left ? node->left : node->right;
	if(node->parent){
		int flag = node == node->parent->left;
		flag ? (node->parent->left = child) : (node->parent->right = child);
	} else {
		tree->root = child;
	}

	if(child)
		child->parent = node->parent;

	free(node);
	return;
	
}

static void color_flip_up(struct rbnode *node)
{
	node->color = RED;
	node->left->color = BLACK;
	node->right->color = BLACK;
}

static void color_flip_down(struct rbnode *node)
{
	node->color = BLACK;
	node->left->color = RED;
	node->right->color = RED;
}

void rotate_left(struct rbtree *tree, struct rbnode *y)
{
	struct rbnode* x = y->right;
	if(!x)
		return;

	x->parent = y->parent;
	if(y->parent){
		int flag = y == y->parent->left;
		flag ? (y->parent->left = x) : (y->parent->right = x);
	} else {
		tree->root = x;
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

void rotate_right(struct rbtree *tree, struct rbnode *y)
{
	struct rbnode *x = y->left;
	if(!x)
		return;

	x->parent = y->parent;
	if(y->parent){
		int flag = y->parent->left == y;
		flag ? (y->parent->left = x) : (y->parent->right = x);
	} else {
		tree->root = x;
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


