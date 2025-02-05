#include "graph.h"
#include <stdlib.h>
#include <stdio.h> // Include the header for stderr
#include <stdlib.h> // Include the header for malloc, realloc, and exit

DependencyGraph *graph_create(int rows, int cols) {
    DependencyGraph *graph = malloc(sizeof(DependencyGraph));
    graph->rows = rows;
    graph->cols = cols;
    
    graph->dependencies = malloc(rows * sizeof(Cell ***));
    for (int i = 0; i < rows; i++) {
        graph->dependencies[i] = malloc(cols * sizeof(Cell **));
        for (int j = 0; j < cols; j++) {
            graph->dependencies[i][j] = NULL;
        }
    }
    return graph;
}

void graph_add_dependency(DependencyGraph *graph, 
                         int src_row, int src_col,
                         int dest_row, int dest_col) {
    if (src_row >= graph->rows || src_col >= graph->cols ||
        dest_row >= graph->rows || dest_col >= graph->cols) return;
    
    // Add dest to src's dependency list
    int count = 0;
    while (graph->dependencies[src_row][src_col][count] != NULL) count++;
    
    graph->dependencies[src_row][src_col] = realloc(
        graph->dependencies[src_row][src_col],
        (count + 2) * sizeof(Cell *)
    );
    
    graph->dependencies[src_row][src_col][count] = &graph->dependencies[dest_row][dest_col];
    graph->dependencies[src_row][src_col][count + 1] = NULL;
}

void graph_get_dependents(DependencyGraph *graph, int row, int col, int **result, int *count) {
    int i = 0;
    while (graph->dependencies[row][col][i]) {
        // Reallocate memory for the array of pointers
        *result = realloc(*result, (*count + 1) * sizeof(int *)); 
        if (*result == NULL) {
            // Handle allocation failure (e.g., print an error and exit)
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1); 
        }

        // Allocate memory for the int array
        (*result)[*count] = malloc(sizeof(int) * 2);
        if ((*result)[*count] == NULL) {
            // Handle allocation failure
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1); 
        }

        (*result)[*count] = row;
        (*result)[*count] = col;
        (*count)++;
        i++;
    }
}

void graph_destroy(DependencyGraph *graph) {
    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; j < graph->cols; j++) {
            free(graph->dependencies[i][j]);
        }
        free(graph->dependencies[i]);
    }
    free(graph->dependencies);
    free(graph);
}