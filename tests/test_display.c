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

// Test function prototypes
void test_scrolling(FILE *output_file);
void test_output_control(FILE *output_file);
void test_int_to_alpha(FILE *output_file);

/**
 * Run all display tests
 */
void run_display_tests(FILE *output_file) {
    test_scrolling(output_file);
    test_output_control(output_file);
    test_int_to_alpha(output_file);
}

/**
 * Test scrolling functions
 */
void test_scrolling(FILE *output_file) {
    fprintf(output_file, "Testing scrolling functions...\n");
    
    // Reset viewport position
    curr_org_r = 1;
    curr_org_c = 1;
    
    // Test set_org function
    fprintf(output_file, "Initial position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    fprintf(output_file, "Setting viewport to Row 5, Col 5\n");
    set_org(5, 5);
    fprintf(output_file, "New position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    // Test scroll function
    fprintf(output_file, "Scrolling 3 rows down, 2 columns right\n");
    scroll(3, 2);
    fprintf(output_file, "New position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    // Test directional scrolling
    fprintf(output_file, "Testing w() - scroll up\n");
    w();
    fprintf(output_file, "New position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    fprintf(output_file, "Testing s() - scroll down\n");
    s();
    fprintf(output_file, "New position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    fprintf(output_file, "Testing a() - scroll left\n");
    a();
    fprintf(output_file, "New position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    fprintf(output_file, "Testing d() - scroll right\n");
    d();
    fprintf(output_file, "New position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    // Test boundary conditions
    fprintf(output_file, "Testing boundary conditions\n");
    
    fprintf(output_file, "Setting viewport to Row 1, Col 1 (minimum)\n");
    set_org(1, 1);
    fprintf(output_file, "New position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    fprintf(output_file, "Attempting to scroll up and left from minimum position\n");
    w();
    a();
    fprintf(output_file, "Position after attempt: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    fprintf(output_file, "Setting viewport to maximum position\n");
    set_org(MAXROW, MAXCOL);
    fprintf(output_file, "Position after setting to max: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    fprintf(output_file, "Attempting to scroll down and right from maximum position\n");
    s();
    d();
    fprintf(output_file, "Position after attempt: Row %d, Col %d\n", curr_org_r, curr_org_c);
    
    fprintf(output_file, "\n");
}

/**
 * Test output control functions
 */
void test_output_control(FILE *output_file) {
    fprintf(output_file, "Testing output control functions...\n");
    
    // Reset output state
    output_enabled = true;
    was_disabled = false;
    
    fprintf(output_file, "Initial output state: %s\n", output_enabled ? "enabled" : "disabled");
    
    // Test disable_output
    fprintf(output_file, "Disabling output\n");
    disable_output();
    fprintf(output_file, "Output state after disable: %s\n", output_enabled ? "enabled" : "disabled");
    fprintf(output_file, "Was disabled flag: %s\n", was_disabled ? "true" : "false");
    
    // Test scrolling while output is disabled
    fprintf(output_file, "Setting viewport to Row 10, Col 10 while output is disabled\n");
    set_org(10, 10);
    fprintf(output_file, "Current position: Row %d, Col %d\n", curr_org_r, curr_org_c);
    fprintf(output_file, "Pending scroll should be stored but not applied\n");
    
    // Test enable_output
    fprintf(output_file, "Enabling output\n");
    enable_output();
    fprintf(output_file, "Output state after enable: %s\n", output_enabled ? "enabled" : "disabled");
    fprintf(output_file, "Position after enabling output: Row %d, Col %d\n", curr_org_r, curr_org_c);
    fprintf(output_file, "Pending scroll should now be applied\n");
    
    fprintf(output_file, "\n");
}

/**
 * Test int_to_alpha function
 */
void test_int_to_alpha(FILE *output_file) {
    fprintf(output_file, "Testing int_to_alpha function...\n");
    
    int test_values[] = {1, 2, 26, 27, 28, 52, 53, 702};
    char expected[][5] = {"A", "B", "Z", "AA", "AB", "AZ", "BA", "ZZ"};
    
    for (int i = 0; i < sizeof(test_values) / sizeof(test_values[0]); i++) {
        char result[MAXCOLWIDTH + 1];
        int_to_alpha(test_values[i], result);
        
        // Trim leading spaces for comparison
        char *trimmed = result;
        while (*trimmed == ' ') trimmed++;
        
        fprintf(output_file, "Column %d -> \"%s\" (expected \"%s\")\n", 
                test_values[i], trimmed, expected[i]);
    }
    
    fprintf(output_file, "\n");
} 