#include "init.h"
#include "io.h"
#include "process.h"
#include "dependent.h"
#include <stdlib.h>
#include"stack.h"
#include<stdio.h>


Parent ***Parent_lst;
Child ***Child_lst;



/** 
 * Function to create Parent_lst (initialize all elements to NULL)
 */
void make_parent_list() {
    Parent_lst = (Parent ***)malloc(MAXROW * sizeof(Parent **));
    for (int i = 0; i < MAXROW; i++) {
        Parent_lst[i] = (Parent **)malloc(MAXCOL * sizeof(Parent *));
        for (int j = 0; j < MAXCOL; j++) {
            Parent_lst[i][j] = NULL;  // Initialize all parent lists to NULL
        }
    }
}

/** 
 * Function to create Child_lst (initialize all elements to NULL)
 */
void make_child_list() {
    Child_lst = (Child ***)malloc(MAXROW * sizeof(Child **));
    for (int i = 0; i < MAXROW; i++) {
        Child_lst[i] = (Child **)malloc(MAXCOL * sizeof(Child *));
        for (int j = 0; j < MAXCOL; j++) {
            Child_lst[i][j] = NULL;  // Initialize all child lists to NULL
        }
    }
}

/**
 * Function to free Parent_lst and all its elements
 */
void free_parent_list() {
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            Parent *curr = Parent_lst[i][j];
            while (curr != NULL) {
                Parent *temp = curr;
                curr = curr->next;
                free(temp);
            }
        }
        free(Parent_lst[i]);  // Free row pointers
    }
    free(Parent_lst);  // Free the entire list
}

/**
 * Function to free Child_lst and all its elements
 */
void free_child_list() {
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            Child *curr = Child_lst[i][j];
            while (curr != NULL) {
                Child *temp = curr;
                curr = curr->next;
                free(temp);
            }
        }
        free(Child_lst[i]);  // Free row pointers
    }
    free(Child_lst);  // Free the entire list
}

/**
 * Function to assign a parent to a child cell
 */
void assign_parent(int r1, int c1, int r2, int c2, ParsedCommand formula) {
    // int act_r1 = r1 - 1, act_c1 = c1 - 1;
    // int act_r2 = r2 - 1, act_c2 = c2 - 1;
    int act_r1 = r1, act_c1 = c1;
    int act_r2 = r2, act_c2 = c2;

    Parent *newParent = (Parent *)malloc(sizeof(Parent));
    newParent->r = act_r1;
    newParent->c = act_c1;
    newParent->formula = formula;
    newParent->next = Parent_lst[act_r2][act_c2];  
    Parent_lst[act_r2][act_c2] = newParent;
}

/**
 * Function to remove a parent from a child cell
 */
void remove_parent(int r1, int c1, int r2, int c2) {
    // int act_r1 = r1 - 1, act_c1 = c1 - 1;
    // int act_r2 = r2 - 1, act_c2 = c2 - 1;
    int act_r1 = r1, act_c1 = c1;
    int act_r2 = r2, act_c2 = c2;
    Parent *head = Parent_lst[act_r2][act_c2];
    if (head == NULL) return;

    if (head->r == act_r1 && head->c == act_c1) {
        Parent_lst[act_r2][act_c2] = head->next;
        free(head);
        return;
    }

    Parent *curr = head;
    while (curr->next != NULL) {
        if (curr->next->r == act_r1 && curr->next->c == act_c1) {
            Parent *temp = curr->next;
            curr->next = temp->next;
            free(temp);
            return;
        }
        curr = curr->next;
    }
}

/**
 * Function to assign a child to a parent cell
 */
void assign_child(int r1, int c1, int r2, int c2, ParsedCommand formula) {
    // int act_r1 = r1 - 1, act_c1 = c1 - 1;
    // int act_r2 = r2 - 1, act_c2 = c2 - 1;
    int act_r1 = r1, act_c1 = c1;
    int act_r2 = r2, act_c2 = c2;

    Child *newChild = (Child *)malloc(sizeof(Child));
    newChild->r = act_r2;
    newChild->c = act_c2;
    newChild->formula = formula;
    newChild->next = Child_lst[act_r1][act_c1];  
    Child_lst[act_r1][act_c1] = newChild;
}

