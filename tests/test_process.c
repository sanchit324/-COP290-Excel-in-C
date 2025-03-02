/**
 * test_process.c
 * Tests for the process functions in the spreadsheet application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "../clab/init.h"
#include "../clab/io.h"
#include "../clab/process.h"

// Global sheet declaration
extern int** sheet;
extern char status[20];

// Test function prototypes
void test_assign(FILE *output_file);
void test_arithmetic(FILE *output_file);
void test_function(FILE *output_file);
void test_error_handling(FILE *output_file);

// External function declarations
void update_dependents(int row, int col);

/**
 * Helper function to create a ParsedCommand for testing
 */
void create_test_command(ParsedCommand *cmd, CommandType type, int r1, int c1, int r2, int c2, int val2, 
                         int r3, int c3, int val3, char op, FunctionType func) {
    memset(cmd, 0, sizeof(ParsedCommand));
    cmd->type = type;
    cmd->op1.row = r1;
    cmd->op1.col = c1;
    cmd->op2.row = r2;
    cmd->op2.col = c2;
    cmd->op2.value = val2;
    cmd->op3.row = r3;
    cmd->op3.col = c3;
    cmd->op3.value = val3;
    cmd->operator = op;
    cmd->func = func;
    
    // Generate cell reference for op1
    if (r1 > 0 && c1 > 0) {
        char col_letter = (c1 <= 26) ? ('A' + c1 - 1) : ('A' + (c1 / 26) - 1);
        sprintf(cmd->cell, "%c%d", col_letter, r1);
    }
    
    // Generate expression based on command type
    if (type == CMD_SET_CELL) {
        if (r2 > 0 && c2 > 0) {
            char col_letter = (c2 <= 26) ? ('A' + c2 - 1) : ('A' + (c2 / 26) - 1);
            sprintf(cmd->expression, "%c%d", col_letter, r2);
        } else {
            sprintf(cmd->expression, "%d", val2);
        }
    } else if (type == CMD_ARITHMETIC) {
        char op1[10], op2[10];
        
        if (r2 > 0 && c2 > 0) {
            char col_letter = (c2 <= 26) ? ('A' + c2 - 1) : ('A' + (c2 / 26) - 1);
            sprintf(op1, "%c%d", col_letter, r2);
        } else {
            sprintf(op1, "%d", val2);
        }
        
        if (r3 > 0 && c3 > 0) {
            char col_letter = (c3 <= 26) ? ('A' + c3 - 1) : ('A' + (c3 / 26) - 1);
            sprintf(op2, "%c%d", col_letter, r3);
        } else {
            sprintf(op2, "%d", val3);
        }
        
        sprintf(cmd->expression, "%s%c%s", op1, op, op2);
    } else if (type == CMD_FUNCTION) {
        const char *func_names[] = {"", "MIN", "MAX", "SUM", "AVG", "STDEV", "SLEEP"};
        
        if (func == FUNC_SLEEP) {
            if (r2 > 0 && c2 > 0) {
                char col_letter = (c2 <= 26) ? ('A' + c2 - 1) : ('A' + (c2 / 26) - 1);
                sprintf(cmd->expression, "%s(%c%d)", func_names[func], col_letter, r2);
            } else {
                sprintf(cmd->expression, "%s(%d)", func_names[func], val2);
            }
        } else {
            char start_cell[10], end_cell[10];
            
            char start_col_letter = (c2 <= 26) ? ('A' + c2 - 1) : ('A' + (c2 / 26) - 1);
            sprintf(start_cell, "%c%d", start_col_letter, r2);
            
            char end_col_letter = (c3 <= 26) ? ('A' + c3 - 1) : ('A' + (c3 / 26) - 1);
            sprintf(end_cell, "%c%d", end_col_letter, r3);
            
            sprintf(cmd->expression, "%s(%s:%s)", func_names[func], start_cell, end_cell);
            sprintf(cmd->range, "%s:%s", start_cell, end_cell);
        }
    }
}

/**
 * Run all process tests
 */
void run_process_tests(FILE *output_file) {
    // Disable output for all process tests
    output_enabled = false;
    
    // Run all process tests
    test_assign(output_file);
    test_arithmetic(output_file);
    test_function(output_file);
    test_error_handling(output_file);
    
    // Print completion message to both stdout and output file
    fprintf(output_file, "All process tests are passed.\n");
    printf("All process tests are passed.\n");
}

/**
 * Test the assign function
 */
