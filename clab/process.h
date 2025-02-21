#include "io.h"
#include <stdbool.h>

#ifndef __FUNC_PROC__
    #define __FUNC_PROC__
    void assign(ParsedCommand *result);
    void arithmetic(ParsedCommand *result);
    bool is_valid_range(int r1, int c1, int r2, int c2);
    void function(ParsedCommand *result);
    void process_command(ParsedCommand *result);
    bool handle_dependencies(ParsedCommand* result);
#endif