/**
 * Function to remove a child from a parent cell
 */
void remove_child(int r1, int c1, int r2, int c2) {
    // int act_r1 = r1 - 1, act_c1 = c1 - 1;
    // int act_r2 = r2 - 1, act_c2 = c2 - 1;
    int act_r1 = r1, act_c1 = c1;
    int act_r2 = r2, act_c2 = c2;

    Child *head = Child_lst[act_r1][act_c1];
    if (head == NULL) return;

    if (head->r == act_r2 && head->c == act_c2) {
        Child_lst[act_r1][act_c1] = head->next;
        free(head);
        return;
    }

    Child *curr = head;
    while (curr->next != NULL) {
        if (curr->next->r == act_r2 && curr->next->c == act_c2) {
            Child *temp = curr->next;
            curr->next = temp->next;
            free(temp);
            return;
        }
        curr = curr->next;
    }
}


/**
 * Function to perform BFS/DFS from a root node to discover relevant nodes and count in-degrees.
 */
void mark_dfs(int r, int c, int **visited, bool *cycle) {
    if (r < 0 || r >= MAXROW || c < 0 || c >= MAXCOL)
        return;

    if(!visited[r][c])visited[r][c] = true;
    else{
        *cycle = true; 
        return;

    }

    Child *child = Child_lst[r][c];
    while (child != NULL) {
        mark_dfs(child->r, child->c,visited,cycle);
        visited[child->r][child->c] = false;
        child = child->next;
    }
}


bool detect_cycle(int root_r, int root_c) {
    int **visited;
    visited = (int **)malloc(MAXROW * sizeof(int *));
    for (int i = 0; i < MAXROW; i++) {
        visited[i] = (int *)malloc(MAXCOL * sizeof(int));
    }

    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            visited[i][j] = false;
        }
    }

    bool cycle = false;
    mark_dfs(root_r, root_c, visited, &cycle);

    // Free memory
    for (int i = 0; i < MAXROW; i++) {
        free(visited[i]);
    }
    free(visited);

    return cycle;
}

AdjNode *build_main_list(int **visited) {
    AdjNode *head = NULL;

    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            if (visited[i][j]) {
                AdjNode *newNode = (AdjNode *)malloc(sizeof(AdjNode));
                newNode->r = i;
                newNode->c = j;
                newNode->next = NULL;
                newNode->down = NULL;
                if (head == NULL)
                    head = newNode;
                else {
                    newNode->down = head;
                    head = newNode;
                }
            }
        }
    }
    return head;
}

void link_children(AdjNode *head) {
    AdjNode *current = head;

    while (current != NULL) {
        int r = current->r;
        int c = current->c;

        Child *child = Child_lst[r][c];
        while (child != NULL) {
            AdjNode *node = (AdjNode *)malloc(sizeof(AdjNode));
            node->r = child->r;
            node->c = child->c;
            node->next = NULL;
            if (current->next == NULL)
                current->next = node;
            else {
                node->next = current->next;
                current->next = node;
            }
            child = child->next;
        }
        current = current->down;
    }
}

void print_adjlst(AdjNode *Lst) {
    AdjNode *tempLst;
    while (Lst != NULL) {
        tempLst = Lst;
        while (tempLst != NULL) {
            printf("(%d, %d) -> ", tempLst->r, tempLst->c);
            tempLst = tempLst->next;
        }
        printf("\n");
        Lst = Lst->down;
    }
}

void topo_sort_dfs(int r, int c,int **visited) {
    if (visited[r][c]) return;
    visited[r][c] = true;

    Child *child = Child_lst[r][c];
    while (child != NULL) {
        topo_sort_dfs(child->r, child->c,visited);
        child = child->next;
    }

    push(r, c);
}

// void topo_sort(int root_r, int root_c) {
//     int **visited;
//     visited = (int **)malloc(MAXROW * sizeof(int *)); 
    
//     for (int i = 0; i < MAXROW; i++) {
//         visited[i] = (int *)malloc(MAXCOL * sizeof(int)); 
//     }

