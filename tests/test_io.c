/**
 * test_io.c
 * Tests for the IO functions in the spreadsheet application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../clab/init.h"
#include "../clab/io.h"

// Test function prototypes
void test_input_parser(FILE *output_file);
void test_cell_validation(FILE *output_file);
void test_range_validation(FILE *output_file);
void test_cell_to_rc(FILE *output_file);

/**
 * Run all IO tests
 */
void run_io_tests(FILE *output_file) {
    test_input_parser(output_file);
    test_cell_validation(output_file);
    test_range_validation(output_file);
    test_cell_to_rc(output_file);
}

/**
 * Test the input_parser function
 */
void test_input_parser(FILE *output_file) {
    fprintf(output_file, "Testing input_parser function...\n");
    
    // Test cases for different command types
    const char *test_inputs[] = {
        "A1=10",                  // Direct value assignment
        "B2=A1",                  // Cell reference assignment
        "C3=A1+B2",               // Arithmetic operation
        "D4=SUM(A1:B2)",          // Range function
        "E5=MIN(A1:C3)",          // Another range function
        "F6=SLEEP(5)",            // Sleep function
        "scroll_to A1",           // Scroll command
        "w",                      // Scroll direction
        "disable_output",         // Control command
        "q",                      // Quit command
        "invalid command"         // Invalid command
    };
    
    int num_tests = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    for (int i = 0; i < num_tests; i++) {
        ParsedCommand result;
        char input[MAX_INPUT_LEN];
        strcpy(input, test_inputs[i]);
        
        input_parser(input, &result);
        
        fprintf(output_file, "Input: \"%s\"\n", test_inputs[i]);
        fprintf(output_file, "  Command Type: %d\n", result.type);
        
        switch (result.type) {
            case CMD_SET_CELL:
                fprintf(output_file, "  Cell: %s, Value/Ref: %s\n", 
                        result.cell, result.expression);
                break;
            case CMD_ARITHMETIC:
                fprintf(output_file, "  Cell: %s, Expression: %s, Operator: %c\n", 
                        result.cell, result.expression, result.operator);
                break;
            case CMD_FUNCTION:
                fprintf(output_file, "  Cell: %s, Function: %d, Expression: %s\n", 
                        result.cell, result.func, result.expression);
                break;
            case CMD_SLEEP:
                fprintf(output_file, "  Sleep Command: %s\n", result.expression);
                break;
            case CMD_SCROLL:
                fprintf(output_file, "  Scroll Target: %s\n", result.scroll_target);
                break;
            case CMD_SCROLL_DIR:
                fprintf(output_file, "  Scroll Direction: %c\n", result.scroll_direction);
                break;
            case CMD_CONTROL:
                fprintf(output_file, "  Control Command: %s\n", result.control_cmd);
                break;
            case CMD_INVALID:
                fprintf(output_file, "  Invalid Command\n");
                break;
        }
        fprintf(output_file, "\n");
    }
}

/**
 * Test the cell validation function
 */
void test_cell_validation(FILE *output_file) {
    fprintf(output_file, "Testing cell validation...\n");
    
    const char *test_cells[] = {
        "A1",       // Valid
        "Z10",      // Valid
        "AA100",    // Valid
        "AAA1000",  // Invalid - too many letters
        "A0",       // Invalid - row 0
        "1A",       // Invalid - format
        "",         // Invalid - empty
        "A-1"       // Invalid - negative row
    };
    
    int num_tests = sizeof(test_cells) / sizeof(test_cells[0]);
    
    for (int i = 0; i < num_tests; i++) {
        bool is_valid = validate_cell(test_cells[i]);
        fprintf(output_file, "Cell: \"%s\" is %s\n", 
                test_cells[i], is_valid ? "valid" : "invalid");
    }
    fprintf(output_file, "\n");
}

/**
 * Test the range validation function
 */
void test_range_validation(FILE *output_file) {
    fprintf(output_file, "Testing range validation...\n");
    
    const char *test_ranges[] = {
        "A1:B2",      // Valid
        "A1:Z10",     // Valid
        "A1:",        // Invalid - incomplete
        ":B2",        // Invalid - incomplete
        "A1B2",       // Invalid - no colon
        "A0:B2",      // Invalid - invalid cell
        "A1:B0",      // Invalid - invalid cell
        "",           // Invalid - empty
    };
    
    int num_tests = sizeof(test_ranges) / sizeof(test_ranges[0]);
    
    for (int i = 0; i < num_tests; i++) {
        bool is_valid = validate_range(test_ranges[i]);
        fprintf(output_file, "Range: \"%s\" is %s\n", 
                test_ranges[i], is_valid ? "valid" : "invalid");
    }
    fprintf(output_file, "\n");
}

/**
 * Test the cell_to_rc function
 */
void test_cell_to_rc(FILE *output_file) {
    fprintf(output_file, "Testing cell_to_rc function...\n");
    
    const char *test_cells[] = {
        "A1",    // Row 1, Col 1
        "B2",    // Row 2, Col 2
        "Z26",   // Row 26, Col 26
        "AA27",  // Row 27, Col 27
        "AB28"   // Row 28, Col 28
    };
    
    int num_tests = sizeof(test_cells) / sizeof(test_cells[0]);
    
    for (int i = 0; i < num_tests; i++) {
        int row, col;
        cell_to_rc(test_cells[i], &row, &col);
        fprintf(output_file, "Cell: \"%s\" -> Row: %d, Col: %d\n", 
                test_cells[i], row, col);
    }
    fprintf(output_file, "\n");
} 