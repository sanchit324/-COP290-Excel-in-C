#include "util.h"
#include <ctype.h>
#include <stdlib.h>

int util_col_to_idx(const char *col_str) {
    int idx = 0;
    while (*col_str) {
        char c = toupper(*col_str);
        if (!isalpha(c)) return -1;
        idx = idx * 26 + (c - 'A' + 1);
        col_str++;
    }
    return idx - 1;
}

int util_row_to_idx(const char *row_str) {
    if (!row_str || !isdigit(*row_str)) return -1;
    return atoi(row_str) - 1;
}

int util_parse_cellref(const char *ref, int *row, int *col) {
    char col_part[4] = {0};
    const char *p = ref;
    int i = 0;
    
    // Extract column part
    while (isalpha(*p) && i < 3) {
        col_part[i++] = *p++;
    }
    if (i == 0) return -1;  // No column letters
    
    // Extract row part
    const char *row_part = p;
    while (*p && isdigit(*p)) p++;
    if (p == row_part) return -1;  // No row numbers
    
    *col = util_col_to_idx(col_part);
    *row = util_row_to_idx(row_part);
    
    return (*col >= 0 && *row >= 0) ? 0 : -1;
}

int util_is_valid_cell(int row, int col, int max_row, int max_col) {
    return (row >= 0) && (col >= 0) && (row < max_row) && (col < max_col);
}