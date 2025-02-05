#ifndef CELL_H
#define CELL_H

typedef struct Cell {
    int value;           // Current integer value of the cell
    char *formula;       // Formula string (NULL if direct value)
    int is_dirty;        // 1 = needs recalculation, 0 = up-to-date
} Cell;

// Create a new cell initialized to 0
Cell *cell_create();

// Free cell memory
void cell_destroy(Cell *cell);

// Set direct value (clears formula)
void cell_set_value(Cell *cell, int value);

// Set formula (marks as dirty)
void cell_set_formula(Cell *cell, const char *formula);

// Mark cell as needing recalculation
void cell_mark_dirty(Cell *cell);

// Check if cell needs recalculation
int cell_is_dirty(const Cell *cell);

#endif