/* based on queueTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#include "LibQueue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Queue newQueue(int s) {
    Queue q;
    q.array = calloc(s, sizeof(int));
    assert(q.array != NULL);
    q.back = 0;
    q.front = 0;
    q.size = s;
    return q;
}

int isEmptyQueue(Queue q) {
    return (q.back == q.front);
}

void queueEmptyError() {
    printf("queue empty\n");
    abort();
}

void doubleQueueSize(Queue *qp) {
    int i;
    int oldSize = qp->size;
    qp->size = 2 * oldSize;
    int *newArray = calloc(qp->size, sizeof(int));  // Initialize new memory

    assert(newArray != NULL);

    // Copy elements from the old array to the new array
    for (i = 0; i < oldSize; i++) {
        newArray[i] = qp->array[(qp->front + i) % oldSize];
    }

    free(qp->array);       // Free the old array
    qp->array = newArray;  // Update the array pointer

    qp->front = 0;
    qp->back = oldSize;  // Update qp->back
}

void enqueue(int item, Queue *qp) {
    qp->array[qp->back] = item;
    qp->back = (qp->back + 1) % qp->size;
    if (qp->back == qp->front) {
        doubleQueueSize(qp);
    }
}

int dequeue(Queue *qp) {
    int item;
    if (isEmptyQueue(*qp)) {
        queueEmptyError();
    }
    item = qp->array[qp->front];
    qp->array[qp->front] = -1;
    qp->front = (qp->front + 1) % qp->size;
    return item;
}

void freeQueue(Queue q) {
    free(q.array);
}

int searchQueue(int item, Queue q) {
    int i;
    int index = q.front;

    for (i = 0; i < q.size; i++) {
        if (q.array[index] == item) {
            return 1;  // item found
        }
        index = (index + 1) % q.size;
    }

    return 0;  // item not found
}

void printQueue(Queue q) {
    int i;
    int index = q.front;

    printf("Queue: ");
    for (i = 0; i < q.size; i++) {
        printf("%d ", q.array[index]);
        index = (index + 1) % q.size;
    }
    printf("\n");
}
