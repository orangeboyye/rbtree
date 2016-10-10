#ifndef RBTREE_H_
#define RBTREE_H_

enum color {BLACK, RED};

struct rbnode
{
	int value;
	enum color color;
	struct rbnode *parent;
	struct rbnode *left;
	struct rbnode *right;
};

int insert(struct rbnode **root, int value);

int delete(struct rbnode **proot, int value);

struct rbnode * find(struct rbnode *root, int value);

int depth(struct rbnode *root);

int nodes_count(struct rbnode *root);

void print_rbtree(struct rbnode * root);

int  is_rbtree(struct rbnode *root);

#endif // RBTREE_H_