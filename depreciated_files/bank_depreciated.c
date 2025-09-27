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

/* Functions Declararion*/
customerADT CreateCustomer(char type, int serviceTime, float amount);
int CheckAllEmpty(queueADT queues[]);

counterT Counter[3];
queueADT CounterQueue[3];

/* Other functions you need and your main program follow... */

int main (void) {
    for (int i = 0; i < 3; i++)
        CounterQueue[i] = EmptyQueue();
    int (*DoneFlag)(queueADT[]);
    DoneFlag = CheckAllEmpty;
    queueADT MainQueue =EmptyQueue();

    
    FILE *fp = fopen("bank.dat", "r");
    if (fp == NULL)
    return 1;
    
    char ch;
    char *line;
    size_t line_len = 0;
    size_t line_size = 16;
    
    char type;
    int serviceTime;
    float amount;

    int count = 0;
    while(count < 10) {
        printf("%d\n", fscanf(fp, "%c %d %f", &type, &serviceTime, &amount));
        printf("%c %d %f\n\n", type, serviceTime, amount);
        count++;
    }

    while (fscanf(fp, "%c", &ch) != EOF) {
        if (ch == '\n') {
            line[line_len] = '\0';

            if (!strlen(line)) {
                Enqueue(MainQueue, CreateCustomer('D', 0, 0));
                continue;
            }
            
            printf("%s\n", line);
            printf("%c\n", line[0]);
            printf("%c\n", line[1]);

            char type = line[0];
            int i, space_idx = 2, row, col;
            char tmp[2][line_len];

            for (i = 2; line[i] != '\0'; i++) {
                printf("for loop %i: __%c__\n", i, line[i]);
                if (line[i] == 32) {
                    printf("space! at %d\n", i);
                    space_idx = i;
                    tmp[0][i] = '\0';
                    continue;
                }
                
                row = (space_idx == 2)? 0: 1;
                col = (row == 0)? (i -2): (i - space_idx - 1);
                tmp[row][col] = line[i];
            }
            
            tmp[1][i-space_idx-1] = '\0';
            
            
            printf("Testing: %c %s %s\n\n", type, tmp[0], tmp[1]);
            int serviceTime = atoi(tmp[0]);
            float amount = atof(tmp[1]);
            Enqueue(MainQueue, CreateCustomer(type, serviceTime, amount));
            
            line_len = 0;
            line_size = 16;
        }

        if (line_len + 1 >= line_size) {
            line_size += 16;
            line = realloc(line, line_size);
        }
    
        line = (char*)realloc(line, sizeof(line) + sizeof(char));
        line[line_len++] = ch;
    }
    fclose(fp);
    
}

customerADT CreateCustomer(char type, int serviceTime, float amount) {
    customerADT customer = (customerADT)malloc(sizeof(*customer));
    printf("Creating... %c %d %f", type, serviceTime, amount);
    if (customer != NULL) {
        customer->customerType = type;
        customer->serviceTime = serviceTime;
        customer->amount = amount;
    }
    return customer;
}

int CheckAllEmpty(queueADT queues[3]) {
    int flag = 1;
    for (int i = 0; !QueueIsEmpty(queues[i]); i++) {
        flag = 0;
        break;
    }
    return flag;
}
