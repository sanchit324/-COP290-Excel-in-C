/**
 * process.c
 * Handles all command processing and cell operations in the spreadsheet
 */

#include <math.h>
#include <unistd.h>
#include <limits.h>
#include "init.h"
#include "io.h"
#include "process.h"
#include "display.h"
#include "string.h"
#include <stdio.h>
#include "dependent.h"
#include <time.h>

// ERROR_VALUE is already defined in init.h, no need to redefine it here

// Add external declaration for status variable
extern char status[20];

// Add this function prototype at the top of the file, after the includes
// and before any function definitions
void update_dependents(int row, int col);


/**
 * Assigns a value to a cell and handles dependencies
 * - Supports direct value assignment and cell reference assignment
 * - Updates dependent cells, especially those with SLEEP functions
 * - Maintains parent-child relationships in dependency graph
 */
void assign(ParsedCommand *result) {
    int r1 = result->op1.row - 1;
    int c1 = result->op1.col - 1;
    int r2 = result->op2.row - 1;
    int c2 = result->op2.col - 1;
    int val = result->op2.value;
    
    // Store original value in case we need to revert
    int original_value = sheet[r1][c1];

    // Remove old dependencies
    Parent *parent = Parent_lst[r1][c1];
    while (parent != NULL) {
        remove_child(parent->r, parent->c, r1, c1);
        parent = parent->next;
    }
    while (Parent_lst[r1][c1] != NULL) {
        remove_parent(Parent_lst[r1][c1]->r, Parent_lst[r1][c1]->c, r1, c1);
    }

    if (r2 == -1 && c2 == -1) {
        // Direct value assignment
        sheet[r1][c1] = val;
    } else {
        // Cell reference assignment
        // Add dependency
        assign_parent(r2, c2, r1, c1, *result);
        assign_child(r2, c2, r1, c1, *result);
        
        // Check for cycles
        if (detect_cycle(r1, c1)) {
            // Remove the dependencies if cycle is detected
            remove_child(r2, c2, r1, c1);
            remove_parent(r2, c2, r1, c1);
            // Restore original value instead of setting ERROR_VALUE
            sheet[r1][c1] = original_value;
            // Set status to "err" for cycle detection
            strcpy(status, "err");
            return;
        }
        
        // Check if the referenced cell has an error value
        if (sheet[r2][c2] == ERROR_VALUE) {
            sheet[r1][c1] = ERROR_VALUE;
        } else {
            sheet[r1][c1] = sheet[r2][c2];
        }
    }
}

/**
 * Performs arithmetic operations between cells or values
 * - Supports +, -, *, / operations
 * - Handles cell references and direct values
 * - Sets ERROR_VALUE for division by zero and propagates to dependents
 */
void arithmetic(ParsedCommand *result) {
    int r1 = result->op1.row - 1;
    int c1 = result->op1.col - 1;
    int r2 = result->op2.row - 1;
    int c2 = result->op2.col - 1;
    int r3 = result->op3.row - 1;
    int c3 = result->op3.col - 1;
    int val2 = result->op2.value;
    int val3 = result->op3.value;
    
    // Store original value in case we need to revert
    int original_value = sheet[r1][c1];

    // Remove old dependencies
    Parent *parent = Parent_lst[r1][c1];
    while (parent != NULL) {
        remove_child(parent->r, parent->c, r1, c1);
        parent = parent->next;
    }
    while (Parent_lst[r1][c1] != NULL) {
        remove_parent(Parent_lst[r1][c1]->r, Parent_lst[r1][c1]->c, r1, c1);
    }

    // Add dependencies for cell references
    if (r2 != -1 && c2 != -1) {
        assign_parent(r2, c2, r1, c1, *result);
        assign_child(r2, c2, r1, c1, *result);
    }
    if (r3 != -1 && c3 != -1) {
        assign_parent(r3, c3, r1, c1, *result);
        assign_child(r3, c3, r1, c1, *result);
    }

    // Check for cycles after adding dependencies
    if (detect_cycle(r1, c1)) {
        // Remove the dependencies if cycle is detected
        if (r2 != -1 && c2 != -1) {
            remove_child(r2, c2, r1, c1);
            remove_parent(r2, c2, r1, c1);
        }
        if (r3 != -1 && c3 != -1) {
            remove_child(r3, c3, r1, c1);
            remove_parent(r3, c3, r1, c1);
        }
        // Restore original value instead of setting ERROR_VALUE
        sheet[r1][c1] = original_value;
        // Set status to "err" for cycle detection
        strcpy(status, "err");
        return;
    }

    int operand1 = (r2 == -1 && c2 == -1) ? val2 : sheet[r2][c2];
    int operand2 = (r3 == -1 && c3 == -1) ? val3 : sheet[r3][c3];

    // Check if any operand is ERROR_VALUE
    if (operand1 == ERROR_VALUE || operand2 == ERROR_VALUE) {
        sheet[r1][c1] = ERROR_VALUE;
        return;
    }

    switch (result->operator) {
        case '+':
            sheet[r1][c1] = operand1 + operand2;
            break;
        case '-':
            sheet[r1][c1] = operand1 - operand2;
            break;
        case '*':
            sheet[r1][c1] = operand1 * operand2;
            break;
        case '/':
            if (operand2 == 0) {
                sheet[r1][c1] = ERROR_VALUE;
            } else {
                sheet[r1][c1] = operand1 / operand2;
            }
            break;
    }
}

