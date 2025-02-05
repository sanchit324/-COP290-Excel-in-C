#ifndef UTIL_H
#define UTIL_H

// Convert column letters to index (A=0, ZZ=675, etc.)
int util_col_to_idx(const char *col_str);

// Convert row string to 0-based index
int util_row_to_idx(const char *row_str);

// Convert cell reference (e.g., "A1") to row/col indices
int util_parse_cellref(const char *ref, int *row, int *col);

// Check if cell coordinates are valid
int util_is_valid_cell(int row, int col, int max_row, int max_col);

#endif