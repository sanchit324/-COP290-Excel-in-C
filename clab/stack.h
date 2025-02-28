/**
 * stack.h
 * Stack data structure implementation for cell dependency tracking
 * Used in topological sorting of cell dependencies
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/**
 * Node structure for the stack
 * Stores row and column coordinates of cells
 */
typedef struct StackNode {
    int r;                    // Row coordinate
    int c;                    // Column coordinate
    struct StackNode *next;   // Pointer to next node
} StackNode;

// Stack operations
void push(int r, int c);          // Push cell coordinates onto stack
StackNode pop();                  // Pop and return top cell coordinates
bool isEmpty();                   // Check if stack is empty
void displayStack();              // Display stack contents (for debugging)

#endif // STACK_H