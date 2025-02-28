#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>  // For high-precision timing
#include <unistd.h>    // For sleep()
#include "init.h"
#include "display.h"
#include "io.h"
#include "process.h"
#include "dependent.h"
#include <stdbool.h>


// Global sheet
int** sheet;

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

    // Variables for input, execution time, and command processing
    char input[MAX_INPUT_LEN];
    double execution_time = 0.0;
    char status[20] = "ok";  // This will store our status messages
    ParsedCommand result;
    struct timeval start, end;

    while (1) {
        // Display the sheet
        display_sheet();

        // Print prompt with current status
        printf("[%.1f] (%s) > ", execution_time, status);

        // Get user input
        input_reader(input);
        input_parser(input, &result);

        // Check for quit command
        if (strcmp(input, "q") == 0) {
            break;
        }

        // Process the input with high-precision timing
        gettimeofday(&start, NULL);
        
        // Check for unrecognized commands or invalid text input
        if (result.type == CMD_INVALID || 
            (result.type == CMD_SET_CELL && !is_numeric_value(&result))) {
            strcpy(status, "unrecognized cmd");
            execution_time = 0.0;
            continue;
        }

        // Check for invalid ranges in formulas
        if (result.type == CMD_FUNCTION) {
            if (!is_valid_range(&result)) {
                strcpy(status, "Invalid range");
                execution_time = 0.0;
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
            strcpy(status, "ok");
        }

        gettimeofday(&end, NULL);
        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
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