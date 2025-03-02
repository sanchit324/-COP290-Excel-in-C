#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

StackNode *top = NULL;

// Push operation for (r, c)
void push(int r, int c) {
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));
    if (!newNode) {
        printf("Heap Overflow! Cannot push (%d, %d)\n", r, c);
        return;
    }
    newNode->r = r;
    newNode->c = c;
    newNode->next = top;
    top = newNode;
}

// Pop operation
StackNode pop() {
    if (top == NULL) {
        printf("Stack Underflow! No elements to pop\n");
        return (StackNode){-1, -1, NULL}; // Return invalid coordinates
    }
    StackNode *temp = top;
    StackNode poppedValue = *temp;
    top = top->next;
    free(temp);
    return poppedValue;
}

// Check if stack is empty
bool isEmpty() {
    return (top == NULL);
}

// Display stack
void displayStack() {
    if (top == NULL) {
        printf("Stack is empty\n");
    } else {
        StackNode *temp = top;
        printf("Stack elements: ");
        while (temp) {
            printf("(%d, %d) ", temp->r, temp->c);
            temp = temp->next;
        }
        printf("\n");
    }
}