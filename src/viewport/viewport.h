#ifndef VIEWPORT_H
#define VIEWPORT_H

typedef struct {
    int top_row;    // Topmost visible row (0-based)
    int left_col;   // Leftmost visible column (0-based)
    int rows;       // Total rows in sheet
    int cols;       // Total columns in sheet
} Viewport;

// Initialize viewport
Viewport viewport_create(int total_rows, int total_cols);

// Scroll commands
void viewport_scroll_up(Viewport *vp);
void viewport_scroll_down(Viewport *vp);
void viewport_scroll_left(Viewport *vp);
void viewport_scroll_right(Viewport *vp);

// Get visible cell range
void viewport_get_visible_range(const Viewport *vp, 
                               int *start_row, int *end_row,
                               int *start_col, int *end_col);

#endif