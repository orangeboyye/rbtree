#ifndef QUEUE_H_
#define QUEUE_H_

#include "rbtree.h"

struct queue
{
	struct rbnode **arr;
	int size;
	int head;
	int tail;
};

struct queue * new_queue(int size);

void free_queue(struct queue *queue);

int enqueue(struct queue *queue, struct rbnode *node);

struct rbnode * dequeue(struct queue * queue);

int empty(struct queue *queue);

int full(struct queue *queue);

#endif // QUEUE_H_