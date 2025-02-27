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
    char status[20] = "ok";
    ParsedCommand result;
    struct timeval start, end;  // High precision timing

    while (1) {
        // Display the sheet
        display_sheet();

        // Print prompt
        printf("[%.1f] (%s) > ", execution_time, status);

        // Get user input
        input_reader(input);
        input_parser(input, &result);

        // Print parsed command details
        printf("Parsed Command:\n");
        printf("Command Type: %d\n", result.type);
        printf("Function Type: %d\n", result.func);
        printf("Command: %s\n", result.command);
        printf("Operand 1: row=%d, col=%d, value=%d\n", result.op1.row, result.op1.col, result.op1.value);
        printf("Operand 2: row=%d, col=%d, value=%d\n", result.op2.row, result.op2.col, result.op2.value);
        printf("Operand 3: row=%d, col=%d, value=%d\n", result.op3.row, result.op3.col, result.op3.value);
        printf("Operator: %c\n", result.operator);
        printf("Cell: %s\n", result.cell);
        printf("Expression: %s\n", result.expression);
        printf("Range: %s\n", result.range);
        printf("Scroll Target: %s\n", result.scroll_target);
        printf("Scroll Direction: %c\n", result.scroll_direction);
        printf("Control Command: %s\n", result.control_cmd);
        printf("Sleep Duration: %d\n", result.sleep_duration);
        printf("Error Code: %d\n", result.error_code);

        // Check for quit command
        if (strcmp(input, "q") == 0) {
            break;
        }

        // Process the input with high-precision timing
        gettimeofday(&start, NULL);
        bool dpcorrect = handle_dependencies(&result);
        if(dpcorrect) topo_sort(result.op1.row-1,result.op1.col-1,&result);
        gettimeofday(&end, NULL);

        // Calculate execution time in seconds with microsecond precision
        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

        // Update status
        strcpy(status, "ok");
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