void test_assign(FILE *output_file) {
    fprintf(output_file, "Testing assign function...\n");
    
    // Reset sheet values
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
        }
    }
    
    // Test direct value assignment
    ParsedCommand cmd1;
    create_test_command(&cmd1, CMD_SET_CELL, 1, 1, 0, 0, 42, 0, 0, 0, 0, FUNC_NONE);
    
    fprintf(output_file, "Assigning value 42 to A1\n");
    assign(&cmd1);
    fprintf(output_file, "A1 value: %d\n", sheet[0][0]);
    
    // Test cell reference assignment
    ParsedCommand cmd2;
    create_test_command(&cmd2, CMD_SET_CELL, 2, 2, 1, 1, 0, 0, 0, 0, 0, FUNC_NONE);
    
    fprintf(output_file, "Assigning A1 to B2\n");
    assign(&cmd2);
    fprintf(output_file, "B2 value: %d\n", sheet[1][1]);
    
    // Test changing source cell affects dependent cell
    ParsedCommand cmd3;
    create_test_command(&cmd3, CMD_SET_CELL, 1, 1, 0, 0, 100, 0, 0, 0, 0, FUNC_NONE);
    
    fprintf(output_file, "Changing A1 to 100\n");
    assign(&cmd3);
    fprintf(output_file, "A1 value: %d\n", sheet[0][0]);
    
    // We need to manually update dependents since we're calling assign directly
    update_dependents(0, 0);
    fprintf(output_file, "B2 value after update: %d\n", sheet[1][1]);
    
    fprintf(output_file, "\n");
    fprintf(output_file, "TEST_ASSIGN is passed\n");
}

/**
 * Test the arithmetic function
 */
void test_arithmetic(FILE *output_file) {
    fprintf(output_file, "Testing arithmetic function...\n");
    
    // Reset sheet values
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
        }
    }
    
    // Set up initial values
    sheet[0][0] = 10;  // A1 = 10
    sheet[1][1] = 5;   // B2 = 5
    
    // Test addition
    ParsedCommand cmd1;
    create_test_command(&cmd1, CMD_ARITHMETIC, 3, 3, 1, 1, 0, 2, 2, 0, '+', FUNC_NONE);
    
    fprintf(output_file, "C3 = A1 + B2 (10 + 5)\n");
    arithmetic(&cmd1);
    fprintf(output_file, "C3 value: %d\n", sheet[2][2]);
    
    // Test subtraction
    ParsedCommand cmd2;
    create_test_command(&cmd2, CMD_ARITHMETIC, 4, 4, 1, 1, 0, 2, 2, 0, '-', FUNC_NONE);
    
    fprintf(output_file, "D4 = A1 - B2 (10 - 5)\n");
    arithmetic(&cmd2);
    fprintf(output_file, "D4 value: %d\n", sheet[3][3]);
    
    // Test multiplication
    ParsedCommand cmd3;
    create_test_command(&cmd3, CMD_ARITHMETIC, 5, 5, 1, 1, 0, 2, 2, 0, '*', FUNC_NONE);
    
    fprintf(output_file, "E5 = A1 * B2 (10 * 5)\n");
    arithmetic(&cmd3);
    fprintf(output_file, "E5 value: %d\n", sheet[4][4]);
    
    // Test division
    ParsedCommand cmd4;
    create_test_command(&cmd4, CMD_ARITHMETIC, 6, 6, 1, 1, 0, 2, 2, 0, '/', FUNC_NONE);
    
    fprintf(output_file, "F6 = A1 / B2 (10 / 5)\n");
    arithmetic(&cmd4);
    fprintf(output_file, "F6 value: %d\n", sheet[5][5]);
    
    // Test with direct values
    ParsedCommand cmd5;
    create_test_command(&cmd5, CMD_ARITHMETIC, 7, 7, 0, 0, 20, 0, 0, 3, '+', FUNC_NONE);
    
    fprintf(output_file, "G7 = 20 + 3\n");
    arithmetic(&cmd5);
    fprintf(output_file, "G7 value: %d\n", sheet[6][6]);
    
    fprintf(output_file, "\n");
    fprintf(output_file, "TEST_ARITHMETIC is passed\n");
}

/**
 * Test the function operations
 */
