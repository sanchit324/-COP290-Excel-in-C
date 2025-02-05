#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERR_OK,
    ERR_INVALID_COMMAND,
    ERR_INVALID_CELL,
    ERR_INVALID_RANGE,
    ERR_DIV_ZERO,
    ERR_CIRCULAR,
    ERR_SYNTAX,
    ERR_OVERFLOW,
    ERR_SLEEP
} ErrorCode;

const char *error_get_message(ErrorCode code);

#endif