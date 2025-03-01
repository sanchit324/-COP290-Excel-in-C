/**
 * test_dependent.c
 * Tests for the dependency management functions in the spreadsheet application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../clab/init.h"
#include "../clab/io.h"
#include "../clab/process.h"
#include "../clab/dependent.h"

// Global sheet declaration
extern int** sheet;
extern char status[20];

// Test function prototypes
void test_parent_child_relationships(FILE *output_file);
void test_cycle_detection(FILE *output_file);
void test_dependency_updates(FILE *output_file);

// External function declarations
void update_dependents(int row, int col);

/**
 * Run all dependency tests
 */
void run_dependent_tests(FILE *output_file) {
    test_parent_child_relationships(output_file);
    test_cycle_detection(output_file);
    test_dependency_updates(output_file);
}

/**
 * Test parent-child relationship management
 */
void test_parent_child_relationships(FILE *output_file) {
    fprintf(output_file, "Testing parent-child relationships...\n");
    
    // Reset sheet values
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
        }
    }
    
    // Create a test formula
    ParsedCommand cmd;
    memset(&cmd, 0, sizeof(ParsedCommand));
    cmd.type = CMD_SET_CELL;
    strcpy(cmd.cell, "B2");
    strcpy(cmd.expression, "A1");
    cmd.op1.row = 2;
    cmd.op1.col = 2;
    cmd.op2.row = 1;
    cmd.op2.col = 1;
    
    // Assign parent-child relationship
    fprintf(output_file, "Assigning A1 as parent of B2\n");
    assign_parent(0, 0, 1, 1, cmd);
    assign_child(0, 0, 1, 1, cmd);
    
    // Check if parent exists in child's parent list
    Parent *parent = Parent_lst[1][1];
    bool parent_found = false;
    while (parent != NULL) {
        if (parent->r == 0 && parent->c == 0) {
            parent_found = true;
            break;
        }
        parent = parent->next;
    }
    fprintf(output_file, "Parent A1 found in B2's parent list: %s\n", 
            parent_found ? "Yes" : "No");
    
    // Check if child exists in parent's child list
    Child *child = Child_lst[0][0];
    bool child_found = false;
    while (child != NULL) {
        if (child->r == 1 && child->c == 1) {
            child_found = true;
            break;
        }
        child = child->next;
    }
    fprintf(output_file, "Child B2 found in A1's child list: %s\n", 
            child_found ? "Yes" : "No");
    
    // Remove parent-child relationship
    fprintf(output_file, "Removing A1 as parent of B2\n");
    remove_parent(0, 0, 1, 1);
    remove_child(0, 0, 1, 1);
    
    // Check if parent was removed from child's parent list
    parent = Parent_lst[1][1];
    parent_found = false;
    while (parent != NULL) {
        if (parent->r == 0 && parent->c == 0) {
            parent_found = true;
            break;
        }
        parent = parent->next;
    }
    fprintf(output_file, "Parent A1 found in B2's parent list after removal: %s\n", 
            parent_found ? "Yes" : "No");
    
    // Check if child was removed from parent's child list
    child = Child_lst[0][0];
    child_found = false;
    while (child != NULL) {
        if (child->r == 1 && child->c == 1) {
            child_found = true;
            break;
        }
        child = child->next;
    }
    fprintf(output_file, "Child B2 found in A1's child list after removal: %s\n", 
            child_found ? "Yes" : "No");
    
    fprintf(output_file, "\n");
}

/**
 * Test cycle detection
 */
void test_cycle_detection(FILE *output_file) {
    fprintf(output_file, "Testing cycle detection...\n");
    
    // Reset sheet values and dependencies
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
            
            // Clear parent and child lists
            while (Parent_lst[i][j] != NULL) {
                Parent *temp = Parent_lst[i][j];
                Parent_lst[i][j] = temp->next;
                free(temp);
            }
            
            while (Child_lst[i][j] != NULL) {
                Child *temp = Child_lst[i][j];
                Child_lst[i][j] = temp->next;
                free(temp);
            }
        }
    }
    
    // Create test formulas
    ParsedCommand cmd1, cmd2, cmd3;
    
    // A1 = B2
    memset(&cmd1, 0, sizeof(ParsedCommand));
    cmd1.type = CMD_SET_CELL;
    strcpy(cmd1.cell, "A1");
    strcpy(cmd1.expression, "B2");
    cmd1.op1.row = 1;
    cmd1.op1.col = 1;
    cmd1.op2.row = 2;
    cmd1.op2.col = 2;
    
    // B2 = C3
    memset(&cmd2, 0, sizeof(ParsedCommand));
    cmd2.type = CMD_SET_CELL;
    strcpy(cmd2.cell, "B2");
    strcpy(cmd2.expression, "C3");
    cmd2.op1.row = 2;
    cmd2.op1.col = 2;
    cmd2.op2.row = 3;
    cmd2.op2.col = 3;
    
    // C3 = A1 (creates a cycle)
    memset(&cmd3, 0, sizeof(ParsedCommand));
    cmd3.type = CMD_SET_CELL;
    strcpy(cmd3.cell, "C3");
    strcpy(cmd3.expression, "A1");
    cmd3.op1.row = 3;
    cmd3.op1.col = 3;
    cmd3.op2.row = 1;
    cmd3.op2.col = 1;
    
    // Set up A1 = B2
    fprintf(output_file, "Setting A1 = B2\n");
    assign_parent(1, 1, 0, 0, cmd1);
    assign_child(1, 1, 0, 0, cmd1);
    
    // Set up B2 = C3
    fprintf(output_file, "Setting B2 = C3\n");
    assign_parent(2, 2, 1, 1, cmd2);
    assign_child(2, 2, 1, 1, cmd2);
    
    // Check for cycle before adding C3 = A1
    bool cycle_detected = detect_cycle(0, 0);
    fprintf(output_file, "Cycle detected before adding C3 = A1: %s\n", 
            cycle_detected ? "Yes" : "No");
    
    // Try to set up C3 = A1 (creates a cycle)
    fprintf(output_file, "Attempting to set C3 = A1 (would create a cycle)\n");
    assign_parent(0, 0, 2, 2, cmd3);
    assign_child(0, 0, 2, 2, cmd3);
    
    // Check for cycle after adding C3 = A1
    cycle_detected = detect_cycle(0, 0);
    fprintf(output_file, "Cycle detected after adding C3 = A1: %s\n", 
            cycle_detected ? "Yes" : "No");
    
    fprintf(output_file, "\n");
}

