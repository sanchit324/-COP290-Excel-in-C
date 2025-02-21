#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "io.h"
#include "display.h"

bool output_enabled = true;
int viewport_row = 1;
int viewport_col = 1;

static char* trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
    return str;
}

bool validate_cell(const char *cell) {
    int len = strlen(cell);
    if(len < 2 || len > 6) return false;
    
    int split_pos = 0;
    while(isalpha(cell[split_pos])) split_pos++;
    if(split_pos == 0 || split_pos > 3) return false;
    
    char row_str[4];
    strncpy(row_str, cell + split_pos, 3);
    int row = atoi(row_str);
    return row >= 1 && row <= MAXROW;
}

bool validate_range(const char *range) {
    char *colon = strchr(range, ':');
    if(!colon || colon == range || colon == range + strlen(range) - 1) return false;
    
    char start[MAX_CELL_LEN], end[MAX_CELL_LEN];
    strncpy(start, range, colon - range);
    start[colon - range] = '\0';
    strcpy(end, colon + 1);
    
    return validate_cell(start) && validate_cell(end);
}

bool validate_function(const char *func) {
    const char *valid_funcs[] = {"MIN", "MAX", "AVG", "SUM", "STDEV", "SLEEP"};
    for(int i = 0; i < 6; i++) {
        if(strcmp(func, valid_funcs[i]) == 0) return true;
    }
    return false;
}

void cell_to_rc(const char *cell, int *row, int *col) {
    int c = 0, r = 0, i = 0;
    while(isalpha(cell[i])) {
        c = c * 26 + (toupper(cell[i]) - 'A' + 1);
        i++;
    }
    r = atoi(cell + i);
    *col = c;
    *row = r;
}

void input_reader(char *inp) {
    memset(inp, 0, MAX_INPUT_LEN);
    if(!fgets(inp, MAX_INPUT_LEN, stdin)) {
        inp[0] = '\0';
        return;
    }
    inp[strcspn(inp, "\n")] = '\0';
    
    if(strlen(inp) == MAX_INPUT_LEN - 1) {
        int c;
        while((c = getchar()) != '\n' && c != EOF);
    }
}

void input_parser(char *inp, ParsedCommand *result) {
    memset(result, 0, sizeof(ParsedCommand));
    result->type = CMD_INVALID;
    result->error_code = 0;
    result->operator = '\0';
    
    char *trimmed = trim_whitespace(inp);
    if(strlen(trimmed) == 0) return;

    strncpy(result->command, trimmed, MAX_EXPR_LEN - 1);

    // Handle control commands
    if(strcmp(trimmed, "q") == 0 ||
       strcmp(trimmed, "disable_output") == 0 || 
       strcmp(trimmed, "enable_output") == 0) {
        result->type = CMD_CONTROL;
        strncpy(result->control_cmd, trimmed, sizeof(result->control_cmd)-1);
        return;
    }

    // Handle scroll_to command
    if(strncmp(trimmed, "scroll_to ", 10) == 0) {
        char *cell = trim_whitespace(trimmed + 10);
        if(validate_cell(cell)) {
            result->type = CMD_SCROLL;
            strncpy(result->scroll_target, cell, sizeof(result->scroll_target)-1);
            cell_to_rc(cell, &result->op1.row, &result->op1.col);
        }
        return;
    }

    // Handle scroll commands
    if(strlen(trimmed) == 1 && strchr("wasd", tolower(*trimmed))) {
        result->type = CMD_SCROLL_DIR;
        result->scroll_direction = tolower(*trimmed);
        return;
    }

    // Handle cell assignments and functions
    char *equals = strchr(trimmed, '=');
    if(equals) {
        *equals = '\0';
        char *cell_part = trim_whitespace(trimmed);
        char *expr_part = trim_whitespace(equals + 1);
        
        if(validate_cell(cell_part)) {
            strncpy(result->cell, cell_part, sizeof(result->cell)-1);
            cell_to_rc(cell_part, &result->op1.row, &result->op1.col);
            
            // Check for functions
            char *func_start = strchr(expr_part, '(');
            char *func_end = strchr(expr_part, ')');
            if(func_start && func_end) {
                char func_name[10];
                sscanf(expr_part, "%[^(]", func_name);
                
                if(validate_function(func_name)) {
                    result->type = CMD_FUNCTION;
                    if(strcmp(func_name, "SLEEP") == 0) {
                        result->func = FUNC_SLEEP;
                        sscanf(expr_part, "SLEEP(%d)", &result->sleep_duration);
                    } else {
                        if(strcmp(func_name, "MIN") == 0) result->func = FUNC_MIN;
                        else if(strcmp(func_name, "MAX") == 0) result->func = FUNC_MAX;
                        else if(strcmp(func_name, "AVG") == 0) result->func = FUNC_AVG;
                        else if(strcmp(func_name, "SUM") == 0) result->func = FUNC_SUM;
                        else if(strcmp(func_name, "STDEV") == 0) result->func = FUNC_STDEV;
                        
                        char *range_start = func_start + 1;
                        char *range_end = func_end;
                        strncpy(result->range, range_start, range_end - range_start);
                        result->range[range_end - range_start] = '\0';
                        
                        if(validate_range(result->range)) {
                            char start_cell[MAX_CELL_LEN], end_cell[MAX_CELL_LEN];
                            sscanf(result->range, "%[^:]:%s", start_cell, end_cell);
                            cell_to_rc(start_cell, &result->op2.row, &result->op2.col);
                            cell_to_rc(end_cell, &result->op3.row, &result->op3.col);
                        } else {
                            result->error_code = 2; // Invalid range
                        }
                    }
                } else {
                    result->error_code = 1; // Invalid function
                }
            } else {
                // Handle arithmetic expressions
                char op1[MAX_CELL_LEN], op2[MAX_CELL_LEN];
                if(sscanf(expr_part, "%[^+-*/]%c%s", op1, &result->operator, op2) == 3) {
                    result->type = CMD_ARITHMETIC;
                    if(validate_cell(op1)) {
                        cell_to_rc(op1, &result->op2.row, &result->op2.col);
                    } else {
                        result->op2.value = atoi(op1);
                    }
                    if(validate_cell(op2)) {
                        cell_to_rc(op2, &result->op3.row, &result->op3.col);
                    } else {
                        result->op3.value = atoi(op2);
                    }
                } else {
                    // Single value assignment
                    result->type = CMD_SET_CELL;
                    if(validate_cell(expr_part)) {
                        cell_to_rc(expr_part, &result->op2.row, &result->op2.col);
                    } else {
                        result->op2.value = atoi(expr_part);
                    }
                }
            }
            strncpy(result->expression, expr_part, sizeof(result->expression)-1);
        }
        return;
    }

    // Handle standalone SLEEP function
    if(strncmp(trimmed, "SLEEP(", 6) == 0) {
        result->type = CMD_FUNCTION;
        result->func = FUNC_SLEEP;
        sscanf(trimmed, "SLEEP(%d)", &result->sleep_duration);
        return;
    }
}
