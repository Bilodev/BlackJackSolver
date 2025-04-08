#include "player.h"

typedef struct queue* Queue;

struct animation {
    int card;
    int ddx;
    int ddy;
    int dx;
    int dy;
    Player dest;
    int dest_hand;
};

Queue newQueue();
int isEmptyQueue(Queue);
int enqueue(Queue, struct animation*);
struct animation* dequeue(Queue);
struct animation* front(Queue q);