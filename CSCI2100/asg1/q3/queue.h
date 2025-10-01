/*
* File: queue.h
* Bold Lines are modified or newly added.
*/

#include <stdlib.h>

typedef struct queueCDT *queueADT;
typedef struct customerCDT *customerADT;
typedef customerADT queueElementT;

queueADT EmptyQueue(void);
void Enqueue(queueADT, queueElementT);
queueElementT Dequeue(queueADT);
int QueueLength(queueADT);
int QueueIsEmpty(queueADT);