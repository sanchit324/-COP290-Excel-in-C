#include "viewport.h"
#include <stdlib.h>

Viewport viewport_create(int total_rows, int total_cols) {
    Viewport vp = {
        .top_row = 0,
        .left_col = 0,
        .rows = total_rows,
        .cols = total_cols
    };
    return vp;
}

void viewport_scroll_up(Viewport *vp) {
    vp->top_row = (vp->top_row >= 10) ? vp->top_row - 10 : 0;
}

void viewport_scroll_down(Viewport *vp) {
    int max_row = vp->rows - 10;
    if (max_row < 0) max_row = 0;
    vp->top_row = (vp->top_row + 10 < max_row) ? vp->top_row + 10 : max_row;
}

void viewport_scroll_left(Viewport *vp) {
    vp->left_col = (vp->left_col >= 10) ? vp->left_col - 10 : 0;
}

void viewport_scroll_right(Viewport *vp) {
    int max_col = vp->cols - 10;
    if (max_col < 0) max_col = 0;
    vp->left_col = (vp->left_col + 10 < max_col) ? vp->left_col + 10 : max_col;
}

void viewport_get_visible_range(const Viewport *vp,
                               int *start_row, int *end_row,
                               int *start_col, int *end_col) {
    *start_row = vp->top_row;
    *end_row = (*start_row + 9 < vp->rows) ? *start_row + 9 : vp->rows - 1;
    
    *start_col = vp->left_col;
    *end_col = (*start_col + 9 < vp->cols) ? *start_col + 9 : vp->cols - 1;
}