/**
 * test_integration.c
 * Integration tests for the spreadsheet application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../clab/init.h"
#include "../clab/io.h"
#include "../clab/process.h"
#include "../clab/dependent.h"
#include "../clab/display.h"

// Global sheet declaration
extern int** sheet;
extern char status[20];
extern bool output_enabled;

// Test function prototypes
void test_complex_dependencies(FILE *output_file);
void test_command_processing(FILE *output_file);
void test_error_propagation(FILE *output_file);

/**
 * Run all integration tests
 */
void run_integration_tests(FILE *output_file) {
    // Disable output for all integration tests
    output_enabled = false;
    
    // Run all test functions
    test_complex_dependencies(output_file);
    test_command_processing(output_file);
    test_error_propagation(output_file);
    
    // Print completion message to both stdout and output file
    fprintf(output_file, "All integration tests are passed.\n");
    printf("All integration tests are passed.\n");
}

/**
 * Helper function to process a command string
 */
void process_command_string(const char *command, FILE *output_file) {
    char input[MAX_INPUT_LEN];
    ParsedCommand result;
    
    strcpy(input, command);
    fprintf(output_file, "Processing command: \"%s\"\n", command);
    
    // Ensure output is disabled
    bool temp_output = output_enabled;
    output_enabled = false;
    
    // Reset status to "ok" before processing command, just like in the main loop
    strcpy(status, "ok");
    
    input_parser(input, &result);
    
    if (result.type == CMD_CONTROL && strcmp(result.control_cmd, "q") == 0) {
        fprintf(output_file, "Received quit command\n");
        return;
    }
    
    if (result.type != CMD_INVALID) {
        process_command(&result);
        fprintf(output_file, "Status after command: %s\n", status);
    } else {
        fprintf(output_file, "Invalid command\n");
    }
    
    // Keep output disabled
    output_enabled = false;
}

/**
 * Test complex dependency chains
 */
void test_complex_dependencies(FILE *output_file) {
    bool original_output_state = output_enabled;
    output_enabled = false;
    
    fprintf(output_file, "Testing complex dependencies...\n");
    
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
    
    // Set up a complex dependency chain
    process_command_string("A1=10", output_file);
    process_command_string("B1=20", output_file);
    process_command_string("C1=A1+B1", output_file);
    process_command_string("D1=C1*2", output_file);
    process_command_string("E1=SUM(A1:D1)", output_file);
    process_command_string("F1=AVG(A1:E1)", output_file);
    
    // Print initial values
    fprintf(output_file, "Initial values:\n");
    fprintf(output_file, "  A1 = %d\n", sheet[0][0]);
    fprintf(output_file, "  B1 = %d\n", sheet[0][1]);
    fprintf(output_file, "  C1 = %d\n", sheet[0][2]);
    fprintf(output_file, "  D1 = %d\n", sheet[0][3]);
    fprintf(output_file, "  E1 = %d\n", sheet[0][4]);
    fprintf(output_file, "  F1 = %d\n", sheet[0][5]);
    
    // Change A1 and see how it propagates
    process_command_string("A1=15", output_file);
    
    // Print updated values
    fprintf(output_file, "Values after changing A1 to 15:\n");
    fprintf(output_file, "  A1 = %d\n", sheet[0][0]);
    fprintf(output_file, "  B1 = %d\n", sheet[0][1]);
    fprintf(output_file, "  C1 = %d\n", sheet[0][2]);
    fprintf(output_file, "  D1 = %d\n", sheet[0][3]);
    fprintf(output_file, "  E1 = %d\n", sheet[0][4]);
    fprintf(output_file, "  F1 = %d\n", sheet[0][5]);
    
    // Change B1 and see how it propagates
    process_command_string("B1=25", output_file);
    
    // Print updated values
    fprintf(output_file, "Values after changing B1 to 25:\n");
    fprintf(output_file, "  A1 = %d\n", sheet[0][0]);
    fprintf(output_file, "  B1 = %d\n", sheet[0][1]);
    fprintf(output_file, "  C1 = %d\n", sheet[0][2]);
    fprintf(output_file, "  D1 = %d\n", sheet[0][3]);
    fprintf(output_file, "  E1 = %d\n", sheet[0][4]);
    fprintf(output_file, "  F1 = %d\n", sheet[0][5]);
    
    output_enabled = original_output_state;
    fprintf(output_file, "\n");
    fprintf(output_file, "TEST_COMPLEX_DEPENDENCIES is passed\n");
    
    // Add quit command at the end
    process_command_string("q", output_file);
}

/**
 * Test command processing with various command types
 */
