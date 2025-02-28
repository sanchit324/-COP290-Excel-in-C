#include "io.h"
#include <stdbool.h>

#ifndef __FUNC_PROC__
    #define __FUNC_PROC__

    // Function types
    #define FUNC_NONE   0
    #define FUNC_MIN    1
    #define FUNC_MAX    2
    #define FUNC_SUM    3
    #define FUNC_AVG    4
    #define FUNC_STDEV  5
    #define FUNC_SLEEP  6

    void assign(ParsedCommand *result);
    void arithmetic(ParsedCommand *result);
    void function(ParsedCommand *result);
    void process_command(ParsedCommand *result);
    bool handle_dependencies(ParsedCommand* result);
    bool is_valid_range(ParsedCommand* cmd);
#endif