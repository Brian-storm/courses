
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void checkPalindrome(stackADT stack);

int main (void) {
    stackADT stringStack = EmptyStack();
    char input[81] = "";

    do {
        fgets(input, sizeof(input), stdin);

        if (strlen(input) == 1)  //'\0' is counted as one strlen
            exit(1);
        
        else {
            for (int i = 0; input[i] != '\n'; i++)
                Push(stringStack, input[i]);
            
            checkPalindrome(stringStack);
            stringStack = EmptyStack();
        }
    } while (1);
}

void checkPalindrome(stackADT stack) {
    stackADT template = EmptyStack();
    int len = StackDepth(stack);
    int isPalindrome = 1;

    for (int i = 0; i < len / 2; i++)
        Push(template, Pop(stack));
    
    if (len % 2) {
        Pop(stack);
        len /= 2;
    }
    
    for (int i = 0; i < len; i++)
        if (Pop(template) != Pop(stack)) {
            isPalindrome = 0;
            break;
        }

    printf("The input is %s palindrome.\n", isPalindrome ? "a" : "NOT a");
}