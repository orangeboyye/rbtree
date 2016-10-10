#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct queue * new_queue(int size)
{
	struct queue *queue = malloc(sizeof(struct queue));
	if (!queue)
		return NULL;
	queue->arr = malloc(sizeof(struct rbnode) * (size+1));
	if (!queue->arr) {
		free(queue);
		return NULL;
	}
	queue->size = size + 1;
	queue->head = 0;
	queue->tail = 0;
	return queue;
}

void free_queue(struct queue *queue)
{
	free(queue->arr);
	free(queue);
}

void print_queue(struct queue *queue)
{
	for (int i = queue->tail; i != queue->head; i = (i+1)%queue->size)
		printf("%2d ", queue->arr[i]->value);
	printf("\n");
}

int enqueue(struct queue *queue, struct rbnode *node)
{
	if (full(queue)){
		struct rbnode **arr = malloc(sizeof(struct rbnode *)*queue->size*2);
		// print_queue(queue);
		struct rbnode *tmp = dequeue(queue);
		int i = 0;
		while(tmp){
			arr[i++] = tmp;
			tmp = dequeue(queue);
		}
		free(queue->arr);
		queue->arr = arr;
		queue->size *= 2;
		queue->tail = 0;
		queue->head = i;
	}
	queue->arr[queue->head] = node;
	queue->head = (queue->head+1)%queue->size;
	// print_queue(queue);
	return 0;
}

struct rbnode * dequeue(struct queue * queue)
{
	if (empty(queue))
		return NULL;
	struct rbnode *tmp = queue->arr[queue->tail];
	queue->tail = (queue->tail+1)%queue->size;
	return tmp;
}

int empty(struct queue *queue)
{
	return queue->tail == queue->head;
}

int full(struct queue *queue)
{
	return (queue->head+1)%queue->size == queue->tail;
}