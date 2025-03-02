/**
 * dependent.h
 * Cell dependency management for the spreadsheet
 * Tracks parent-child relationships between cells and handles dependency updates
 */

#include "io.h"
#include "stdlib.h"
#include "init.h"

#ifndef __DEPEND__ 
    #define __DEPEND__

/**
 * Parent node structure
 * Represents a cell that another cell depends on
 */
typedef struct Parent {
    int r;                      // Row coordinate
    int c;                      // Column coordinate
    struct Parent *next;        // Next parent in list
    ParsedCommand formula;      // Formula that created the dependency
} Parent;

/**
 * Child node structure
 * Represents a cell that depends on another cell
 */
typedef struct Child {
    int r;                      // Row coordinate
    int c;                      // Column coordinate
    struct Child *next;         // Next child in list
    ParsedCommand formula;      // Formula that created the dependency
} Child;

/**
 * Adjacency list node for dependency graph
 * Used in cycle detection and topological sorting
 */
typedef struct AdjNode {
    int r, c;                   // Cell coordinates
    struct AdjNode *next;       // Next node in same level
    struct AdjNode *down;       // Next level in graph
} AdjNode;

#endif

#ifndef __DEPEND_FUNC__
    #define __DEPEND_FUNC__

// Global dependency tracking lists
extern Parent ***Parent_lst;    // 2D array of parent lists
extern Child ***Child_lst;      // 2D array of child lists

// List management functions
void make_parent_list();        // Initialize parent list structure
void make_child_list();         // Initialize child list structure
void free_parent_list();        // Clean up parent list memory
void free_child_list();         // Clean up child list memory

// Dependency management functions
void assign_parent(int r1, int c1, int r2, int c2, ParsedCommand formula);  // Add parent dependency
void remove_parent(int r1, int c1, int r2, int c2);                         // Remove parent dependency
void assign_child(int r1, int c1, int r2, int c2, ParsedCommand formula);   // Add child dependency
void remove_child(int r1, int c1, int r2, int c2);                         // Remove child dependency

// Dependency analysis functions
bool detect_cycle(int root_r, int root_c);                                 // Check for circular dependencies
void topo_sort(int root_r, int root_c, ParsedCommand* result);            // Topologically sort dependencies

#endif