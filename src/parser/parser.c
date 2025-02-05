#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h> // Include the header for stderr
#include <stdlib.h> // Include the header for malloc, realloc, and exit

// Helper: Convert column letters to index (A=0, Z=25, AA=26, etc.)
int col_to_idx(const char *col) {
    int idx = 0;
    while (*col) {
        idx = idx * 26 + (toupper(*col) - 'A' + 1);
        col++;
    }
    return idx - 1;
}

// Helper: Parse cell reference (e.g., A1)
void parse_cellref(const char *str, int *row, int *col) {
    char col_part[4] = {0};
    int i = 0;
    
    while (isalpha(str[i])) {
        col_part[i] = str[i];
        i++;
    }
    *col = col_to_idx(col_part);
    *row = atoi(str + i) - 1;
}

ASTNode *parse_formula(const char *formula) {
    // Simplified parsing logic (full implementation would use recursive descent)
    ASTNode *node = malloc(sizeof(ASTNode));
    
    if (isdigit(*formula)) {
        node->type = NODE_VALUE;
        node->data.value = atoi(formula);
    } else if (isalpha(*formula)) {
        node->type = NODE_CELLREF;
        parse_cellref(formula, &node->data.cellref.row, &node->data.cellref.col);
    } else if (*formula == '+' || *formula == '-' || *formula == '*' || *formula == '/') {
        node->type = NODE_OPERATION;
        node->data.operation.op = *formula;
        // Simplified: assumes left/right are single terms
        node->data.operation.left = parse_formula(formula + 1);
        node->data.operation.right = NULL;
    }
    return node;
}

int validate_range(const char *start, const char *end) {
    int start_row, start_col, end_row, end_col;
    parse_cellref(start, &start_row, &start_col);
    parse_cellref(end, &end_row, &end_col);
    
    return (start_row <= end_row) && (start_col <= end_col);
}

void get_dependencies(ASTNode *node, int **deps, int *count) {
    if (!node) return;

    if (node->type == NODE_CELLREF) {
        *deps = realloc(*deps, (*count + 1) * sizeof(int *));
        if (*deps == NULL) {
            // Handle allocation error
            return;
        }
        (*deps)[*count] = malloc(sizeof(int)); // Allocate for int
        if ((*deps)[*count] == NULL) {
            // Handle allocation error
            return;
        }
        (*deps)[*count] = node->data.cellref.row;
        (*deps)[*count] = node->data.cellref.col;
        (*count)++;
    }

    if (node->type == NODE_OPERATION) {
        get_dependencies(node->data.operation.left, deps, count);
        get_dependencies(node->data.operation.right, deps, count);
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;
    
    if (node->type == NODE_OPERATION) {
        free_ast(node->data.operation.left);
        free_ast(node->data.operation.right);
    }
    free(node);
}