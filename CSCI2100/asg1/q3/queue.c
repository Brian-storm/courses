/*
 * File: queue.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct cellT{
    queueElementT customer;
    struct cellT *next;
} cellT;

struct queueCDT {
    cellT *front, *rear;
};

queueADT EmptyQueue(void) {
    queueADT queue = (queueADT)malloc(sizeof(*queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void Enqueue(queueADT queue, queueElementT element) {
    cellT *cp = (cellT*)malloc(sizeof(cellT*));
    cp->customer = element;
    cp->next = NULL;

    if (QueueIsEmpty(queue)) {
        queue->front = cp;
    }
    else
        queue->rear->next = cp;
    queue->rear = cp;  // I almost forgot this line
}

queueElementT Dequeue(queueADT queue) {
    if (QueueIsEmpty(queue)) {
        printf("Queue is empty.\n");
        exit(EXIT_FAILURE);
    }
    
    queueElementT result = queue->front->customer;
    queue->front = queue->front->next;
    return result;
}

int QueueLength(queueADT queue) {
    int cnt = 0;
    cellT *cp;

    for (cp = queue->front; cp != NULL; cp = cp->next)
        cnt++;
    
    return cnt;
}

int QueueIsEmpty(queueADT queue) {
    return queue->front == NULL;
}