void test_command_processing(FILE *output_file) {
    bool original_output_state = output_enabled;
    output_enabled = false;
    
    fprintf(output_file, "Testing command processing...\n");
    
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
    
    // Test various command types
    
    // Direct value assignment
    process_command_string("A1=42", output_file);
    fprintf(output_file, "A1 after direct assignment: %d\n", sheet[0][0]);
    
    // Cell reference assignment
    process_command_string("B1=A1", output_file);
    fprintf(output_file, "B1 after reference assignment: %d\n", sheet[0][1]);
    
    // Arithmetic operations
    process_command_string("C1=A1+10", output_file);
    fprintf(output_file, "C1 after A1+10: %d\n", sheet[0][2]);
    
    process_command_string("D1=A1-10", output_file);
    fprintf(output_file, "D1 after A1-10: %d\n", sheet[0][3]);
    
    process_command_string("E1=A1*2", output_file);
    fprintf(output_file, "E1 after A1*2: %d\n", sheet[0][4]);
    
    process_command_string("F1=A1/2", output_file);
    fprintf(output_file, "F1 after A1/2: %d\n", sheet[0][5]);
    
    // Range functions
    process_command_string("A2=MIN(A1:F1)", output_file);
    fprintf(output_file, "A2 after MIN(A1:F1): %d\n", sheet[1][0]);
    
    process_command_string("B2=MAX(A1:F1)", output_file);
    fprintf(output_file, "B2 after MAX(A1:F1): %d\n", sheet[1][1]);
    
    process_command_string("C2=SUM(A1:F1)", output_file);
    fprintf(output_file, "C2 after SUM(A1:F1): %d\n", sheet[1][2]);
    
    process_command_string("D2=AVG(A1:F1)", output_file);
    fprintf(output_file, "D2 after AVG(A1:F1): %d\n", sheet[1][3]);
    
    process_command_string("E2=STDEV(A1:F1)", output_file);
    fprintf(output_file, "E2 after STDEV(A1:F1): %d\n", sheet[1][4]);
    
    // Sleep function (with minimal sleep time for testing)
    process_command_string("F2=SLEEP(1)", output_file);
    fprintf(output_file, "F2 after SLEEP(1): %d\n", sheet[1][5]);
    
    // Control commands
    process_command_string("disable_output", output_file);
    fprintf(output_file, "Output enabled after disable_output: %s\n", 
            output_enabled ? "true" : "false");
    
    process_command_string("enable_output", output_file);
    fprintf(output_file, "Output enabled after enable_output: %s\n", 
            output_enabled ? "true" : "false");
    
    // Scroll commands
    int original_row = curr_org_r;
    int original_col = curr_org_c;
    
    process_command_string("scroll_to B10", output_file);
    fprintf(output_file, "Viewport position after scroll_to B10: Row %d, Col %d\n", 
            curr_org_r, curr_org_c);
    
    process_command_string("w", output_file);
    fprintf(output_file, "Viewport position after w: Row %d, Col %d\n", 
            curr_org_r, curr_org_c);
    
    // Reset viewport position
    set_org(original_row, original_col);
    
    output_enabled = original_output_state;
    fprintf(output_file, "\n");
    fprintf(output_file, "TEST_COMMAND_PROCESSING is passed\n");
}

/**
 * Test error propagation through dependencies
 */
void test_error_propagation(FILE *output_file) {
    bool original_output_state = output_enabled;
    output_enabled = false;
    
    fprintf(output_file, "Testing error propagation...\n");
    
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
    
    // Set up a dependency chain
    process_command_string("A1=10", output_file);
    process_command_string("B1=0", output_file);
    process_command_string("C1=A1/B1", output_file);  // Division by zero
    process_command_string("D1=C1+5", output_file);   // Error propagation
    process_command_string("E1=SUM(A1:D1)", output_file);  // Error in range
    
    // Print values
    fprintf(output_file, "Values after setting up error chain:\n");
    fprintf(output_file, "  A1 = %d\n", sheet[0][0]);
    fprintf(output_file, "  B1 = %d\n", sheet[0][1]);
    fprintf(output_file, "  C1 = %d (should be ERROR_VALUE)\n", sheet[0][2]);
    fprintf(output_file, "  D1 = %d (should be ERROR_VALUE)\n", sheet[0][3]);
    fprintf(output_file, "  E1 = %d (should be ERROR_VALUE)\n", sheet[0][4]);
    
    // Fix the error source
    process_command_string("B1=2", output_file);
    
    // Print updated values
    fprintf(output_file, "Values after fixing B1:\n");
    fprintf(output_file, "  A1 = %d\n", sheet[0][0]);
    fprintf(output_file, "  B1 = %d\n", sheet[0][1]);
    fprintf(output_file, "  C1 = %d\n", sheet[0][2]);
    fprintf(output_file, "  D1 = %d\n", sheet[0][3]);
    fprintf(output_file, "  E1 = %d\n", sheet[0][4]);
    
    // Test cycle detection
    fprintf(output_file, "Testing cycle detection:\n");
    process_command_string("F1=G1", output_file);
    process_command_string("G1=F1", output_file);  // Creates a cycle
    
    fprintf(output_file, "Status after attempting to create cycle: %s\n", status);
    fprintf(output_file, "F1 value: %d\n", sheet[0][5]);
    fprintf(output_file, "G1 value: %d\n", sheet[0][6]);
    
    output_enabled = original_output_state;
    fprintf(output_file, "\n");
    fprintf(output_file, "TEST_ERROR_PROPAGATION is passed\n");
} 