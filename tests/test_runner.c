/**
 * test_runner.c
 * Main test runner for the spreadsheet application
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

// Define global variables
int** sheet;
char status[20] = "ok";
extern bool output_enabled;

// Define MAXROW and MAXCOL for testing
int MAXROW = 100;  // Default test size
int MAXCOL = 100;  // Default test size

// Function prototypes for test suites
void run_io_tests(FILE *output_file);
void run_process_tests(FILE *output_file);
void run_dependent_tests(FILE *output_file);
void run_display_tests(FILE *output_file);
void run_integration_tests(FILE *output_file);

// Global sheet declaration (matches the one in init.c)
// extern int** sheet;
// extern char status[20];

/**
 * Initialize the sheet for testing
 */
void init_test_sheet() {
    // Allocate memory for row pointers
    sheet = (int **)malloc(MAXROW * sizeof(int *));
    if (sheet == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Allocate memory for each row
    for (int i = 0; i < MAXROW; i++) {
        sheet[i] = (int *)malloc(MAXCOL * sizeof(int));
        if (sheet[i] == NULL) {
            printf("Memory allocation failed!\n");
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(sheet[j]);
            }
            free(sheet);
            exit(1);
        }
    }

    // Initialize all elements to 0
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
        }
    }
    
    // Initialize dependency lists
    make_parent_list();
    make_child_list();
    
    // Set default display size
    displayr = 10;
    displayc = 10;
}

/**
 * Clean up the sheet after testing
 */
void cleanup_test_sheet() {
    // Free allocated memory
    for (int i = 0; i < MAXROW; i++) {
        free(sheet[i]); // Free each row
    }
    free(sheet); // Free the row pointers
    
    // Free dependency lists
    free_parent_list();
    free_child_list();
}

/**
 * Main function to run all tests
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // Initialize test environment
    init_test_sheet();
    
    // Enable test mode and disable output
    set_test_mode(true);
    output_enabled = false;
    
    // Open output file
    FILE *output_file = fopen(argv[2], "w");
    if (!output_file) {
        printf("Error: Could not open %s\n", argv[2]);
        return 1;
    }
    
    // Run all test suites
    fprintf(output_file, "===== RUNNING ALL TESTS =====\n\n");
    
    // Run IO tests
    fprintf(output_file, "Running IO tests...\n");
    run_io_tests(output_file);
    cleanup_test_sheet();
    init_test_sheet();
    
    // Run process tests
    fprintf(output_file, "\nRunning process tests...\n");
    run_process_tests(output_file);
    cleanup_test_sheet();
    init_test_sheet();
    
    // Run dependent tests
    fprintf(output_file, "\nRunning dependent tests...\n");
    run_dependent_tests(output_file);
    cleanup_test_sheet();
    init_test_sheet();
    
    // Run display tests
    fprintf(output_file, "\nRunning display tests...\n");
    run_display_tests(output_file);
    cleanup_test_sheet();
    init_test_sheet();
    
    // Run integration tests
    fprintf(output_file, "\nRunning integration tests...\n");
    run_integration_tests(output_file);
    
    // Final cleanup
    cleanup_test_sheet();
    
    // Close output file
    fclose(output_file);
    
    // Print final success message
    printf("All test suites completed successfully.\n");
    
    return 0;
} 
