#include "io.h"
#include "stdlib.h"
#include "init.h"

#ifndef __DEPEND__ 
    #define __DEPEND__

typedef struct Parent{
    int r;
    int c;
    struct Parent *next;
    ParsedCommand formula;
} Parent;

typedef struct Child{
    int r;
    int c;
    struct Child *next;
    ParsedCommand formula;
} Child;

typedef struct AdjNode {
    int r, c;
    struct AdjNode *next;
    struct AdjNode *down;
} AdjNode;


#endif

#ifndef __DEPEND_FUNC__
    #define __DEPEND_FUNC__
// Global pointers to parent and child lists
extern Parent ***Parent_lst;
extern Child ***Child_lst;

// Function prototypes
void make_parent_list();
void make_child_list();
void free_parent_list();
void free_child_list();

void assign_parent(int r1, int c1, int r2, int c2, ParsedCommand formula);
void remove_parent(int r1, int c1, int r2, int c2);

void assign_child(int r1, int c1, int r2, int c2, ParsedCommand formula);
void remove_child(int r1, int c1, int r2, int c2);

bool detect_cycle(int root_r, int root_c);
void topo_sort(int root_r, int root_c,ParsedCommand* result);
#endif