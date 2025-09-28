/*
 * File: bank.c
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"

/* struct customerCDT for customerADT ... */
struct customerCDT
{
    char customerType;
    int serviceTime;
    float amount;
};

/* There are three counters and three queues ... */
typedef struct counter
{
    int maxQueueLength;
    float netAmountReceived;
    int numberOfCustomersServed;
} counterT;

counterT Counter[3];
queueADT CounterQueue[3];

/* Other functions you need and your main program follow... */


/* Functions Declararion*/
customerADT CreateCustomer(char type, int serviceTime, float amount);
int CheckEmptyQueues(queueADT queues[]);
void ServeCustomer(counterT counters[], queueADT queues[], int[][2]);


int main (void) {
    for (int i = 0; i < 3; i++)
        CounterQueue[i] = EmptyQueue();
    queueADT MainQueue = EmptyQueue();
    
    FILE *fp = fopen("bank.dat", "r");
    if (fp == NULL)
    return 1;
    
    char type, dump;
    int serviceTime;
    float amount;

    int flag = 1;
    while(flag) {
        int fscan_res = fscanf(fp, "%c %d %f", &type, &serviceTime, &amount);
        
        if (fscan_res == 3) {
            fscanf(fp, "%c", &dump); // consume the new line character of the line
            Enqueue(MainQueue, CreateCustomer(type, serviceTime, amount));
        }
        else if (fscan_res == 1) {
            Enqueue(MainQueue, CreateCustomer('D', 0, 0));
        } else {
            flag = 0;  //EOF or error
        }
    }
    
    fclose(fp);
    
    /* Checking the MainQueue*/
    // printf("\n\n\n\n");
    // while (!QueueIsEmpty(MainQueue)) {
    //     customerADT customer = Dequeue(MainQueue);
    //     printf("%c ", customer->customerType);
    //     printf("%d ", customer->serviceTime);
    //     printf("%f\n", customer->amount);
    // }
    
    int totalWait[3][2] = {{0, 0}, {1, 0}, {2, 0}};
    int minuteCnt = 0;
    while(!QueueIsEmpty(MainQueue) || CheckEmptyQueues(CounterQueue) < 3) {
        // printf("\n\n\n\n%d\n", minuteCnt);
        // printf("MainQueue empty %d. #EmptyQueues %d\n", QueueIsEmpty(MainQueue), CheckEmptyQueues(CounterQueue));

        customerADT customer = Dequeue(MainQueue);
        if (customer != NULL) {
            switch (customer->customerType) {
                case 'C': {
                    Enqueue(CounterQueue[0], customer);
                    totalWait['C' - 'A'][0] += (QueueLength(CounterQueue[0]) > 1)? 1: 0;
                    break;
                }
                case 'B': {
                    int idx = (QueueLength(CounterQueue[0]) < QueueLength(CounterQueue[1]))? 0: 1;
                    Enqueue(CounterQueue[idx], customer);
                    totalWait['B' - 'A'][0] += (QueueLength(CounterQueue[idx]) > 1)? 1: 0;
                    break;
                }
                case 'A': {
                    int comp = (QueueLength(CounterQueue[0]) < QueueLength(CounterQueue[1]))? 0: 1;
                    int idx = (QueueLength(CounterQueue[comp]) < QueueLength(CounterQueue[2]))? comp: 2;
                    Enqueue(CounterQueue[idx], customer);
                    totalWait['A' - 'A'][0] += (QueueLength(CounterQueue[idx]) > 1)? 1: 0;
                    break;
                }
                default:
                    break;
            }
        }
        
        for (int i = 0; i < 3; i++)
            totalWait[i][1] += totalWait[i][0];
        
        // Update the counter & total queuing hours
        ServeCustomer(Counter, CounterQueue, totalWait);
        minuteCnt += 1;
        // printf("Empty? MainQueue and #CounterQueue %d %d %d\n", QueueIsEmpty(MainQueue), CheckEmptyQueues(CounterQueue), minuteCnt);
    }


    // Average time Type A customer need to wait (floating point value with 2 decimal places).
    printf("%.2f\n", totalWait['A' - 'A'][1]);

    // Average time Type B customer need to wait (floating point value with 2 decimal places).
    printf("%.2f\n", totalWait['B' - 'A'][1]);

    // Average time Type C customer need to wait (floating point value with 2 decimal places).
    printf("%.2f\n", totalWait['C' - 'A'][1]);

    // Maximum lengths of customer queues for counter 2 (int value).
    printf("%d\n", Counter[2].maxQueueLength);

    // Maximum lengths of customer queues for counter 1 (int value).
    printf("%d\n", Counter[1].maxQueueLength);

    // Maximum lengths of customer queues for counter 0 (int value).
    printf("%d\n", Counter[0].maxQueueLength);

    // Net amount of money of the counter 2 (floating point value with 2 decimal places).
    printf("%.2f\n", Counter[2].netAmountReceived);

    // Net amount of money of the counter 1 (floating point value with 2 decimal places).
    printf("%.2f\n", Counter[1].netAmountReceived);

    // Net amount of money of the counter 0 (floating point value with 2 decimal places).
    printf("%.2f\n", Counter[0].netAmountReceived);

    // Total number of customers counter 2 serves (int value).
    printf("%d\n", Counter[2].numberOfCustomersServed);

    // Total number of customers counter 1 serves (int value).
    printf("%d\n", Counter[1].numberOfCustomersServed);

    // Total number of customers counter 0 serves (int value).
    printf("%d\n", Counter[0].numberOfCustomersServed);

}

customerADT CreateCustomer(char type, int serviceTime, float amount) {
    customerADT customer = (customerADT)malloc(sizeof(*customer));
    if (customer != NULL) {
        customer->customerType = type;
        customer->serviceTime = serviceTime;
        customer->amount = amount;
    }
    return customer;
}

void ServeCustomer(counterT counters[], queueADT queues[], int totalWait[][2]) {
    for (int i = 0; i < 3; i++) {
        int a = QueueLength(queues[i]) - 1;
        int b = counters[i].maxQueueLength;
        counters[i].maxQueueLength = (a > b)? a: b;
        counters[i].maxQueueLength = (counters[i].maxQueueLength < 0)? 0: counters[i].maxQueueLength;
        
        queueElementT customer = QueueFrontPeek(queues[i]);
        if (customer == NULL)
            continue;
        
        printf("Customer %i servicetime %d\n", i, customer->serviceTime);
        if (--customer->serviceTime == 0) {
            Dequeue(queues[i]);
            counters[i].numberOfCustomersServed += 1;
            counters[i].netAmountReceived += customer->amount;
            printf("Counter %d amount %.2f\n", i, counters[i].netAmountReceived);
            
            customerADT customerNext = QueueFrontPeek(queues[i]);
            if (customerNext != NULL)
                totalWait[customerNext->customerType - 'A'][0]--;
        }

    }
}

int CheckEmptyQueues(queueADT queues[]) {
    int cnt = 0;
    for (int i = 0; i < 3; i++) {
        if (QueueIsEmpty(queues[i]))
            cnt++;
    }
    return cnt;
}