//     for (int i = 0; i < MAXROW; i++) {
//         for (int j = 0; j < MAXCOL; j++) {
//             visited[i][j] = false;
//         }
//     }

//     topo_sort_dfs(root_r, root_c,visited);

//     printf("\nTopological Order: ");
//     while (!isEmpty()) {
//         StackNode node = pop();
//         printf("(%d, %d) ", node.r, node.c);
//     }
//     printf("\n");
//         // Freeing memory after usage
//     for (int i = 0; i < MAXROW; i++) {
//         free(visited[i]); // Free each row
//     }
//     free(visited); // Free the array of pointers
// }


// void topo_sort(int root_r, int root_c) {
//     int **visited;
//     visited = (int **)malloc(MAXROW * sizeof(int *)); 
    
//     for (int i = 0; i < MAXROW; i++) {
//         visited[i] = (int *)malloc(MAXCOL * sizeof(int)); 
//     }

//     for (int i = 0; i < MAXROW; i++) {
//         for (int j = 0; j < MAXCOL; j++) {
//             visited[i][j] = false;
//         }
//     }

//     topo_sort_dfs(root_r, root_c, visited);

//     printf("\nTopological Order with Formulas:\n");

//     StackNode prevNode = pop();  
//     printf("(%d, %d) ", prevNode.r, prevNode.c);  // Print first node

//     while (!isEmpty()) {
//         StackNode currNode = pop();

//         // Find the formula linking prevNode to currNode
//         Child *child = Child_lst[prevNode.r][prevNode.c];
//         ParsedCommand formula;
//         int found = 0;
//         while (child != NULL) {
//             if (child->r == currNode.r && child->c == currNode.c) {
//                 formula = child->formula;
//                 process_command(&formula);
//                 found = 1;
//                 break;
//             }
//             child = child->next;
//         }

//         if (found) {
//             printf("--[%s]--> (%d, %d) ", formula.expression, currNode.r, currNode.c);
//         } else {
//             printf("--[NO FORMULA]--> (%d, %d) ", currNode.r, currNode.c);
//         }

//         prevNode = currNode;  // Move to next node
//     }
//     printf("\n");

//     // Free memory
//     for (int i = 0; i < MAXROW; i++) {
//         free(visited[i]);
//     }
//     free(visited);
// }

void topo_sort(int root_r, int root_c, ParsedCommand *result) {
    int **visited;
    visited = (int **)malloc(MAXROW * sizeof(int *)); 
    
    for (int i = 0; i < MAXROW; i++) {
        visited[i] = (int *)malloc(MAXCOL * sizeof(int)); 
    }

    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            visited[i][j] = false;
        }
    }

    topo_sort_dfs(root_r, root_c, visited);

    printf("\nTopological Order with Formulas:\n");

    if (isEmpty()) {
        printf("No dependencies found.\n");
        return;
    }

    StackNode prevNode = pop();
    printf("(%d, %d) ", prevNode.r, prevNode.c);  // Print first node
    
    // First formula is the user-provided one
    if (result) {
        printf("--[%s]--> ", result->expression);
        process_command(result);
    } else {
        printf("--[NO FORMULA]--> ");
    }

    while (!isEmpty()) {
        StackNode currNode = pop();

        // Find the formula linking prevNode to currNode
        Child *child = Child_lst[prevNode.r][prevNode.c];
        ParsedCommand formula;
        int found = 0;
        
        while (child != NULL) {
            if (child->r == currNode.r && child->c == currNode.c) {
                formula = child->formula;
                process_command(&formula);
                found = 1;
                break;
            }
            child = child->next;
        }

        if (found) {
            printf("(%d, %d) --[%s]--> ", currNode.r, currNode.c, formula.expression);
        } else {
            printf("(%d, %d) --[NO FORMULA]--> ", currNode.r, currNode.c);
        }

        prevNode = currNode;  // Move to next node
    }
    printf("\n");

    // Free memory
    for (int i = 0; i < MAXROW; i++) {
        free(visited[i]);
    }
    free(visited);
}