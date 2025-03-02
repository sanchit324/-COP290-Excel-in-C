/**
 * test_init.c
 * Test-specific initialization functions for the spreadsheet application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../clab/init.h"
#include "../clab/dependent.h"

// We don't define any global variables here to avoid duplicate symbols
// Instead, we use the ones already defined in the original files

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
    
    // Set default display size and reset status
    // These variables are defined in the original files
    displayr = 10;
    displayc = 10;
    curr_org_r = 0;
    curr_org_c = 0;
    strcpy(status, "ok");
    
    // Disable output for testing
    output_enabled = false;
}

/**
 * Clean up the sheet after testing
 */
void cleanup_test_sheet() {
    // Free allocated memory
    if (sheet != NULL) {
        for (int i = 0; i < MAXROW; i++) {
            if (sheet[i] != NULL) {
                free(sheet[i]); // Free each row
            }
        }
        free(sheet); // Free the row pointers
        sheet = NULL;
    }
    
    // Free dependency lists
    free_parent_list();
    free_child_list();
}

/**
 * Set test mode flag
 */
void set_test_mode(bool mode) {
    // This function can be used to enable/disable test-specific behavior
    output_enabled = !mode; // Disable output in test mode
}

/**
 * Set viewport origin
 */
void set_org(int r, int c) {
    curr_org_r = r;
    curr_org_c = c;
} 