/**
 * Validates if a range of cells is within bounds and properly ordered
 * @param r1,c1 Starting cell coordinates (0-based)
 * @param r2,c2 Ending cell coordinates (0-based)
 * @return true if range is valid, false otherwise
 */
bool is_valid_range(ParsedCommand* cmd) {
    // For range operations
    if (cmd->func == FUNC_MIN || cmd->func == FUNC_MAX || cmd->func == FUNC_SUM || 
        cmd->func == FUNC_AVG || cmd->func == FUNC_STDEV) {
        
        int r1 = cmd->op2.row - 1;
        int c1 = cmd->op2.col - 1;
        int r2 = cmd->op3.row - 1;
        int c2 = cmd->op3.col - 1;

        // Check if range is forward-moving
        if (r1 > r2 || c1 > c2) {
            return false;
        }

        // Check if range is within bounds
        if (r1 < 0 || r1 >= MAXROW || c1 < 0 || c1 >= MAXCOL ||
            r2 < 0 || r2 >= MAXROW || c2 < 0 || c2 >= MAXCOL) {
            return false;
        }

        return true;
    }
    return true;  // Non-range operations are always valid
}

/**
 * Main dependency handler for all cell operations
 * - Creates and maintains dependency relationships
 * - Detects and prevents circular dependencies
 * - Updates dependent cells when source cells change
 * - Handles both direct assignments and range operations
 */
bool handle_dependencies(ParsedCommand *result) {
    // Handle special commands first
    if (result->type == CMD_SCROLL || 
        result->type == CMD_SCROLL_DIR || 
        result->type == CMD_CONTROL || 
        result->type == CMD_INVALID) {
        process_command(result);
        return false;
    }

    int r1 = result->op1.row - 1;
    int c1 = result->op1.col - 1;
    
    // Store the original value to check if it changed
    int original_value = sheet[r1][c1];

    // Process the current cell
    if (result->type == CMD_SET_CELL) {
        assign(result);
    } else if (result->type == CMD_ARITHMETIC) {
        arithmetic(result);
    } else if (result->type == CMD_FUNCTION) {
        function(result);
    }

    // Only update dependents if the value actually changed
    if (sheet[r1][c1] != original_value) {
        // Update all dependent cells recursively
        update_dependents(r1, c1);
    }
    
    return true;
}

/**
 * Processes function commands (MIN, MAX, AVG, SUM, STDEV, SLEEP)
 * - Handles range-based operations
 * - Special handling for SLEEP function:
 *   * Supports both cell reference and direct value for duration
 *   * Creates dependencies for cell-based sleep durations
 *   * Validates sleep duration (0-3600 seconds)
 *   * Updates cell value after sleep
 */
