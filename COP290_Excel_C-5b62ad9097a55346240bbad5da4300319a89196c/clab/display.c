#include <stdio.h>
#include <limits.h>  // For INT_MIN
#include "init.h"
#include "display.h"
#include "io.h"  // For output_enabled

int curr_org_r = 1;
int curr_org_c = 1;
int displayr = 10;
int displayc = 10;
int cellwidth = 8;

int pending_scroll_row = 0;
int pending_scroll_col = 0;
bool accumulating_scrolls = false;

#define DEFAULT_SCROLL 10

void int_to_alpha(int x, char* alpha) {
    if (x > MAXCOL) return;
    int i = MAXCOLWIDTH - 1;
    alpha[i + 1] = '\0';

    while (x != 0) {
        alpha[i] = (char)('A' - 1 + ((x % 26) == 0 ? 26 : (x % 26)));
        x -= ((x % 26) == 0 ? 26 : (x % 26));
        x /= 26;
        i--;
    }
    while (i >= 0) {
        alpha[i] = ' ';
        i--;
    }
}

void apply_pending_scroll() {
    if (pending_scroll_row != 0 || pending_scroll_col != 0) {
        set_org(curr_org_r + pending_scroll_row, curr_org_c + pending_scroll_col);
        pending_scroll_row = 0;
        pending_scroll_col = 0;
    }
}

void display_sheet() {
    if (!output_enabled) return;

    apply_pending_scroll();  // Apply pending scrolls if any

    int max_display_r = (curr_org_r + displayr - 1 > MAXROW) ? (MAXROW - curr_org_r + 1) : displayr;
    int max_display_c = (curr_org_c + displayc - 1 > MAXCOL) ? (MAXCOL - curr_org_c + 1) : displayc;
    
    printf("%*s", cellwidth, "");
    for (int i = 0; i < max_display_c; i++) {
        char temp[MAXCOLWIDTH + 1] = "   ";
        int_to_alpha(curr_org_c + i, temp);
        printf("%*s", cellwidth, temp);
    }
    printf("\n");

    for (int j = 0; j < max_display_r; j++) {
        printf("%-*d", cellwidth, curr_org_r + j);
        for (int i = 0; i < max_display_c; i++) {
            int value = sheet[curr_org_r + j - 1][curr_org_c + i - 1];
            if (value == INT_MIN || value == ERROR_VALUE) {
                printf("%*s", cellwidth, "ERR");
            } else {
                printf("%*d", cellwidth, value);
            }
        }
        printf("\n");
    }
}

void set_org(int r, int c) {
    if (!output_enabled) {
        pending_scroll_row = r - curr_org_r;
        pending_scroll_col = c - curr_org_c;
        accumulating_scrolls = true;
        return;
    }

    curr_org_r = (r < 1) ? 1 : (r > MAXROW ? MAXROW : r);
    curr_org_c = (c < 1) ? 1 : (c > MAXCOL ? MAXCOL : c);
}

void scroll(int dr, int dc) {
    set_org(curr_org_r + dr, curr_org_c + dc);
}

void w() {
    if (!output_enabled) {
        pending_scroll_row -= DEFAULT_SCROLL;
        accumulating_scrolls = true;
        return;
    }
    if (curr_org_r > 1) {
        set_org(curr_org_r - DEFAULT_SCROLL, curr_org_c);
    }
}

void a() {
    if (!output_enabled) {
        pending_scroll_col -= DEFAULT_SCROLL;
        accumulating_scrolls = true;
        return;
    }
    if (curr_org_c > 1) {
        set_org(curr_org_r, curr_org_c - DEFAULT_SCROLL);
    }
}

void s() {
    if (!output_enabled) {
        pending_scroll_row += DEFAULT_SCROLL;
        accumulating_scrolls = true;
        return;
    }
    if (curr_org_r + displayr - 1 < MAXROW) {
        int new_row = curr_org_r + DEFAULT_SCROLL;
        if (new_row + displayr - 1 > MAXROW) {
            new_row = MAXROW - displayr + 1;
        }
        set_org(new_row, curr_org_c);
    }
}

void d() {
    if (!output_enabled) {
        pending_scroll_col += DEFAULT_SCROLL;
        accumulating_scrolls = true;
        return;
    }
    if (curr_org_c + displayc - 1 < MAXCOL) {
        int new_col = curr_org_c + DEFAULT_SCROLL;
        if (new_col + displayc - 1 > MAXCOL) {
            new_col = MAXCOL - displayc + 1;
        }
        set_org(curr_org_r, new_col);
    }
}

// Call this function when output is re-enabled
void enable_output_handler() {
    if (accumulating_scrolls) {
        apply_pending_scroll();
        accumulating_scrolls = false;
    }
}