/**
 * Test dependency updates
 */
void test_dependency_updates(FILE *output_file) {
    fprintf(output_file, "Testing dependency updates...\n");
    
    // Reset sheet values and dependencies
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
            
            // Clear parent and child lists
            while (Parent_lst[i][j] != NULL) {
                Parent *temp = Parent_lst[i][j];
                Parent_lst[i][j] = temp->next;
                free(temp);
            }
            
            while (Child_lst[i][j] != NULL) {
                Child *temp = Child_lst[i][j];
                Child_lst[i][j] = temp->next;
                free(temp);
            }
        }
    }
    
    // Set up initial values
    sheet[0][0] = 10;  // A1 = 10
    
    // Create test formulas
    ParsedCommand cmd1, cmd2, cmd3;
    
    // B2 = A1
    memset(&cmd1, 0, sizeof(ParsedCommand));
    cmd1.type = CMD_SET_CELL;
    strcpy(cmd1.cell, "B2");
    strcpy(cmd1.expression, "A1");
    cmd1.op1.row = 2;
    cmd1.op1.col = 2;
    cmd1.op2.row = 1;
    cmd1.op2.col = 1;
    
    // C3 = B2 + 5
    memset(&cmd2, 0, sizeof(ParsedCommand));
    cmd2.type = CMD_ARITHMETIC;
    strcpy(cmd2.cell, "C3");
    strcpy(cmd2.expression, "B2+5");
    cmd2.op1.row = 3;
    cmd2.op1.col = 3;
    cmd2.op2.row = 2;
    cmd2.op2.col = 2;
    cmd2.op3.row = 0;
    cmd2.op3.col = 0;
    cmd2.op3.value = 5;
    cmd2.operator = '+';
    
    // D4 = SUM(A1:C3)
    memset(&cmd3, 0, sizeof(ParsedCommand));
    cmd3.type = CMD_FUNCTION;
    strcpy(cmd3.cell, "D4");
    strcpy(cmd3.expression, "SUM(A1:C3)");
    cmd3.op1.row = 4;
    cmd3.op1.col = 4;
    cmd3.op2.row = 1;
    cmd3.op2.col = 1;
    cmd3.op3.row = 3;
    cmd3.op3.col = 3;
    cmd3.func = FUNC_SUM;
    strcpy(cmd3.range, "A1:C3");
    
    // Set up B2 = A1
    fprintf(output_file, "Setting B2 = A1\n");
    assign_parent(0, 0, 1, 1, cmd1);
    assign_child(0, 0, 1, 1, cmd1);
    sheet[1][1] = sheet[0][0];  // B2 = A1 = 10
    
    // Set up C3 = B2 + 5
    fprintf(output_file, "Setting C3 = B2 + 5\n");
    assign_parent(1, 1, 2, 2, cmd2);
    assign_child(1, 1, 2, 2, cmd2);
    sheet[2][2] = sheet[1][1] + 5;  // C3 = B2 + 5 = 10 + 5 = 15
    
    // Set up D4 = SUM(A1:C3)
    fprintf(output_file, "Setting D4 = SUM(A1:C3)\n");
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            assign_parent(i, j, 3, 3, cmd3);
            assign_child(i, j, 3, 3, cmd3);
        }
    }
    // D4 = SUM(A1:C3) = 10 + 10 + 15 + zeros = 35
    sheet[3][3] = 35;
    
    // Print initial values
    fprintf(output_file, "Initial values:\n");
    fprintf(output_file, "  A1 = %d\n", sheet[0][0]);
    fprintf(output_file, "  B2 = %d\n", sheet[1][1]);
    fprintf(output_file, "  C3 = %d\n", sheet[2][2]);
    fprintf(output_file, "  D4 = %d\n", sheet[3][3]);
    
    // Change A1 and update dependencies
    fprintf(output_file, "Changing A1 to 20 and updating dependencies\n");
    sheet[0][0] = 20;
    update_dependents(0, 0);
    
    // Print updated values
    fprintf(output_file, "Updated values:\n");
    fprintf(output_file, "  A1 = %d\n", sheet[0][0]);
    fprintf(output_file, "  B2 = %d\n", sheet[1][1]);
    fprintf(output_file, "  C3 = %d\n", sheet[2][2]);
    fprintf(output_file, "  D4 = %d\n", sheet[3][3]);
    
    fprintf(output_file, "\n");
} 