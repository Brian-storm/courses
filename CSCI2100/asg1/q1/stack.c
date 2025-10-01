
#include <stdlib.h>
#include "stack.h"


typedef struct cellT {
    stackElementT element;
    struct cellT *above; //missed the 'struct' KW
} cellT;

struct stackCDT {
    cellT* bottomOfStack;
};

stackADT EmptyStack(void) {
    stackADT stack = (stackADT)malloc(sizeof(stackADT));
    stack->bottomOfStack = NULL;
    return stack;
}

/* Discarded Function
cellT* findTopOfStack(stackADT stack) {
    cellT *cp;
    for (cp = stack->bottomOfStack; cp->above != NULL; cp = cp->above);
    return cp;
}
*/

void Push(stackADT stack, stackElementT element) {
    cellT *cp = (cellT*)malloc(sizeof(cellT));
    cp->element = element;
    cp->above = NULL;

    // No need to split into 2 cases to check StackEmpty
    cellT *top;
    for (top = stack->bottomOfStack; top->above != NULL; top = top->above);
    top->above = cp;
}

stackElementT Pop(stackADT stack) {
    if (StackIsEmpty(stack))
        exit(EXIT_FAILURE);
    else {
        cellT *cp0 = NULL, *cp1;
        for (cp1 = stack->bottomOfStack; cp1->above != NULL; cp1 = cp1->above)
            cp0 = cp1;
        
        if (cp0 == NULL)
            stack->bottomOfStack = NULL;
        else
            cp0->above = NULL;

        return cp1->element;  // I almost forgot to kill the top cell, aka the 'cp0' part here
    }
}

stackElementT Peek(stackADT stack) {
    if (StackIsEmpty(stack))
        exit(EXIT_FAILURE);
    else {
        cellT *cp;
        for (cp = stack->bottomOfStack; cp->above != NULL; cp = cp->above);
        return cp->element;
    }
}

int StackDepth(stackADT stack) {
    int count = 0;
    cellT *cp;
    for (cp = stack->bottomOfStack; cp->above != NULL; cp = cp->above)
        count++;
    return count;
}

int StackIsEmpty(stackADT stack) {
    return stack->bottomOfStack == NULL;
}