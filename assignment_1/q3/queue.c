
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
        queue->rear = cp;
    }
    else
        queue->rear->next = cp;
    queue->rear = cp;  // I almost forgot this line
}

queueElementT Dequeue(queueADT queue) {
    if (QueueIsEmpty(queue))
        return NULL;
    
    queueElementT result = queue->front->customer;
    queue->front = queue->front->next;
    return result;
}

queueElementT QueueFrontPeek(queueADT queue) {
    if ((queue != NULL) && (queue->front != NULL) && (queue->front->customer != NULL))
        return queue->front->customer;
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