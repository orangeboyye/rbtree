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

struct rbtree
{
	struct rbnode *root;
	int size;
	int depth;
};


int insert(struct rbtree *tree, int value);

int delete(struct rbtree *tree, int value);

struct rbnode * find(struct rbtree *tree, int value);

#endif // RBTREE_H_