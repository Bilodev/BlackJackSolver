#include "queue.h"

#include <stdlib.h>

#define MAXQUEUE 100

struct queue {
    struct animation arr[MAXQUEUE];
    int head;
    int tail;
};

struct queue* newQueue()
{
    struct queue* q = malloc(sizeof(struct queue));
    q->head = q->tail = 0;
    return q;
}
int isEmptyQueue(struct queue* q) { return q->head == q->tail; }

int enqueue(struct queue* q, struct animation* i)
{
    if ((q->tail + 1) % MAXQUEUE == q->head) return 0;
    q->arr[q->tail] = *i;
    q->tail = (q->tail + 1) % MAXQUEUE;
    return 1;
}
struct animation* dequeue(struct queue* q)
{
    if (isEmptyQueue(q)) return NULL;
    return &q->arr[q->head++];
}

struct animation* front(struct queue* q)
{
    if (isEmptyQueue(q)) return NULL;
    return &q->arr[q->head];
}
