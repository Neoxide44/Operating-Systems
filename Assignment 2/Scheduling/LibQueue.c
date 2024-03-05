#include "LibQueue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Queue newQueue(int s) {
    Queue q;
    q.array = malloc(s * sizeof(Process *));
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
    qp->array = realloc(qp->array, qp->size * sizeof(Process *));
    assert(qp->array != NULL);

    // Initialize the newly allocated elements
    for (i = oldSize; i < qp->size; i++) {
        qp->array[i] = NULL;
    }

    for (i = 0; i < qp->back; i++) { /* eliminate the split configuration */
        qp->array[oldSize + i] = qp->array[i];
    }
    qp->back = qp->back + oldSize; /* update qp->back */
}

void enqueue(Process *item, Queue *qp) {  // Change parameter type from Process to Process *
    qp->array[qp->back] = item;
    qp->back = (qp->back + 1) % qp->size;
    if (qp->back == qp->front) {
        doubleQueueSize(qp);
    }
}

Process *dequeue(Queue *qp) {  // Change return type from Process to Process *
    Process *item;
    if (isEmptyQueue(*qp)) {
        queueEmptyError();
    }
    item = qp->array[qp->front];
    qp->front = (qp->front + 1) % qp->size;
    return item;
}

void freeQueue(Queue q) {
    if (q.front < q.back)
        for (int i = q.front; i <= q.back; i++)
            free_process(*(q.array[i]));  // Dereference the pointer to free the process

    free(q.array);
}
