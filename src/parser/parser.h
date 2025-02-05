#ifndef PARSER_H
#define PARSER_H

#include "../cell/cell.h"

typedef enum {
    NODE_VALUE,
    NODE_CELLREF,
    NODE_OPERATION,
    NODE_FUNCTION
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int value;              // For constants
        struct {                // For cell references
            int row;
            int col;
        } cellref;
        struct {                // For operations
            char op;
            struct ASTNode *left;
            struct ASTNode *right;
        } operation;
        struct {                // For functions
            char *name;
            struct ASTNode *args;
        } function;
    } data;
} ASTNode;

// Parse formula into AST
ASTNode *parse_formula(const char *formula);

// Extract cell dependencies from AST
void get_dependencies(ASTNode *node, int **deps, int *count);

// Validate range syntax (e.g., A1:B2)
int validate_range(const char *start, const char *end);

// Free AST memory
void free_ast(ASTNode *node);

#endif