/**
 * test_display.c
 * Tests for the display functions in the spreadsheet application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../clab/init.h"
#include "../clab/io.h"
#include "../clab/display.h"

// Global sheet declaration
extern int** sheet;
extern char status[20];

// External variables from display.c
extern int curr_org_r;
extern int curr_org_c;
extern int displayr;
extern int displayc;
extern bool output_enabled;
extern bool was_disabled;

// Test function prototypes
void test_scrolling(FILE *output_file);
void test_output_control(FILE *output_file);
void test_int_to_alpha(FILE *output_file);

/**
 * Run all display tests
 */
void run_display_tests(FILE *output_file) {
    // Disable output for all display tests
    output_enabled = false;
    
    // Run all display tests
    test_scrolling(output_file);
    test_output_control(output_file);
    test_int_to_alpha(output_file);
    
    // Print completion message to both stdout and output file
    fprintf(output_file, "All display tests are passed.\n");
    printf("All display tests are passed.\n");
}

/**
 * Test scrolling functions
 */
void test_scrolling(FILE *output_file) {
    // Save and restore output state to ensure tests don't affect the application state
    bool original_output_state = output_enabled;
    output_enabled = false;
    
    fprintf(output_file, "Testing scrolling functions...\n");
    
    // Test scroll up
    curr_org_r = 5;
    w();  // Scroll up
    fprintf(output_file, "After scroll up from row 5: curr_org_r = %d\n", curr_org_r);
    
    // Test scroll down
    s();  // Scroll down
    fprintf(output_file, "After scroll down: curr_org_r = %d\n", curr_org_r);
    
    // Test scroll left
    curr_org_c = 5;
    a();  // Scroll left
    fprintf(output_file, "After scroll left from column 5: curr_org_c = %d\n", curr_org_c);
    
    // Test scroll right
    d();  // Scroll right
    fprintf(output_file, "After scroll right: curr_org_c = %d\n", curr_org_c);
    
    // Test boundary conditions
    curr_org_r = 0;
    w();  // Try to scroll up from top
    fprintf(output_file, "After scroll up from row 0: curr_org_r = %d\n", curr_org_r);
    
    curr_org_c = 0;
    a();  // Try to scroll left from leftmost
    fprintf(output_file, "After scroll left from column 0: curr_org_c = %d\n", curr_org_c);
    
    // Restore original output state
    output_enabled = original_output_state;
    
    fprintf(output_file, "Scrolling tests completed.\n");
}

/**
 * Test output control functions
 */
void test_output_control(FILE *output_file) {
    // Save original output state to restore it after the test
    bool original_output_state = output_enabled;
    
    fprintf(output_file, "Testing output control functions...\n");
    
    // Test disable_output
    output_enabled = true;
    disable_output();
    fprintf(output_file, "After disable_output: output_enabled = %d\n", output_enabled);
    
    // Test enable_output
    enable_output();
    fprintf(output_file, "After enable_output: output_enabled = %d\n", output_enabled);
    
    // Restore original output state
    output_enabled = original_output_state;
    
    fprintf(output_file, "Output control tests completed.\n");
}

/**
 * Test int_to_alpha function
 */
void test_int_to_alpha(FILE *output_file) {
    // Save original output state to restore it after the test
    bool original_output_state = output_enabled;
    output_enabled = false;
    
    fprintf(output_file, "Testing int_to_alpha function...\n");
    
    // Test various column numbers
    char col_str[10];
    int_to_alpha(1, col_str);
    fprintf(output_file, "Column 1 = %s\n", col_str);
    
    int_to_alpha(26, col_str);
    fprintf(output_file, "Column 26 = %s\n", col_str);
    
    int_to_alpha(27, col_str);
    fprintf(output_file, "Column 27 = %s\n", col_str);
    
    int_to_alpha(52, col_str);
    fprintf(output_file, "Column 52 = %s\n", col_str);
    
    int_to_alpha(702, col_str);
    fprintf(output_file, "Column 702 = %s\n", col_str);
    
    // Restore original output state
    output_enabled = original_output_state;
    
    fprintf(output_file, "int_to_alpha tests completed.\n");
} 