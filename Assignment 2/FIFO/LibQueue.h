/* based on queueTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#ifndef LIBQUEUE_H
#define LIBQUEUE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Queue {
    int *array;
    int back;
    int front;
    int size;
} Queue;

Queue newQueue(int s);
int isEmptyQueue(Queue q);
void queueEmptyError();
void doubleQueueSize(Queue *qp);
void enqueue(int item, Queue *qp);
int dequeue(Queue *qp);
void freeQueue(Queue q);
int searchQueue(int item, Queue q);
void printQueue(Queue q);

#endif
