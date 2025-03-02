/**
 * process.h
 * Core processing functions for spreadsheet operations
 * Handles cell assignments, arithmetic operations, and function evaluations
 */

#include "io.h"
#include <stdbool.h>

#ifndef __FUNC_PROC__
    #define __FUNC_PROC__

    // Function type constants for cell operations
    #define FUNC_NONE   0    // Direct value or arithmetic operation
    #define FUNC_MIN    1    // Minimum value in range
    #define FUNC_MAX    2    // Maximum value in range
    #define FUNC_SUM    3    // Sum of values in range
    #define FUNC_AVG    4    // Average of values in range
    #define FUNC_STDEV  5    // Standard deviation of values in range
    #define FUNC_SLEEP  6    // Sleep operation

    // Core processing functions
    void assign(ParsedCommand *result);                  // Handle cell assignments
    void arithmetic(ParsedCommand *result);              // Process arithmetic operations
    void function(ParsedCommand *result);                // Execute spreadsheet functions
    void process_command(ParsedCommand *result);         // Main command processor
    bool handle_dependencies(ParsedCommand* result);      // Manage cell dependencies
    bool is_valid_range(ParsedCommand* cmd);             // Validate cell ranges
    bool is_numeric_value(ParsedCommand* cmd);           // Check for valid numeric input

#endif