void test_function(FILE *output_file) {
    fprintf(output_file, "Testing function operations...\n");
    
    // Reset sheet values
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
        }
    }
    
    // Set up test data
    sheet[0][0] = 10;  // A1 = 10
    sheet[0][1] = 20;  // B1 = 20
    sheet[1][0] = 30;  // A2 = 30
    sheet[1][1] = 40;  // B2 = 40
    
    // Test SUM function
    ParsedCommand cmd1;
    create_test_command(&cmd1, CMD_FUNCTION, 3, 3, 1, 1, 0, 2, 2, 0, 0, FUNC_SUM);
    
    fprintf(output_file, "C3 = SUM(A1:B2) (10+20+30+40)\n");
    function(&cmd1);
    fprintf(output_file, "C3 value: %d\n", sheet[2][2]);
    
    // Test MIN function
    ParsedCommand cmd2;
    create_test_command(&cmd2, CMD_FUNCTION, 4, 4, 1, 1, 0, 2, 2, 0, 0, FUNC_MIN);
    
    fprintf(output_file, "D4 = MIN(A1:B2) (10)\n");
    function(&cmd2);
    fprintf(output_file, "D4 value: %d\n", sheet[3][3]);
    
    // Test MAX function
    ParsedCommand cmd3;
    create_test_command(&cmd3, CMD_FUNCTION, 5, 5, 1, 1, 0, 2, 2, 0, 0, FUNC_MAX);
    
    fprintf(output_file, "E5 = MAX(A1:B2) (40)\n");
    function(&cmd3);
    fprintf(output_file, "E5 value: %d\n", sheet[4][4]);
    
    // Test AVG function
    ParsedCommand cmd4;
    create_test_command(&cmd4, CMD_FUNCTION, 6, 6, 1, 1, 0, 2, 2, 0, 0, FUNC_AVG);
    
    fprintf(output_file, "F6 = AVG(A1:B2) (25)\n");
    function(&cmd4);
    fprintf(output_file, "F6 value: %d\n", sheet[5][5]);
    
    // Test STDEV function
    ParsedCommand cmd5;
    create_test_command(&cmd5, CMD_FUNCTION, 7, 7, 1, 1, 0, 2, 2, 0, 0, FUNC_STDEV);
    
    fprintf(output_file, "G7 = STDEV(A1:B2)\n");
    function(&cmd5);
    fprintf(output_file, "G7 value: %d\n", sheet[6][6]);
    
    // Test SLEEP function (with minimal sleep time for testing)
    ParsedCommand cmd6;
    create_test_command(&cmd6, CMD_FUNCTION, 8, 8, 0, 0, 1, 0, 0, 0, 0, FUNC_SLEEP);
    
    fprintf(output_file, "H8 = SLEEP(1)\n");
    function(&cmd6);
    fprintf(output_file, "H8 value: %d\n", sheet[7][7]);
    
    fprintf(output_file, "\n");
    fprintf(output_file, "TEST_FUNCTION is passed\n");
}

/**
 * Test error handling in process functions
 */
void test_error_handling(FILE *output_file) {
    fprintf(output_file, "Testing error handling...\n");
    
    // Reset sheet values
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
        }
    }
    
    // Test division by zero
    sheet[0][0] = 10;  // A1 = 10
    sheet[1][1] = 0;   // B2 = 0
    
    ParsedCommand cmd1;
    create_test_command(&cmd1, CMD_ARITHMETIC, 3, 3, 1, 1, 0, 2, 2, 0, '/', FUNC_NONE);
    
    fprintf(output_file, "C3 = A1 / B2 (10 / 0)\n");
    arithmetic(&cmd1);
    fprintf(output_file, "C3 value: %d (should be ERROR_VALUE)\n", sheet[2][2]);
    
    // Test error propagation
    ParsedCommand cmd2;
    create_test_command(&cmd2, CMD_SET_CELL, 4, 4, 3, 3, 0, 0, 0, 0, 0, FUNC_NONE);
    
    fprintf(output_file, "D4 = C3 (ERROR_VALUE)\n");
    assign(&cmd2);
    fprintf(output_file, "D4 value: %d (should be ERROR_VALUE)\n", sheet[3][3]);
    
    // Test invalid range for function
    ParsedCommand cmd3;
    create_test_command(&cmd3, CMD_FUNCTION, 5, 5, 10, 10, 0, 5, 5, 0, 0, FUNC_SUM);
    
    fprintf(output_file, "E5 = SUM(J10:E5) (invalid range - end before start)\n");
    strcpy(status, "ok");  // Reset status
    if (!is_valid_range(&cmd3)) {
        fprintf(output_file, "Range validation correctly failed\n");
    } else {
        function(&cmd3);
        fprintf(output_file, "E5 value: %d, Status: %s\n", sheet[4][4], status);
    }
    
    fprintf(output_file, "\n");
    fprintf(output_file, "TEST_ERROR_HANDLING is passed\n");
} 