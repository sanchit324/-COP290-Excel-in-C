#include "cell.h"
#include <stdlib.h>
#include <string.h>

Cell *cell_create() {
    Cell *cell = malloc(sizeof(Cell));
    if (!cell) return NULL;
    
    cell->value = 0;
    cell->formula = NULL;
    cell->is_dirty = 0;
    return cell;
}

void cell_destroy(Cell *cell) {
    if (cell) {
        if (cell->formula) free(cell->formula);
        free(cell);
    }
}

void cell_set_value(Cell *cell, int value) {
    if (!cell) return;
    
    // Clear existing formula
    if (cell->formula) {
        free(cell->formula);
        cell->formula = NULL;
    }
    
    cell->value = value;
    cell->is_dirty = 0;
}

void cell_set_formula(Cell *cell, const char *formula) {
    if (!cell || !formula) return;
    
    // Free old formula if exists
    if (cell->formula) free(cell->formula);
    
    // Allocate and copy new formula
    cell->formula = malloc(strlen(formula) + 1);
    if (cell->formula) {
        strcpy(cell->formula, formula);
        cell->is_dirty = 1;  // Mark for recalculation
    }
}

void cell_mark_dirty(Cell *cell) {
    if (cell) cell->is_dirty = 1;
}

int cell_is_dirty(const Cell *cell) {
    return cell ? cell->is_dirty : 0;
}