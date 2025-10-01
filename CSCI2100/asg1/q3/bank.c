/*
 * File: bank.c
 */

#include <stdio.h>
#include <string.h>
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
int CheckEmptyQueues(void);
customerADT CreateCustomer(char type, int serviceTime, float amount);
void UpdateCounter(customerADT customer, int remainingTime[3], int customerTrack[][3]);
void ServeCustomer(int remainingTime[3], int customerTrack[][3]);
void AnnounceResult(int customerTrack[][3]);


int main (void) {
    for (int i = 0; i < 3; i++) {
        Counter[i].maxQueueLength = 0;
        Counter[i].netAmountReceived = 0;
        Counter[i].numberOfCustomersServed = 0;
        CounterQueue[i] = EmptyQueue();
    }
    
    FILE *fp = fopen("bank.dat", "r");
    if (fp == NULL)
        return 1;
    
    char type, dump;
    int serviceTime;
    float amount;

    int isReadFile = 1;
    int minuteCnt = 0;
    int remainingTime[3] = {0}; // (Categorized by counterIdx)
    int customerTrack[3][3] = {0};  //#totalQueuingTime, #curTotalCustomer, #overallTotalCustomer (Categorized by customerType)

    while(isReadFile || CheckEmptyQueues() != 3) {
        // printf("\n");
        // printf("ReadFile? #EmptyQueues? %d %d\n", isReadFile, CheckEmptyQueues());

        // Read file
        int fscan_res = fscanf(fp, "%c %d %f", &type, &serviceTime, &amount);
        // printf("MinuteCnt %d th => ", minuteCnt);
        
        if (isReadFile) {
            customerADT customer = NULL;
            
            if (fscan_res == 3) {
                fscanf(fp, "%c", &dump); // consume the new line character in the end of the line
                customer = CreateCustomer(type, serviceTime, amount);
                // printf("entry of type %c\n", customer->customerType);
            } else if (fscan_res == 1) {
                // printf("no one enters\n", minuteCnt);
            } else if (fscan_res == EOF) {
                isReadFile = 0;
                // printf("EOF!\n");
            } else {
                exit(EXIT_FAILURE);
            }

            if (customer != NULL) {
                UpdateCounter(customer, remainingTime, customerTrack);
            }
        } else {
            fclose(fp);
        }

        // Update CounterQueue and total queuing hours
        ServeCustomer(remainingTime, customerTrack);
        // printf("Counter012 remainingTime %d %d %d\n", remainingTime[0], remainingTime[1], remainingTime[2]);

        minuteCnt += 1;
    }
    
    AnnounceResult(customerTrack);
    return 0;
}

int CheckEmptyQueues() {
    int cnt = 0;
    for (int i = 0; i < 3; i++) {
        if (QueueIsEmpty(CounterQueue[i]))
            cnt++;
    }
    return cnt;
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

void UpdateCounter(customerADT customer, int remainingTime[3], int customerTrack[][3]) {
    // Find their corresponding counterIdx
    int counterIdx = 0;
    if (customer->customerType == 'B') {
        counterIdx = (QueueLength(CounterQueue[0]) < QueueLength(CounterQueue[1]))? 0: 1;
    } else if (customer->customerType == 'A') {
        int comp = (QueueLength(CounterQueue[0]) < QueueLength(CounterQueue[1]))? 0: 1;
        counterIdx = (QueueLength(CounterQueue[comp]) < QueueLength(CounterQueue[2]))? comp: 2;
    }

    // Assign the queuing time if queue is empty
    if (QueueIsEmpty(CounterQueue[counterIdx])) {
        remainingTime[counterIdx] =  customer->serviceTime;
    }

    int typeIdx = customer->customerType - 'A';
    
    // Enqueue and Update Counter
    Enqueue(CounterQueue[counterIdx], customer);
    int queueLength = QueueLength(CounterQueue[counterIdx]) - 1;
    if (queueLength > Counter[counterIdx].maxQueueLength) {
        Counter[counterIdx].maxQueueLength = queueLength;
    }
    
    // Track the data related to waiting
    if (QueueLength(CounterQueue[counterIdx]) > 1) {
        customerTrack[typeIdx][1]++;  // track #curTotalCustomer
    }
    customerTrack[typeIdx][2]++;  // overall number of customer increases
}

void ServeCustomer(int remainingTime[3], int customerTrack[][3]) {
    // Looping each customerType
    for (int i = 0; i < 3; i++) {
        customerTrack[i][0] += customerTrack[i][1];  // [i][1] people waits for [i][0] minutes in total
    }

    // Looping each counter
    customerADT customer = NULL;
    for (int i = 0; i < 3; i++) {
        if ((remainingTime[i] > 0) && (--remainingTime[i] == 0)) {
            // printf("Done queuing!\n", remainingTime[i]);
            customerADT firstCustomer = Dequeue(CounterQueue[i]);

            if (!QueueIsEmpty(CounterQueue[i])) {
                customerADT secondCustomer = Dequeue(CounterQueue[i]);
                remainingTime[i] = secondCustomer->serviceTime;
                customerTrack[secondCustomer->customerType - 'A'][1]--;
                
                // Restore the original queue
                int len = QueueLength(CounterQueue[i]);
                Enqueue(CounterQueue[i], secondCustomer);
                for (int _ = 0; _ < len; _++) {
                    Enqueue(CounterQueue[i], Dequeue(CounterQueue[i]));
                }
            }
            
            Counter[i].netAmountReceived += firstCustomer->amount;
            Counter[i].numberOfCustomersServed++;
            // printf("Counter %d amount %.2f\n", i, counters[i].netAmountReceived);

        }
    }
    // printf("A customerTrack: %d %d %d\n", customerTrack[0][0], customerTrack[0][1], customerTrack[0][2]);
    // printf("B customerTrack: %d %d %d\n", customerTrack[1][0], customerTrack[1][1], customerTrack[1][2]);
    // printf("C customerTrack: %d %d %d\n", customerTrack[2][0], customerTrack[2][1], customerTrack[2][2]);
}

void AnnounceResult(int customerTrack[][3]) {
   // Average time Type A~C customer need to wait (floating point value with 2 decimal places).
    for (int i = 0; i < 3; i++) {
        int divisor = (customerTrack[i][2])? customerTrack[i][2]: 1;
        printf("%.2f\n", (float)customerTrack[i][0] / divisor);
    }

    // Maximum lengths of customer queues for counter 2 (int value).
    for (int i = 2; i >= 0; i--) {
        printf("%d\n", Counter[i].maxQueueLength);
    }
    
    // Net amount of money of the counter 2 (floating point value with 2 decimal places).
    for (int i = 2; i>= 0; i--) {
        printf("%.2f\n", Counter[i].netAmountReceived);
    }

    // Total number of customers counter 2 serves (int value).
    for (int i = 2; i >= 0; i--) {
        printf("%d\n", Counter[i].numberOfCustomersServed);    
    }
}