void function(ParsedCommand *result) {
    int r1 = result->op1.row - 1;
    int c1 = result->op1.col - 1;
    int r2 = result->op2.row - 1;
    int c2 = result->op2.col - 1;
    int r3 = result->op3.row - 1;
    int c3 = result->op3.col - 1;
    
    // Store original value in case we need to revert
    int original_value = sheet[r1][c1];
    
    // Remove old dependencies
    Parent *parent = Parent_lst[r1][c1];
    while (parent != NULL) {
        remove_child(parent->r, parent->c, r1, c1);
        parent = parent->next;
    }
    while (Parent_lst[r1][c1] != NULL) {
        remove_parent(Parent_lst[r1][c1]->r, Parent_lst[r1][c1]->c, r1, c1);
    }
    
    if (result->func == FUNC_SLEEP) {
        // Get sleep duration from cell reference or direct value
        int sleep_duration;
        
        // Add dependency if using cell reference
        if (result->op2.row != 0 && result->op2.col != 0) {
            assign_parent(r2, c2, r1, c1, *result);
            assign_child(r2, c2, r1, c1, *result);
            
            // Check for cycles
            if (detect_cycle(r1, c1)) {
                remove_child(r2, c2, r1, c1);
                remove_parent(r2, c2, r1, c1);
                // Restore original value instead of setting ERROR_VALUE
                sheet[r1][c1] = original_value;
                // Set status to "err" for cycle detection
                strcpy(status, "err");
                return;
            }
            
            sleep_duration = sheet[r2][c2];
        } else {
            sleep_duration = result->op2.value;
        }

        // Validate sleep duration
        if (sleep_duration < 0 || sleep_duration > 3600) {
            sheet[r1][c1] = 0;
            return;
        }

        // Perform sleep and store the duration
        // Use nanosleep for more precise timing
        struct timespec ts;
        ts.tv_sec = sleep_duration;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
        
        sheet[r1][c1] = sleep_duration;
        return;
    }

    // For range operations, add dependencies for all cells in the range
    if (result->func == FUNC_MIN || result->func == FUNC_MAX || 
        result->func == FUNC_SUM || result->func == FUNC_AVG || 
        result->func == FUNC_STDEV) {
        
        // Validate range
        if (!is_valid_range(result)) {
            // Restore original value instead of setting ERROR_VALUE
            sheet[r1][c1] = original_value;
            return;
        }
        
        // Add dependencies for all cells in the range
        for (int i = r2; i <= r3; i++) {
            for (int j = c2; j <= c3; j++) {
                assign_parent(i, j, r1, c1, *result);
                assign_child(i, j, r1, c1, *result);
            }
        }
        
        // Check for cycles
        if (detect_cycle(r1, c1)) {
            // Remove all dependencies if cycle is detected
            for (int i = r2; i <= r3; i++) {
                for (int j = c2; j <= c3; j++) {
                    remove_child(i, j, r1, c1);
                    remove_parent(i, j, r1, c1);
                }
            }
            // Restore original value instead of setting ERROR_VALUE
            sheet[r1][c1] = original_value;
            // Set status to "err" for cycle detection
            strcpy(status, "err");
            return;
        }
    }

    // Initialize variables for range operations
    int sum = 0;
    int count = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    int std_dev = 0;

    // Calculate range statistics
    for (int i = r2; i <= r3; i++) {
        for (int j = c2; j <= c3; j++) {
            int value = sheet[i][j];
            if (value == ERROR_VALUE) {
                sheet[r1][c1] = ERROR_VALUE;
                return;
            }
            sum += value;
            count++;
            if (value < min) min = value;
            if (value > max) max = value;

        }
    }

     if (count <= 1) std_dev =  0;  // Avoid division by zero
    int mean;
    double variance = 0.0;
    mean = sum / count;


    for (int i = r2; i <= r3; i++) {
        for (int j = c2; j <= c3; j++) {
            variance += (sheet[i][j] - mean) * (sheet[i][j] - mean);
        }
    }
    // Calculate variance
    variance /= count;

    // Return integer standard deviation (rounded)
    std_dev =  (int)round(sqrt(variance));



    switch (result->func) {
        case FUNC_MIN:
            sheet[r1][c1] = min;
            break;
        case FUNC_MAX:
            sheet[r1][c1] = max;
            break;
        case FUNC_SUM:
            sheet[r1][c1] = sum;
            break;
        case FUNC_AVG:
            sheet[r1][c1] = (count > 0) ? sum / count : ERROR_VALUE;
            break;
        case FUNC_STDEV:
             sheet[r1][c1] = std_dev;
            break;
        case FUNC_SLEEP:
            // Sleep function is handled separately, nothing to do here
            break;
        case FUNC_NONE:
            // This should never happen in this context
            sheet[r1][c1] = ERROR_VALUE;
            break;
    }
}

