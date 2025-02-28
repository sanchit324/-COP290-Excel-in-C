/**
 * io.h
 * Input/Output handling and command parsing for the spreadsheet application
 * Defines command types, structures, and input processing functions
 */

#ifndef __INP__BOOL__
    #define __INP__BOOL__
    #include <stdbool.h>
#endif

#ifndef __INP_SPC__
    #define __INP_SPC__

    // Maximum lengths for various input components
    #define MAX_INPUT_LEN 256    // Maximum length of input command
    #define MAX_CELL_LEN 7       // Maximum length of cell reference (e.g., "AAA999")
    #define MAX_EXPR_LEN 200     // Maximum length of formula/expression
    #define MAX_RANGE_LEN 25     // Maximum length of range reference (e.g., "A1:B10")

#endif

#ifndef __INP_CMD__
    #define __INP_CMD__

/**
 * Enumeration of all possible command types in the spreadsheet
 */
typedef enum {
    CMD_SET_CELL,    // Set value or formula in a cell
    CMD_SCROLL,      // Scroll to specific cell
    CMD_SCROLL_DIR,  // Scroll in direction (w,a,s,d)
    CMD_CONTROL,     // Control commands (enable/disable output)
    CMD_SLEEP,       // Sleep command
    CMD_ARITHMETIC,  // Arithmetic operations
    CMD_FUNCTION,    // Function operations (MIN,MAX,etc)
    CMD_INVALID      // Invalid/unrecognized command
} CommandType;

/**
 * Enumeration of supported spreadsheet functions
 */
typedef enum {
    FUNC_NONE,     // No function (direct value/arithmetic)
    FUNC_MIN,      // Minimum value in range
    FUNC_MAX,      // Maximum value in range
    FUNC_SUM,      // Sum of values in range
    FUNC_AVG,      // Average of values in range
    FUNC_STDEV,    // Standard deviation of values in range
    FUNC_SLEEP     // Sleep function
} FunctionType;

/**
 * Structure to represent a cell reference or value operand
 */
typedef struct {
    int row;     // Row number (1-based)
    int col;     // Column number (1-based)
    int value;   // Direct value (used when row/col are 0)
} Operand;

/**
 * Structure to hold a parsed command with all its components
 */
typedef struct {
    CommandType type;                    // Type of command
    FunctionType func;                   // Function type (if applicable)
    char command[MAX_EXPR_LEN];         // Original command string
    Operand op1;                        // Target cell
    Operand op2;                        // First operand
    Operand op3;                        // Second operand
    char cell[MAX_CELL_LEN];           // Target cell reference
    char expression[MAX_EXPR_LEN];     // Formula/expression
    char range[MAX_RANGE_LEN];         // Range reference
    char scroll_target[MAX_CELL_LEN];  // Target cell for scroll
    char scroll_direction;              // Scroll direction (w,a,s,d)
    char control_cmd[16];              // Control command string
    int sleep_duration;                // Sleep duration in seconds
    int error_code;                    // Error code if command fails
    char operator;                     // Arithmetic operator (+,-,*,/)
} ParsedCommand;

// Global flags for output control and viewport position
extern bool output_enabled;
extern int viewport_row;
extern int viewport_col;

#endif

#ifndef __INP_FNC__
    #define __INP_FNC__
    
    // Input processing functions
    void input_reader(char *inp);                    // Read raw input from user
    void input_parser(char *inp, ParsedCommand *result);  // Parse input into command structure
    bool validate_cell(const char *cell);            // Validate cell reference format
    bool validate_range(const char *range);          // Validate range reference format
    bool validate_function(const char *func);        // Validate function name
    void cell_to_rc(const char *cell, int *row, int *col);  // Convert cell reference to numbers
    bool is_number(const char* str);                 // Check if string is valid number
#endif