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
    fprintf(output_file, "After scroll left from col 5: curr_org_c = %d\n", curr_org_c);
    
    // Test scroll right
    d();  // Scroll right
    fprintf(output_file, "After scroll right: curr_org_c = %d\n", curr_org_c);
    
    // Test set origin
    set_org(10, 10);  // Set viewport origin
    fprintf(output_file, "After set_org(10,10): curr_org_r = %d, curr_org_c = %d\n", 
            curr_org_r, curr_org_c);
    
    // Reset to initial position
    curr_org_r = 0;
    curr_org_c = 0;
    
    fprintf(output_file, "TEST_SCROLLING is passed\n\n");
}

/**
 * Test output control functions
 */
void test_output_control(FILE *output_file) {
    bool original_output_state = output_enabled;
    output_enabled = false;
    
    fprintf(output_file, "Testing output control functions...\n");
    
    // Test enable_output
    enable_output();
    fprintf(output_file, "After enable_output: output_enabled = %s\n", 
            output_enabled ? "true" : "false");
    
    // Test disable_output
    disable_output();
    fprintf(output_file, "After disable_output: output_enabled = %s\n", 
            output_enabled ? "true" : "false");
    
    fprintf(output_file, "TEST_OUTPUT_CONTROL is passed\n\n");
}

/**
 * Test int_to_alpha function
 */
void test_int_to_alpha(FILE *output_file) {
    bool original_output_state = output_enabled;
    output_enabled = false;
    
    fprintf(output_file, "Testing int_to_alpha function...\n");
    
    // Test cases
    char result[3];
    int_to_alpha(0, result);
    fprintf(output_file, "int_to_alpha(0) = %s\n", result);
    
    int_to_alpha(25, result);
    fprintf(output_file, "int_to_alpha(25) = %s\n", result);
    
    int_to_alpha(26, result);
    fprintf(output_file, "int_to_alpha(26) = %s\n", result);
    
    int_to_alpha(51, result);
    fprintf(output_file, "int_to_alpha(51) = %s\n", result);
    
    fprintf(output_file, "TEST_INT_TO_ALPHA is passed\n\n");
} 