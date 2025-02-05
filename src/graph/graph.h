#ifndef GRAPH_H
#define GRAPH_H

#include "../cell/cell.h"

typedef struct DependencyGraph {
    int rows;
    int cols;
    Cell ****dependencies;  // dependencies[src_row][src_col] = list of dependent cells
} DependencyGraph;

// Initialize graph
DependencyGraph *graph_create(int rows, int cols);

// Add dependency: dest depends on src
void graph_add_dependency(DependencyGraph *graph, 
                         int src_row, int src_col,
                         int dest_row, int dest_col);

// Get cells needing recalculation when a cell changes
void graph_get_dependents(DependencyGraph *graph,
                         int row, int col,
                         int **result, int *count);

// Free graph memory
void graph_destroy(DependencyGraph *graph);

#endif