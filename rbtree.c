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
		tree->depth++;
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
	if(tree->root->color == RED){
		tree->root->color = BLACK;
		tree->depth++;
	}
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
		// left child
			brother = parent->right;
			nephew = brother->left;
			if(nephew && nephew->color == RED){
				nephew->color = BLACK;
				rotate_right(tree, brother);
				rotate_left(tree, parent);
				self->color = RED;
				if(red_borrowed){
					red_borrowed = 0;
					self->color = BLACK;
				}
				break;
			} else {
				int old_red_borrowed = red_borrowed;
				red_borrowed = 0;
				if(parent->color == BLACK)
					red_borrowed = 1;
				color_flip_down(parent);
				if(old_red_borrowed)
					self->color = BLACK;
				rotate_left(tree, parent);
				if(red_borrowed){
					self = brother;
					continue;
				} else {
					break;
				}
			}
		// left child end
		} else {
		// right child 
			brother = parent->left;
			nephew = brother->left;
			if(brother && brother->color == RED){
				if(brother->right && brother->right->left && brother->right->left->color == RED){
					struct rbnode *n = brother->right->left;
					rotate_right(tree, parent);
					rotate_right(tree, parent);
					n->color = BLACK;
					self->color = RED;
					if(red_borrowed){
						red_borrowed = 0;
						self->color = BLACK;
					}
					rotate_left(tree, brother);
					break;
				} else {
					rotate_right(tree, parent);
					color_flip_down(parent);
					if(red_borrowed){
						red_borrowed = 0;
						self->color = BLACK;
					}
					break;
				}
			} else if(nephew && nephew->color == RED){
				rotate_right(tree, parent);
				nephew->color = BLACK;
				self->color = RED;
				if(red_borrowed){
					red_borrowed = 0;
					self->color = BLACK;
				}
				break;
			} else {
				int old_red_borrowed = red_borrowed;
				red_borrowed = 0;
				if(parent->color == BLACK)
					red_borrowed = 1;
				color_flip_down(parent);
				if(old_red_borrowed)
					self->color = BLACK;
				if(red_borrowed){
					self = parent;
					continue;
				} else {
					break;
				}
			}
		// right child end
		}
	}

	if(red_borrowed)
		tree->depth--;

	struct rbnode *child;
	delete:
	child = node->left ? node->left : node->right;
	if(node->parent){
		int flag = node == node->parent->left;
		flag ? (node->parent->left = child) : (node->parent->right = child);
	} else {
		tree->root = child;
		if(!tree->root)
			tree->depth--;
	}
	if(child)
		child->parent = node->parent;
	free(node);
}

void delete_fix_compact(struct rbtree *tree, struct rbnode *node)
{
	if(node->left && node->left->color == RED)
		rotate_right(tree, node);
	if(node->color == RED)
		goto delete;

	struct rbnode *x = node, *y, *z, *w;
	int red_borrowed = 0;
	while(x->parent){
		y = x->parent;
		if(x == y->left){
			z = y->right;
			w = z->left;
			if(w && w->color == RED){
				w->color = BLACK;
				red_borrowed ? (red_borrowed = 0) : (x->color = RED);
				rotate_right(tree, z);
				rotate_left(tree, y);
			} else {
				red_borrowed ? (red_borrowed = 0) : (x->color = RED);
				z->color = RED;
				y->color == RED ? (y->color = BLACK) : (red_borrowed = 1);
				rotate_left(tree, y);
				x = z;
			}
		} else {
			int rotated = 0;
			if(y->left && y->left->color == RED)
				rotate_right(tree, y), rotated = 1;
			z = y->left;
			w = z->left;
			if(w && w->color == RED){
				w->color = BLACK;
				red_borrowed ? (red_borrowed = 0) : (x->color = RED);
				rotate_right(tree, y);
				if(rotated)
					rotate_left(tree, z->parent);
			} else {
				red_borrowed ? (red_borrowed = 0) : (x->color = RED);
				z->color = RED;
				y->color == RED ? (y->color = BLACK) : (red_borrowed = 1);
				x = y;
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


