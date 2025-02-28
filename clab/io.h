#ifndef __INP__BOOL__
    #define __INP__BOOL__
    #include <stdbool.h>
#endif

#ifndef __INP_SPC__
    #define __INP_SPC__

    #define MAX_INPUT_LEN 256
    #define MAX_CELL_LEN 7
    #define MAX_EXPR_LEN 200
    #define MAX_RANGE_LEN 25

#endif

#ifndef __INP_CMD__
    #define __INP_CMD__

typedef enum {
    CMD_SET_CELL,
    CMD_SCROLL,
    CMD_SCROLL_DIR,
    CMD_CONTROL,
    CMD_SLEEP,
    CMD_ARITHMETIC,
    CMD_FUNCTION,
    CMD_INVALID
} CommandType;

typedef enum {
    FUNC_NONE,
    FUNC_MIN,
    FUNC_MAX,
    FUNC_SUM,
    FUNC_AVG,
    FUNC_STDEV,
    FUNC_SLEEP
} FunctionType;

typedef struct {
    int row;
    int col;
    int value;
} Operand;

typedef struct {
    CommandType type;
    FunctionType func;
    char command[MAX_EXPR_LEN];
    Operand op1;
    Operand op2;
    Operand op3;
    char cell[MAX_CELL_LEN];
    char expression[MAX_EXPR_LEN];
    char range[MAX_RANGE_LEN];
    char scroll_target[MAX_CELL_LEN];
    char scroll_direction;
    char control_cmd[16];
    int sleep_duration;
    int error_code;
    char operator; // New field to store the arithmetic operator
} ParsedCommand;

extern bool output_enabled;
extern int viewport_row;
extern int viewport_col;

#endif


#ifndef __INP_FNC__
    #define __INP_FNC__
    void input_reader(char *inp);
    void input_parser(char *inp, ParsedCommand *result);
    bool validate_cell(const char *cell);
    bool validate_range(const char *range);
    bool validate_function(const char *func);
    void cell_to_rc(const char *cell, int *row, int *col);
    bool is_number(const char* str);
#endif