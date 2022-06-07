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

struct rbroot
{
	struct rbnode *root;
	int size;
	int depth;
};


int insert(struct rbroot *root, int value);

int delete(struct rbroot *root, int value);

struct rbnode * find(struct rbroot *root, int value);

#endif // RBTREE_H_