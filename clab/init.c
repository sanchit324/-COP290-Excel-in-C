#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   
#include <unistd.h>   
#include "init.h"
#include "display.h"
#include "io.h"
#include "process.h"
#include "dependent.h"
#include <stdbool.h>


// Global sheet
int** sheet;
// Make status variable global and accessible from other files
char status[20] = "ok";

int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number of rows> <number of columns>\n", argv[0]);
        return 1;
    }

    // Parse command line arguments for display size
    int input_rows = atoi(argv[1]);
    int input_cols = atoi(argv[2]);

    // Validate the parsed integers
    if (input_rows <= 0 || input_rows > MAXROW || input_cols <= 0 || input_cols > MAXCOL) {
        fprintf(stderr, "Invalid dimensions. Please enter values from 1 to %d for rows and 1 to %d for columns.\n", MAXROW, MAXCOL);
        return 1;
    }

    // Set the display size
    displayr = input_rows;
    displayc = input_cols;

    // Allocate memory for row pointers
    sheet = (int **)malloc(MAXROW * sizeof(int *));
    if (sheet == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
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
            return 1;
        }
    }

    // Initialize all elements to 0
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOL; j++) {
            sheet[i][j] = 0;
        }
    }
    make_parent_list();
    make_child_list();

    // Display initial empty sheet
    display_sheet();

    // Variables for input, execution time, and command processing
    char input[MAX_INPUT_LEN];
    double execution_time = 0.0;
    ParsedCommand result;
    time_t start, end;  // Changed to time_t for wall clock time

    while (1) {
        // Print prompt with current status
        printf("[%.1f] (%s) > ", execution_time, status);

        // Get user input
        input_reader(input);
        input_parser(input, &result);

        // Reset status to "ok" at the beginning of each command cycle
        // This ensures previous errors don't persist
        strcpy(status, "ok");

        // Check for quit command
        if (strcmp(input, "q") == 0) {
            break;
        }

        // Process the input with timing
        start = time(NULL);  // Start timing
        
        // Handle control commands (disable_output, enable_output)
        if (result.type == CMD_CONTROL) {
            strcpy(status, "ok");  // Set status to "ok" for control commands
            
            // Process the command
            process_command(&result);
            
            end = time(NULL);
            execution_time = difftime(end, start);
            
            // Special handling for enable_output command
            if (strcmp(result.control_cmd, "enable_output") == 0) {
                if (!was_disabled) {  // If output wasn't previously disabled
                    display_sheet();
                }
                was_disabled = false;  // Reset the flag
            }
            
            continue;
        }
        
        // Reset status for valid scroll commands
        if (result.type == CMD_SCROLL_DIR || result.type == CMD_SCROLL) {
            strcpy(status, "ok");
            process_command(&result);
            end = time(NULL);
            execution_time = difftime(end, start);
            // Display sheet after scroll commands if output is enabled
            if (output_enabled) {
                display_sheet();
            }
            continue;
        }

        // Check for unrecognized commands or invalid text input
        if (result.type == CMD_INVALID || 
            (result.type == CMD_SET_CELL && !is_numeric_value(&result))) {
            strcpy(status, "unrecognized cmd");
            execution_time = 0.0;
            // Display sheet even for unrecognized commands
            if (output_enabled) {
                display_sheet();
            }
            continue;
        }

        // Check for invalid ranges in formulas
        if (result.type == CMD_FUNCTION) {
            if (!is_valid_range(&result)) {
                strcpy(status, "Invalid range");
                execution_time = 0.0;
                // Display sheet even for invalid ranges
                if (output_enabled) {
                    display_sheet();
                }
                continue;
            }
        }

        // Process command
        bool dpcorrect = handle_dependencies(&result);
        if(dpcorrect) {
            // Only show topology for non-sleep functions
            if (result.func != FUNC_SLEEP) {
                topo_sort(result.op1.row-1, result.op1.col-1, &result);
            }
            // Only set status to "ok" if it wasn't already set to "err" by cycle detection
            if (strcmp(status, "err") != 0) {
                strcpy(status, "ok");
            }
        }

        end = time(NULL);  // End timing
        execution_time = difftime(end, start);  // Get difference in seconds

        // Process other commands and display sheet if output is enabled
        // Skip processing if it's already been handled by handle_dependencies
        if (!dpcorrect) {
            process_command(&result);
        }
        
        if (output_enabled) {
            display_sheet();
        }
    }

    // Free allocated memory
    for (int i = 0; i < MAXROW; i++) {
        free(sheet[i]); // Free each row
    }
    free(sheet); // Free the row pointers
    free_parent_list();
    free_child_list();
    return 0;
}