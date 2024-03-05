#ifndef LIBQUEUE_H
#define LIBQUEUE_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Process.h"

typedef struct Queue {
    Process **array;  // Change from Process * to Process **
    int back;
    int front;
    int size;
} Queue;

Queue newQueue(int s);
int isEmptyQueue(Queue q);
void queueEmptyError();
void doubleQueueSize(Queue *qp);
void enqueue(Process *item, Queue *qp);  // Change parameter type from Process to Process *
Process *dequeue(Queue *qp);             // Change return type from Process to Process *
void freeQueue(Queue q);

#endif
