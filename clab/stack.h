#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

// Define stack node structure
typedef struct StackNode {
    int r, c;
    struct StackNode *next;
} StackNode;

// Stack function prototypes
void push(int r, int c);
StackNode pop();
bool isEmpty();
void displayStack();

#endif // STACK_H