/**
 * Main command processor
 * - Routes commands to appropriate handlers
 * - Handles all command types:
 *   * Cell operations (SET, ARITHMETIC, FUNCTION)
 *   * Navigation (SCROLL, SCROLL_DIR)
 *   * Control commands (enable/disable_output)
 *   * Sleep commands
 */
void process_command(ParsedCommand *result) {
    switch (result->type) {
        case CMD_SET_CELL:
            assign(result);
            break;
        case CMD_SCROLL:
            set_org(result->op1.row, result->op1.col);
            break;
        case CMD_SCROLL_DIR:
            // Validate scroll direction before processing
            if (result->scroll_direction == 'w' || 
                result->scroll_direction == 'a' || 
                result->scroll_direction == 's' || 
                result->scroll_direction == 'd') {
                switch (result->scroll_direction) {
                    case 'w': w(); break;
                    case 'a': a(); break;
                    case 's': s(); break;
                    case 'd': d(); break;
                }
            }
            break;
        case CMD_CONTROL:
            if (strcmp(result->control_cmd, "disable_output") == 0) {
                disable_output();
            } else if (strcmp(result->control_cmd, "enable_output") == 0) {
                enable_output();
            }
            break;
        case CMD_SLEEP:
            sleep((unsigned int) result->sleep_duration);
            break;
        case CMD_FUNCTION:
            function(result);
            break;
        case CMD_ARITHMETIC:
            arithmetic(result);
            break;
        case CMD_INVALID:
            // Handle invalid command
            break;
    }
}

/**
 * Checks if the value being assigned is a valid numeric value
 */
bool is_numeric_value(ParsedCommand* cmd) {
    // For direct cell assignments
    if (cmd->type == CMD_SET_CELL) {
        // If it's a cell reference, it's valid
        if (cmd->op2.row != 0 || cmd->op2.col != 0) {
            return true;
        }
        
        // If it's a direct value, check if the expression is numeric
        char* endptr;
        strtol(cmd->expression, &endptr, 10);
        // If endptr points to the end of string, the entire string was numeric
        return (*endptr == '\0');
    }
    return true;  // Other command types are considered valid
}

/**
 * Recursively updates all cells that depend on the given cell
 */
void update_dependents(int row, int col) {
    Child *child = Child_lst[row][col];
    
    while (child != NULL) {
        int child_r = child->r;
        int child_c = child->c;
        ParsedCommand cmd = child->formula;
        
        // Store the previous value to check if it changed
        int previous_value = sheet[child_r][child_c];
        
        // Process the child's formula, but skip SLEEP operations during dependency updates
        if (cmd.type == CMD_ARITHMETIC) {
            arithmetic(&cmd);
        } else if (cmd.type == CMD_FUNCTION && cmd.func != FUNC_SLEEP) {
            function(&cmd);
        } else if (cmd.type == CMD_SET_CELL) {
            assign(&cmd);
        } else if (cmd.type == CMD_FUNCTION && cmd.func == FUNC_SLEEP) {
            // For SLEEP functions, just update the value without sleeping again
            int r1 = cmd.op1.row - 1;
            int c1 = cmd.op1.col - 1;
            int r2 = cmd.op2.row - 1;
            int c2 = cmd.op2.col - 1;
            
            // Get sleep duration without sleeping
            int sleep_duration;
            if (cmd.op2.row != 0 && cmd.op2.col != 0) {
                sleep_duration = sheet[r2][c2];
            } else {
                sleep_duration = cmd.op2.value;
            }
            
            // Just update the cell value without sleeping
            if (sleep_duration >= 0 && sleep_duration <= 3600) {
                sheet[r1][c1] = sleep_duration;
            }
        }
        
        // Only recursively update if the value actually changed
        if (sheet[child_r][child_c] != previous_value) {
            // Recursively update this child's dependents
            update_dependents(child_r, child_c);
        }
        
        child = child->next;
    }
}
