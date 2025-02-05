#include "error.h"

const char *error_get_message(ErrorCode code) {
    static const char *messages[] = {
        [ERR_OK] = "ok",
        [ERR_INVALID_COMMAND] = "Invalid command",
        [ERR_INVALID_CELL] = "Invalid cell reference",
        [ERR_INVALID_RANGE] = "Invalid range",
        [ERR_DIV_ZERO] = "Division by zero",
        [ERR_CIRCULAR] = "Circular dependency",
        [ERR_SYNTAX] = "Syntax error",
        [ERR_OVERFLOW] = "Value overflow",
        [ERR_SLEEP] = "Sleep interrupted"
    };
    return messages[code];
}